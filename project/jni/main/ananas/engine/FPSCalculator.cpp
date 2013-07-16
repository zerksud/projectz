/*
 * FPSCalculator.cpp
 *
 *  Created on: Mar 29, 2013
 *      Author: zerksud
 */

#include "FPSCalculator.h"

#include <ananas/util/StringHelpers.h>
#include <ananas/util/CommonHelpers.h>

namespace ann {

FPSCalculator::FPSCalculator() {
    mFrameCount = 0;
    mCollectedTime = 0;
}

FPSCalculator::~FPSCalculator() {
}

void FPSCalculator::update(time_t dt) {
    mFrameCount++;
    mCollectedTime += dt;

    if (mCollectedTime >= kFPSUpdatePeriod) {
        float fps = static_cast<float>(mFrameCount) / mCollectedTime * 1000.0f;
        mFrameCount = 0;
        mCollectedTime = 0;

        util::helpers::logDebugValueChange("fps", util::stringFormat("%.2f", fps));
    }
}

} /* namespace ann */
