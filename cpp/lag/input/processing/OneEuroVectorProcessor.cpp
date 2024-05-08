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
#include "OneEuroVectorProcessor.h"
#include <cmath>

namespace  lag {

    const std::string OneEuroVectorProcessor::scheme = "one-euro-vector";

    OneEuroVectorProcessor::OneEuroVectorProcessor(URI uri, ConfigDict *cfg)
        : OneEuroProcessor(uri, cfg) {

        this->normFilter = new OneEuroFilter(this->_freq, this->_mincutoff,
                                             this->_beta, this->_dcutoff) ;
        this->filterX = new OneEuroFilter(this->_freq, this->_mincutoff,
                                             this->_beta, this->_dcutoff) ;
        this->filterY = new OneEuroFilter(this->_freq, this->_mincutoff,
                                             this->_beta, this->_dcutoff) ;
    }

    URI
    OneEuroVectorProcessor::getURI() {
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
    OneEuroVectorProcessor::reset() {
        this->normFilter->reset() ;
        this->filterX->reset();
        this->filterY->reset();
    }

    InputEvent
    OneEuroVectorProcessor::process(const InputEvent *prediction, const InputEvent *lag) {
        double dX = prediction->x - lag->x ;
        double dY = prediction->y - lag->y ;
        double norm = sqrt(dX*dX + dY*dY) ;

        if (!norm)
            return *prediction;

        double filteredNorm = this->normFilter->filter(norm, prediction->t) ;

        InputEvent filteredEvent(*prediction);
        filteredEvent.x = lag->x + dX / norm * filteredNorm ;
        filteredEvent.y = lag->y + dY / norm * filteredNorm ;

        InputEvent filteredEvent2(*prediction);
        filteredEvent2.x = this->filterX->filter(filteredEvent.x, prediction->t);
        filteredEvent2.y = this->filterY->filter(filteredEvent.y, prediction->t);

        return filteredEvent2 ;
    }
}

