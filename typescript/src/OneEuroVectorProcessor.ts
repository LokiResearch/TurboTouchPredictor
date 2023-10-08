/**
 *
 * This Source Code is subject to the terms of
 * TURBOTOUCH PREDICTOR version 1.0 licence
 * available in LICENCE.txt
 *
 * Author: Gery Casiez
 */

import { OneEuroFilter } from '1eurofilter'

export class OneEuroVectorProcessor {
    private normFilter : OneEuroFilter;
    private filterX : OneEuroFilter;
    private filterY : OneEuroFilter;

	constructor(freq : number, mincutoff : number, beta : number) {
        this.normFilter = new OneEuroFilter(freq, mincutoff, beta, 1);
        this.filterX = new OneEuroFilter(freq, mincutoff, beta, 1);
        this.filterY = new OneEuroFilter(freq, mincutoff, beta, 1);
    }

    public setParameters(freq : number, mincutoff : number, beta : number) {
        this.normFilter.setParameters(freq, mincutoff, beta);
        this.filterX.setParameters(freq, mincutoff, beta);
        this.filterY.setParameters(freq, mincutoff, beta);
        this.reset();
    }

    reset() {
        this.normFilter.reset();
        this.filterX.reset();
        this.filterY.reset();
    }

    process(prediction: {x: number, y: number, t:number}, lag: {x: number, y: number}) {
        let dX = prediction.x - lag.x ;
        let dY = prediction.y - lag.y ;
        let norm = Math.sqrt(dX*dX + dY*dY);

        if (norm === undefined || norm == 0)
            return prediction;

        let filteredNorm = this.normFilter.filter(norm, prediction.t);

        let filteredEvent = {x: undefined, y: undefined};
        filteredEvent.x = lag.x + dX / norm * filteredNorm;
        filteredEvent.y = lag.y + dY / norm * filteredNorm;

        let filteredEvent2 = {x: undefined, y: undefined};
        filteredEvent2.x = this.filterX.filter(filteredEvent.x, prediction.t);
        filteredEvent2.y = this.filterY.filter(filteredEvent.y, prediction.t);

        return filteredEvent2;
    }
}