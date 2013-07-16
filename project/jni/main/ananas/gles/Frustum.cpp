/*
 * Frustum.cpp
 *
 *  Created on: Mar 19, 2013
 *      Author: zerksud
 */

#include "Frustum.h"

namespace ann {
namespace gles {

Frustum::Frustum(const glm::mat4& vpMatrix) {
    const glm::mat4& m = vpMatrix;

    for (int i = 0; i < 4; ++i) {
        mPlanes[0][i] = m[i][3] - m[i][0]; // right plane
        mPlanes[1][i] = m[i][3] + m[i][0]; // left plane
        mPlanes[2][i] = m[i][3] + m[i][1]; // bottom plane
        mPlanes[3][i] = m[i][3] - m[i][1]; // top plane
        mPlanes[4][i] = m[i][3] - m[i][2]; // far plane
        mPlanes[5][i] = m[i][3] + m[i][2]; // near plane
    }

    for (int i = 0; i < kPlaneCount; ++i) {
        mPlanes[i] = glm::normalize(mPlanes[i]);
    }
}

Frustum::~Frustum() {
}

bool Frustum::isVisible(const BoundingBox& bb, const glm::mat4& mMatrix) const {
    if (bb.isEmpty()) {
        return false;
    }

    glm::vec4 minP = mMatrix * glm::vec4(bb.getMinPoint(), 1.0f);
    glm::vec4 maxP = mMatrix * glm::vec4(bb.getMaxPoint(), 1.0f);

    float x1 = minP[0];
    float y1 = minP[1];
    float z1 = minP[2];
    float x2 = maxP[0];
    float y2 = maxP[1];
    float z2 = maxP[2];

    for (int i = 0; i < kPlaneCount; i++) {
        const glm::vec4& p = mPlanes[i];

        if (p[0] * x1 + p[1] * y1 + p[2] * z1 + p[3] > 0) {
            continue;
        }
        if (p[0] * x2 + p[1] * y1 + p[2] * z1 + p[3] > 0) {
            continue;
        }
        if (p[0] * x1 + p[1] * y2 + p[2] * z1 + p[3] > 0) {
            continue;
        }
        if (p[0] * x2 + p[1] * y2 + p[2] * z1 + p[3] > 0) {
            continue;
        }
        if (p[0] * x1 + p[1] * y1 + p[2] * z2 + p[3] > 0) {
            continue;
        }
        if (p[0] * x2 + p[1] * y1 + p[2] * z2 + p[3] > 0) {
            continue;
        }
        if (p[0] * x1 + p[1] * y2 + p[2] * z2 + p[3] > 0) {
            continue;
        }
        if (p[0] * x2 + p[1] * y2 + p[2] * z2 + p[3] > 0) {
            continue;
        }

        return false;
    }

    return true;
}

} /* namespace gles */
} /* namespace ann */
