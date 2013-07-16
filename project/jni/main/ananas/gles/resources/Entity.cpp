/*
 * Entity.cpp
 *
 *  Created on: Mar 19, 2013
 *      Author: zerksud
 */

#include "Entity.h"

#include <ananas/Engine.h>

#include <ananas/util/Constants.h>

namespace ann {
namespace gles {

Entity::Entity() {
    mMesh = nullptr;
    mShaderProgram = nullptr;
    mTexture = nullptr;

    mGeometryChangedCallback = nullptr;
}

void Entity::addMeshLoadedObserver() {
    util::NotificationCenter::getInstance().addObserver(util::Constants::kMeshLoaded, this, [this](const util::Dictionary& dict) {
        if (this->mGeometryChangedCallback != nullptr && dict["name"].asString() == mMesh->getFileName()) {
            this->mGeometryChangedCallback();
        }
    });
}

Entity::Entity(const Entity& other) {
    mMesh = other.mMesh;
    mShaderProgram = other.mShaderProgram;
    mTexture = other.mTexture;

    mGeometryChangedCallback = nullptr;
    addMeshLoadedObserver();
}

Entity::Entity(std::string meshFileName, std::string shaderName, std::string textureFileName) {
    GLESLayer* glLayer = Engine::getInstance().getGLESLayer();
    mMesh = glLayer->getMeshManager()->createMesh(meshFileName);
    mShaderProgram = glLayer->getShaderManager()->createShaderProgram(shaderName);
    mTexture = glLayer->getTextureManager()->createTexture(textureFileName);

    mGeometryChangedCallback = nullptr;
    addMeshLoadedObserver();
}

Entity::~Entity() {
    if (mMesh != nullptr) {
        util::NotificationCenter::getInstance().removeObserver(util::Constants::kMeshLoaded, this);
    }
}

Entity& Entity::operator =(const Entity& other) {
    if (this != &other) {
        mMesh = other.mMesh;
        mShaderProgram = other.mShaderProgram;
        mTexture = other.mTexture;
        mGeometryChangedCallback = other.mGeometryChangedCallback;
    }

    return *this;
}

bool Entity::operator ==(const Entity& other) const {   //TODO: use actual gl resource names?
    bool equals = mMesh == other.mMesh &&
            mShaderProgram == mShaderProgram &&
            mTexture == other.mTexture;
    return equals;
}

bool Entity::operator !=(const Entity& other) const {
    return !(*this == other);
}

Mesh* Entity::getMesh() const {
    return mMesh;
}

ShaderProgram* Entity::getShaderProgram() const {
    return mShaderProgram;
}

void Entity::setGeometryChangedCallBack(GeometryChangedCallback callback) {
    mGeometryChangedCallback = callback;
}

Texture* Entity::getTexture() const {
    return mTexture;
}

bool Entity::isLoaded() const {
    return (mShaderProgram->isLoaded() && mTexture->isLoaded() && mMesh->isLoaded());
}

bool less(const Entity& left, const Entity& right) {
    GLuint leftShader = left.getShaderProgram()->getProgramHandle();
    GLuint rigthShader = right.getShaderProgram()->getProgramHandle();
    GLuint leftTexture = left.getTexture()->getGLName();
    GLuint rightTexture = right.getTexture()->getGLName();
    if (leftShader < rigthShader) {
        return true;
    } else if (leftShader > rigthShader) {
        return false;
    } else if (leftTexture < rightTexture) {
        return true;
    } else if (leftTexture > rightTexture) {
        return false;
    } else {
        return less(*(left.getMesh()), *(right.getMesh()));
    }
}

} /* namespace gles */
} /* namespace ann */
