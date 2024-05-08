/* -*- mode: c++ -*-
 *
 * lag/input/processing/OneEuroVectorProcessor.cpp --
 *
 * This Source Code is subject to the terms of
 * TURBOTOUCH PREDICTOR version 1.0 licence
 * available in LICENCE.txt
 *
 * Authors: Sébastien Poulmane
 *
 *
 * OneEuroVectorProcessor is like OneEuroProcssor but act on the vector
 * between the last lagging point and last predicted point instead of
 * predicted point positions.
 *
 */
#ifndef ONEEUROVECTORPROCESSOR_H
#define ONEEUROVECTORPROCESSOR_H

#include <lag/input/processing/OneEuroProcessor.h>
#include <lag/utils/URI.h>


namespace lag {

    class OneEuroVectorProcessor : public OneEuroProcessor {

        OneEuroFilter *normFilter ;
        OneEuroFilter *filterX;
        OneEuroFilter *filterY;

    public:

        static const std::string scheme ;

        OneEuroVectorProcessor(URI, ConfigDict*) ;
        virtual URI getURI() ;
        void reset() ;
        InputEvent process(const InputEvent *prediction, const InputEvent *lag) ;
    };
}

#endif // ONEEUROVECTORPROCESSOR_H
