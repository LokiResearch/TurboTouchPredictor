/**
 *
 * This Source Code is subject to the terms of
 * TURBOTOUCH PREDICTOR version 1.0 licence
 * available in LICENCE.txt
 *
 * Author: Gery Casiez
 *
 */

import { OneEuroVectorProcessor } from './OneEuroVectorProcessor';
import cfg from "./TTPconfig.json";

export class TurboTouchPredictor {
  private filter : OneEuroVectorProcessor;
  private latencyCompensated: number;
  private update1euroInternalFreq: boolean;
  private lastState: string;
  private px: {alpha: number, lambda: number, gamma: number[], a: number[]};
  private py: {alpha: number, lambda: number, gamma: number[], a: number[]};
  private Sc: number;
  private V0: number;
  private alpha_gain: number;
  private Ts: number;
  private int_steps: number;
  private transients_duration: number;
  private count_transients: number;
  private lx: number;
  private ly: number;
  private steps_counter_x: number;
  private steps_counter_y: number;
  private sx: {s1: number, s2: number, s3: number, s4: number, s5: number};
  private sy: {s1: number, s2: number, s3: number, s4: number, s5: number};
  private alpha: number;
  private oneEuroEnabled: boolean;

  constructor() {
    this.filter = new OneEuroVectorProcessor(1, 1, 0);
    this.latencyCompensated = 0;
    this.update1euroInternalFreq = false;
  }

  /**
   * Sets the parameters of the predictor for the given prediction duration
   * 
   * @param l - Prediction in ms. Allowed values: 16, 32, 48, 64
   */

  private applyParametersForLatComp(l: number) {
    this.lastState = "NotInteracting";
    this.px = {alpha: cfg[l].alpha_x, lambda: cfg[l].lambda_x, gamma: [cfg[l].gamma1_x, cfg[l].gamma2_x, cfg[l].gamma3_x, cfg[l].gamma4_x, 0, 0, 0, 0, 0, 0], a: [0, 0, 0, 0, 0]};
    this.py = {alpha: cfg[l].alpha_y, lambda: cfg[l].lambda_y, gamma: [cfg[l].gamma1_y, cfg[l].gamma2_y, cfg[l].gamma3_y, cfg[l].gamma4_y, 0, 0, 0, 0, 0, 0], a: [0, 0, 0, 0, 0]};

    this.px.a[0] = 5*this.px.alpha;
    this.px.a[1] = 10*Math.pow(this.px.alpha,2.0);
    this.px.a[2] = 10*Math.pow(this.px.alpha,3.0);
    this.px.a[3] = 5*Math.pow(this.px.alpha,4.0);
    this.px.a[4] = Math.pow(this.px.alpha,5.0);

    this.py.a[0] = 5*this.py.alpha;
    this.py.a[1] = 10*Math.pow(this.py.alpha,2.0);
    this.py.a[2] = 10*Math.pow(this.py.alpha,3.0);
    this.py.a[3] = 5*Math.pow(this.py.alpha,4.0);
    this.py.a[4] = Math.pow(this.py.alpha,5.0);

    // ------------------------------------------
    
    // Sc should be set to the maximum amplitude of the differentiated signal
    // Unit: that of the signal, i.e. pixels
    this.Sc = 1e4;

    this.V0 = cfg[l].V0;
    this.V0 = this.V0 * this.V0;
    this.alpha_gain = cfg[l].alpha_gain;
    this.Ts = cfg[l].ts;
    this.int_steps = 2;
    this.transients_duration = cfg[l].transients;
    this.count_transients = 0;

    // ------------------------------------------
    
    this.lx = this.ly = 0;
    this.steps_counter_x = this.steps_counter_y = 0;

    this.sx = {s1: 0, s2: 0, s3: 0, s4: 0, s5: 0};
    this.sy = {s1: 0, s2: 0, s3: 0, s4: 0, s5: 0};
    this.alpha = 0;

    // ------------------------------------------

    this.oneEuroEnabled = cfg[l].one_euro_enabled;
    
    this.update1euroInternalFreq = cfg[l].one_euro_intern_freq_update;
    this.filter.setParameters(cfg[l].one_euro_freq, cfg[l].one_euro_mincutoff, cfg[l].one_euro_beta);
  }

  /**
   * Sets the parameters of the predictor for the given amount of compensation
   * 
   * @param comp - Compensation amount in ms. Allowed values: 0, 16, 32, 48, 64
   */

  public setAmountOfCompensation(comp: number) {
    const allowedValues = [0, 16, 32, 48, 64];
    if (allowedValues.includes(comp)) {
      this.latencyCompensated = comp;
      if (comp > 0) this.applyParametersForLatComp(comp);
      this.reset();
    } else {
      console.log("This value is not allowed");
    }
  }

  /**
   * Predicts a point from the current lagging one
   * 
   * @param e - e.x: x coordinate in pixels, e.y: y coordinate in pixels, e.t: timestamp in nanoseconds, e.state: "Interacting" or "NotInteracting"
   * 
   * @returns - predicted point p, p.x: x corrdinate, p.y: t corrdinate, p.t: timestamp in nanoseconds
   */

  public predict(e: {x: number, y: number, t: number, state: string}) {
    if (this.latencyCompensated == 0)
      return e;

    // Update HOM
    let valueX = e.x, valueY = e.y;
    let DerX = [], DerY = [];
    this.steps_counter_x = this.updateHOM(e, this.sx, valueX, this.px, DerX, this.steps_counter_x, "X");
    this.steps_counter_y = this.updateHOM(e, this.sy, valueY, this.py, DerY, this.steps_counter_y, "Y");

    // Compute V
    let V = Math.pow(DerX[0], 2.0) + Math.pow(DerY[0], 2.0);

    // Update alpha
    let alpha_star = (V > this.V0) ? 1 : 0;
    this.alpha = this.alpha + this.alpha_gain * (alpha_star - this.alpha);

    let ts = undefined;
    if (this.update1euroInternalFreq) ts = e.t;

    let predicted = {t: ts,
                     x: this.predictIntern(e.x, this.px, DerX, this.alpha),
                     y: this.predictIntern(e.y, this.py, DerY, this.alpha)};
    //console.log("Pred = " + predicted.x + " " + predicted.y);

    this.count_transients++;
    let res = undefined;
    if (this.count_transients <= this.transients_duration)
        res = e; // Skip first events in order to avoid noises.
    else
        res = predicted;

    if (this.oneEuroEnabled) {
      if (e.state != "Interacting") {
        this.filter.reset();
      }
      res = this.filter.process(res, e);
    }

    return res;
  } 

  private D_DiffHOMD4(state: {s1: number, s2: number, s3: number, s4: number, s5: number},
                      u: number, 
                      p: {alpha: number, lambda: number, gamma: number[], a: number[]}) {
    var e = state.s1-u ;
    if (e > 0.0)
      return {s1: -p.a[0]*Math.pow(e,1+p.lambda)+state.s2,
              s2: -p.a[1]*Math.pow(e,1+2*p.lambda)+state.s3,
              s3: -p.a[2]*Math.pow(e,1+3*p.lambda)+state.s4,
              s4: -p.a[3]*Math.pow(e,1+4*p.lambda)+state.s5,
              s5: -p.a[4]*Math.pow(e,1+5*p.lambda)
            };
    else
      return {s1: p.a[0]*Math.pow(-e,1+p.lambda)+state.s2,
              s2: p.a[1]*Math.pow(-e,1+2*p.lambda)+state.s3,
              s3: p.a[2]*Math.pow(-e,1+3*p.lambda)+state.s4,
              s4: p.a[3]*Math.pow(-e,1+4*p.lambda)+state.s5,
              s5: p.a[4]*Math.pow(-e,1+5*p.lambda)
            };   
  }

  public reset() {
    this.count_transients = 0;
    this.filter.reset();
  }

  private updateHOM(e: {x: number, y: number, t: number, state: string}, 
                    s: {s1: number, s2: number, s3: number, s4: number, s5: number},
                    x: number, 
                    p: {alpha: number, lambda: number, gamma: number[], a: number[]},
                    Der: number[],
                    steps_counter: number, 
                    axis: string) {
    let doReset =
      (e.state == "Interacting" && this.lastState != "Interacting")
      || (e.state != "Interacting" && this.lastState == "Interacting") ;

    if (doReset) {
      s.s1 = x /this.Sc;
      s.s2 = s.s3 = s.s4 = s.s5 = 0.0;
      Der[0] = Der[1] = Der[2] = 0.0;
      if (e.state == "Interacting" && this.lastState != "Interacting")
        steps_counter = this.transients_duration;
    } else {
      let dt = this.Ts / this.int_steps;
      let n;
      for (n = 0; n < this.int_steps; n++) {
        let diffhom = this.D_DiffHOMD4(s, x/this.Sc, p);
        s.s1 = s.s1 + diffhom.s1 * dt;
        s.s2 = s.s2 + diffhom.s2 * dt;
        s.s3 = s.s3 + diffhom.s3 * dt;
        s.s4 = s.s4 + diffhom.s4 * dt;
        s.s5 = s.s5 + diffhom.s5 * dt;
      }
      if (steps_counter > 0)
        steps_counter--;
    }
    
    let transient_scale = Math.max(1.0-steps_counter/(this.transients_duration/4.0), 0.0);
    Der[0] = s.s2 * this.Sc * transient_scale;
    Der[1] = s.s3 * this.Sc * transient_scale;
    Der[2] = s.s4 * this.Sc * transient_scale;

    this.lastState = e.state;

    return steps_counter
  }

  private predictIntern(x: number,
                        p: {alpha: number, lambda: number, gamma: number[], a: number[]},
                        Der: number[], 
                        alpha: number) {
    var Delta = 0;
    Delta += Der[0] * p.gamma[1];
    Delta += Der[1] * p.gamma[2];
    Delta += Der[2] * p.gamma[3];
    Delta += Math.pow(Der[0],2.0) * p.gamma[4];
    Delta += Math.pow(Der[1],2.0) * p.gamma[5];
    Delta += Math.pow(Der[2],2.0) * p.gamma[6];
    Delta += Math.abs(Der[0]) * p.gamma[7];
    Delta += Math.abs(Der[1]) * p.gamma[8];
    Delta += Math.abs(Der[2]) * p.gamma[9];
    return x + alpha*Delta;
  }

}