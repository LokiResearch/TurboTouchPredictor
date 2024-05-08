/* -*- mode: c++ -*-
 *
 * lag/utils/ExponentialSmoothing.cpp --
 *
 * This Source Code is subject to the terms of
 * TURBOTOUCH PREDICTOR version 1.0 licence
 * available in LICENCE.txt
 *
 * Authors: Nicolas Roussel
 *
 *
 */

#include <lag/utils/ExponentialSmoothing.h>

namespace lag {

  // -----------------------------------------------------------------------

  SingleExponentialSmoothing::SingleExponentialSmoothing(double alpha, variant_t variant) {
    v = variant ;
    y = s = 0.0 ; // they will be initialized on first call anyway
    setAlpha(alpha) ;
    initialized = false ;
  }

  void
  SingleExponentialSmoothing::setAlpha(double alpha) {
    if (alpha<=0.0 || alpha>1.0)
      throw std::range_error("alpha should be in (0.0, 1.0]") ;
    a = alpha ;
  }

  double
  SingleExponentialSmoothing::smooth(double raw) {
    double smoothed ;
    if (initialized) {
      smoothed = a*(v==HUNTER?y:raw) + (1.0-a)*s ;
    } else {
      smoothed = raw ; // FIXME: arbitrary choice, but the simplest one...
      initialized = true ;
    }
    y = raw ;
    s = smoothed ;
    return smoothed ;
  }

  double
  SingleExponentialSmoothing::smoothWithAlpha(double value, double alpha) {
    setAlpha(alpha) ;
    return smooth(value) ;
  }
  
  // -----------------------------------------------------------------------

  DoubleExponentialSmoothing::DoubleExponentialSmoothing(double alpha, double gamma) {
    setAlpha(alpha) ;
    setGamma(gamma) ;
    y = s = b = 0.0 ; // they will be initialized on first call anyway
    initialized = false ;
  }

  void
  DoubleExponentialSmoothing::setAlpha(double alpha) {
    if (alpha<0.0 || alpha>1.0) 
      throw std::range_error("alpha should be in [0.0, 1.0]") ;
    a = alpha ;
  }

  void
  DoubleExponentialSmoothing::setGamma(double gamma) {
    if (gamma<0.0 || gamma>1.0) 
      throw std::range_error("gamma should be in [0.0, 1.0]") ;
    g = gamma ;
  }

  double
  DoubleExponentialSmoothing::smooth(double raw) {
    double smoothed ;
    double trend ;
    if (initialized) {
      smoothed = a*raw + (1.0-a)*(s+b) ;
      trend = g*(smoothed-s) + (1.0-g)*b ;
    } else {
      smoothed = raw ; // FIXME: arbitrary choice, but the simplest one...
      trend = 0.0 ;    // FIXME: arbitrary choice, but the simplest one...
      initialized = true ;
    }
    y = raw ;
    s = smoothed ;
    b = trend ;
    return smoothed ;
  }

  // -----------------------------------------------------------------------

}
