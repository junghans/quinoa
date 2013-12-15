//******************************************************************************
/*!
  \file      src/Control/RNGSSEGrammar.h
  \author    J. Bakosi
  \date      Sat 14 Dec 2013 03:39:07 PM MST
  \copyright Copyright 2005-2012, Jozsef Bakosi, All rights reserved.
  \brief     RNGSSE grammar
  \details   RNGSSE grammar
*/
//******************************************************************************
#ifndef RNGSSEGrammar_h
#define RNGSSEGrammar_h

namespace tk {
//! RNGSSE grammar definition: state, actions, grammar
namespace rngsse {

  //! rng: match any one of the RNGSSE random number generators
  struct rng :
         pegtl::sor< kw::rngsse_gm19::pegtl_string,
                     kw::rngsse_gm29::pegtl_string,
                     kw::rngsse_gm31::pegtl_string,
                     kw::rngsse_gm55::pegtl_string,
                     kw::rngsse_gm61::pegtl_string,
                     kw::rngsse_gq581::pegtl_string,
                     kw::rngsse_gq583::pegtl_string,
                     kw::rngsse_gq584::pegtl_string,
                     kw::rngsse_mt19937::pegtl_string,
                     kw::rngsse_lfsr113::pegtl_string,
                     kw::rngsse_mrg32k3a::pegtl_string > {};

  //! insert RNGSSE parameter
  template< typename Stack, typename keyword, typename option, typename field,
            typename sel, typename vec, typename... tags >
  struct rng_option :
         tk::grm::process< Stack,
                           typename keyword::pegtl_string,
                           tk::grm::insert_option< Stack,
                                                   option,
                                                   field,
                                                   sel, vec, tags... >,
                           pegtl::alpha > {};

  //! RNGSSE seed
  template< typename Stack, typename sel, typename vec, typename... tags >
  struct seed :
         tk::grm::process< Stack,
                           tk::kw::seed::pegtl_string,
                           tk::grm::Insert_field< Stack,
                                                  quinoa::ctr::seed,
                                                  sel, vec, tags... > > {};

  //! rngs blocks
  template< typename Stack, typename sel, typename vec, typename... tags >
  struct rngs :
         pegtl::ifmust<
           tk::grm::scan< rng,
                          tk::grm::store_back_option< Stack,
                                                      quinoa::ctr::RNG,
                                                      sel, vec > >,
           tk::grm::block< Stack,
                           seed< Stack, sel, vec, tags... > > > {};

} // rngsse::
} // tk::

#endif // RNGSSEGrammar_h
