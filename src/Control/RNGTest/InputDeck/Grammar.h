//******************************************************************************
/*!
  \file      src/Control/RNGTest/InputDeck/Grammar.h
  \author    J. Bakosi
  \date      Sat 19 Oct 2013 08:21:31 AM MDT
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

  using namespace pegtl;
  using namespace tk::grm;

  //! PEGTLParsed type specialized to RNGTest's input deck parser
  using PEGTLInputDeck = quinoa::ctr::PEGTLParsed< ctr::InputDeck,
                                                   file_input< ctr::Location >,
                                                   ctr::cmd,
                                                   ctr::CmdLine >;

  // RNGTest's InputDeck State

  //! Everything is stored in Stack during parsing
  using Stack = PEGTLInputDeck;

  // RNGTest's InputDeck actions

  //! convert and put option in state at position given by tags
  template< class OptionType, typename... tags >
  struct store_option : action_base< store_option<OptionType, tags...> > {
    static void apply(const std::string& value, Stack& stack) {
      tk::Option<OptionType> opt;
      //! Emit warning on overwrite
      if (stack.get<tags...>() != ctr::InputDeckDefaults.get<tags...>()) {
        std::cout << "\n>>> PARSER WARNING: Multiple definitions for '"
                  << opt.group() << "' option. Overwriting '"
                  << opt.name(stack.get<tags...>()) << "' with '"
                  << opt.name(opt.value(value)) << "'.\n\n";
      }
      stack.set<tags...>(opt.value(value));
    }
  };

  // RNGTest's InputDeck grammar

  //! rng: one of the random number generators
  struct rng :
         sor< quinoa::kw::mkl_mcg31::pegtl_string,
              quinoa::kw::mkl_r250::pegtl_string,
              quinoa::kw::mkl_mrg32k3a::pegtl_string,
              quinoa::kw::mkl_mcg59::pegtl_string,
              quinoa::kw::mkl_wh::pegtl_string,
              quinoa::kw::mkl_mt19937::pegtl_string,
              quinoa::kw::mkl_mt2203::pegtl_string,
              quinoa::kw::mkl_sfmt19937::pegtl_string,
              quinoa::kw::mkl_sobol::pegtl_string,
              quinoa::kw::mkl_niederr::pegtl_string,
              quinoa::kw::mkl_iabstract::pegtl_string,
              quinoa::kw::mkl_dabstract::pegtl_string,
              quinoa::kw::mkl_sabstract::pegtl_string,
              quinoa::kw::mkl_nondeterm::pegtl_string > {};

  // common to all RNG test suites
  struct rngtest_common :
         tk::grm::vector< Stack,
                            kw::end::pegtl_string,
                            kw::rngs::pegtl_string,
                            Store_back<Stack,ctr::generator> > {};

  //! title
  struct title :
         ifmust< readkw<kw::title::pegtl_string>,
                 quoted<Stack,Set<Stack,ctr::title>> > {};

  // smallcrush block
  struct smallcrush :
         ifmust< tk::grm::scan<kw::smallcrush::pegtl_string,
                               store_option<ctr::Battery,
                                            ctr::selected,
                                            ctr::battery>>,
                 rngtest_common > {};

  // crush block
  struct crush :
         ifmust< tk::grm::scan<kw::crush::pegtl_string,
                               store_option<ctr::Battery,
                                            ctr::selected,
                                            ctr::battery>>,
                 rngtest_common > {};

  // bigcrush block
  struct bigcrush :
         ifmust< tk::grm::scan<kw::bigcrush::pegtl_string,
                               store_option<ctr::Battery,
                                            ctr::selected,
                                            ctr::battery>>,
                 rngtest_common > {};

  //! batteries
  struct battery :
         sor< smallcrush,
              crush,
              bigcrush > {};

  //! main keywords
  struct keywords :
         sor< title/*,
              battery*/ > {};

  //! ignore: comments and empty lines
  struct ignore :
         sor< comment, until<eol, space> > {};

  //! entry point: parse keywords and ignores until eof
  struct read_file :
         until< eof, sor<keywords, ignore, unknown<Stack,Error::KEYWORD>> > {};

} // deck::
} // rngtest::

#endif // RNGTestInputDeckGrammar_h
