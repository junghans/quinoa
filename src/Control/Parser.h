//******************************************************************************
/*!
  \file      src/Control/Parser.h
  \author    J. Bakosi
  \date      Thu Oct  3 16:24:43 2013
  \copyright Copyright 2005-2012, Jozsef Bakosi, All rights reserved.
  \brief     Parser base
  \details   Parser base
*/
//******************************************************************************
#ifndef Parser_h
#define Parser_h

#include <Base.h>

namespace quinoa {

//! Parser base
class Parser {

  protected:
    //! Constructor
    explicit Parser(Base& base) : m_base(base) {}

    //! Destructor
    virtual ~Parser() noexcept = default;

    //! Parse interface
    virtual void parse() = 0;

    const Base& m_base;                  //!< Essentials

  private:
    //! Don't permit copy constructor
    Parser(const Parser&) = delete;
    //! Don't permit copy assigment
    Parser& operator=(const Parser&) = delete;
    //! Don't permit move constructor
    Parser(Parser&&) = delete;
    //! Don't permit move assigment
    Parser& operator=(Parser&&) = delete;
};

} // namespace quinoa

#endif // Parser_h
