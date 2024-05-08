/* -*- mode: c++ -*-
 *
 * lag/input/processing/OneEuroProcessor.cpp --
 *
 * This Source Code is subject to the terms of
 * TURBOTOUCH PREDICTOR version 1.0 licence
 * available in LICENCE.txt
 *
 * Authors: Sébastien Poulmane
 *
 *
 */
#include "OneEuroProcessor.h"

namespace lag {

    const std::string OneEuroProcessor::scheme = "one-euro";

    OneEuroProcessor::OneEuroProcessor(URI uri, ConfigDict *cfg) {
        ConfigDict config ;
        if (cfg) config.updateFrom(*cfg) ;
        configureFromQueryString(config, scheme+"-freq", uri.query, "freq") ;
        configureFromQueryString(config, scheme+"-mincutoff", uri.query, "min-cutoff") ;
        configureFromQueryString(config, scheme+"-beta", uri.query, "beta") ;
        configureFromQueryString(config, scheme+"-dcutoff", uri.query, "dcutoff") ;
        _freq = config.setdefault<double>(scheme+"-freq", 60) ;
        _mincutoff = config.setdefault<double>(scheme+"-mincutoff", 1.0) ;
        _beta = config.setdefault<double>(scheme+"-beta", 0.002) ;
        _dcutoff =  config.setdefault<double>(scheme+"-dcutoff", 1.0) ;
        this->filter = new TwoEuroFilter(_freq, _mincutoff, _beta, _dcutoff) ;
    }

    URI
    OneEuroProcessor::getURI() {
        std::stringstream uri ;
        uri << scheme
            << ":?"
            << "freq=" << this->_freq
            << "&min-cutoff=" << this->_mincutoff
            << "&beta=" << this->_beta
            << "&dcutoff=" << this->_dcutoff ;
        return uri.str() ;
    }

    void
    OneEuroProcessor::reset() {
        filter->reset();
    }

    InputEvent
    OneEuroProcessor::process(const InputEvent *prediction, const InputEvent */*lag*/) {
        InputEvent filteredEvent(*prediction) ;
        this->filter->filter(prediction->x, prediction->y,
                             filteredEvent.x, filteredEvent.y, prediction->t) ;
        return filteredEvent ;
    }
}
