// *****************************************************************************
/*!
  \file      src/NoWarning/rngtest.decl.h
  \author    J. Bakosi
  \date      Wed 04 May 2016 10:36:58 AM MDT
  \copyright 2012-2015, Jozsef Bakosi, 2016, Los Alamos National Security, LLC.
  \brief     Include rngtest.decl.h with turning off specific compiler warnings
*/
// *****************************************************************************
#ifndef nowarning_rngtest_decl_h
#define nowarning_rngtest_decl_h

#if defined(__clang__)
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wunused-parameter"
  #pragma clang diagnostic ignored "-Wextra-semi"
  #pragma clang diagnostic ignored "-Wreserved-id-macro"
  #pragma clang diagnostic ignored "-Wold-style-cast"
  #pragma clang diagnostic ignored "-Wreserved-id-macro"
#elif defined(__GNUC__)
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wredundant-decls"
  #pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#include "../Main/rngtest.decl.h"

#if defined(__clang__)
  #pragma clang diagnostic pop
#elif defined(__GNUC__)
  #pragma GCC diagnostic pop
#endif

#endif // nowarning_rngtest_decl_h
