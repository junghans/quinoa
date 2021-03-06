// *****************************************************************************
/*!
  \file      src/Control/Breeze/Options/Hydro.h
  \author    J. Bakosi
  \date      Mon 01 Jun 2015 02:21:51 PM MDT
  \copyright 2012-2015, Jozsef Bakosi, 2016, Los Alamos National Security, LLC.
  \brief     Hydro model options
  \details   Hydro model options
*/
// *****************************************************************************
#ifndef BreezeHydroOptions_h
#define BreezeHydroOptions_h

#include <boost/mpl/vector.hpp>

#include "Toggle.h"
#include "Keywords.h"
#include "PUPUtil.h"

namespace breeze {
namespace ctr {

//! Hydro model types
//! \author J. Bakosi
enum class HydroType : uint8_t { NO_HYDRO=0,
                                 SLM,
                                 GLM };

//! \brief Pack/Unpack HydroType: forward overload to generic enum class packer
//! \author J. Bakosi
inline void operator|( PUP::er& p, HydroType& e ) { PUP::pup( p, e ); }

//! \brief Hydro model options: outsource searches to base templated on enum
//!   type
//! \author J. Bakosi
class Hydro : public tk::Toggle< HydroType > {

  public:
    //! Valid expected choices to make them also available at compile-time
    //! \author J. Bakosi
    using keywords = boost::mpl::vector< kw::hydro_slm
                                       , kw::hydro_glm
                                       >;

    //! \brief Options constructor
    //! \details Simply initialize in-line and pass associations to base, which
    //!    will handle client interactions
    //! \author J. Bakosi
    explicit Hydro() :
      Toggle< HydroType >(
        //! Group, i.e., options, name
        "Hydrodynamics",
        //! Enums -> names
        { { HydroType::NO_HYDRO, "n/a" },
          { HydroType::SLM, kw::hydro_slm::name() },
          { HydroType::GLM, kw::hydro_glm::name() } },
        //! keywords -> Enums
        { { "no_hydro", HydroType::NO_HYDRO },
          { kw::hydro_slm::string(), HydroType::SLM },
          { kw::hydro_glm::string(), HydroType::GLM } } ) {}
};

} // ctr::
} // breeze::

#endif // BreezeHydroOptions_h
