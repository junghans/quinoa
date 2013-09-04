//******************************************************************************
/*!
  \file      src/Physics/HomMix/HomMix.h
  \author    J. Bakosi
  \date      Wed Sep  4 08:05:59 2013
  \copyright Copyright 2005-2012, Jozsef Bakosi, All rights reserved.
  \brief     Homogeneous material mixing
  \details   Homogeneous material mixing
*/
//******************************************************************************
#ifndef HomMix_h
#define HomMix_h

#include <map>

#include <Physics.h>
#include <Timer.h>

namespace quinoa {

class Memory;
class Paradigm;
class QuinoaControl;

//! HomMix : Physics
class HomMix : public Physics {

  public:
    //! Constructor
    explicit HomMix(Memory* const memory,
                    Paradigm* const paradigm,
                    const QuinoaControl& control,
                    Timer* const timer);

    //! Destructor
    ~HomMix() noexcept override = default;

    //! Initialize model
    void init() override;

    //! Solve model
    void solve() override;

  private:
    //! Don't permit copy constructor
    HomMix(const HomMix&) = delete;
    //! Don't permit copy assigment
    HomMix& operator=(const HomMix&) = delete;
    //! Don't permit move constructor
    HomMix(HomMix&&) = delete;
    //! Don't permit move assigment
    HomMix& operator=(HomMix&&) = delete;

    //! One-liner report
    void reportHeader() const;
    void report(const uint64_t it,
                const uint64_t nstep,
                const real t,
                const real dt,
                const bool wroteJpdf,
                const bool wroteGlob,
                const bool wrotePlot);

    //! Advance
    void advance(real dt);

    //! Output joint scalar PDF
    void outJpdf(const real t);

    const TimerIdx m_totalTime;           //!< Timer measuring the total run    
};

} // namespace quinoa

#endif // HomMix_h
