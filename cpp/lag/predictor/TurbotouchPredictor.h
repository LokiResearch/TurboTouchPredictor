/* -*- mode: c++ -*-
 *
 * lag/predictor/TurbotouchPredictor.h --
 *
 * This Source Code is subject to the terms of
 * TURBOTOUCH PREDICTOR version 1.0 licence
 * available in LICENCE.txt
 *
 * Authors: Stanislav Aranovskiy, Sébastien Poulmane
 *
 *
 */

#ifndef lag_predictor_TurbotouchPredictor_h
#define lag_predictor_TurbotouchPredictor_h

#include <lag/predictor/Predictor.h>

#include <deque>

namespace lag {

  class TurbotouchPredictor : public Predictor {

  protected:

    struct Parameters {
      double alpha, lambda ;
      double gamma[10] ;
      double a[5] ;
    } px, py ;

    struct State {
      double s1 ;
      double s2 ;
      double s3 ;
      double s4 ;
      double s5 ;
      State(void) { s1 = s2 = s3 = s4 = s5 = 0.0 ; }
      State(double s1v, double s2v, double s3v, double s4v, double s5v)
	: s1(s1v)
	, s2(s2v)
	, s3(s3v)
	, s4(s4v)
	, s5(s5v)
      {
	/* Empty constructor */
      }
      inline State operator *(double d) const
      {
	return State(s1*d,s2*d,s3*d,s4*d,s5*d);
      }
      inline State operator +(const State &s) const
      {
	return State(s1+s.s1,s2+s.s2,s3+s.s3,s4+s.s4,s5+s.s5);
      }
    };

    // Prediction attributes
    double Sc;
    double V0;
    double alpha_gain;
    double Ts;
    int int_steps;
    int transients_duration;
    int count_transients;

    // Prediction states
    double lx, ly;                   // Last measured values
    InputEvent::State lastState;
    int steps_counter_x, steps_counter_y;

    State D_DiffHOMD4(const State&, const double u, const Parameters&);
    void updateHOM(const InputEvent&, State &,
			   const double x, const Parameters&,
			   double (&Der)[3], int& steps_counter);

    double predict(const double x, const Parameters&,
		   const double Der[3], double alpha) const;

  public:

    static const std::string scheme ;
    
    TurbotouchPredictor(URI uri, ConfigDict *cfg=0) ;

    URI getURI(void) ;
    void reset(void) ;
    InputEvent update(InputEvent &e, TimeStamp::inttime lag) ;

  };
}

#endif
