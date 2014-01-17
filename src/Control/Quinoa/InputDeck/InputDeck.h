//******************************************************************************
/*!
  \file      src/Control/Quinoa/InputDeck/InputDeck.h
  \author    J. Bakosi
  \date      Thu 16 Jan 2014 09:45:22 PM MST
  \copyright Copyright 2005-2012, Jozsef Bakosi, All rights reserved.
  \brief     Quinoa's input deck
  \details   Quinoa's input deck
*/
//******************************************************************************
#ifndef QuinoaInputDeck_h
#define QuinoaInputDeck_h

#include <limits>

#include <Control.h>
#include <Option.h>
#include <Quinoa/CmdLine/CmdLine.h>

namespace quinoa {
namespace ctr {

//! InputDeck : Control< specialized to Quinoa >, see Types.h,
class InputDeck :
  public tk::Control< // tag           type
                      tag::title,      std::string,
                      tag::selected,   selects,
                      tag::incpar,     incpars,
                      tag::component,  components,
                      tag::interval,   intervals,
                      tag::cmd,        CmdLine,
                      tag::param,      parameters,
                      tag::stat,       std::vector< Product > > {

  public:
    //! Constructor: set all defaults
    InputDeck() {
      // Default title
      set< tag::title >( "" );
      // Default selections
      set< tag::selected, tag::geometry >( GeometryType::NO_GEOMETRY );
      set< tag::selected, tag::physics >( PhysicsType::NO_PHYSICS );
      set< tag::selected, tag::position >( PositionType::NO_POSITION );
      set< tag::selected, tag::mass >( MassType::NO_MASS );
      set< tag::selected, tag::hydro >( HydroType::NO_HYDRO );
      set< tag::selected, tag::energy >( EnergyType::NO_ENERGY );
      set< tag::selected, tag::mix >( MixType::NO_MIX );
      set< tag::selected, tag::frequency >( FrequencyType::NO_FREQUENCY );
      set< tag::selected, tag::mixrate >( MixRateType::NO_MIXRATE );
      // Default time incrementation parameters
      set< tag::incpar, tag::nstep >( std::numeric_limits< uint64_t >::max() );
      set< tag::incpar, tag::term >( 1.0 );
      set< tag::incpar, tag::dt >( 0.5 );
      // Default number of components
      set< tag::component, tag::nposition >( 0 );
      set< tag::component, tag::ndensity >( 0 );
      set< tag::component, tag::nvelocity >( 0 );
      set< tag::component, tag::nscalar >( 0 );
      set< tag::component, tag::nfrequency >( 0 );
      set< tag::component, tag::npar >( 1 );
      // Default intervals
      set< tag::interval, tag::tty >( 1 );
      set< tag::interval, tag::dump >( 1 );
      set< tag::interval, tag::plot >( 1 );
      set< tag::interval, tag::pdf >( 1 );
      set< tag::interval, tag::glob >( 1 );
      // Default beta mass model parameters
      set< tag::param, tag::beta, tag::atwood >( 0.5 );
      // Default Dirichlet mix model parameters
      set< tag::param, tag::dirichlet, tag::b >( std::vector< tk::real >() );
      set< tag::param, tag::dirichlet, tag::S >( std::vector< tk::real >() );
      set< tag::param, tag::dirichlet, tag::kappa >( std::vector< tk::real >() );
      // Default generalized Dirichlet mix model parameters
      set< tag::param, tag::gendirichlet, tag::b >( std::vector< tk::real >() );
      set< tag::param, tag::gendirichlet, tag::S >( std::vector< tk::real >() );
      set< tag::param, tag::gendirichlet, tag::kappa >( std::vector< tk::real >() );
      set< tag::param, tag::gendirichlet, tag::c >( std::vector< tk::real >() );
      // Default gamma mix model parameters
      set< tag::param, tag::gamma, tag::c1 >( 0.5 );
      set< tag::param, tag::gamma, tag::c2 >( 0.73 );
      set< tag::param, tag::gamma, tag::c3 >( 5.0 );
      set< tag::param, tag::gamma, tag::c4 >( 0.25 );
      // Default simplified Langevin hydro model parameters
      set< tag::param, tag::slm, tag::c0 >( 2.1 );
      // Default generalized Langevin hydro model parameters
      set< tag::param, tag::slm, tag::c0 >( 2.1 );
      // Default requested statistics
      set< tag::stat >( std::vector< Product >() );
    }

    //! Return total number of particle properties
    uint32_t nprop() const noexcept {
      return get< tag::component, tag::nposition >() +
             get< tag::component, tag::ndensity >() +
             get< tag::component, tag::nvelocity> () +
             get< tag::component, tag::nscalar >() +
             get< tag::component, tag::nfrequency >();
    }

    //! Return position offset
    int positionOffset() const noexcept {
      return 0;
    }
    //! Return density offset
    int densityOffset() const noexcept {
      return get< tag::component, tag::nposition >();
    }
    //! Return velocity offset
    int velocityOffset() const noexcept {
      return get< tag::component, tag::nposition >() +
             get< tag::component, tag::ndensity >();
    }
    //! Return scalar offset
    int scalarOffset() const noexcept {
      return get< tag::component, tag::nposition >() +
             get< tag::component, tag::ndensity >() +
             get< tag::component, tag::nvelocity >();
    }

    //! Return offset for term::quantity
    int termOffset(Quantity q) const noexcept {
      int offset = 0;
      if (q == Quantity::SCALAR)
        offset += get< tag::component, tag::nvelocity >();
      if (q == Quantity::VELOCITY_Z)
        offset += get< tag::component, tag::nvelocity >();
      if (q == Quantity::VELOCITY_Y)
        offset += get< tag::component, tag::nvelocity >();
      if (q == Quantity::VELOCITY_X)
        offset += get< tag::component, tag::ndensity >();
      if (q == Quantity::DENSITY)
        offset += 3 * get< tag::component, tag::nposition >();
      return offset;
    }

  private:
    //! Don't permit copy constructor
    InputDeck(const InputDeck&) = delete;
    //! Don't permit copy assigment
    InputDeck& operator=(const InputDeck&) = delete;
    //! Don't permit move constructor
    InputDeck(InputDeck&&) = delete;
    //! Don't permit move assigment
    InputDeck& operator=(InputDeck&&) = delete;
};

//! InputDeck defaults
static const InputDeck InputDeckDefaults;

} // ctr::
} // quinoa::

#endif // QuinoaInputDeck_h
