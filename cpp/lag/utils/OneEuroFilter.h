/* -*- mode: c++ -*-
 *
 * lag/utils/OneEuroFilter.h --
 *
 * This Source Code is subject to the terms of
 * TURBOTOUCH PREDICTOR version 1.0 licence
 * available in LICENCE.txt
 *
 * Authors: Nicolas Roussel
 *
 *
 */

#ifndef lag_utils_OneEuroFilter_h
#define lag_utils_OneEuroFilter_h

#include <lag/utils/TimeStamp.h>
#include <lag/utils/ExponentialSmoothing.h>

#include <iostream>

namespace lag {

  class OneEuroFilter {
    
  protected:

    double freq ;
    double mincutoff ;
    double beta_ ;
    double dcutoff ;
    SingleExponentialSmoothing *x ;
    SingleExponentialSmoothing *dx ;
    TimeStamp::inttime lasttime ;

    double alpha(double cutoff) ;

  public:

    OneEuroFilter(double freq, 
		  double mincutoff=1.0, double beta_=0.0, double dcutoff=1.0) ;

    void reset() ;

    void setFrequency(double f) ;

    double frequency(void) ;

    void setMinCutoff(double mc) ;
    double minCutoff(void) ;

    void setBeta(double b) ;
    double beta(void) ;

    void setDerivateCutoff(double dc) ;
    double derivateCutoff(void) ;

    double filter(double value, TimeStamp::inttime timestamp=TimeStamp::undef) ;

    bool hasLastRawValue(void) ;
    double lastRawValue(void) ;
    double lastFilteredValue(void) ;

    ~OneEuroFilter(void) ;

  } ;

  class TwoEuroFilter {
    
  protected:

    OneEuroFilter *oef_x, *oef_y ;
    
  public:

    TwoEuroFilter(double freq, 
		  double mincutoff=1.0, double beta_=0.0, double dcutoff=1.0) {
      oef_x = new OneEuroFilter(freq, mincutoff, beta_, dcutoff) ;
      oef_y = new OneEuroFilter(freq, mincutoff, beta_, dcutoff) ;
    }

    void filter(double x, double y,
		double &fx, double &fy,
		TimeStamp::inttime timestamp=TimeStamp::undef) {
      fx = oef_x->filter(x, timestamp) ;
      fy = oef_y->filter(y, timestamp) ;
    }

    void reset() {
       oef_x->reset();
       oef_y->reset();
    }

    ~TwoEuroFilter(void) {
      delete oef_x ;
      delete oef_y ;
    }
    
  } ;
    
}

#endif
