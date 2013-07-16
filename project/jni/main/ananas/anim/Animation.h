/*
 * Animation.h
 *
 *  Created on: Mar 26, 2013
 *      Author: zerksud
 */

#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <ctime>

#include <ananas/gles/Transformable.h>

namespace ann {
namespace anim {

class Animation {
public:
    Animation(gles::Transformable* object, time_t duration);
    virtual ~Animation();

    time_t animate(time_t dt);
    bool done() const;
    bool instant() const;

protected:
    virtual void setUpdateDeltaAsRemainder() = 0;
    virtual void setUpdateDeltaWithMult(float mult) = 0;
    virtual void transformWithCurrentUpdateDelta(gles::Transformable* object) = 0;

private:
    gles::Transformable* mObject;
    time_t mDuration;
    time_t mElapsedDuration;
};

} /* namespace anim */
} /* namespace ann */
#endif /* ANIMATION_H_ */
