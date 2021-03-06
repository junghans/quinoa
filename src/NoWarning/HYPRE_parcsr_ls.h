// *****************************************************************************
/*!
  \file      src/NoWarning/HYPRE_parcsr_ls.h
  \author    J. Bakosi
  \date      Wed 16 Nov 2016 10:19:08 PM MST
  \copyright 2012-2015, Jozsef Bakosi, 2016, Los Alamos National Security, LLC.
  \brief     Include HYPRE_parcsr_ls.h with turning off specific compiler
             warnings
*/
// *****************************************************************************
#ifndef nowarning_HYPRE_parcsr_ls_h
#define nowarning_HYPRE_parcsr_ls_h

#if defined(__clang__)
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wreserved-id-macro"
  #pragma clang diagnostic ignored "-Wold-style-cast"
  #pragma clang diagnostic ignored "-Wcast-align"
  #pragma clang diagnostic ignored "-Wdeprecated"
  #pragma clang diagnostic ignored "-Wdocumentation"
  #pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#elif defined(__GNUC__)
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wcast-qual"
#elif defined(__INTEL_COMPILER)
  #pragma warning( push )
  #pragma warning( disable: 7 )
#endif

#include <HYPRE_parcsr_ls.h>

#if defined(__clang__)
  #pragma clang diagnostic pop
#elif defined(__GNUC__)
  #pragma GCC diagnostic pop
#elif defined(__INTEL_COMPILER)
  #pragma warning( pop )
#endif

#endif // nowarning_HYPRE_parcsr_ls_h
