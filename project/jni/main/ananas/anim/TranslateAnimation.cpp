/*
 * TranslateAnimation.cpp
 *
 *  Created on: Mar 26, 2013
 *      Author: zerksud
 */

#include "TranslateAnimation.h"

#include <ananas/util/ZLogger.h>

namespace ann {
namespace anim {

TranslateAnimation::TranslateAnimation(gles::Transformable* object, glm::vec3 diffVector, time_t duration) :
        Animation(object, duration) {
    mDelta = diffVector;
}

TranslateAnimation::~TranslateAnimation() {
}

void TranslateAnimation::setUpdateDeltaAsRemainder() {
    mUpdateDelta = mDelta - mElapsedDelta;
}

void TranslateAnimation::setUpdateDeltaWithMult(float mult) {
    mUpdateDelta = mult * mDelta;
}

void TranslateAnimation::transformWithCurrentUpdateDelta(gles::Transformable* object) {
    object->translate(mUpdateDelta);

    mElapsedDelta += mUpdateDelta;
}

} /* namespace anim */
} /* namespace ann */
