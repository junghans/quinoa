//******************************************************************************
/*!
  \file      src/Main/Driver.h
  \author    J. Bakosi
  \date      Thu 03 Oct 2013 09:23:59 PM MDT
  \copyright Copyright 2005-2012, Jozsef Bakosi, All rights reserved.
  \brief     Driver base
  \details   Driver base
*/
//******************************************************************************
#ifndef Driver_h
#define Driver_h

namespace quinoa {

//! Driver base class
class Driver {

  public:
    //! Constructor
    explicit Driver() = default;

    //! Destructor
    virtual ~Driver() noexcept = default;

    //! Execute
    virtual void execute() const = 0;

  private:
    //! Don't permit copy constructor
    Driver(const Driver&) = delete;
    //! Don't permit assigment constructor
    Driver& operator=(const Driver&) = delete;
    //! Don't permit move constructor
    Driver(Driver&&) = delete;
    //! Don't permit move assignment
    Driver& operator=(Driver&&) = delete;
};

} // namespace quinoa

#endif // Driver_h
