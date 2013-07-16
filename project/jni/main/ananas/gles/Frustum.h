/*
 * Frustum.h
 *
 *  Created on: Mar 19, 2013
 *      Author: zerksud
 */

#ifndef FRUSTUM_H_
#define FRUSTUM_H_

#include <glm/glm.hpp>

#include "BoundingBox.h"

namespace ann {
namespace gles {

class Frustum {
public:
    Frustum(const glm::mat4& vpMatrix);
    virtual ~Frustum();

    bool isVisible(const BoundingBox&, const glm::mat4& mMatrix) const;

private:
    const static int kPlaneCount = 6;
    glm::vec4 mPlanes[kPlaneCount];
};

} /* namespace gles */
} /* namespace ann */
#endif /* FRUSTUM_H_ */
