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

#ifndef lag_predictor_Predictor_h
#define lag_predictor_Predictor_h

#include <lag/input/InputEvent.h>
#include <lag/utils/URI.h>
#include <lag/utils/ConfigDict.h>


#include <list>
#include <string>

namespace lag {

  class Predictor {

  protected:

    void configureFromQueryString(ConfigDict &cfg, const std::string &cfg_key,
				  const std::string &query, const std::string &query_key) ;
	
  public:

    static Predictor *create(const char *uri, ConfigDict *cfg=0) ;
    static Predictor *create(std::string uri, ConfigDict *cfg=0) ;

    static std::list<std::string> getSchemes(void) ;
    
    virtual URI getURI(void) = 0 ;
    virtual void reset(void) = 0 ;
    virtual InputEvent update(InputEvent &e, TimeStamp::inttime lag=TimeStamp::undef) = 0 ;

    virtual ~Predictor(void) {}
    
  } ;

}

#endif
