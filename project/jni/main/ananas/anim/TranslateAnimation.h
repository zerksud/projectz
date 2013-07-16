/*
 * TranslateAnimation.h
 *
 *  Created on: Mar 26, 2013
 *      Author: zerksud
 */

#ifndef TRANSLATEANIMATION_H_
#define TRANSLATEANIMATION_H_

#include "Animation.h"

namespace ann {
namespace anim {

class TranslateAnimation: public Animation {
public:
    TranslateAnimation(gles::Transformable* object, glm::vec3 diffVector, time_t duration);
    virtual ~TranslateAnimation();

    virtual void setUpdateDeltaAsRemainder();
    virtual void setUpdateDeltaWithMult(float mult);
    virtual void transformWithCurrentUpdateDelta(gles::Transformable* object);

private:
    glm::vec3 mDelta;
    glm::vec3 mElapsedDelta;
    glm::vec3 mUpdateDelta;
};

} /* namespace anim */
} /* namespace ann */
#endif /* TRANSLATEANIMATION_H_ */
