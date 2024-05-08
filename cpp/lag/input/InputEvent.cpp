/* -*- mode: c++ -*-
 *
 * lag/input/InputEvent.cpp --
 *
 * This Source Code is subject to the terms of
 * TURBOTOUCH PREDICTOR version 1.0 licence
 * available in LICENCE.txt
 *
 * Authors: Nicolas Roussel
 *
 *
 */

#include <lag/input/InputEvent.h>

#include <stdexcept>
#include <sstream>

namespace lag {

  const uint32_t InputEvent::DefaultChannel = 0 ;
  
  // -----------------------------------------------------------------------
  
  const char *
  InputEvent::postype2cstr(PositionType s) {
    if (s==Absolute) return "Absolute" ;
    if (s==Relative) return "Relative" ;
    std::cerr << "InputEvent::postype2cstr argument (" << s << ") is not a valid PositionType" << std::endl ;
    return "???" ;
  }

  InputEvent::PositionType
  InputEvent::str2postype(std::string s) {
    if (s=="Absolute") return Absolute ;
    if (s=="Relative") return Relative ;
    std::cerr << "InputEvent::str2postype argument (" << s << ") is not a valid PositionType" << std::endl ;
    return Absolute ;
  }

  const char *
  InputEvent::state2cstr(State s) {
    if (s==OutOfRange) return "OutOfRange" ;
    if (s==Tracking) return "Tracking" ;
    if (s==Interacting) return "Interacting" ;
    std::cerr << "InputEvent::state2cstr argument (" << s << ") is not a valid State" << std::endl ;
    return "OutOfRange" ;
  }

  InputEvent::State
  InputEvent::str2state(std::string s) {
    if (s=="Tracking") return Tracking ;
    if (s=="Interacting") return Interacting ;
    if (s=="OutOfRange") return OutOfRange ;
    std::cerr << "InputEvent::str2state argument (" << s << ") is not a valid State" << std::endl ;
    return OutOfRange ;
  }

  // -----------------------------------------------------------------------
  
  InputEvent::InputEvent(void) {
    clear() ;
  }

  InputEvent::InputEvent(uint32_t source, 
			 uint32_t channel,
			 PositionType postype,
			 State state,
			 TimeStamp::inttime t, double x, double y) {
    this->source = source ;
    this->channel = channel ;
    this->postype = postype ;
    this->state = state ;    
    this->t = t ;
    this->x = x ;
    this->y = y ;
  }

  InputEvent::InputEvent(const InputEvent &other) {
    source = other.source ;
    channel = other.channel ;
    postype = other.postype ;    
    state = other.state ;    
    t = other.t ;
    this->x = other.x ;
    this->y = other.y ;
  }

  // -----------------------------------------------------------------------

  void
  InputEvent::clear(void) {
    source = 0 ;
    channel = 0 ;
    postype = Absolute ;
    state = OutOfRange ;
    t = TimeStamp::undef ;    
    x = y = 0.0 ;
  }
  
  // -----------------------------------------------------------------------
  
  std::string
  InputEvent::asString(bool readable) {
    std::stringstream result ;
    if (readable)
      result << "0"
	     << " " << channel
	     << " " << postype2cstr(postype)
	     << " " << state2cstr(state)	
	     << " " << TimeStamp::createAsStringFrom(t)
	     << " " << x << " " << y ;
    else
      result << source
	     << " " << channel
	     << " " << int(postype)	
	     << " " << int(state)
	     << " " << t
	     << " " << x << " " << y ;
    return result.str() ;
  }

  InputEvent
  InputEvent::fromString(std::string s, bool readable) {
    std::stringstream is(s) ;
    return fromStream(is, readable) ;
  }
  
  InputEvent
  InputEvent::fromStream(std::istream &is, bool readable) {
    uint32_t source, channel ;
    double x = 0.0, y = 0.0 ;
    if (readable) {
      std::string state, postype, t ;
      is >> source >> channel >> postype >> state >> t >> x >> y ;
      return InputEvent(source,
			channel, str2postype(postype),
			str2state(state),
			TimeStamp::createAsIntFrom(t), x, y) ;
    } else {
      int state = 0, postype = 0 ;      
      TimeStamp::inttime t = TimeStamp::undef ;
      is >> source >> channel >> postype >> state >> t >> x >> y ;
      return InputEvent(source,
			channel, PositionType(postype),
			State(state),
			t, x, y) ;
    }
  }

}
