/* -*- mode: c++ -*-
 *
 * lag/utils/OneEuroFilter.cpp --
 *
 * This Source Code is subject to the terms of
 * TURBOTOUCH PREDICTOR version 1.0 licence
 * available in LICENCE.txt
 *
 * Authors: Nicolas Roussel
 *
 *
 */

#include <lag/utils/OneEuroFilter.h>

#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace lag {

  double
  OneEuroFilter::alpha(double cutoff) {
    double te = 1.0 / freq ;
    double tau = 1.0 / (2*M_PI*cutoff) ;
    return 1.0 / (1.0 + tau/te) ;
  }

  void
  OneEuroFilter::setFrequency(double f) {
    if (f<=0) throw std::range_error("freq should be >0") ;
    freq = f ;
  }

  double
  OneEuroFilter::frequency(void) {
    return freq ;
  }

  void
  OneEuroFilter::setMinCutoff(double mc) {
    if (mc<=0) throw std::range_error("mincutoff should be >0") ;
    mincutoff = mc ;
  }

  double
  OneEuroFilter::minCutoff(void) {
    return mincutoff ;
  }

  void
  OneEuroFilter::setBeta(double b) {
    beta_ = b ;
  }

  double
  OneEuroFilter::beta(void) {
    return beta_ ;
  }

  void
  OneEuroFilter::setDerivateCutoff(double dc) {
    if (dc<=0) throw std::range_error("dcutoff should be >0") ;
    dcutoff = dc ;
  }

  double
  OneEuroFilter::derivateCutoff(void) {
    return dcutoff ;
  }

  OneEuroFilter::OneEuroFilter(double freq, 
			       double mincutoff, double beta_, double dcutoff) {
    setFrequency(freq) ;
    setMinCutoff(mincutoff) ;
    setBeta(beta_) ;
    setDerivateCutoff(dcutoff) ;
    x = new SingleExponentialSmoothing(alpha(mincutoff)) ;
    dx = new SingleExponentialSmoothing(alpha(dcutoff)) ;
    lasttime = TimeStamp::undef ;
  }

  void
  OneEuroFilter::reset() {
    x->reset() ;
    dx->reset();
  }

  double 
  OneEuroFilter::filter(double value, TimeStamp::inttime timestamp) {
    // FIXME: do we really want that all the time?
    // update the sampling frequency based on timestamps
    if (lasttime!=TimeStamp::undef && timestamp!=TimeStamp::undef && timestamp!=lasttime) {
      if (timestamp < lasttime)
        std::cerr << "OneEuroFilter::filter: timestamp (" << timestamp << ") < lasttime (" << lasttime << ")" << std::endl;
      else
        freq = (double)TimeStamp::one_second / (timestamp-lasttime) ;
    }
    
    lasttime = timestamp ;

    // estimate the current variation per second
    double dvalue = x->hasLastRawValue() 
      ? (value - x->lastRawValue())*freq // FIXME: lastRawValue or lastFilteredValue (like in CHI'12 paper)?
      : value ;                          // FIXME: 0.0 or value?
    double edvalue = dx->smoothWithAlpha(dvalue, alpha(dcutoff)) ;

    // use it to update the cutoff frequency
    double cutoff = mincutoff + beta_*fabs(edvalue) ;

    // filter the given value
    return x->smoothWithAlpha(value, alpha(cutoff)) ;
  }

  bool
  OneEuroFilter::hasLastRawValue(void) {
    return x->hasLastRawValue() ;
  }

  double
  OneEuroFilter::lastRawValue(void) {
    return x->lastRawValue() ;
  }

  double
  OneEuroFilter::lastFilteredValue(void) {
    return x->lastSmoothedValue() ;
  }

  OneEuroFilter::~OneEuroFilter(void) {
    delete x ;
    delete dx ;
  }

}
