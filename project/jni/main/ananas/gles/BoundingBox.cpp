/*
 * BoundingBox.cpp
 *
 *  Created on: Mar 18, 2013
 *      Author: zerksud
 */

#include "BoundingBox.h"

#include <algorithm>

#include <ananas/util/ZLogger.h>

#define VERTEX_SIZE 3
#define CORNERS_COUNT   8

namespace ann {
namespace gles {

BoundingBox::BoundingBox() {
    mMinVertex = glm::vec3(0.0f);
    mMaxVertex = glm::vec3(0.0f);
    mExtent = EMPTY;
}

BoundingBox::BoundingBox(BoundingBox& other) :
        BoundingBox() {
    mExtent = other.mExtent;
    if (mExtent != EMPTY) {
        mMinVertex = glm::vec3(other.mMinVertex);
        mMaxVertex = glm::vec3(other.mMaxVertex);
    }
}

BoundingBox::BoundingBox(glm::vec3 minVertex, glm::vec3 maxVertex) :
        BoundingBox() {
    for (int i = 0; i < VERTEX_SIZE; ++i) {
        if (minVertex[i] > maxVertex[i]) {
            LOGE("minPoint[%d] > maxPoint[%d] - minPoint must be lesser than maxPoint", i, i);
            return;
        }
    }
    mMinVertex = glm::vec3(minVertex);
    mMaxVertex = glm::vec3(maxVertex);
    mExtent = FINITE;
}

BoundingBox::~BoundingBox() {
}

bool BoundingBox::isEmpty() const {
    return (mExtent == EMPTY);
}

const glm::vec3& BoundingBox::getMinPoint() const {
    if (isEmpty()) {
        LOGE("Can't get min point from empty BoundingBox");
    }
    return mMinVertex;
}

const glm::vec3& BoundingBox::getMaxPoint() const {
    if (isEmpty()) {
        LOGE("Can't get max point from empty BoundingBox");
    }
    return mMaxVertex;
}

void BoundingBox::transform(glm::mat4 matrix) {
    if (isEmpty()) {
        return;
    }

    glm::vec4 *corners = new glm::vec4[CORNERS_COUNT];
    corners[0] = glm::vec4(mMinVertex, 1.0f);

    corners[1] = glm::vec4(corners[0]);
    corners[1][0] = mMaxVertex[0];

    corners[2] = glm::vec4(corners[1]);
    corners[2][1] = mMaxVertex[1];

    corners[3] = glm::vec4(corners[2]);
    corners[3][0] = mMinVertex[0];

    corners[4] = glm::vec4(corners[3]);
    corners[4][2] = mMaxVertex[2];

    corners[5] = glm::vec4(corners[4]);
    corners[5][1] = mMinVertex[1];

    corners[6] = glm::vec4(corners[5]);
    corners[6][0] = mMaxVertex[0];

    corners[7] = glm::vec4(corners[6]);
    corners[7][1] = mMaxVertex[1];

    for (int i = 0; i < CORNERS_COUNT; ++i) {
        corners[i] = matrix * corners[i];   //TODO: multiplication order?
    }

    mMinVertex = glm::vec3(corners[0]);
    mMaxVertex = glm::vec3(corners[0]);

    for (int i = 1; i < CORNERS_COUNT; ++i) {
        for (int j = 0; j < VERTEX_SIZE; ++j) {
            mMinVertex[j] = std::min(mMinVertex[j], corners[i][j]);
            mMaxVertex[j] = std::max(mMaxVertex[j], corners[i][j]);
        }
    }
}

void BoundingBox::merge(const BoundingBox* other) {
    if (other == nullptr) {
        LOGE("Can't merge with nullptr BoundingBox");
        return;
    }
    if (other->isEmpty()) {
        return;
    }

    if (isEmpty()) {
        mExtent = other->mExtent;
        mMinVertex = glm::vec3(other->mMinVertex);
        mMaxVertex = glm::vec3(other->mMaxVertex);
    } else {
        for (int j = 0; j < VERTEX_SIZE; ++j) {
            mMinVertex[j] = std::min(mMinVertex[j], other->mMinVertex[j]);
            mMaxVertex[j] = std::max(mMaxVertex[j], other->mMaxVertex[j]);
        }
    }
}

} /* namespace gles */
} /* namespace ann */
