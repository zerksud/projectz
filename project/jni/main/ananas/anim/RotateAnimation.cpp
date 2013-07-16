/*
 * RotateAnimation.cpp
 *
 *  Created on: Mar 26, 2013
 *      Author: zerksud
 */

#include "RotateAnimation.h"

namespace ann {
namespace anim {

RotateAnimation::RotateAnimation(gles::Transformable* object, float angle, float x, float y, float z, time_t duration) :
        Animation(object, duration) {
    mDelta = angle;
    mDeltaElapsed = 0.0f;
    mUpdateDelta = 0.0f;
    mAxes = glm::vec3(x, y, z);
}

RotateAnimation::~RotateAnimation() {
}

void RotateAnimation::setUpdateDeltaAsRemainder() {
    mUpdateDelta = mDelta - mDeltaElapsed;
}

void RotateAnimation::setUpdateDeltaWithMult(float mult) {
    mUpdateDelta = mult * mDelta;
}

void RotateAnimation::transformWithCurrentUpdateDelta(gles::Transformable* object) {
    object->rotate(mUpdateDelta, mAxes.x, mAxes.y, mAxes.z);

    mDeltaElapsed += mUpdateDelta;
}

} /* namespace anim */
} /* namespace ann */
