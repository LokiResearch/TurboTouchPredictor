/* -*- mode: c++ -*-
 *
 * lag/input/InputEvent.h --
 *
 * This Source Code is subject to the terms of
 * TURBOTOUCH PREDICTOR version 1.0 licence
 * available in LICENCE.txt
 *
 * Authors: Nicolas Roussel
 *
 *
 */

#ifndef lag_input_InputEvent_h
#define lag_input_InputEvent_h

#include <lag/utils/TimeStamp.h>

#include <iostream>

namespace lag {

  
  struct InputEvent {

    typedef enum {
      Relative=1,
      Absolute=2,
    } PositionType ;
    
    typedef enum {
      // See http://www.billbuxton.com/3state.html      
      OutOfRange=1,
      Tracking=2,
      Interacting=4
    } State ;
	  
    uint32_t source ;               // Source tag to know where the event comes from
    uint32_t channel ;              // Channel ID to support multiple channels (e.g. multitouch)
    PositionType postype  ;         // Is the position information absolute or relative?
    State state ;                   // Current interaction state
    TimeStamp::inttime t ;          // Time the event took place
    double x, y ;                   // Position

    static const uint32_t DefaultChannel ;
    
    InputEvent(void) ;
    InputEvent(uint32_t source,
	       uint32_t channel,
	       PositionType postype,
	       State state,
	       TimeStamp::inttime t, double x, double y) ;
    InputEvent(const InputEvent &other) ;

    void clear(void) ;

    static const char *postype2cstr(PositionType postype) ;
    static PositionType str2postype(std::string postype) ;

    static const char *state2cstr(State state) ;
    static State str2state(std::string state) ;

    std::string asString(bool readable=true) ;
    static InputEvent fromString(std::string s, bool readable=false) ;
    static InputEvent fromStream(std::istream &is, bool readable=false) ;
    
  } ;

}

#endif
