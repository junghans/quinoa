//******************************************************************************
/*!
  \file      src/Random/RandomException.h
  \author    J. Bakosi
  \date      Fri Apr 26 15:01:01 2013
  \copyright Copyright 2005-2012, Jozsef Bakosi, All rights reserved.
  \brief     RandomException class declaration
  \details   RandomException class declaration
*/
//******************************************************************************
#ifndef RandomException_h
#define RandomException_h

#include <string>

using namespace std;

#include <Exception.h>

namespace Quinoa {

//! RandomException types
enum RndExceptType { RND_MKL=0,             //!< MKL error
                     RND_UNIMPLEMENTED,     //!< Unimplemented exception
                     NUM_RND_EXCEPT
};

//! Random exception error messages
const string RndMsg[NUM_RND_EXCEPT] = {
  "MKL exception: ",
  "Unimplemented random number generator exception"
};

//! RandomException : Exception
class RandomException : public Exception {

  public:
    //! Constructor
    explicit RandomException(const ExceptType except,
                             const RndExceptType rndExcept,
                             const string& file,
                             const string& func,
                             const unsigned int& line) :
      Exception(except, file, func, line), m_except(rndExcept) {}

    //! Destructor
    virtual ~RandomException() noexcept = default;

    //! Handle RandomException
    virtual ErrCode handleException(Driver* const driver);

  protected:
    //! Permit copy constructor only for children
    RandomException(const RandomException&) = default;
    //! Permit move constructor only for children
    RandomException(RandomException&&) = default;

  private:
    //! Don't permit copy assignment
    RandomException& operator=(const RandomException&) = delete;
    //! Don't permit move assignment
    RandomException& operator=(RandomException&&) = delete;

    //! Random exception type (RND_UNIMPLEMENTED, etc.)
    const RndExceptType m_except;
};

} // namespace Quinoa

#endif // RandomException_h
