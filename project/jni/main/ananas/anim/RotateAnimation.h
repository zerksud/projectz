/*
 * RotateAnimation.h
 *
 *  Created on: Mar 26, 2013
 *      Author: zerksud
 */

#ifndef ROTATEANIMATION_H_
#define ROTATEANIMATION_H_

#include "Animation.h"

namespace ann {
namespace anim {

class RotateAnimation: public Animation {
public:
    RotateAnimation(gles::Transformable* object, float angle, float x, float y, float z, time_t duration);
    virtual ~RotateAnimation();

    virtual void setUpdateDeltaAsRemainder();
    virtual void setUpdateDeltaWithMult(float mult);
    virtual void transformWithCurrentUpdateDelta(gles::Transformable* object);

private:
    float mDelta;
    float mDeltaElapsed;
    float mUpdateDelta;
    glm::vec3 mAxes;
};

} /* namespace anim */
} /* namespace ann */
#endif /* ROTATEANIMATION_H_ */
