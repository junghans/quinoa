// *****************************************************************************
/*!
  \file      src/Control/Walker/CmdLine/Grammar.h
  \author    J. Bakosi
  \date      Mon 01 Jun 2015 02:17:40 PM MDT
  \copyright 2012-2015, Jozsef Bakosi, 2016, Los Alamos National Security, LLC.
  \brief     Walker's command line grammar definition
  \details   Grammar definition for parsing the command line. We use the Parsing
  Expression Grammar Template Library (PEGTL) to create the grammar and the
  associated parser. Credit goes to Colin Hirsch (pegtl@cohi.at) for PEGTL. Word
  of advice: read from the bottom up.
*/
// *****************************************************************************
#ifndef WalkerCmdLineGrammar_h
#define WalkerCmdLineGrammar_h

#include "CommonGrammar.h"
#include "PEGTLParsed.h"
#include "Keywords.h"

namespace walker {
//! Walker command line grammar definition
namespace cmd {

  //! \brief PEGTLParsed type specialized to Walker's command line parser
  //! \details PEGTLCmdLine is practically CmdLine equipped with PEGTL location
  //!    information so the location can be tracked during parsing.
  //! \author J. Bakosi
  using PEGTLCmdLine =
    tk::ctr::PEGTLParsed< ctr::CmdLine, pegtl::string_input< ctr::Location > >;

  //! \brief Specialization of tk::grm::use for Walker's command line parser
  //! \author J. Bakosi
  template< typename keyword >
  using use = tk::grm::use< keyword, ctr::CmdLine::keywords >;

  // Walker's CmdLine state

  //! Everything is stored in Stack during parsing
  using Stack = PEGTLCmdLine;

  // Walker's CmdLine grammar

  //! verbose (i.e., verbose or quiet output)
  struct verbose :
         tk::grm::process_cmd_switch< Stack,
                                      use< kw::verbose >,
                                      tag::verbose > {};

  //! virtualization parameter
  struct virtualization :
         tk::grm::process_cmd< Stack,
                               use< kw::virtualization >,
                               tk::grm::Store< Stack, tag::virtualization >,
                               tk::grm::number > {};

  //! io parameter
  template< typename keyword, typename io_tag >
  struct io :
         tk::grm::process_cmd< Stack,
                               keyword,
                               tk::grm::Store< Stack, tag::io, io_tag > > {};

  //! help on control file keywords
  struct helpctr :
         tk::grm::process_cmd_switch< Stack,
                                      use< kw::helpctr >,
                                      tag::helpctr > {};

  //! help on command-line parameters
  struct help :
         tk::grm::process_cmd_switch< Stack,
                                      use< kw::help >,
                                      tag::help > {};

  //! help on a command-line keyword
  struct helpkw :
         tk::grm::process_cmd< Stack,
                               use< kw::helpkw >,
                               tk::grm::helpkw< Stack >,
                               pegtl::alnum > {};

  //! command line keywords
  struct keywords :
         pegtl::sor< verbose,
                     help,
                     helpctr,
                     helpkw,
                     virtualization,
                     io< use< kw::control >, tag::control >,
                     io< use< kw::pdf >, tag::pdf >,
                     io< use< kw::stat >, tag::stat > > {};

  //! entry point: parse keywords and until end of string
  struct read_string :
         tk::grm::read_string< Stack, keywords > {};

} // cmd::
} // walker::

#endif // WalkerCmdLineGrammar_h
