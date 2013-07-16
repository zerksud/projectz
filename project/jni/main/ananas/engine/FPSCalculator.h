/*
 * FPSCalculator.h
 *
 *  Created on: Mar 29, 2013
 *      Author: zerksud
 */

#ifndef FPSCALCULATOR_H_
#define FPSCALCULATOR_H_

#include "Updatable.h"

namespace ann {

class FPSCalculator: public Updatable {
public:
    FPSCalculator();
    virtual ~FPSCalculator();

    virtual void update(time_t dt);

private:
    static const int kFPSUpdatePeriod = 500;

    int mFrameCount;
    time_t mCollectedTime;
};

} /* namespace ann */
#endif /* FPSCALCULATOR_H_ */
