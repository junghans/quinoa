// *****************************************************************************
/*!
  \file      src/NoWarning/beta_distribution.h
  \author    J. Bakosi
  \date      Thu 06 Oct 2016 10:01:46 AM MDT
  \copyright 2012-2015, Jozsef Bakosi, 2016, Los Alamos National Security, LLC.
  \brief     Include boost/random/beta_distribution.hpp with turning off
             specific compiler warnings
*/
// *****************************************************************************
#ifndef nowarning_beta_distribution_h
#define nowarning_beta_distribution_h

#if defined(__clang__)
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
  #pragma clang diagnostic ignored "-Wfloat-equal"
  #pragma clang diagnostic ignored "-Wreserved-id-macro"
#elif defined(__GNUC__)
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wfloat-equal"
#endif

#include <boost/random/beta_distribution.hpp>

#if defined(__clang__)
  #pragma clang diagnostic pop
#elif defined(__GNUC__)
  #pragma GCC diagnostic pop
#endif

#endif // nowarning_beta_distribution_h
