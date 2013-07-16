/*
 * Camera.h
 *
 *  Created on: Mar 19, 2013
 *      Author: zerksud
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <glm/glm.hpp>

#include "SceneNode.h"

namespace ann {
namespace gles {

class Camera {
public:
    Camera();
    virtual ~Camera();

    void attachToNode(SceneNode* node);
    SceneNode* getParentNode() const;
    glm::mat4 getViewMatrix() const;

private:
    SceneNode* mParent;
};

} /* namespace gles */
} /* namespace ann */
#endif /* CAMERA_H_ */
