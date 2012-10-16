//******************************************************************************
/*!
  \file      src/Base/MKLRandom.h
  \author    J. Bakosi
  \date      Mon 15 Oct 2012 08:24:25 PM MDT
  \copyright Copyright 2005-2012, Jozsef Bakosi, All rights reserved.
  \brief     MKL-based random number generator
  \details   MKL-based random number generator
*/
//******************************************************************************
#ifndef MKLRandom_h
#define MKLRandom_h

#include <vector>

#include <mkl_vsl.h>

#include <Memory.h>
#include <Random.h>

using namespace std;

namespace Quinoa {

//! Probability distributions
enum Distribution { UNIFORM=0,        //!< Uniform
                    GAUSSIAN,         //!< Gaussian
                    GAMMA,            //!< Gamma
                    NUM_DIST_TYPES
};

//! Random number stream
struct RndStreams {
  long long int chunk;          //!< Number of numbers generated by every thread
  long long int remainder;      //!< Leftover
  VSLStreamStatePtr* stream;    //!< Array of pointers to thread-streams
  MemoryEntry* rnd;             //!< Pointer to random numbers

  //! On-the-fly constructor
  RndStreams(const Int Chunk,
             const Int Remainder,
             VSLStreamStatePtr* Stream,
             MemoryEntry* Rnd) :
    chunk(Chunk), remainder(Remainder), stream(Stream), rnd(Rnd) {}
};

//! MKL-based random number generator
class MKLRandom : Random {

  public:
    //! Constructor: Setup random number generator streams
    MKLRandom(const long long int nthreads, const uInt seed, Memory* memory) :
      Random(nthreads, seed), m_memory(memory) {}

    //! Destructor: Destroy random number generator streams
    ~MKLRandom();

    //! Add random table
    void addTable(const int brng,
                  const Distribution dist,
                  const long long int number,
                  const string name);

    //! Regenerate random numbers in all tables
    void regenTables();

  private:
    //! Don't permit copy constructor
    MKLRandom(const MKLRandom&) = delete;
    //! Don't permit copy assigment
    MKLRandom& operator=(const MKLRandom&) = delete;
    //! Don't permit move constructor
    MKLRandom(MKLRandom&&) = delete;
    //! Don't permit move assigment
    MKLRandom& operator=(MKLRandom&&) = delete;

    //! Call MKL's vslNewStream() and handle error
    void newStream(VSLStreamStatePtr* stream,
                   const int& brng,
                   const unsigned int& seed);

    //! Call MKL's vslCopyStream() and handle error
    void copyStream(VSLStreamStatePtr* newstream,
                    const VSLStreamStatePtr& srcstream);

    //! Call MKL's vslSkipaheadStream() and handle error
    void skipAheadStream(VSLStreamStatePtr& stream,
                         const long long int& nskip);

    //! Call MKL's vdRngUniform() and handle error
    void uniform(const Int& method, 
                 VSLStreamStatePtr& stream,
                 const Int& n,
                 Real* r,
                 const Real& a,
                 const Real& b);

    //! Memory object pointer
    Memory* m_memory;

    //! Stream tables to generate fixed numbers of random numbers with fixed
    //! properties using Random::m_nthreads
    vector<RndStreams> m_table;
};

} // namespace Quinoa

#endif // MKLRandom_h
