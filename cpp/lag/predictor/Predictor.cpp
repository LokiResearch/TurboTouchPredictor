/* -*- mode: c++ -*-
 *
 * lag/predictor/Predictor.h --
 *
 * This Source Code is subject to the terms of
 * TURBOTOUCH PREDICTOR version 1.0 licence
 * available in LICENCE.txt
 *
 * Authors: Nicolas Roussel
 *
 *
 */

#include <lag/predictor/Predictor.h>
#include <lag/utils/URI.h>

#include <lag/predictor/TurbotouchPredictor.h>

#include <algorithm>

#include <iostream>
#include <iomanip>

namespace lag {
  
  Predictor *
  Predictor::create(const char *uri, ConfigDict *cfg) {
    std::string uri_s ;
    if (uri) uri_s = uri ;
    return create(uri_s, cfg) ;
  }

  Predictor *
  Predictor::create(std::string uri_s, ConfigDict *cfg) {
    URI uri(uri_s) ;
    if (uri.scheme==TurbotouchPredictor::scheme) return new TurbotouchPredictor(uri, cfg);
    throw std::runtime_error("Unknown Predictor subclass ("+uri.scheme+")") ;
  }

  std::list<std::string> 
  Predictor::getSchemes(void) {
    std::list<std::string> schemes ;
    schemes.push_back(TurbotouchPredictor::scheme) ;
    return schemes ;
  }

  void
  Predictor::configureFromQueryString(ConfigDict &cfg,
				      const std::string &cfg_key,
				      const std::string &query,
				      const std::string &query_key) {
    std::string value ;
    URI::getQueryArg(query, query_key, &value) ;
    if (!value.empty()) cfg.set(cfg_key, value) ;
  }

}
