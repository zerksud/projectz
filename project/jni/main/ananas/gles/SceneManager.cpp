/*
 * SceneManager.cpp
 *
 *  Created on: Mar 19, 2013
 *      Author: zerksud
 */

#include "SceneManager.h"

#include <algorithm>
#include <stack>

#include <glm/gtc/type_ptr.hpp>

#include <ananas/Engine.h>

#include <ananas/util/NotificationCenter.h>
#include <ananas/util/CommonHelpers.h>
#include <ananas/util/StringHelpers.h>

#include "Helpers.h"
#include <ananas/util/Constants.h>
#include <ananas/util/ZLogger.h>

#define WIREFRAME_SHADER_NAME   "wireframe"

namespace ann {
namespace gles {

SceneManager::SceneManager() {
    initialize();
    mWireFrame = false;

    util::NotificationCenter::getInstance().addObserver(util::Constants::kUIDebugToggleWireframe, this, [this](const ann::util::Dictionary& dict) {
        this->toogleWireFrame();
    });
}

SceneManager::~SceneManager() {
    clearMemory();
}

void SceneManager::clear() {
    clearMemory();
    initialize();
}

void SceneManager::clearMemory() {
    delete mRootSceneNode;
    delete mRootHUDNode;
}

void SceneManager::initialize() {
    mRootSceneNode = new SceneNode();

    SceneNode* cameraNode = new SceneNode();
    mRootSceneNode->attachNode(cameraNode);

    mCamera.attachToNode(cameraNode);

    mRootHUDNode = new SceneNode();

    mProjectionMatrix = glm::mat4(1.0f);

    mFrustumChanged = true;
}

void SceneManager::setProjectionMatrix(const glm::mat4& pMatrix) {
    mProjectionMatrix = pMatrix;
    mFrustumChanged = true;
}

void SceneManager::attachCamera(SceneNode* node) {
    // TODO: check if SceneManager owns this camera
    // TODO: release default cameraNode?

    mCamera.attachToNode(node);
}

SceneNode* SceneManager::getRootSceneNode() {
    return mRootSceneNode;
}

void SceneManager::renderQueue(const glm::mat4& globalViewMatrix, const glm::mat4& globalVPMatrix, const glm::mat4& rootTransorm, const RenderQueue& queue) {
    const float kLightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    ShaderProgram* wireFrameShader = nullptr;
    if (mWireFrame) {
        ShaderManager* sm = Engine::getInstance().getGLESLayer()->getShaderManager();
        wireFrameShader = sm->createShaderProgram(WIREFRAME_SHADER_NAME);
    }

    glm::mat4 viewMatrix = globalViewMatrix * rootTransorm;
    glm::mat4 vpMatrix = globalVPMatrix * rootTransorm;

    glm::mat4 elemMVMatrix;
    glm::mat4 elemMVPMatrix;
    ShaderProgram* currentShader = nullptr;
    bool shaderChanged = false;
    Texture* currentTexture = nullptr;
    const Entity* currentEntity = nullptr;

    for (RenderQueueElement elem : queue) {
        const Entity* entity = elem.entity;

        glm::mat4& elemGlobalMatrix = elem.globalMatrix;
        elemMVMatrix = viewMatrix * elemGlobalMatrix;
        elemMVPMatrix = vpMatrix * elemGlobalMatrix;

        ShaderProgram* shader = entity->getShaderProgram();

        if (mWireFrame) {
            shader = wireFrameShader;
        }

        shaderChanged = false;
        if (currentShader != shader) {
            currentShader = shader;
            shaderChanged = true;

            shader->glUseProgram();
            HANDLE_GL_ERRORS("glError after glUseProgram");

            int lightPosHandle = shader->getLightPosHandle();
            if (lightPosHandle >= 0) {
                glUniform3fv(lightPosHandle, 1, kLightPos);
            }
        }

        Texture* texture = entity->getTexture();
        if (texture != nullptr) {
            if (shaderChanged || currentTexture != texture) {
                currentTexture = texture;

                int textureName = texture->getGLName();
                int textureHandle = shader->getTextureHandle();
                if (textureName >= 0 && textureHandle >= 0) {
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, textureName);
                    glUniform1i(textureHandle, 0);  //TODO: why 0?
                }
            }
        }
        HANDLE_GL_ERRORS("glError after glBindTexture");

        int mvMatrixHandle = shader->getMVMatrixHandle();
        if (mvMatrixHandle >= 0) {
            glUniformMatrix4fv(mvMatrixHandle, 1, false, glm::value_ptr(elemMVMatrix));
        }
        HANDLE_GL_ERRORS("glError after glUniformMatrix4fv(mvMatrixHandle");

        int mvpMatrixHandle = shader->getMVPMatrixHandle();
        if (mvpMatrixHandle >= 0) {
            glUniformMatrix4fv(mvpMatrixHandle, 1, false, glm::value_ptr(elemMVPMatrix));
        }
        HANDLE_GL_ERRORS("glError after glUniformMatrix4fv(mvpMatrixHandle");

        if (shaderChanged || currentEntity != entity) {
            currentEntity = entity;
            entity->getMesh()->enableAttribArrays(*shader);
        }
        HANDLE_GL_ERRORS("glError after enableAttribArrays");

        entity->getMesh()->drawArrays();

        HANDLE_GL_ERRORS("glError after drawArrays");
    }
}

void SceneManager::render() {
    glm::mat4 viewMatrix, vpMatrix;

    if (mRootSceneNode->geometryChanged() || mFrustumChanged) {
        mRootSceneNode->updateGlobalMatrix();
        mRootSceneNode->updateBoundingBox();

        viewMatrix = mCamera.getViewMatrix();  //TODO: use reference?
        vpMatrix = mProjectionMatrix * viewMatrix;

        Frustum frustum(vpMatrix);
        mFrustumChanged = false;

        rebuildRenderQueue(&frustum, mRootSceneNode, &mSceneRenderQueue);
    } else {
        viewMatrix = mCamera.getViewMatrix();   // TODO: cache matrices?
        vpMatrix = mProjectionMatrix * viewMatrix;
    }

    if (mRootHUDNode->geometryChanged()) {
        mRootHUDNode->updateGlobalMatrix();

        rebuildRenderQueue(nullptr, mRootHUDNode, &mHUDRenderQueue);
    }

    renderQueue(viewMatrix, vpMatrix, glm::mat4(1.0), mSceneRenderQueue);

    glClear(GL_DEPTH_BUFFER_BIT);   // TODO: move to GLESLayer?

    renderQueue(viewMatrix, vpMatrix, mCamera.getParentNode()->getGlobalMatrix(), mHUDRenderQueue);

    HANDLE_GL_ERRORS("glError after render");
}

bool lessRenderQueueElement(const SceneManager::RenderQueueElement& left, const SceneManager::RenderQueueElement& right) {
    return less(*left.entity, *right.entity);
}

void SceneManager::toogleWireFrame() {
    mWireFrame = !mWireFrame;
    Mesh::setWireFrame(mWireFrame);
}

SceneNode* SceneManager::getRootHUDNode() {
    return mRootHUDNode;
}

void SceneManager::rebuildRenderQueue(const Frustum* frustum, const SceneNode* rootNode, RenderQueue* queue) {
    queue->clear();

    std::stack<const SceneNode*> sceneNodeStack;
    sceneNodeStack.push(rootNode);

    while (!sceneNodeStack.empty()) {
        const SceneNode* node = sceneNodeStack.top();
        sceneNodeStack.pop();

        const glm::mat4& nodeGlobalMatrix = node->getGlobalMatrix();

        if (frustum == nullptr || frustum->isVisible(*node->getBoundingBox(), nodeGlobalMatrix)) {
            const Entity* entity = node->getEntity();

            if (entity != nullptr && entity->isLoaded()) {
                RenderQueueElement elem;
                elem.entity = entity;
                elem.globalMatrix = nodeGlobalMatrix;
                queue->push_back(elem);
            }

            for (SceneNode* childNode : node->getChildNodes()) {
                sceneNodeStack.push(childNode);
            }
        }
    }

    std::sort(queue->begin(), queue->end(), lessRenderQueueElement);

    if (frustum != nullptr) {
        util::helpers::logDebugValueChange("objects", util::stringFormat("%lu", queue->size()));
    }
}

} /* namespace gles */
} /* namespace ann */
