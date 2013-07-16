/*
 * Animation.cpp
 *
 *  Created on: Mar 26, 2013
 *      Author: zerksud
 */

#include "Animation.h"

namespace ann {
namespace anim {

Animation::Animation(gles::Transformable* object, time_t duration) {
    mObject = object;
    mDuration = duration;
    mElapsedDuration = 0;
}

Animation::~Animation() {
}

time_t Animation::animate(time_t dt) {
    time_t updateDuration = dt;

    if (updateDuration > mDuration - mElapsedDuration) {
        updateDuration = mDuration - mElapsedDuration;
        setUpdateDeltaAsRemainder();
    } else {
        float deltaMult = static_cast<float>(updateDuration) / mDuration;
        setUpdateDeltaWithMult(deltaMult);
    }

    mElapsedDuration += updateDuration;

    transformWithCurrentUpdateDelta(mObject);

    time_t timeRemainder = dt - updateDuration;

    return timeRemainder;
}

bool Animation::done() const {
    return (mDuration > 0 && mElapsedDuration >= mDuration);
}

bool Animation::instant() const {
    return (mDuration == 0);
}

} /* namespace anim */
} /* namespace ann */
