/* -*- mode: c++ -*-
 *
 * lag/predictor/TurbotouchPredictor.cpp --
 *
 * This Source Code is subject to the terms of
 * TURBOTOUCH PREDICTOR version 1.0 licence
 * available in LICENCE.txt
 *
 * Authors: Stanislav Aranovskiy, Sébastien Poulmane
 *
 *
 */

#include <lag/predictor/TurbotouchPredictor.h>

#include <cmath>

namespace  lag {

  const std::string TurbotouchPredictor::scheme = "turbotouch" ;
  
  TurbotouchPredictor::TurbotouchPredictor(URI /*uri*/, ConfigDict *cfg) {
    ConfigDict config ;
    if (cfg) config.updateFrom(*cfg) ;
    
    // alpha is a unitless parameter that should not be changed
    // gamma1 and gamma2 are unitless tuning parameters
    std::string alpha_name = scheme+"-alpha";
    std::string lambda_name = scheme+"-lambda";
    std::string gamma_name = scheme+"-gamma";
    this->px.alpha = config.setdefault<double>(std::string(alpha_name+"-x"), 30);
    this->py.alpha = config.setdefault<double>(std::string(alpha_name+"-y"), 30);
    this->px.lambda = config.setdefault<double>(std::string(lambda_name+"-x"), -0.1);
    this->py.lambda = config.setdefault<double>(std::string(lambda_name+"-y"), -0.1);
    for (int i=0; i<10; i++) {
#if ANDROID
        this->px.gamma[i] = config.setdefault<double>(std::string(gamma_name+to_string(i+1)+"-x"), 0);
        this->py.gamma[i] = config.setdefault<double>(std::string(gamma_name+to_string(i+1)+"-y"), 0);
#else
        this->px.gamma[i] = config.setdefault<double>(std::string(gamma_name+std::to_string(i+1)+"-x"), 0);
        this->py.gamma[i] = config.setdefault<double>(std::string(gamma_name+std::to_string(i+1)+"-y"), 0);
#endif
    }

    this->px.a[0] = 5*this->px.alpha;
    this->px.a[1] = 10*pow(this->px.alpha,2.0);
    this->px.a[2] = 10*pow(this->px.alpha,3.0);
    this->px.a[3] = 5*pow(this->px.alpha,4.0);
    this->px.a[4] = pow(this->px.alpha,5.0);

    this->py.a[0] = 5*this->py.alpha;
    this->py.a[1] = 10*pow(this->py.alpha,2.0);
    this->py.a[2] = 10*pow(this->py.alpha,3.0);
    this->py.a[3] = 5*pow(this->py.alpha,4.0);
    this->py.a[4] = pow(this->py.alpha,5.0);

    // ------------------------------------------
    
    // Sc should be set to the maximum amplitude of the differentiated signal
    // Unit: that of the signal, i.e. pixels
    this->Sc = config.setdefault<double>(scheme+"-sc", 1e4) ;

    this->V0 = config.setdefault<double>(scheme+"-V0", 105.0);
    this->V0 *= this->V0;
    this->alpha_gain = config.setdefault<double>(scheme+"-alpha_gain", 0.2752);
    this->Ts = config.setdefault<double>(scheme+"-ts", 0.001);
    this->int_steps = config.setdefault<int>(scheme+"-intsteps", 2);
    this->transients_duration = config.setdefault<int>(scheme+"-transients", 40);
    this->count_transients = 0;

    // ------------------------------------------
    
    this->lx = this->ly = 0;
    this->steps_counter_x = this->steps_counter_y = 0;
  }

  URI
  TurbotouchPredictor::getURI(void) {
    std::stringstream uri ;
    uri << scheme << ":" ;
    return uri.str() ;
  }
  
  void
  TurbotouchPredictor::reset(void) {
      count_transients = 0;
  }

  InputEvent
  TurbotouchPredictor::update(InputEvent &e, TimeStamp::inttime lag) {
    if (lag==TimeStamp::undef) lag = TimeStamp::createAsInt() - e.t ;
    
    static State sx, sy ;
    static double alpha ;

    // Update HOM
    double valueX = e.x, valueY = e.y;
    double DerX[3], DerY[3];
    this->updateHOM(e, sx, valueX, px, DerX, steps_counter_x);
    this->updateHOM(e, sy, valueY, py, DerY, steps_counter_y);

    // Compute V
    double V = pow(DerX[0], 2.0) + pow(DerY[0], 2.0);

    // Update alpha
    double alpha_star = (V > this->V0) ? 1 : 0;
    alpha = alpha + this->alpha_gain * (alpha_star - alpha);

    InputEvent predicted = InputEvent(e.source, e.channel, e.postype, e.state,
                                      e.t+lag,
                                      predict(e.x, px, DerX, alpha),
                                      predict(e.y, py, DerY, alpha)) ;

    count_transients++;
    if (count_transients <= this->transients_duration)
        return e; // Skip first events in order to avoid noises.
    else
        return predicted;

  }

  TurbotouchPredictor::State
  TurbotouchPredictor::D_DiffHOMD4(const State& state,
				   double u,
				   const Parameters& p) {
    State diff ;    
    double e = state.s1-u ;
    if (e > 0.0)
      diff = State(-p.a[0]*pow(e,1+p.lambda)+state.s2,
		   -p.a[1]*pow(e,1+2*p.lambda)+state.s3,
		   -p.a[2]*pow(e,1+3*p.lambda)+state.s4,
		   -p.a[3]*pow(e,1+4*p.lambda)+state.s5,
		   -p.a[4]*pow(e,1+5*p.lambda)) ;
    else
      diff = State(p.a[0]*pow(-e,1+p.lambda)+state.s2,
		   p.a[1]*pow(-e,1+2*p.lambda)+state.s3,
		   p.a[2]*pow(-e,1+3*p.lambda)+state.s4,
		   p.a[3]*pow(-e,1+4*p.lambda)+state.s5,
		   p.a[4]*pow(-e,1+5*p.lambda)) ;
    return diff ;    
  }

  void
  TurbotouchPredictor::updateHOM(const InputEvent& e, State& s,
				 const double x, const Parameters& p,
				 double (&Der)[3], int& steps_counter) {
    // FIXME: we might want the reset to be triggered externally
    bool doReset =
      (e.state==InputEvent::State::Interacting && lastState!=InputEvent::State::Interacting)
      || (e.state!=InputEvent::State::Interacting && lastState==InputEvent::State::Interacting) ;

    if (doReset) {
      s.s1 = x /this->Sc;
      s.s2 = s.s3 = s.s4 = s.s5 = 0.0;
      Der[0] = Der[1] = Der[2] = 0.0;
      // FIXME: this should be triggered by reset
      if (e.state==InputEvent::State::Interacting && lastState!=InputEvent::State::Interacting)
    steps_counter = this->transients_duration ;
    } else {
      double dt = this->Ts / this->int_steps;
      for (int n = 0; n < this->int_steps; n++)
    s = s + D_DiffHOMD4(s, x/this->Sc, p) * dt;
      if (steps_counter > 0)
    steps_counter--;
    }
    
    double transient_scale = std::max(1.0-steps_counter/(this->transients_duration/4.0), 0.0);
    Der[0] = s.s2 * this->Sc * transient_scale;
    Der[1] = s.s3 * this->Sc * transient_scale;
    Der[2] = s.s4 * this->Sc * transient_scale;

    this->lastState = e.state ;
  }

  double
  TurbotouchPredictor::predict(const double x, const Parameters& p,
			       const double Der[], double alpha) const {
    double Delta = 0;
    Delta += Der[0] * p.gamma[1];
    Delta += Der[1] * p.gamma[2];
    Delta += Der[2] * p.gamma[3];
    Delta += pow(Der[0],2.0) * p.gamma[4];
    Delta += pow(Der[1],2.0) * p.gamma[5];
    Delta += pow(Der[2],2.0) * p.gamma[6];
    Delta += fabs(Der[0]) * p.gamma[7];
    Delta += fabs(Der[1]) * p.gamma[8];
    Delta += fabs(Der[2]) * p.gamma[9];
    return x + alpha*Delta;
  }

}
