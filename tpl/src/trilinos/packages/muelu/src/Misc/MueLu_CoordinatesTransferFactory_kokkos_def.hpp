// @HEADER
//
// ***********************************************************************
//
//        MueLu: A package for multigrid based preconditioning
//                  Copyright 2012 Sandia Corporation
//
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact
//                    Jonathan Hu       (jhu@sandia.gov)
//                    Andrey Prokopenko (aprokop@sandia.gov)
//                    Ray Tuminaro      (rstumin@sandia.gov)
//
// ***********************************************************************
//
// @HEADER
#ifndef MUELU_COORDINATESTRANSFER_FACTORY_KOKKOS_DEF_HPP
#define MUELU_COORDINATESTRANSFER_FACTORY_KOKKOS_DEF_HPP

#include "MueLu_CoordinatesTransferFactory_kokkos_decl.hpp"

#include <Xpetra_ImportFactory.hpp>
#include <Xpetra_IO.hpp>
#include <Xpetra_MultiVectorFactory.hpp>
#include <Xpetra_MapFactory.hpp>

#include "MueLu_Aggregates_kokkos.hpp"
#include "MueLu_CoarseMapFactory_kokkos.hpp"
#include "MueLu_Level.hpp"
#include "MueLu_Monitor.hpp"
#include "MueLu_Utilities_kokkos.hpp"

namespace MueLu {

  template <class Scalar, class LocalOrdinal, class GlobalOrdinal, class DeviceType>
  RCP<const ParameterList> CoordinatesTransferFactory_kokkos<Scalar,LocalOrdinal,GlobalOrdinal,Kokkos::Compat::KokkosDeviceWrapperNode<DeviceType>>::GetValidParameterList() const {
    RCP<ParameterList> validParamList = rcp(new ParameterList());

    validParamList->set< RCP<const FactoryBase> >("Coordinates",    Teuchos::null, "Factory for coordinates generation");
    validParamList->set< RCP<const FactoryBase> >("Aggregates",     Teuchos::null, "Factory for coordinates generation");
    validParamList->set< RCP<const FactoryBase> >("CoarseMap",      Teuchos::null, "Generating factory of the coarse map");
    validParamList->set< int >                   ("write start",    -1,            "First level at which coordinates should be written to file");
    validParamList->set< int >                   ("write end",      -1,            "Last level at which coordinates should be written to file");

    return validParamList;
  }

  template <class Scalar, class LocalOrdinal, class GlobalOrdinal, class DeviceType>
  void CoordinatesTransferFactory_kokkos<Scalar, LocalOrdinal, GlobalOrdinal, Kokkos::Compat::KokkosDeviceWrapperNode<DeviceType>>::DeclareInput(Level& fineLevel, Level& coarseLevel) const {
    static bool isAvailableCoords = false;

    if (coarseLevel.GetRequestMode() == Level::REQUEST)
      isAvailableCoords = coarseLevel.IsAvailable("Coordinates", this);

    if (isAvailableCoords == false) {
      Input(fineLevel, "Coordinates");
      Input(fineLevel, "Aggregates");
      Input(fineLevel, "CoarseMap");
    }
  }

  template <class Scalar, class LocalOrdinal, class GlobalOrdinal, class DeviceType>
  void CoordinatesTransferFactory_kokkos<Scalar, LocalOrdinal, GlobalOrdinal, Kokkos::Compat::KokkosDeviceWrapperNode<DeviceType>>::Build(Level& fineLevel, Level& coarseLevel) const {
    FactoryMonitor m(*this, "Build", coarseLevel);

    typedef Xpetra::MultiVector<double,LO,GO,NO> doubleMultiVector;

    GetOStream(Runtime0) << "Transferring coordinates" << std::endl;

    if (coarseLevel.IsAvailable("Coordinates", this)) {
      GetOStream(Runtime0) << "Reusing coordinates" << std::endl;
      return;
    }

    auto aggregates = Get<RCP<Aggregates_kokkos> >(fineLevel, "Aggregates");
    auto fineCoords = Get<RCP<doubleMultiVector> >(fineLevel, "Coordinates");
    auto coarseMap  = Get<RCP<const Map> >        (fineLevel, "CoarseMap");

    // coarseMap is being used to set up the domain map of tentative P, and therefore, the row map of Ac
    // Therefore, if we amalgamate coarseMap, logical nodes in the coordinates vector would correspond to
    // logical blocks in the matrix

    ArrayView<const GO> elementAList = coarseMap->getNodeElementList();
    LO                  blkSize      = 1;
    if (rcp_dynamic_cast<const StridedMap>(coarseMap) != Teuchos::null)
      blkSize = rcp_dynamic_cast<const StridedMap>(coarseMap)->getFixedBlockSize();

    GO                  indexBase    = coarseMap->getIndexBase();
    size_t              numElements  = elementAList.size() / blkSize;
    Array<GO>           elementList(numElements);

    // Amalgamate the map
    for (LO i = 0; i < Teuchos::as<LO>(numElements); i++)
      elementList[i] = (elementAList[i*blkSize]-indexBase)/blkSize + indexBase;

    RCP<const Map>         uniqueMap      = fineCoords->getMap();
    RCP<const Map>         coarseCoordMap = MapFactory::Build(coarseMap->lib(), Teuchos::OrdinalTraits<Xpetra::global_size_t>::invalid(), elementList, indexBase, coarseMap->getComm());
    RCP<doubleMultiVector> coarseCoords   = Xpetra::MultiVectorFactory<double,LO,GO,NO>::Build(coarseCoordMap, fineCoords->getNumVectors());

    // Create overlapped fine coordinates to reduce global communication
    RCP<doubleMultiVector> ghostedCoords = fineCoords;
    if (aggregates->AggregatesCrossProcessors()) {
      RCP<const Map>    nonUniqueMap = aggregates->GetMap();
      RCP<const Import> importer     = ImportFactory::Build(uniqueMap, nonUniqueMap);

      ghostedCoords = Xpetra::MultiVectorFactory<double,LO,GO,NO>::Build(nonUniqueMap, fineCoords->getNumVectors());
      ghostedCoords->doImport(*fineCoords, *importer, Xpetra::INSERT);
    }

    // Get some info about aggregates
    int  myPID        = uniqueMap->getComm()->getRank();
    LO   numAggs      = aggregates->GetNumAggregates();
    auto aggSizes     = aggregates->ComputeAggregateSizes(true, true); // FIXME: not a view
    auto vertex2AggID = aggregates->GetVertex2AggId()->template getLocalView<DeviceType>();
    auto procWinner   = aggregates->GetProcWinner()  ->template getLocalView<DeviceType>();

    auto fineCoordsView   = fineCoords  ->template getLocalView<DeviceType>();
    auto coarseCoordsView = coarseCoords->template getLocalView<DeviceType>();

    // Fill in coarse coordinates
    typename AppendTrait<decltype(coarseCoordsView), Kokkos::Atomic>::type coarseCoordsViewAtomic = coarseCoordsView;
    for (size_t j = 0; j < fineCoords->getNumVectors(); j++) {
      Kokkos::parallel_for("CoordinatesTransferF:Build:coord", vertex2AggID.size(), KOKKOS_LAMBDA(const LO lnode) {
        if (procWinner(lnode, 0) == myPID)
          coarseCoordsViewAtomic(vertex2AggID(lnode, 0),j) += fineCoordsView(lnode,j);
      });

      Kokkos::parallel_for("CoordinatesTransferF:Build:agg", numAggs, KOKKOS_LAMBDA(const LO agg) {
        coarseCoordsView(agg,j) /= aggSizes[agg];
      });
    }

    Set<RCP<doubleMultiVector> >(coarseLevel, "Coordinates", coarseCoords);

    const ParameterList& pL = GetParameterList();
    int writeStart = pL.get<int>("write start"), writeEnd = pL.get<int>("write end");
    if (writeStart == 0 && fineLevel.GetLevelID() == 0 && writeStart <= writeEnd) {
      std::string fileName = "coordinates_before_rebalance_level_" + toString(fineLevel.GetLevelID()) + ".m";
      Xpetra::IO<double,LO,GO,NO>::Write(fileName, *fineCoords);
    }
    if (writeStart <= coarseLevel.GetLevelID() && coarseLevel.GetLevelID() <= writeEnd) {
      std::string fileName = "coordinates_before_rebalance_level_" + toString(coarseLevel.GetLevelID()) + ".m";
      Xpetra::IO<double,LO,GO,NO>::Write(fileName,*coarseCoords);
    }
  }

} // namespace MueLu

#endif // MUELU_COORDINATESTRANSFER_FACTORY_KOKKOS_DEF_HPP
