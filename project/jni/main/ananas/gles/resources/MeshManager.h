/*
 * MeshManager.h
 *
 *  Created on: Mar 18, 2013
 *      Author: zerksud
 */

#ifndef MESHMANAGER_H_
#define MESHMANAGER_H_

#include <string>
#include <unordered_map>

#include "Mesh.h"

namespace ann {
namespace gles {

class MeshManager {
public:
    MeshManager();
    virtual ~MeshManager();

    Mesh* createMesh(std::string fileName);
    void glContextRecreated();
    void glContextDestroyed();

private:
    typedef std::string MeshName;
    typedef std::pair<const MeshName, Mesh*> MeshMapPair;
    typedef std::unordered_map<MeshName, Mesh*> MeshMap;

    MeshMap mMeshes;
};

} /* namespace gles */
} /* namespace ann */
#endif /* MESHMANAGER_H_ */
