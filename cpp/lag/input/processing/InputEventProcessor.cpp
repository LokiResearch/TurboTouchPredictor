/* -*- mode: c++ -*-
 *
 * lag/input/source/InputEventProcessor.cpp --
 *
 * This Source Code is subject to the terms of
 * TURBOTOUCH PREDICTOR version 1.0 licence
 * available in LICENCE.txt
 *
 * Authors: Nicolas Roussel
 *
 *
 */

#include <lag/input/processing/InputEventProcessor.h>
#include <lag/input/processing/OneEuroProcessor.h>
#include <lag/input/processing/OneEuroVectorProcessor.h>

namespace lag {
    
  InputEventProcessor *
  InputEventProcessor::create(const char *uri, ConfigDict *cfg) {
    std::string uri_s ;
    if (uri) uri_s = uri ;
    return create(uri_s, cfg) ;
  }

  InputEventProcessor *
  InputEventProcessor::create(std::string uri_s, ConfigDict *cfg) {
    URI uri(uri_s) ;
    if (uri.scheme==OneEuroProcessor::scheme) return new OneEuroProcessor(uri, cfg) ;
    if (uri.scheme==OneEuroVectorProcessor::scheme) return new OneEuroVectorProcessor(uri, cfg) ;
    // if (uri.scheme==...::scheme) return new ...(uri) ;
    return NULL ; // A null processor means that it will do nothing on input events.
  }

  std::list<std::string> 
  InputEventProcessor::getSchemes(void) {
    std::list<std::string> schemes ;
    schemes.push_back(OneEuroProcessor::scheme) ;
    // schemes.push_back(...::scheme) ;
    return schemes ;
  }

  void
  InputEventProcessor::configureFromQueryString(ConfigDict &cfg,
                      const std::string &cfg_key,
                      const std::string &query,
                      const std::string &query_key) {
    std::string value ;
    URI::getQueryArg(query, query_key, &value) ;
    if (!value.empty()) cfg.set(cfg_key, value) ;
  }
  
}
