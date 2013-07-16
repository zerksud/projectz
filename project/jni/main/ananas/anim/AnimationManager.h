/*
 * AnimationManager.h
 *
 *  Created on: Mar 26, 2013
 *      Author: zerksud
 */

#ifndef ANIMATIONMANAGER_H_
#define ANIMATIONMANAGER_H_

#include <vector>

#include <ananas/engine/Updatable.h>
#include <ananas/gles/Transformable.h>

#include "Animation.h"

namespace ann {
namespace anim {

class AnimationManager: public Updatable {
public:
    AnimationManager();
    virtual ~AnimationManager();

    virtual void update(time_t dt);

    void translate(gles::Transformable* object, glm::vec3 diffVector, time_t duration);
    void rotate(gles::Transformable* object, float angle, float x, float y, float z, time_t duration);

private:
    typedef std::vector<Animation*> AnimationList;

    static bool invalidTransformable(gles::Transformable* object);
    static bool invalidDuration(time_t duration);

    AnimationList mAnimations;
};

} /* namespace anim */
} /* namespace ann */
#endif /* ANIMATIONMANAGER_H_ */
