/* -*- mode: c++ -*-
 *
 * lag/input/processing/OneEuroProcessor.h --
 *
 * This Source Code is subject to the terms of
 * TURBOTOUCH PREDICTOR version 1.0 licence
 * available in LICENCE.txt
 *
 * Authors: Sébastien Poulmane
 *
 *
 */
#ifndef ONEEUROPROCESSOR_H
#define ONEEUROPROCESSOR_H

#include <lag/input/processing/InputEventProcessor.h>
#include <lag/utils/OneEuroFilter.h>
#include <lag/utils/URI.h>

namespace lag {

    class OneEuroProcessor : public InputEventProcessor {

        TwoEuroFilter *filter ;

    protected:
        double _freq ;
        double _mincutoff ;
        double _beta ;
        double _dcutoff ;

    public:

        static const std::string scheme;

        OneEuroProcessor(URI uri, ConfigDict *cfg=0) ;
        virtual ~OneEuroProcessor(void) {
            if (filter)
                delete filter;
        }
        virtual URI getURI() ;
        void reset() ;
        InputEvent process(const InputEvent *prediction, const InputEvent *lag=0) ;
    };
}

#endif // ONEEUROPROCESSOR_H
