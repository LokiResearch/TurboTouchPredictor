/* -*- mode: c++ -*-
 *
 * lag/utils/ExponentialSmoothing.h --
 *
 * This Source Code is subject to the terms of
 * TURBOTOUCH PREDICTOR version 1.0 licence
 * available in LICENCE.txt
 *
 * Authors: Nicolas Roussel
 *
 *
 */

#ifndef lag_utils_ExponentialSmoothing_h
#define lag_utils_ExponentialSmoothing_h

#include <stdexcept>

namespace lag {

  // -----------------------------------------------------------------------

  class ExponentialSmoothing {

  protected:

    bool initialized ;
    double y ; // last raw value
    double s ; // last smoothed value

  public:

    virtual double smooth(double value) = 0 ;

    bool hasLastRawValue(void) const { return initialized ; }
    double lastRawValue(void) const { return y ; }
    double lastSmoothedValue(void) const { return s ; }

    void reset() { initialized = false ; }

    virtual ~ExponentialSmoothing(void) {}

  } ;

  // -----------------------------------------------------------------------

  class SingleExponentialSmoothing : public ExponentialSmoothing {
    
    // Exponentially weighted moving average (EWMA)
    // See http://www.itl.nist.gov/div898/handbook/pmc/section4/pmc431.htm

  public:

    typedef enum {
      ROBERTS=0,  // will use the given raw value
      HUNTER=1    // will use the previous raw value
    } variant_t ;

  protected:

    variant_t v ; // ROBERTS or HUNTER (see above)
    double a ;    // alpha, the smoothing constant

  public:

    SingleExponentialSmoothing(double alpha, variant_t variant=ROBERTS) ;

    variant_t variant(void) const { return v ; }

    double alpha(void) const { return a ; }
    void setAlpha(double alpha) ;

    double smooth(double value) ;
    double smoothWithAlpha(double value, double alpha) ;
 
  } ;

  // -----------------------------------------------------------------------

  class DoubleExponentialSmoothing : public ExponentialSmoothing {
    
    // See http://www.itl.nist.gov/div898/handbook/pmc/section4/pmc433.htm

  protected:

    double a ; // alpha, the first smoothing constant
    double g ; // gamma, the second smoothing constant
    double b ; // last trend

  public:

    DoubleExponentialSmoothing(double alpha, double gamma) ;

    double alpha(void) const { return a ; }
    void setAlpha(double alpha) ;

    double gamma(void) const { return g ; }
    void setGamma(double gamma) ;

    double smooth(double value) ;

    double lastTrendValue(void) const { return b ; }

  } ;

  // -----------------------------------------------------------------------

}

#endif
