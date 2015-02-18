//******************************************************************************
/*!
  \file      src/DiffEq/SkewNormal.h
  \author    J. Bakosi
  \date      Fri 13 Feb 2015 02:50:20 PM MST
  \copyright 2012-2015, Jozsef Bakosi.
  \brief     System of skew-normal SDEs
  \details   This file implements the time integration of a system of stochastic
    differential equations (SDEs), whose invariant is the joint [skew-normal
    distribution](http://en.wikipedia.org/wiki/Skew_normal_distribution).

    In a nutshell, the equation integrated governs a set of scalars,
    \f$x_\alpha\f$, \f$\alpha\!=\!1,\dots,N\f$, as
    \f[
       \mathrm{d}x_\alpha(t) = -\frac{1}{T_\alpha}\left[x_\alpha -
       \lambda_\alpha\sigma^2_\alpha\sqrt{\frac{2}{\pi}} \cdot
       \frac{\exp{\left(-\lambda_\alpha^2x^2_\alpha/2\right)}}{1+\mathrm{erf}
       \left( \lambda_\alpha x_\alpha/\sqrt{2}\right)} \right] \mathrm{d}t +
       \sqrt{\frac{2\sigma^2_\alpha}{T_\alpha}}\mathrm{d}W_\alpha(t).
    \f]
    The invariant distribution is the joint skew-normal distribution
    \f[
       p(x_\alpha) = \frac{1}{\sigma_\alpha\sqrt{2\pi}} \exp\left(
       -\frac{x^2_\alpha}{2\sigma^2_\alpha} \right) \left[1 + \mathrm{erf}\left(
       \frac{\lambda_\alpha x_\alpha}{\sqrt{2}}\right) \right].
    \f]
    Here \f$\mathrm{erf}(y) = 2/\sqrt{\pi} \int_0^y \exp(-u^2) \mathrm{d}u\f$,
    \f$T_\alpha\f$ are time scales, \f$\sigma_\alpha\f$ are variance parameters,
    \f$\mathrm{d}W_\alpha(t)\f$ is an isotropic [Wiener
    process](http://en.wikipedia.org/wiki/Wiener_process) with independent
    increments, and \f$\lambda_\alpha\f$ are the parameters that control the
    asymmetry and skewness for variable \f$x_\alpha\f$: \f$\lambda_\alpha<0\f$
    and \f$\lambda_\alpha>0\f$ give negative and positive skewness,
    respectively, while \f$\lambda_\alpha=0\f$ reduces the system to a set of
    independent Ornstein-Uhlenbeck processes. See also
    DiffEq/DiagOrnsteinUhlenbeck.h. For more details on the skew-normal
    distribution, see http://www.jstor.org/stable/2337278.
*/
//******************************************************************************
#ifndef SkewNormal_h
#define SkewNormal_h

#include <cmath>

#include <InitPolicy.h>
#include <SkewNormalCoeffPolicy.h>
#include <RNG.h>

namespace walker {

extern ctr::InputDeck g_inputdeck;
extern std::map< tk::ctr::RawRNGType, tk::RNG > g_rng;

//! \brief Skew-normal SDE used polymorphically with DiffEq
//! \details The template arguments specify policies and are used to configure
//!   the behavior of the class. The policies are:
//!   - Init - initialization policy, see DiffEq/InitPolicy.h
//!   - Coefficients - coefficients policy, see DiffEq/SkewNormalCoeffPolicy.h
template< class Init, class Coefficients >
class SkewNormal {

  public:
    //! \brief Constructor
    //! \param[in] c Index specifying which system of skew-normal SDEs to
    //!   construct. There can be multiple skew-normal ... end blocks in a
    //!   control file. This index specifies which skew-normal SDE system to
    //!   instantiate. The index corresponds to the order in which the
    //!   skew-normal ... end blocks are given the control file.
    //! \author J. Bakosi
    explicit SkewNormal( unsigned int c ) :
      m_depvar(
        g_inputdeck.get< tag::param, tag::skewnormal, tag::depvar >().at(c) ),
      m_ncomp(
        g_inputdeck.get< tag::component >().get< tag::skewnormal >().at(c) ),
      m_offset(
        g_inputdeck.get< tag::component >().offset< tag::skewnormal >(c) ),
      m_rng( g_rng.at( tk::ctr::raw(
        g_inputdeck.get< tag::param, tag::skewnormal, tag::rng >().at(c) ) ) ),
      coeff(
        m_ncomp,
        g_inputdeck.get< tag::param, tag::skewnormal, tag::timescale >().at(c),
        g_inputdeck.get< tag::param, tag::skewnormal, tag::sigmasq >().at(c),
        g_inputdeck.get< tag::param, tag::skewnormal, tag::lambda >().at(c),
        m_T, m_sigmasq, m_lambda ) {}

    //! Initalize SDE, prepare for time integration
    //! \param[inout] particles Array of particle properties 
    //! \param[in] stat Statistics object for accessing moments 
    //! \author J. Bakosi
    void initialize( tk::ParProps& particles, const tk::Statistics& stat ) {
      //! Set initial conditions using initialization policy
      Init( { particles } );
      //! Pre-lookup required statistical moments
      coeff.lookup( stat, m_depvar );
    }

    //! \brief Advance particles according to the system of skew-normal SDEs
    //! \author J. Bakosi
    void advance( tk::ParProps& particles, int stream, tk::real dt ) {
      const auto npar = particles.npar();
      for (auto p=decltype(npar){0}; p<npar; ++p) {
        // Generate Gaussian random numbers with zero mean and unit variance
        tk::real dW[m_ncomp];
        m_rng.gaussian( stream, m_ncomp, dW );

        // Advance all m_ncomp scalars
        for (tk::ctr::ncomp_type i=0; i<m_ncomp; ++i) {
          tk::real& x = particles( p, i, m_offset );
          tk::real d = 2.0 * m_sigmasq[i] / m_T[i] * dt;
          d = (d > 0.0 ? std::sqrt(d) : 0.0);
          x += - ( x - m_lambda[i] * m_sigmasq[i]
                       * std::sqrt( 2.0 / pi() )
                       * std::exp( - m_lambda[i] * m_lambda[i] * x * x / 2.0 )
                       / ( 1.0 + std::erf( m_lambda[i] * x / std::sqrt(2.0) ) )
                   ) / m_T[i] * dt
                 + d*dW[i];
        }
      }
    }

  private:
    const char m_depvar;                //!< Dependent variable
    const tk::ctr::ncomp_type m_ncomp;    //!< Number of components
    const int m_offset;                   //!< Offset SDE operates from
    const tk::RNG& m_rng;                 //!< Random number generator

    //! Coefficients
    std::vector< kw::sde_T::info::expect::type > m_T;
    std::vector< kw::sde_sigmasq::info::expect::type > m_sigmasq;
    std::vector< kw::sde_lambda::info::expect::type > m_lambda;

    //! Coefficients policy
    Coefficients coeff;

    //! Return the value of Pi
    constexpr tk::real pi() const { return std::atan(1.0) * 4.0; }
};

} // walker::

#endif // SkewNormal_h
