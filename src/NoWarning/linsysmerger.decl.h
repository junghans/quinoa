// *****************************************************************************
/*!
  \file      src/NoWarning/linsysmerger.decl.h
  \author    J. Bakosi
  \date      Tue 02 Aug 2016 08:18:21 AM MDT
  \copyright 2012-2015, Jozsef Bakosi, 2016, Los Alamos National Security, LLC.
  \brief     Include linsysmerger.decl.h with turning off specific compiler
             warnings
*/
// *****************************************************************************
#ifndef nowarning_linsysmerger_decl_h
#define nowarning_linsysmerger_decl_h

#if defined(__clang__)
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wreserved-id-macro"
  #pragma clang diagnostic ignored "-Wunused-parameter"
  #pragma clang diagnostic ignored "-Wold-style-cast"
  #pragma clang diagnostic ignored "-Wextra-semi"
#elif defined(__GNUC__)
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wshadow"
  #pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#include "../LinSys/linsysmerger.decl.h"

#if defined(__clang__)
  #pragma clang diagnostic pop
#elif defined(__GNUC__)
  #pragma GCC diagnostic pop
#endif

#endif // nowarning_linsysmerger_decl_h
