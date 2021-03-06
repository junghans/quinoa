// *****************************************************************************
/*!
  \file      src/Control/Inciter/InputDeck/Parser.C
  \author    J. Bakosi
  \date      Wed 04 May 2016 10:32:45 AM MDT
  \copyright 2012-2015, Jozsef Bakosi, 2016, Los Alamos National Security, LLC.
  \brief     Inciter's input deck file parser
  \details   This file declares the input deck, i.e., control file, parser for
    the computational shock hydrodynamics tool, Inciter.
*/
// *****************************************************************************

#include <ostream>
#include <type_traits>

#include "NoWarning/pegtl.h"

#include "Print.h"
#include "Tags.h"
#include "Inciter/Types.h"
#include "Inciter/InputDeck/InputDeck.h"
#include "Inciter/InputDeck/Parser.h"
#include "Inciter/InputDeck/Grammar.h"

namespace tk {
namespace grm {

extern tk::Print g_print;

} // grm::
} // tk::

using inciter::InputDeckParser;

InputDeckParser::InputDeckParser( const tk::Print& print,
                                  const ctr::CmdLine& cmdline,
                                  ctr::InputDeck& inputdeck ) :
  FileParser( cmdline.get< tag::io, tag::control >() )
// *****************************************************************************
//  Constructor
//! \param[in] print Pretty printer
//! \param[in] cmdline Command line stack
//! \param[in,out] inputdeck Input deck stack where data is stored during
//!    parsing
//! \author  J. Bakosi
// *****************************************************************************
{
  // Create PEGTL file input from std::string
  pegtl::file_input< ctr::Location > input( m_filename );

  // Create PEGTLInputDeck to store parsed input deck data which derives from
  // InputDeck and has location() used during parse
  deck::PEGTLInputDeck id( input, cmdline );

  // Reset parser's output stream to that of print's. This is so that mild
  // warnings emitted during parsing can be output using the pretty printer.
  // Usually, errors and warnings are simply accumulated during parsing and
  // printed during diagnostics after the parser has finished. Howver, in some
  // special cases we can provide a more user-friendly message right during
  // parsing since there is more information available to construct a more
  // sensible message. This is done in e.g., tk::grm::store_option. Resetting
  // the global g_print, to that of passed in as the constructor argument allows
  // not to have to create a new pretty printer, but use the existing one.
  tk::grm::g_print.reset( print.save() );

  // Parse control file by populating the underlying tagged tuple:
  // basic_parse() below gives debug info during parsing, use it for debugging
  // the parser itself, i.e., when modifying the grammar, otherwise, use
  // dummy_parse() to compile faster
  pegtl::dummy_parse< deck::read_file >( input, id );

  // Echo errors and warnings accumulated during parsing
  diagnostics( print, id.get< tag::error >() );

  // Strip input deck (and its underlying tagged tuple) from PEGTL instruments
  // and transfer it out
  inputdeck = std::move( id );
}
