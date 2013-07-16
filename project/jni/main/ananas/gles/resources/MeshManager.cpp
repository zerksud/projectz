/*
 * MeshManager.cpp
 *
 *  Created on: Mar 18, 2013
 *      Author: zerksud
 */

#include "MeshManager.h"

#include <strings.h>

#include <glm/glm.hpp>

#include <ananas/Engine.h>
#include <ananas/android/AndroidLayer.h>
#include <ananas/util/ZLogger.h>

#include <ananas/util/Constants.h>
#include <ananas/util/NotificationCenter.h>

namespace ann {
namespace gles {

MeshManager::MeshManager() {
}

MeshManager::~MeshManager() {
    for (MeshMapPair& e : mMeshes) {
        Mesh *mesh = e.second;
        delete mesh;
    }
}

void memcpyAdvanceSrc(void* dst, android::ConstBuffPtr* src, size_t count) {
    memcpy(dst, *src, count);
    *src += count;
}

void streamReadVec3(android::ConstBuffPtr* stream, glm::vec3* vector) {
    float value;
    for (int i = 0; i < 3; ++i) {
        memcpyAdvanceSrc(&value, stream, sizeof(value));
        (*vector)[i] = value;
    }
}

void loadMeshFromFile(Mesh *mesh, const std::string& fileName) {
    LOGV("Reading data from %s...", fileName.c_str());

    android::AssetManager *am = android::AndroidLayer::getInstance().getAssetManager();
    std::string fullName = util::Constants::kMeshesLocation + fileName;
    const android::RawBuff* buff = am->loadData(fullName.c_str());

    if (buff->getBuff() == nullptr) {
        LOGE("Can't read mesh from file %s", fileName.c_str());
        return;
    }
    LOGV("Got file buffer for %s", fileName.c_str());

    android::ConstBuffPtr stream = buff->getBuff();

    char header[5];
    memcpyAdvanceSrc(header, &stream, 4);
    header[4] = 0;

    if (strcmp(header, "BOBJ") != 0) {
        LOGE("Expected BOBJ header instead of '%s'", header);
        return;
    }

    glm::vec3 minPosition, maxPosition;
    streamReadVec3(&stream, &minPosition);
    streamReadVec3(&stream, &maxPosition);
    BoundingBox boundingBox(minPosition, maxPosition);


    size_t vertexBuffSizeInBytes = 0;
    memcpyAdvanceSrc(&vertexBuffSizeInBytes, &stream, sizeof(vertexBuffSizeInBytes));
    size_t vertexBuffSize = vertexBuffSizeInBytes / sizeof(float);

    float* vertexBuff = new float[vertexBuffSize];
    memcpyAdvanceSrc(vertexBuff, &stream, vertexBuffSizeInBytes);


    size_t indexBuffSizeInBytes = 0;
    memcpyAdvanceSrc(&indexBuffSizeInBytes, &stream, sizeof(indexBuffSizeInBytes));
    size_t indexBuffSize = indexBuffSizeInBytes / sizeof(GLushort);

    GLushort* indexBuff = new GLushort[indexBuffSize];
    memcpyAdvanceSrc(indexBuff, &stream, indexBuffSizeInBytes);


    mesh->load(vertexBuffSize, vertexBuff, indexBuffSize, indexBuff, boundingBox);

    delete[] vertexBuff;
    delete[] indexBuff;
}

void postMeshLoadedNotification(const std::string fileName) {
    util::Dictionary dict;
    dict["name"] = fileName;
    util::NotificationCenter::getInstance().postNotification(util::Constants::kMeshLoaded, dict);
}

Mesh* MeshManager::createMesh(std::string fileName) {
    MeshMap::const_iterator i = mMeshes.find(fileName);
    Mesh* mesh = nullptr;
    if (i != mMeshes.end()) {
        mesh = i->second;
    } else {
        mesh = new Mesh(fileName);

        if (Engine::getInstance().getGLESLayer()->glContextIsActive()) {
            loadMeshFromFile(mesh, fileName);   // TODO: resource should know if it was actually loaded - add isLoaded() method to Mesh/Texture/etc

            postMeshLoadedNotification(fileName);
        }

        mMeshes.insert(MeshMapPair(fileName, mesh));
    }

    return mesh;
}

void MeshManager::glContextRecreated() {
    for (MeshMapPair& e : mMeshes) {
        Mesh *mesh = e.second;
        ann::Engine::getInstance().getGLThread()->addTask([this, mesh]() {
            loadMeshFromFile(mesh, mesh->getFileName());
            postMeshLoadedNotification(mesh->getFileName());
        });
    }
}

void MeshManager::glContextDestroyed() {
    for (MeshMapPair& e : mMeshes) {
        Mesh *mesh = e.second;
        mesh->release();
    }
}

} /* namespace gles */
} /* namespace ann */
