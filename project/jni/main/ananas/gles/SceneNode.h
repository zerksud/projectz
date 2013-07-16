/*
 * SceneNode.h
 *
 *  Created on: Mar 19, 2013
 *      Author: zerksud
 */

#ifndef SCENENODE_H_
#define SCENENODE_H_

#include <vector>

#include <glm/glm.hpp>

#include "Transformable.h"
#include "BoundingBox.h"
#include "resources/Entity.h"

namespace ann {
namespace gles {

class SceneNode;

typedef std::vector<SceneNode*> SceneNodeList;

//TODO: method to check if SceneNode has SceneRootNode as ancestor

class SceneNode: public Transformable {
public:
    SceneNode();
    SceneNode(const SceneNode& other);
    virtual ~SceneNode();

    SceneNode* getParent() const;

    BoundingBox* getBoundingBox() const;
    void updateBoundingBox();

    bool geometryChanged() const;

    void attachNode(SceneNode* node);
    void detachNode(SceneNode* node);
    void detachAllNodes();  //TODO: remove as unused?
    const SceneNodeList& getChildNodes() const;

    void attachEntity(const Entity& entity);
    void detachEntity();
    const Entity* getEntity() const;

    const glm::mat4& getModelMatrix() const;

    void translate(float dx, float dy, float dz);
    void translate(glm::vec3 diffVector);
    void rotate(float angle, float x, float y, float z);
    void scale(float s);
    //TODO: setPositionMatrix and others unused matrix methods?

    const glm::mat4& getGlobalMatrix() const;
    void updateGlobalMatrix();

private:
    void notifyBoundingBoxNeedsUpdate();
    void notifyParentBoundingBoxNeedsUpdate();

    void multPositionMatrixLeft(const glm::mat4& matrix);
    void multRotationMatrixLeft(const glm::mat4& matrix);

    void notifyGlobalMatrixUpdate();
    void notifyChildGlobalMatrixNeedsUpdate();  //TODO: total mess?

    void setupEntityChangedCallback(Entity* entity);

    void clearEntity();

    SceneNode* mParentNode;
    SceneNodeList mChildNodes;

    mutable glm::mat4 mModelMatrix;
    mutable bool mModelMatrixNeedsUpdate;

    glm::mat4 mPositionMatrix;
    glm::mat4 mRotationMatrix;
    glm::mat4 mScaleMatrix;

    glm::mat4 mGlobalMatrix;
    bool mGlobalMatrixNeedsUpdate;

    bool mChildGlobalMatrixNeedsUpdate;

    Entity* mEntity;

    BoundingBox* mBoundingBox;
    bool mBoundingBoxNeedsUpdate;
};

} /* namespace gles */
} /* namespace ann */
#endif /* SCENENODE_H_ */
