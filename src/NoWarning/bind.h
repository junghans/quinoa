// *****************************************************************************
/*!
  \file      src/NoWarning/bind.h
  \author    J. Bakosi
  \date      Fri 30 Sep 2016 12:42:18 PM MDT
  \copyright 2012-2015, Jozsef Bakosi, 2016, Los Alamos National Security, LLC.
  \brief     Include boost/bind/bind.hpp with turning off specific compiler
             warnings
*/
// *****************************************************************************
#ifndef nowarning_bind_h
#define nowarning_bind_h

#if defined(__clang__)
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
  #pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif

#include <boost/bind/bind.hpp>

#if defined(__clang__)
  #pragma clang diagnostic pop
#endif

#endif // nowarning_bind_h
