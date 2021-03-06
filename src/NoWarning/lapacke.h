// *****************************************************************************
/*!
  \file      src/NoWarning/lapacke.h
  \author    J. Bakosi
  \date      Tue 10 May 2016 02:26:26 PM MDT
  \copyright 2012-2015, Jozsef Bakosi, 2016, Los Alamos National Security, LLC.
  \brief     Include lapacke.h with turning off specific compiler warnings
*/
// *****************************************************************************
#ifndef nowarning_lapacke_h
#define nowarning_lapacke_h

#if defined(__clang__)
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif

#include <lapacke.h>

#if defined(__clang__)
  #pragma clang diagnostic pop
#endif

#endif // nowarning_lapacke_h
