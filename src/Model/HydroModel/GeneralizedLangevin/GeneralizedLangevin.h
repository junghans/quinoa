//******************************************************************************
/*!
  \file      src/Model/HydroModel/GeneralizedLangevin/GeneralizedLangevin.h
  \author    J. Bakosi
  \date      Mon 21 Jan 2013 10:48:47 AM MST
  \copyright Copyright 2005-2012, Jozsef Bakosi, All rights reserved.
  \brief     Generalized Langevin hydrodynamics model
  \details   Generalized Langevin hydrodynamics model
*/
//******************************************************************************
#ifndef GeneralizedLangevin_h
#define GeneralizedLangevin_h

#include <QuinoaTypes.h>
#include <HydroModel.h>

namespace Quinoa {

//! GeneralizedLangevin : HydroModel
class GeneralizedLangevin : public HydroModel {

  public:
    //! Constructor
    GeneralizedLangevin();

    //! Destructor
    virtual ~GeneralizedLangevin() {}

    //! Echo information on the generalized Langevin model
    virtual void echo();

  private:
    //! Don't permit copy constructor
    GeneralizedLangevin(const GeneralizedLangevin&) = delete;
    //! Don't permit copy assigment
    GeneralizedLangevin& operator=(const GeneralizedLangevin&) = delete;
    //! Don't permit move constructor
    GeneralizedLangevin(GeneralizedLangevin&&) = delete;
    //! Don't permit move assigment
    GeneralizedLangevin& operator=(GeneralizedLangevin&&) = delete;
};

} // namespace Quinoa

#endif // GeneralizedLangevin_h
