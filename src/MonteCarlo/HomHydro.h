//******************************************************************************
/*!
  \file      src/MonteCarlo/HomHydro.h
  \author    J. Bakosi
  \date      Tue 28 Jan 2014 05:04:54 PM MST
  \copyright Copyright 2005-2012, Jozsef Bakosi, All rights reserved.
  \brief     Homogeneous hydrodynamics
  \details   Homogeneous hydrodynamics
*/
//******************************************************************************
#ifndef HomHydro_h
#define HomHydro_h

#include <Physics.h>

namespace quinoa {

//! HomHydro : Physics
class HomHydro : public Physics {

  public:
    //! Constructor
    explicit HomHydro( const Base& base ) : Physics( base ) {
      ErrChk( hydro(), tk::ExceptType::FATAL, "No hydro model specified" );
    }

    //! Destructor
    ~HomHydro() override = default;

    //! Run
    void run() override;

  private:
    //! Don't permit copy constructor
    HomHydro(const HomHydro&) = delete;
    //! Don't permit copy assigment
    HomHydro& operator=(const HomHydro&) = delete;
    //! Don't permit move constructor
    HomHydro(HomHydro&&) = delete;
    //! Don't permit move assigment
    HomHydro& operator=(HomHydro&&) = delete;
    
    //! Advance
    void advance(tk::real dt);

    //! Output joint PDF
    void outJpdf(const tk::real t);
};

} // quinoa::

#endif // HomHydro_h
