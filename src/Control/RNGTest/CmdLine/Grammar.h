// *****************************************************************************
/*!
  \file      src/Control/RNGTest/CmdLine/Grammar.h
  \author    J. Bakosi
  \date      Mon 01 Jun 2015 02:15:56 PM MDT
  \copyright 2012-2015, Jozsef Bakosi, 2016, Los Alamos National Security, LLC.
  \brief     RNGTest's command line grammar definition
  \details   Grammar definition for parsing the command line. We use the Parsing
  Expression Grammar Template Library (PEGTL) to create the grammar and the
  associated parser. Credit goes to Colin Hirsch (pegtl@cohi.at) for PEGTL. Word
  of advice: read from the bottom up.
*/
// *****************************************************************************
#ifndef RNGTestCmdLineGrammar_h
#define RNGTestCmdLineGrammar_h

#include "CommonGrammar.h"
#include "PEGTLParsed.h"
#include "Keywords.h"

namespace rngtest {
//! RNGTest command line grammar definition
namespace cmd {

  //! PEGTLParsed type specialized to RNGTest's command line parser
  //! \details PEGTLCmdLine is practically CmdLine equipped with PEGTL location
  //!    information so the location can be tracked during parsing.
  //! \author J. Bakosi
  using PEGTLCmdLine =
    tk::ctr::PEGTLParsed< ctr::CmdLine, pegtl::string_input< ctr::Location > >;

  //! \brief Specialization of tk::grm::use for RNGTest's command line parser
  //! \author J. Bakosi
  template< typename keyword >
  using use = tk::grm::use< keyword, ctr::CmdLine::keywords >;

  // RNGTest's CmdLine state

  //! Everything is stored in Stack during parsing
  //! \author J. Bakosi
  using Stack = PEGTLCmdLine;

  // RNGTest's CmdLine grammar

  //! \brief Match and set verbose switch (i.e., verbose or quiet output)
  //! \author J. Bakosi
  struct verbose :
         tk::grm::process_cmd_switch< Stack,
                                      use< kw::verbose >,
                                      tag::verbose > {};

  //! \brief Match and set control (i.e., input deck) file name
  //! \author J. Bakosi
  struct control :
         tk::grm::process_cmd< Stack,
                               use< kw::control >,
                               tk::grm::Store< Stack,
                                               tag::io,
                                               tag::control > > {};

  //! \brief Match help on control file keywords
  //! \author J. Bakosi
  struct helpctr :
         tk::grm::process_cmd_switch< Stack,
                                      use< kw::helpctr >,
                                      tag::helpctr > {};

  //! \brief Match help on command-line parameters
  //! \author J. Bakosi
  struct help :
         tk::grm::process_cmd_switch< Stack,
                                      use< kw::help >,
                                      tag::help > {};

  //! \brief Match help on a command-line keyword
  //! \author J. Bakosi
  struct helpkw :
         tk::grm::process_cmd< Stack,
                               use< kw::helpkw >,
                               tk::grm::helpkw< Stack >,
                               pegtl::alnum > {};

  //! Match all command line keywords
  //! \author J. Bakosi
  struct keywords :
         pegtl::sor< verbose, control, help, helpctr, helpkw > {};

  //! \brief Grammar entry point: parse keywords until end of string
  //! \author J. Bakosi
  struct read_string :
         tk::grm::read_string< Stack, keywords > {};

} // cmd::
} // rngtest::

#endif // RNGTestCmdLineGrammar_h
