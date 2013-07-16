/*
 * SceneNode.cpp
 *
 *  Created on: Mar 19, 2013
 *      Author: zerksud
 */

#include "SceneNode.h"

#include <algorithm>

#include <glm/gtx/transform.hpp>

#include <ananas/util/ZLogger.h>

namespace ann {
namespace gles {

SceneNode::SceneNode() {
    mParentNode = nullptr;
    mModelMatrixNeedsUpdate = true;
    mGlobalMatrixNeedsUpdate = true;
    mChildGlobalMatrixNeedsUpdate = false;  //TODO: default value on java side?

    mEntity = nullptr;

    mBoundingBox = new BoundingBox();
    mBoundingBoxNeedsUpdate = true;
}

SceneNode::SceneNode(const SceneNode& other) {
    mParentNode = nullptr;
    mPositionMatrix = other.mPositionMatrix;
    mRotationMatrix = other.mRotationMatrix;
    mScaleMatrix = other.mScaleMatrix;
    mModelMatrixNeedsUpdate = true; //TODO: copy model matrix from other?

    mGlobalMatrixNeedsUpdate = true;
    mChildNodes.reserve(other.mChildNodes.size());
    for (SceneNode* childNode : other.getChildNodes()) {
        SceneNode *copyNode = new SceneNode(*childNode);
        attachNode(copyNode);
    }

    mEntity = nullptr;
    if (other.mEntity != nullptr) {
        attachEntity(*other.mEntity);
    }

    mBoundingBox = new BoundingBox(*other.mBoundingBox);
    mBoundingBoxNeedsUpdate = other.mBoundingBoxNeedsUpdate;
}

SceneNode::~SceneNode() {
    for (SceneNode* childNode : mChildNodes) {
        delete childNode;
    }

    clearEntity();

    delete mBoundingBox;
}

SceneNode* SceneNode::getParent() const {
    return mParentNode;
}

BoundingBox* SceneNode::getBoundingBox() const {
    return mBoundingBox;
}

void SceneNode::updateBoundingBox() {
    if (mBoundingBoxNeedsUpdate) {
        delete mBoundingBox;    //TODO: reset method for inplace update?

        mBoundingBox = new BoundingBox();

        for (SceneNode* node : mChildNodes) {
            node->updateBoundingBox();
            BoundingBox* bb = new BoundingBox(*node->getBoundingBox());
            bb->transform(node->getModelMatrix());
            mBoundingBox->merge(bb);
            delete bb;  //TODO: remove continuous memory allocation?
        }

        if (mEntity != nullptr) {
            mBoundingBox->merge(mEntity->getMesh()->getBoundingBox());
        }

        mBoundingBoxNeedsUpdate = false;
    }
}

bool SceneNode::geometryChanged() const {
    return mBoundingBoxNeedsUpdate || mChildGlobalMatrixNeedsUpdate;
}

void SceneNode::attachNode(SceneNode* node) {
    if (node == nullptr) {
        LOGE("Can't attach nullptr node");
        return;
    }

    if (std::find(mChildNodes.begin(), mChildNodes.end(), node) == mChildNodes.end()) {
        if (node->mParentNode != nullptr) {
            node->mParentNode->detachNode(node);
        }
        node->mParentNode = this;
        mChildNodes.push_back(node);

        notifyBoundingBoxNeedsUpdate();
        notifyChildGlobalMatrixNeedsUpdate();   //TODO: why?
    } else {
        LOGE("Can't attach already attached node");
    }
}

void SceneNode::detachNode(SceneNode* node) {
    if (node == nullptr) {
        LOGE("Can't detach nullptr node");
        return;
    }

    SceneNodeList::iterator nodePos = std::find(mChildNodes.begin(), mChildNodes.end(), node);
    if (nodePos != mChildNodes.end()) {
        node->mParentNode = nullptr;
        mChildNodes.erase(nodePos);

        notifyBoundingBoxNeedsUpdate();
    } else {
        LOGE("Can't detach not attached node");
    }
}

void SceneNode::detachAllNodes() {
    for (SceneNode* node : mChildNodes) {
        node->mParentNode = nullptr;    //TODO: who owns SceneNodes? SceneManager?
    }

    mChildNodes.clear();
    notifyBoundingBoxNeedsUpdate();
}

const SceneNodeList& SceneNode::getChildNodes() const {
    return mChildNodes;
}

void SceneNode::clearEntity() {
    if (mEntity != nullptr) {
        delete mEntity;
        mEntity = nullptr;
    }
}

void SceneNode::attachEntity(const Entity& entity) {
    if (mEntity != nullptr && *mEntity == entity) {
        LOGE("Can't attach already attached entity");
        return;
    }

    Entity* entityCopy = new Entity(entity);
    setupEntityChangedCallback(entityCopy);

    clearEntity();
    mEntity = entityCopy;

    notifyBoundingBoxNeedsUpdate();
}

void SceneNode::detachEntity() {
    if (mEntity != nullptr) {
        clearEntity();  // Yeah, sad double check
        notifyBoundingBoxNeedsUpdate();
    } else {
        LOGE("Can't detach nonexistent attached entity");
    }
}

const Entity* SceneNode::getEntity() const {
    return mEntity;
}

const glm::mat4& SceneNode::getModelMatrix() const {
    if (mModelMatrixNeedsUpdate) {
        mModelMatrix = mPositionMatrix * mRotationMatrix * mScaleMatrix;
        mModelMatrixNeedsUpdate = false;
    }

    return mModelMatrix;
}

void SceneNode::translate(float dx, float dy, float dz) {
    multPositionMatrixLeft(glm::translate(dx, dy, dz));
}

void SceneNode::translate(glm::vec3 diffVector) {
    translate(diffVector.x, diffVector.y, diffVector.z);
}

void SceneNode::rotate(float angle, float x, float y, float z) {
    multRotationMatrixLeft(glm::rotate(angle, x, y, z));
}

void SceneNode::scale(float s) {
    if (s <= 0.0f) {
        LOGE("Can't scale with factor %f", s);
        return;
    }

    mScaleMatrix *= glm::scale(s, s, s);

    mModelMatrixNeedsUpdate = true;
    notifyGlobalMatrixUpdate();
    notifyParentBoundingBoxNeedsUpdate();
}

const glm::mat4& SceneNode::getGlobalMatrix() const {
    return mGlobalMatrix;
}

void SceneNode::updateGlobalMatrix() {
    if (mGlobalMatrixNeedsUpdate) {
        if (mParentNode != nullptr) {
            mGlobalMatrix = mParentNode->getGlobalMatrix() * getModelMatrix();
        } else {
            mGlobalMatrix = getModelMatrix();
        }

        mGlobalMatrixNeedsUpdate = false;
    }

    if (mChildGlobalMatrixNeedsUpdate) {
        for (SceneNode* node : mChildNodes) {
            node->updateGlobalMatrix();
        }
        mChildGlobalMatrixNeedsUpdate = false;
    }
}

void SceneNode::notifyBoundingBoxNeedsUpdate() {
    if (!mBoundingBoxNeedsUpdate) {
        mBoundingBoxNeedsUpdate = true;
        notifyParentBoundingBoxNeedsUpdate();
    }
}

void SceneNode::notifyParentBoundingBoxNeedsUpdate() {
    if (mParentNode != nullptr) {
        mParentNode->notifyBoundingBoxNeedsUpdate();
    }
}

void SceneNode::multPositionMatrixLeft(const glm::mat4& matrix) {
    mPositionMatrix = matrix * mPositionMatrix;

    mModelMatrixNeedsUpdate = true;
    notifyGlobalMatrixUpdate();
    notifyParentBoundingBoxNeedsUpdate();
}

void SceneNode::multRotationMatrixLeft(const glm::mat4& matrix) {
    mRotationMatrix = matrix * mRotationMatrix;

    mModelMatrixNeedsUpdate = true;
    notifyGlobalMatrixUpdate();
    notifyParentBoundingBoxNeedsUpdate();
}

void SceneNode::notifyGlobalMatrixUpdate() {
    if (!mGlobalMatrixNeedsUpdate) {
        mGlobalMatrixNeedsUpdate = true;

        for (SceneNode* node : mChildNodes) {
            node->notifyGlobalMatrixUpdate();
        }

        if (mParentNode != nullptr) {
            mParentNode->notifyChildGlobalMatrixNeedsUpdate();
        }
    }
}

void SceneNode::notifyChildGlobalMatrixNeedsUpdate() {
    if (!mChildGlobalMatrixNeedsUpdate) {
        mChildGlobalMatrixNeedsUpdate = true;

        if (mParentNode != nullptr) {
            mParentNode->notifyChildGlobalMatrixNeedsUpdate();
        }
    }
}

void SceneNode::setupEntityChangedCallback(Entity* entity) {
    entity->setGeometryChangedCallBack([this]() {
        notifyBoundingBoxNeedsUpdate();
    });
}

} /* namespace gles */
} /* namespace ann */
