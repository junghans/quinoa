// *****************************************************************************
/*!
  \file      src/Control/Keyword.h
  \author    J. Bakosi
  \date      Mon 09 May 2016 03:53:57 PM MDT
  \copyright 2012-2015, Jozsef Bakosi, 2016, Los Alamos National Security, LLC.
  \brief     Generic definition of a keyword
  \details   Generic definition of all keywords - both command-line arguments
    and control file keywords.
*/
// *****************************************************************************
#ifndef Keyword_h
#define Keyword_h

#include "NoWarning/optional.h"

#include "NoWarning/pegtl.h"

#include "Has.h"

namespace kw {

//! \brief Keyword alias helper
//! \details This struct is used to define both a type and a value for a keyword
//!   alias, which is a single character. Used for command-line arguments, e.g.,
//!   --help, -h, where 'h' is the alias for keyword 'help'.
//! \see Control/Keywords.h
//! \author J. Bakosi
template< int Char >
struct Alias {
  using type = pegtl::one< Char >;
  static const int value = Char;
};

//! \brief Keyword code helper
//! \details This struct is used to define both a type and a value for a keyword
//!   code, which is a single character. Used for printing out policy code.
//! \see Control/Keywords.h
//! \author J. Bakosi
template< int Char >
struct Code {
  using type = pegtl::one< Char >;
  static const int value = Char;
};

//! \brief Generic definition of a keyword
//! \details A keyword is a struct that collects the information that makes up a
//!    keyword. The requirement on the first template argument, Info, is that it
//!    must define the name(), shortDescription(,) and longDescription() member
//!    functions returning compile-time (static) std::strings. The
//!    shortDescription() member function is used to return a short description
//!    of what the keyword is used for, while the longDescription() member
//!    function is used for a longer, e.g., a paragraph-long, description on
//!    what the keyword can be used for and how it can and should be used. The
//!    last template parameters are a list of character constants, specifying
//!    the case-sensitive characters that make up the keyword, which is then
//!    matched by the parser. The keyword must be at least one character long,
//!    but otherwise its length is only limited by the compiler's recursion
//!    handling capability of variadic templates. While the name(),
//!    shortDescription() and longDescription() member functions of Info are
//!    required, there are also optional ones, such as
//!    Info::exptect::description(), which, if defined, must also be static and
//!    return a std::string, describing the type the particular keyword expects
//!    during parsing. This is optional since not every keyword expects a value
//!    (or values) of a particular type. For example, the keyword 'end' is
//!    simply used to close a block, and what follows does not have a
//!    relationship to the keyword. A counterexample is is 'title', which
//!    expects a double-quoted string.
//! \see For example client-code and more detailed documentation on the possible
//!    fields, see Control/Keywords.h.
//! \author J. Bakosi
template< typename Info, int Char, int... Chars >
struct keyword {

  //! \brief Accessor to keyword as pegtl::string
  using pegtl_string = pegtl::string< Char, Chars... >;

  //! \brief Accessor to keyword as std::string
  //! \return Keyword as std::string
  static std::string string() {
    return ( sizeof...( Chars ) ) ?
           ( pegtl::escaper< Char, Chars... >::result() ) :
           ( pegtl::escape( Char ) );
  }

  //! \brief Accessor to required short name of a keyword
  //! \return Name of keyword as std::string
  static std::string name() { return Info::name(); }

  //! \brief Accessor to required short description of a keyword
  //! \return Short help as std::string
  static std::string shortDescription() { return Info::shortDescription(); }

  //! \brief Accessor to required long description of a keyword
  //! \return Long help as std::string
  static std::string longDescription() { return Info::longDescription(); }

  //! \brief Bring template argument 'Info' to scope as 'info'
  //! \details This is used to access, e.g., Info::alias, etc., if exist.
  //! \see tk::grm::alias
  //! \see tk::grm::readcmd
  using info = Info;

  //! \brief Overloads to optional alias accessor depending on the existence of
  //!   Info::alias
  //! \return An initialized (or uninitialized) boost::optional< std::string >
  //! \details As to why type Info has to be aliased to a local type T for
  //!   SFINAE to work, see http://stackoverflow.com/a/22671495. Though an alias
  //!   is only a single character, it returns it as std::string since
  //!   pegtl::escape returns std::string.
  //! \see http://www.boost.org/doc/libs/release/libs/optional/doc/html/index.html
  //! \see http://en.cppreference.com/w/cpp/language/sfinae
  //! \see http://en.cppreference.com/w/cpp/types/enable_if
  template< typename T = Info, typename std::enable_if<
    tk::HasTypedefAlias< T >::value, int >::type = 0 >
  static boost::optional< std::string > alias()
  { return pegtl::escape( Info::alias::value ); }

  template< typename T = Info, typename std::enable_if<
    !tk::HasTypedefAlias< T >::value, int >::type = 0 >
  static boost::optional< std::string > alias()
  { return boost::none; }

  //! \brief Overloads to optional policy code accessor depending on the
  //!   existence of Info::Code
  //! \return An initialized (or uninitialized) boost::optional< std::string >
  //! \details As to why type Info has to be aliased to a local type T for
  //!   SFINAE to work, see http://stackoverflow.com/a/22671495. Though a code
  //!   is only a single character, it returns it as std::string since
  //!   pegtl::escape returns std::string.
  //! \see http://www.boost.org/doc/libs/release/libs/optional/doc/html/index.html
  //! \see http://en.cppreference.com/w/cpp/language/sfinae
  //! \see http://en.cppreference.com/w/cpp/types/enable_if
  template< typename T = Info, typename std::enable_if<
    tk::HasTypedefCode< T >::value, int >::type = 0 >
  static boost::optional< std::string > code()
  { return pegtl::escape( Info::code::value ); }

  template< typename T = Info, typename std::enable_if<
    !tk::HasTypedefCode< T >::value, int >::type = 0 >
  static boost::optional< std::string > code()
  { return boost::none; }

  //! \brief Overloads to optional expected type description depending on the
  //!   existence of Info::expect::description
  //! \return An initialized (or uninitialized) boost::optional< std::string >
  //! \details As to why type Info has to be aliased to a local type T for
  //!   SFINAE to work, see http://stackoverflow.com/a/22671495.
  //! \see http://www.boost.org/doc/libs/release/libs/optional/doc/html/index.html
  //! \see http://en.cppreference.com/w/cpp/language/sfinae
  //! \see http://en.cppreference.com/w/cpp/types/enable_if
  template< typename T = Info, typename std::enable_if<
    tk::HasFunctionExpectDescription< T >::value, int >::type = 0 >
  static boost::optional< std::string > expt()
  { return Info::expect::description(); }

  template< typename T = Info, typename std::enable_if<
    !tk::HasFunctionExpectDescription< T >::value, int >::type = 0 >
  static boost::optional< std::string > expt()
  { return boost::none; }

  //! \brief Overloads to optional expected choices description depending on the
  //!   existence of Info::expect::choices
  //! \return An initialized (or uninitialized) boost::optional< std::string >
  //! \details As to why type Info has to be aliased to a local type T for
  //!   SFINAE to work, see http://stackoverflow.com/a/22671495.
  //! \see http://www.boost.org/doc/libs/release/libs/optional/doc/html/index.html
  //! \see http://en.cppreference.com/w/cpp/language/sfinae
  //! \see http://en.cppreference.com/w/cpp/types/enable_if
  template< typename T = Info, typename std::enable_if<
    tk::HasFunctionExpectChoices< T >::value, int >::type = 0 >
  static boost::optional< std::string > choices()
  { return Info::expect::choices(); }

  template< typename T = Info, typename std::enable_if<
    !tk::HasFunctionExpectChoices< T >::value, int >::type = 0 >
  static boost::optional< std::string > choices()
  { return boost::none; }
};

} // kw::

#endif // Keyword_h
