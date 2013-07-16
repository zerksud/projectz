/*
 * Camera.cpp
 *
 *  Created on: Mar 19, 2013
 *      Author: zerksud
 */

#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include <ananas/util/ZLogger.h>

namespace ann {
namespace gles {

Camera::Camera() {
    mParent = nullptr;
}

Camera::~Camera() {
}

void Camera::attachToNode(SceneNode* node) {
    mParent = node;
}

SceneNode* Camera::getParentNode() const {
    return mParent;
}

glm::mat4 Camera::getViewMatrix() const {
    if (mParent == nullptr) {
        LOGE("Can't construct viewMatrix for Camera not attached to any node");
        return glm::mat4(1.0f);
    }

    const glm::mat4& globalMatrix = mParent->getGlobalMatrix();

    glm::vec4 eye = globalMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 center = globalMatrix * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
    glm::vec4 up = globalMatrix * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    up -= eye;

    glm::mat4 viewMatrix = glm::lookAt(glm::vec3(eye), glm::vec3(center), glm::vec3(up));

    return viewMatrix;
}

} /* namespace gles */
} /* namespace ann */
