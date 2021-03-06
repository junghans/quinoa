// @HEADER
//
// ***********************************************************************
//
//   Zoltan2: A package of combinatorial algorithms for scientific computing
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
// Questions? Contact Karen Devine      (kddevin@sandia.gov)
//                    Erik Boman        (egboman@sandia.gov)
//                    Siva Rajamanickam (srajama@sandia.gov)
//
// ***********************************************************************
//
// @HEADER

/*! \file Zoltan2_XpetraRowMatrixAdapter.hpp
    \brief Defines the XpetraRowMatrixAdapter class.
*/

#ifndef _ZOLTAN2_XPETRAROWMATRIXADAPTER_HPP_
#define _ZOLTAN2_XPETRAROWMATRIXADAPTER_HPP_

#include <Zoltan2_MatrixAdapter.hpp>
#include <Zoltan2_StridedData.hpp>
#include <Zoltan2_XpetraTraits.hpp>
#include <Zoltan2_PartitioningHelpers.hpp>

#include <Xpetra_RowMatrix.hpp>

namespace Zoltan2 {

//////////////////////////////////////////////////////////////////////////////
/*!  \brief Provides access for Zoltan2 to Xpetra::RowMatrix data.

    \todo we assume FillComplete has been called.  We should support
                objects that are not FillCompleted.
    \todo add RowMatrix

    The \c scalar_t type, representing use data such as matrix values, is
    used by Zoltan2 for weights, coordinates, part sizes and
    quality metrics.
    Some User types (like Tpetra::RowMatrix) have an inherent scalar type,
    and some
    (like Tpetra::RowGraph) do not.  For such objects, the scalar type is
    set by Zoltan2 to \c float.  If you wish to change it to double, set
    the second template parameter to \c double.

*/

template <typename User, typename UserCoord=User>
  class XpetraRowMatrixAdapter : public MatrixAdapter<User,UserCoord> {
public:

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  typedef typename InputTraits<User>::scalar_t scalar_t;
  typedef typename InputTraits<User>::lno_t    lno_t;
  typedef typename InputTraits<User>::gno_t    gno_t;
  typedef typename InputTraits<User>::part_t   part_t;
  typedef typename InputTraits<User>::node_t   node_t;
  typedef Xpetra::RowMatrix<scalar_t, lno_t, gno_t, node_t> xmatrix_t;
  typedef MatrixAdapter<User,UserCoord> base_adapter_t;
  typedef User user_t;
  typedef UserCoord userCoord_t;
#endif

  /*! \brief Destructor
   */
  ~XpetraRowMatrixAdapter() { }

  /*! \brief Constructor   
   *    \param inmatrix The users Epetra, Tpetra, or Xpetra RowMatrix object 
   *    \param numWeightsPerRow If row weights will be provided in setRowWeights(),
   *        the set \c numWeightsPerRow to the number of weights per row.
   */
  XpetraRowMatrixAdapter(const RCP<const User> &inmatrix,
                         int numWeightsPerRow=0);

  /*! \brief Specify a weight for each entity of the primaryEntityType.
   *    \param weightVal A pointer to the weights for this index.
   *    \stride          A stride to be used in reading the values.  The
   *        index \c idx weight for entity \k should be found at
   *        <tt>weightVal[k*stride]</tt>.
   *    \param idx  A value between zero and one less that the \c numWeightsPerRow 
   *                  argument to the constructor.
   *
   * The order of weights should correspond to the order of the primary 
   * entity type; see, e.g.,  setRowWeights below.
   */

  void setWeights(const scalar_t *weightVal, int stride, int idx = 0);

  /*! \brief Specify a weight for each row.
   *    \param weightVal A pointer to the weights for this index.
   *    \stride          A stride to be used in reading the values.  The
   *        index \c idx weight for row \k should be found at
   *        <tt>weightVal[k*stride]</tt>.
   *    \param idx  A value between zero and one less that the \c numWeightsPerRow 
   *                  argument to the constructor.
   *
   * The order of weights should correspond to the order of rows
   * returned by
   *   \code
   *       theMatrix->getRowMap()->getNodeElementList();
   *   \endcode
   */

  void setRowWeights(const scalar_t *weightVal, int stride, int idx = 0);

  /*! \brief Specify an index for which the weight should be
              the degree of the entity
   *    \param idx Zoltan2 will use the entity's 
   *         degree as the entity weight for index \c idx.
   */
  void setWeightIsDegree(int idx);

  /*! \brief Specify an index for which the row weight should be
              the global number of nonzeros in the row
   *    \param idx Zoltan2 will use the global number of nonzeros in a row
   *         as the row weight for index \c idx.
   */
  void setRowWeightIsNumberOfNonZeros(int idx);

  ////////////////////////////////////////////////////
  // The MatrixAdapter interface.
  ////////////////////////////////////////////////////

  size_t getLocalNumRows() const { 
    return matrix_->getNodeNumRows();
  }

  size_t getLocalNumColumns() const { 
    return matrix_->getNodeNumCols();
  }

  size_t getLocalNumEntries() const {
    return matrix_->getNodeNumEntries();
  }

  bool CRSViewAvailable() const { return true; }

  void getRowIDsView(const gno_t *&rowIds) const 
  {
    ArrayView<const gno_t> rowView = rowMap_->getNodeElementList();
    rowIds = rowView.getRawPtr();
  }

  void getCRSView(const lno_t *&offsets, const gno_t *&colIds) const
  {
    offsets = offset_.getRawPtr();
    colIds = columnIds_.getRawPtr();
  }

  void getCRSView(const lno_t *&offsets, const gno_t *&colIds,
                    const scalar_t *&values) const
  {
    offsets = offset_.getRawPtr();
    colIds = columnIds_.getRawPtr();
    values = values_.getRawPtr();
  }


  int getNumWeightsPerRow() const { return nWeightsPerRow_; }

  void getRowWeightsView(const scalar_t *&weights, int &stride,
                           int idx = 0) const
  {
    env_->localInputAssertion(__FILE__, __LINE__,
      "invalid weight index",
      idx >= 0 && idx < nWeightsPerRow_, BASIC_ASSERTION);
    size_t length;
    rowWeights_[idx].getStridedList(length, weights, stride);
  }

  bool useNumNonzerosAsRowWeight(int idx) const { return numNzWeight_[idx];}

  template <typename Adapter>
    void applyPartitioningSolution(const User &in, User *&out,
         const PartitioningSolution<Adapter> &solution) const;

  template <typename Adapter>
    void applyPartitioningSolution(const User &in, RCP<User> &out,
         const PartitioningSolution<Adapter> &solution) const;

private:

  RCP<Environment> env_;    // for error messages, etc.

  RCP<const User> inmatrix_;
  RCP<const xmatrix_t> matrix_;
  RCP<const Xpetra::Map<lno_t, gno_t, node_t> > rowMap_;
  RCP<const Xpetra::Map<lno_t, gno_t, node_t> > colMap_;
  lno_t base_;
  ArrayRCP<lno_t> offset_;
  ArrayRCP<gno_t> columnIds_;  // TODO:  KDD Is it necessary to copy and store
  ArrayRCP<scalar_t> values_;  // TODO:  the matrix here?  Would prefer views.

  int nWeightsPerRow_;
  ArrayRCP<StridedData<lno_t, scalar_t> > rowWeights_;
  ArrayRCP<bool> numNzWeight_;

  bool mayHaveDiagonalEntries;
};

/////////////////////////////////////////////////////////////////
// Definitions
/////////////////////////////////////////////////////////////////

template <typename User, typename UserCoord>
  XpetraRowMatrixAdapter<User,UserCoord>::XpetraRowMatrixAdapter(
    const RCP<const User> &inmatrix, int nWeightsPerRow):
      env_(rcp(new Environment)),
      inmatrix_(inmatrix), matrix_(), rowMap_(), colMap_(), base_(),
      offset_(), columnIds_(),
      nWeightsPerRow_(nWeightsPerRow), rowWeights_(), numNzWeight_(),
      mayHaveDiagonalEntries(true)
{
  typedef StridedData<lno_t,scalar_t> input_t;
  matrix_ = rcp_const_cast<const xmatrix_t>(
           XpetraTraits<User>::convertToXpetra(rcp_const_cast<User>(inmatrix)));
  rowMap_ = matrix_->getRowMap();
  colMap_ = matrix_->getColMap();
  base_ = rowMap_->getIndexBase();

  size_t nrows = matrix_->getNodeNumRows();
  size_t nnz = matrix_->getNodeNumEntries();
  size_t maxnumentries = 
         matrix_->getNodeMaxNumRowEntries(); // Diff from CrsMatrix
 
  offset_.resize(nrows+1, 0);
  columnIds_.resize(nnz);
  values_.resize(nnz);
  ArrayRCP<lno_t> indices(maxnumentries); // Diff from CrsMatrix
  ArrayRCP<scalar_t> nzs(maxnumentries);  // Diff from CrsMatrix
  lno_t next = 0;
  for (size_t i=0; i < nrows; i++){
    lno_t row = i + base_;
    matrix_->getLocalRowCopy(row, indices(), nzs(), nnz); // Diff from CrsMatrix
    for (size_t j=0; j < nnz; j++){
      values_[next] = nzs[j];
      // TODO - this will be slow
      //   Is it possible that global columns ids might be stored in order?
      columnIds_[next++] = colMap_->getGlobalElement(indices[j]);
    }
    offset_[i+1] = offset_[i] + nnz;
  } 

  if (nWeightsPerRow_ > 0){
    rowWeights_ = arcp(new input_t [nWeightsPerRow_], 0, nWeightsPerRow_, true);
    numNzWeight_ = arcp(new bool [nWeightsPerRow_], 0, nWeightsPerRow_, true);
    for (int i=0; i < nWeightsPerRow_; i++)
      numNzWeight_[i] = false;
  }
}

////////////////////////////////////////////////////////////////////////////
template <typename User, typename UserCoord>
  void XpetraRowMatrixAdapter<User,UserCoord>::setWeights(
    const scalar_t *weightVal, int stride, int idx)
{
  if (this->getPrimaryEntityType() == MATRIX_ROW)
    setRowWeights(weightVal, stride, idx);
  else {
    // TODO:  Need to allow weights for columns and/or nonzeros
    std::ostringstream emsg;
    emsg << __FILE__ << "," << __LINE__
         << " error:  setWeights not yet supported for"
         << " columns or nonzeros."
         << std::endl;
    throw std::runtime_error(emsg.str());
  }
}

////////////////////////////////////////////////////////////////////////////
template <typename User, typename UserCoord>
  void XpetraRowMatrixAdapter<User,UserCoord>::setRowWeights(
    const scalar_t *weightVal, int stride, int idx)
{
  typedef StridedData<lno_t,scalar_t> input_t;
  env_->localInputAssertion(__FILE__, __LINE__,
    "invalid row weight index",
    idx >= 0 && idx < nWeightsPerRow_, BASIC_ASSERTION);
  size_t nvtx = getLocalNumRows();
  ArrayRCP<const scalar_t> weightV(weightVal, 0, nvtx*stride, false);
  rowWeights_[idx] = input_t(weightV, stride);
}

////////////////////////////////////////////////////////////////////////////
template <typename User, typename UserCoord>
  void XpetraRowMatrixAdapter<User,UserCoord>::setWeightIsDegree(
    int idx)
{
  if (this->getPrimaryEntityType() == MATRIX_ROW)
    setRowWeightIsNumberOfNonZeros(idx);
  else {
    // TODO:  Need to allow weights for columns and/or nonzeros
    std::ostringstream emsg;
    emsg << __FILE__ << "," << __LINE__
         << " error:  setWeightIsNumberOfNonZeros not yet supported for"
         << " columns" << std::endl;
    throw std::runtime_error(emsg.str());
  }
}

////////////////////////////////////////////////////////////////////////////
template <typename User, typename UserCoord>
  void XpetraRowMatrixAdapter<User,UserCoord>::setRowWeightIsNumberOfNonZeros(
    int idx)
{
  env_->localInputAssertion(__FILE__, __LINE__,
    "invalid row weight index",
    idx >= 0 && idx < nWeightsPerRow_, BASIC_ASSERTION);

  numNzWeight_[idx] = true;
}

////////////////////////////////////////////////////////////////////////////
template <typename User, typename UserCoord>
  template <typename Adapter>
    void XpetraRowMatrixAdapter<User,UserCoord>::applyPartitioningSolution(
      const User &in, User *&out, 
      const PartitioningSolution<Adapter> &solution) const
{ 
  // Get an import list (rows to be received)
  size_t numNewRows;
  ArrayRCP<gno_t> importList;
  try{
    numNewRows = Zoltan2::getImportList<Adapter,
                                        XpetraRowMatrixAdapter<User,UserCoord> >
                                       (solution, this, importList);
  }
  Z2_FORWARD_EXCEPTIONS;

  // Move the rows, creating a new matrix.
  RCP<User> outPtr = XpetraTraits<User>::doMigration(in, numNewRows,
                                                     importList.getRawPtr());
  out = outPtr.get();
  outPtr.release();
}

////////////////////////////////////////////////////////////////////////////
template <typename User, typename UserCoord>
  template <typename Adapter>
    void XpetraRowMatrixAdapter<User,UserCoord>::applyPartitioningSolution(
      const User &in, RCP<User> &out, 
      const PartitioningSolution<Adapter> &solution) const
{ 
  // Get an import list (rows to be received)
  size_t numNewRows;
  ArrayRCP<gno_t> importList;
  try{
    numNewRows = Zoltan2::getImportList<Adapter,
                                        XpetraRowMatrixAdapter<User,UserCoord> >
                                       (solution, this, importList);
  }
  Z2_FORWARD_EXCEPTIONS;

  // Move the rows, creating a new matrix.
  out = XpetraTraits<User>::doMigration(in, numNewRows,
                                        importList.getRawPtr());
}

}  //namespace Zoltan2
  
#endif
