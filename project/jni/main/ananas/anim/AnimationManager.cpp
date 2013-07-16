/*
 * AnimationManager.cpp
 *
 *  Created on: Mar 26, 2013
 *      Author: zerksud
 */

#include "AnimationManager.h"

#include <algorithm>

#include "TranslateAnimation.h"
#include "RotateAnimation.h"

#include <ananas/util/ZLogger.h>

namespace ann {
namespace anim {

AnimationManager::AnimationManager() {
}

AnimationManager::~AnimationManager() {
}

void AnimationManager::update(time_t dt) {
    time_t durationLeft = dt;
    AnimationList animations2Erase;
    for (Animation* animation : mAnimations) {
        if (durationLeft <= 0 && !animation->instant()) {
            break;
        }

        durationLeft = animation->animate(durationLeft);
        if (animation->done()) {
            animations2Erase.push_back(animation);
        }
    }

    mAnimations.erase(std::remove_if(mAnimations.begin(), mAnimations.end(), [&animations2Erase](Animation* animation) {
        AnimationList::iterator pos = std::find(animations2Erase.begin(), animations2Erase.end(), animation);
        if (pos != animations2Erase.end()) {
            delete animation;
            animations2Erase.erase(pos);

            return true;
        }

        return false;
    }), mAnimations.end());
}

void AnimationManager::translate(gles::Transformable* object, glm::vec3 diffVector, time_t duration) {
    if (invalidTransformable(object) || invalidDuration(duration)) {
        return;
    }

    Animation* animation = new TranslateAnimation(object, diffVector, duration);
    mAnimations.push_back(animation);
}

void AnimationManager::rotate(gles::Transformable* object, float angle, float x, float y, float z, time_t duration) {
    if (invalidTransformable(object) || invalidDuration(duration)) {
        return;
    }

    Animation* animation = new RotateAnimation(object, angle, x, y, z, duration);
    mAnimations.push_back(animation);
}

bool AnimationManager::invalidTransformable(gles::Transformable* object) {
    if (object == nullptr) {
        LOGE("Can't transform nullptr object");
        return true;
    }

    return false;
}

bool AnimationManager::invalidDuration(time_t duration) {
    if (duration < 0) {
        LOGE("Can't transform object for negative duration %ld", duration);
        return true;
    }

    return false;
}

} /* namespace anim */
} /* namespace ann */
