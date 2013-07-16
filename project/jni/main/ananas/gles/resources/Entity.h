/*
 * Entity.h
 *
 *  Created on: Mar 19, 2013
 *      Author: zerksud
 */

#ifndef ENTITY_H_
#define ENTITY_H_

#include <functional>
#include <string>

#include "Mesh.h"
#include "ShaderProgram.h"
#include "Texture.h"

namespace ann {
namespace gles {

typedef std::function<void ()> GeometryChangedCallback;

class Entity {
public:
    Entity(const Entity& other);
    Entity(std::string meshFileName, std::string shaderName, std::string textureFileName);
    virtual ~Entity();

    void setGeometryChangedCallBack(GeometryChangedCallback callback);

    bool operator==(const Entity& other) const;
    bool operator!=(const Entity& other) const;

    Mesh* getMesh() const;
    ShaderProgram* getShaderProgram() const;
    Texture* getTexture() const;

    bool isLoaded() const;

private:
    Entity();
    Entity& operator=(const Entity& other);

    void addMeshLoadedObserver();
    Mesh* mMesh;
    ShaderProgram* mShaderProgram;
    Texture* mTexture;

    GeometryChangedCallback mGeometryChangedCallback;
};

bool less(const Entity& left, const Entity& right);

} /* namespace gles */
} /* namespace ann */
#endif /* ENTITY_H_ */
