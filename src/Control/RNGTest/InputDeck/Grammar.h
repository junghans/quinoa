//******************************************************************************
/*!
  \file      src/Control/RNGTest/InputDeck/Grammar.h
  \author    J. Bakosi
  \date      Mon 11 Nov 2013 09:27:50 AM MST
  \copyright Copyright 2005-2012, Jozsef Bakosi, All rights reserved.
  \brief     Random number generator test suite grammar definition
  \details   Random number generator test suite input deck grammar definition.
  We use the Parsing Expression Grammar Template Library (PEGTL) to create the
  grammar and the associated parser. Credit goes to Colin Hirsch (pegtl@cohi.at)
  for PEGTL. Word of advice: read from the bottom up.
*/
//******************************************************************************
#ifndef RNGTestInputDeckGrammar_h
#define RNGTestInputDeckGrammar_h

#include <Macro.h>
#include <Exception.h>
#include <Option.h>
#include <Grammar.h>
#include <PEGTLParsed.h>
#include <RNGTest/Types.h>
#include <RNGTest/InputDeck/Keywords.h>

namespace rngtest {
namespace deck {

  //! PEGTLParsed type specialized to RNGTest's input deck parser
  using PEGTLInputDeck =
          quinoa::ctr::PEGTLParsed< ctr::InputDeck,
                                    pegtl::file_input< ctr::Location >,
                                    ctr::cmd,
                                    ctr::CmdLine >;

  // RNGTest's InputDeck State

  //! Everything is stored in Stack during parsing
  using Stack = PEGTLInputDeck;

  // RNGTest's InputDeck actions

  //! put option in state at position given by tags
  template< class OptionType, typename... tags >
  struct store_option : pegtl::action_base< store_option<OptionType,tags...> > {
    static void apply( const std::string& value, Stack& stack ) {
      tk::Option< OptionType > opt;
      //! Emit warning on overwrite
      if (stack.get< tags... >() != ctr::InputDeckDefaults.get< tags... >()) {
        std::cout << "\n>>> PARSER WARNING: Multiple definitions for '"
                  << opt.group() << "' option. Overwriting '"
                  << opt.name( stack.get< tags... >() ) << "' with '"
                  << opt.name( opt.value( value ) ) << "'.\n\n";
      }
      stack.set< tags... >( opt.value( value ) );
    }
  };

  // RNGTest's InputDeck grammar

  //! title
  struct title :
         pegtl::ifmust< tk::grm::readkw< tk::kw::title::pegtl_string >,
                        tk::grm::quoted< Stack,
                                         tk::grm::Set<Stack, ctr::title> > > {};

  //! MKL RNG seed
  struct mkl_seed :
         tk::grm::process< Stack,
                           tk::kw::seed::pegtl_string,
                           tk::grm::Insert_field< Stack,
                                                  quinoa::ctr::seed,
                                                  ctr::selected,
                                                  ctr::rng,
                                                  ctr::param,
                                                  ctr::mklrng > > {};

  //! MKL uniform method
  struct mkl_uniform_method :
         tk::grm::process< Stack,
                           tk::kw::uniform_method::pegtl_string,
                           tk::grm::Insert_option<
                             Stack,
                             quinoa::ctr::MKLUniformMethod,
                             quinoa::ctr::uniform_method,
                             ctr::selected,
                             ctr::rng,
                             ctr::param,
                             ctr::mklrng >,
                           pegtl::alpha > {};

  //! MKL Gaussian method
  struct mkl_gaussian_method :
         tk::grm::process< Stack,
                           tk::kw::gaussian_method::pegtl_string,
                           tk::grm::Insert_option<
                             Stack,
                             quinoa::ctr::MKLGaussianMethod,
                             quinoa::ctr::gaussian_method,
                             ctr::selected,
                             ctr::rng,
                             ctr::param,
                             ctr::mklrng >,
                           pegtl::alpha > {};

  //! mklrngs blocks
  struct mklrngs :
         pegtl::ifmust<
           tk::grm::scan< tk::grm::mklrng,
                          tk::grm::Store_back_option< Stack,
                                                      quinoa::ctr::RNG,
                                                      ctr::selected,
                                                      ctr::rng > >,
           tk::grm::block< Stack,
                           mkl_seed,
                           mkl_uniform_method,
                           mkl_gaussian_method > > {};
          
  //! rngs
  struct rngs :
         pegtl::sor< mklrngs > {};

  // TestU01 batteries
  template< typename battery_kw >
  struct testu01 :
         pegtl::ifmust< tk::grm::scan< typename battery_kw::pegtl_string,
                                       store_option< ctr::Battery,
                                                     ctr::selected,
                                                     ctr::battery > >,
                        tk::grm::block< Stack, rngs > > {};

  //! batteries
  struct battery :
         pegtl::sor< testu01< kw::smallcrush >,
                     testu01< kw::crush >,
                     testu01< kw::bigcrush > > {};

  //! main keywords
  struct keywords :
         pegtl::sor< title,
                     battery > {};

  //! ignore: comments and empty lines
  struct ignore :
         pegtl::sor< tk::grm::comment,
                     pegtl::until< pegtl::eol, pegtl::space > > {};

  //! entry point: parse keywords and ignores until eof
  struct read_file :
         tk::grm::read_file< Stack, keywords, ignore > {};

} // deck::
} // rngtest::

#endif // RNGTestInputDeckGrammar_h
