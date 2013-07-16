/*
 * SceneManager.h
 *
 *  Created on: Mar 19, 2013
 *      Author: zerksud
 */

#ifndef SCENEMANAGER_H_
#define SCENEMANAGER_H_

#include "Camera.h"
#include "Frustum.h"
#include "SceneNode.h"

namespace ann {
namespace gles {

class SceneManager {
public:
    SceneManager();
    virtual ~SceneManager();

    void clear();

    void setProjectionMatrix(const glm::mat4& pMatrix);
    void attachCamera(SceneNode* node);

    SceneNode* getRootSceneNode();
    SceneNode* getRootHUDNode();

    void render();

private:
    struct RenderQueueElement {
        glm::mat4 globalMatrix; //TODO: use reference?
        const Entity* entity;
    };
    friend bool lessRenderQueueElement(const SceneManager::RenderQueueElement& left, const SceneManager::RenderQueueElement& right);
    typedef std::vector<RenderQueueElement> RenderQueue;    //TODO: use list?

    void clearMemory();
    void initialize();

    void toogleWireFrame();

    static void rebuildRenderQueue(const Frustum* frustum, const SceneNode* rootNode, RenderQueue* queue);
    void renderQueue(const glm::mat4& viewMatrix, const glm::mat4& vpMatrix, const glm::mat4& baseModelMatrix, const RenderQueue& queue);

    bool mWireFrame;

    SceneNode* mRootSceneNode;
    SceneNode* mRootHUDNode;
    Camera mCamera;

    glm::mat4 mProjectionMatrix;
    bool mFrustumChanged;

    RenderQueue mSceneRenderQueue;
    RenderQueue mHUDRenderQueue;
};

} /* namespace gles */
} /* namespace ann */
#endif /* SCENEMANAGER_H_ */
