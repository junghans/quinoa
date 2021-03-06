// @HEADER
// ************************************************************************
//
//               Rapid Optimization Library (ROL) Package
//                 Copyright (2014) Sandia Corporation
//
// Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive
// license for use of this work by or on behalf of the U.S. Government.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact lead developers:
//              Drew Kouri   (dpkouri@sandia.gov) and
//              Denis Ridzal (dridzal@sandia.gov)
//
// ************************************************************************
// @HEADER

#ifndef ROL_MIXEDQUANTILEQUADRANGLE_HPP
#define ROL_MIXEDQUANTILEQUADRANGLE_HPP

#include "ROL_RiskMeasure.hpp"
#include "ROL_PlusFunction.hpp"
#include "ROL_RiskVector.hpp"

#include "Teuchos_Array.hpp"
#include "Teuchos_ParameterList.hpp"

namespace ROL {

template<class Real>
class MixedQuantileQuadrangle : public RiskMeasure<Real> {
private:
  Teuchos::RCP<PlusFunction<Real> > plusFunction_;

  Teuchos::Array<Real> prob_;
  Teuchos::Array<Real> coeff_;

  Teuchos::RCP<Vector<Real> > dualVector_;
  std::vector<Real> xvar_;
  std::vector<Real> vvar_;

  std::vector<Real> vec_;

  int size_;

  bool firstReset_;

public:

  MixedQuantileQuadrangle( Teuchos::ParameterList &parlist )
    : RiskMeasure<Real>(), firstReset_(true) {
    Teuchos::ParameterList &list
      = parlist.sublist("SOL").sublist("Risk Measure").sublist("Mixed-Quantile Quadrangle");
    // Grab probability and coefficient arrays
    prob_  = Teuchos::getArrayFromStringParameter<Real>(list,"Probability Array");
    coeff_ = Teuchos::getArrayFromStringParameter<Real>(list,"Coefficient Array");
    // Check array sizes
    int pSize = prob_.size(), cSize = coeff_.size();
    TEUCHOS_TEST_FOR_EXCEPTION((pSize!=cSize),std::invalid_argument,
      ">>> ERROR (ROL::MixedQuantileQuadrangle): Probability and coefficient arrays have different sizes!");
    size_ = pSize;
    // Check array elements
    Real sum = 0.0;
    for (int i = 0; i < size_; i++) {
      TEUCHOS_TEST_FOR_EXCEPTION((prob_[i]>1. || prob_[i]<0.), std::invalid_argument,
        ">>> ERROR (ROL::MixedQuantileQuadrangle): Element of probability array out of range!");
      TEUCHOS_TEST_FOR_EXCEPTION((coeff_[i]>1. || coeff_[i]<0.), std::invalid_argument,
        ">>> ERROR (ROL::MixedQuantileQuadrangle): Element of coefficient array out of range!");
      sum += coeff_[i];
    }
    TEUCHOS_TEST_FOR_EXCEPTION((std::abs(sum-1.) > std::sqrt(ROL_EPSILON)),std::invalid_argument,
      ">>> ERROR (ROL::MixedQuantileQuadrangle): Coefficients do not sum to one!");
    // Build (approximate) plus function
    plusFunction_ = Teuchos::rcp(new PlusFunction<Real>(list));
    // Initialize temporary storage
    xvar_.clear(); xvar_.resize(size_,0.0);
    vvar_.clear(); vvar_.resize(size_,0.0);
    vec_.clear();  vec_.resize(size_,0.0);
  }

  MixedQuantileQuadrangle(const std::vector<Real> &prob,
                          const std::vector<Real> &coeff,
                          const Teuchos::RCP<PlusFunction<Real> > &pf )
    : RiskMeasure<Real>(), plusFunction_(pf), firstReset_(true) {
    prob_.clear(); coeff_.clear();
    // Check array sizes
    int pSize = prob.size(), cSize = coeff.size();
    TEUCHOS_TEST_FOR_EXCEPTION((pSize!=cSize),std::invalid_argument,
      ">>> ERROR (ROL::MixedQuantileQuadrangle): Probability and coefficient arrays have different sizes!");
    size_ = pSize;
    // Check array elements
    Real sum = 0.0;
    for (int i = 0; i < size_; i++) {
      TEUCHOS_TEST_FOR_EXCEPTION((prob[i]>1. || prob[i]<0.), std::invalid_argument,
        ">>> ERROR (ROL::MixedQuantileQuadrangle): Element of probability array out of range!");
      TEUCHOS_TEST_FOR_EXCEPTION((coeff[i]>1. || coeff[i]<0.), std::invalid_argument,
        ">>> ERROR (ROL::MixedQuantileQuadrangle): Element of coefficient array out of range!");
      prob_.push_back(prob_[i]);
      coeff_.push_back(coeff_[i]);
      sum += coeff[i];
    }
    TEUCHOS_TEST_FOR_EXCEPTION((std::abs(sum-1.) > std::sqrt(ROL_EPSILON)),std::invalid_argument,
      ">>> ERROR (ROL::MixedQuantileQuadrangle): Coefficients do not sum to one!");
    // Initialize temporary storage
    xvar_.clear(); xvar_.resize(size_,0.0);
    vvar_.clear(); vvar_.resize(size_,0.0);
    vec_.clear();  vec_.resize(size_,0.0);
  }

  void reset(Teuchos::RCP<Vector<Real> > &x0, const Vector<Real> &x) {
    RiskMeasure<Real>::reset(x0,x);
    for (int i = 0; i < size_; i++) {
      xvar_[i] = Teuchos::dyn_cast<const RiskVector<Real> >(
                 Teuchos::dyn_cast<const Vector<Real> >(x)).getStatistic(i);
      vec_[i]  = 0.0;
    }
    if ( firstReset_ ) {
      dualVector_ = (x0->dual()).clone();
      firstReset_ = false;
    }
    dualVector_->zero();
  }

  void reset(Teuchos::RCP<Vector<Real> > &x0, const Vector<Real> &x,
             Teuchos::RCP<Vector<Real> > &v0, const Vector<Real> &v) {
    reset(x0,x);
    v0 = Teuchos::rcp_const_cast<Vector<Real> >(Teuchos::dyn_cast<const RiskVector<Real> >(
           Teuchos::dyn_cast<const Vector<Real> >(v)).getVector());
    for (int i = 0; i < size_; i++) {
      vvar_[i] = Teuchos::dyn_cast<const RiskVector<Real> >(
                 Teuchos::dyn_cast<const Vector<Real> >(v)).getStatistic(i);
    }
  }

  void update(const Real val, const Real weight) {
    Real pf = 0.0;
    for (int i = 0; i < size_; i++) {
      pf = plusFunction_->evaluate(val-xvar_[i],0);
      RiskMeasure<Real>::val_ += weight*coeff_[i]/(1.0-prob_[i])*pf;
    }
  }

  void update(const Real val, const Vector<Real> &g, const Real weight) {
    Real pf = 0.0, c = 0.0;
    for (int i = 0; i < size_; i++) {
      pf = plusFunction_->evaluate(val-xvar_[i],1);
      c  = weight*coeff_[i]/(1.0-prob_[i])*pf;
      vec_[i] -= c;
      RiskMeasure<Real>::g_->axpy(c,g);
    }
  }

  void update(const Real val, const Vector<Real> &g, const Real gv, const Vector<Real> &hv,
              const Real weight) {
    Real pf1 = 0.0, pf2 = 0.0, c = 0.0;
    for (int i = 0; i < size_; i++) {
      pf1 = plusFunction_->evaluate(val-xvar_[i],1);
      pf2 = plusFunction_->evaluate(val-xvar_[i],2);
      c   = weight*coeff_[i]/(1.0-prob_[i])*pf2*(gv-vvar_[i]);
      vec_[i] -= c;
      //c  *= (gv-vvar_[i]);
      RiskMeasure<Real>::hv_->axpy(c,g);
      c = weight*coeff_[i]/(1.0-prob_[i])*pf1;
      RiskMeasure<Real>::hv_->axpy(c,hv);
    }
  }

  Real getValue(SampleGenerator<Real> &sampler) {
    Real val  = RiskMeasure<Real>::val_;
    Real cvar = 0.0;
    sampler.sumAll(&val,&cvar,1);
    for (int i = 0; i < size_; i++) {
      cvar += coeff_[i]*xvar_[i];
    }
    return cvar;
  }

  void getGradient(Vector<Real> &g, SampleGenerator<Real> &sampler) {
    RiskVector<Real> &gs = Teuchos::dyn_cast<RiskVector<Real> >(Teuchos::dyn_cast<Vector<Real> >(g));
    std::vector<Real> var(size_,0.0);
    sampler.sumAll(&vec_[0],&var[0],size_);
    
    sampler.sumAll(*(RiskMeasure<Real>::g_),*dualVector_);
    for (int i = 0; i < size_; i++) {
      var[i] += coeff_[i];
    }
    gs.setStatistic(var);
    gs.setVector(*(Teuchos::rcp_dynamic_cast<Vector<Real> >(dualVector_))); 
  }

  void getHessVec(Vector<Real> &hv, SampleGenerator<Real> &sampler) {
    RiskVector<Real> &hs = Teuchos::dyn_cast<RiskVector<Real> >(Teuchos::dyn_cast<Vector<Real> >(hv));
    std::vector<Real> var(size_,0.0);
    sampler.sumAll(&vec_[0],&var[0],size_);

    sampler.sumAll(*(RiskMeasure<Real>::hv_),*dualVector_);
//    for (int i = 0; i < size_; i++) {
//      var[i] *= coeff_[i]/(1.0-prob_[i]);
//    }
    hs.setStatistic(var);
    hs.setVector(*(Teuchos::rcp_dynamic_cast<Vector<Real> >(dualVector_)));
  }
};

}

#endif
