/* -*- mode: c++ -*-
 *
 * lag/input/source/InputEventProcessor.h --
 *
 * This Source Code is subject to the terms of
 * TURBOTOUCH PREDICTOR version 1.0 licence
 * available in LICENCE.txt
 *
 * Authors: Nicolas Roussel
 *
 *
 */

#ifndef lag_input_InputEventProcessor_h
#define lag_input_InputEventProcessor_h

#include <lag/input/InputEvent.h>
#include <lag/utils/URI.h>
#include <lag/utils/ConfigDict.h>

#include <list>

namespace lag {

  class InputEventProcessor {
    
  public:

    static InputEventProcessor *create(const char *uri, ConfigDict *cfg=0) ;
    static InputEventProcessor *create(std::string uri, ConfigDict *cfg=0) ;

    static std::list<std::string> getSchemes(void) ;

    virtual URI getURI(void) = 0 ;

    virtual void reset(void) = 0 ;
    virtual InputEvent process(const InputEvent *prediction, const InputEvent *lag=0) = 0 ;
    
    void configureFromQueryString(ConfigDict &cfg,
                                  const std::string &cfg_key,
                                  const std::string &query,
                                  const std::string &query_key) ;

    virtual ~InputEventProcessor(void) {}
  } ;

}

#endif
