/*
 * Mesh.cpp
 *
 *  Created on: Mar 18, 2013
 *      Author: zerksud
 */

#include "Mesh.h"

#include <limits>

#include <ananas/util/ZLogger.h>
#include <ananas/gles/Helpers.h>

namespace ann {
namespace gles {

bool Mesh::sWireFrame = false;

void Mesh::setWireFrame(bool value) {
    sWireFrame = value;
}

Mesh::Mesh(std::string fileName) {
    mName = fileName;

    mVertexBuff = 0;

    mIndexCount = 0;
    mIndexBuff = 0;
    mIndices = nullptr;

    mLoaded = false;
}

Mesh::~Mesh() {
    if (mIndices != nullptr) {
        delete[] mIndices;
    }
}

void Mesh::load(size_t vertexDataSize, const float_t* vertexData, size_t indexCount, const GLushort* indexBuff, const BoundingBox& boundingBox) {
    mBoundingBox = boundingBox;

    LOGV("push mesh data to GPU", mName.c_str());

    mVertexBuff = makeGLBuffer(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertexDataSize * kBytesPerFloat), vertexData);
    mIndexBuff = makeGLBuffer(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indexCount * kBytesPerShort), indexBuff);
    mIndexCount = indexCount;

    mIndices = new GLushort[indexCount];
    memcpy(mIndices, indexBuff, indexCount * sizeof(GLushort));

    LOGV("mesh data was pushed to GPU successfully", mName.c_str());

    mLoaded = true;
}

bool Mesh::isLoaded() const {
    return mLoaded;
}

const std::string& Mesh::getFileName() const {
    return mName;
}

void enableAttribArray(GLint attribHandle, int itemSize, int stride, int offset) {
    if (attribHandle >= 0) {
        glEnableVertexAttribArray(attribHandle);
        glVertexAttribPointer(attribHandle, itemSize, GL_FLOAT, false, stride, reinterpret_cast<void*>(offset));
    }
}

void Mesh::enableAttribArrays(const ShaderProgram& shader) const {
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuff);
    HANDLE_GL_ERRORS("glError after glBindBuffer");

    enableAttribArray(shader.getPositionHandle(), kPositionDataSize, kVertexDataSizeInBytes, 0);
    HANDLE_GL_ERRORS("glError after enableAttribArray for PositionHandle");

    enableAttribArray(shader.getNormalHandle(), kNormalDataSize, kVertexDataSizeInBytes, kNormalDataOffsetOnBytes);
    HANDLE_GL_ERRORS("glError after enableAttribArray for NormalHandle");

    enableAttribArray(shader.getTexCoordinateHandle(), kTexCoordinateSize, kVertexDataSizeInBytes, kTexCoordinateDataOffsetInBytes);
    HANDLE_GL_ERRORS("glError after enableAttribArray for TexCoordinateHandle");
}

void Mesh::drawArrays() const {
    if (!sWireFrame) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuff);
        glDrawElements(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_SHORT, 0);
    } else {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        for (int i = 0; i < mIndexCount; i += 3) {
            glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_SHORT, &(mIndices[i]));
        }
    }
}

void Mesh::release() {
    if (mLoaded) {
        glDeleteBuffers(1, &mVertexBuff);
        mVertexBuff = 0;

        glDeleteBuffers(1, &mIndexBuff);
        mIndexBuff = 0;

        mLoaded = false;
    }
}

const BoundingBox* Mesh::getBoundingBox() const {
    return &mBoundingBox;   //TODO: use pointer?
}

GLuint Mesh::makeGLBuffer(GLenum target, GLsizeiptr size, const GLvoid* data) {
    GLuint buffer = 0;
    glGenBuffers(1, &buffer);
    glBindBuffer(target, buffer);
    glBufferData(target, size, data, GL_STATIC_DRAW);
    glBindBuffer(target, 0);

    return buffer;
}

bool less(const Mesh& left, const Mesh& right) {
    if (left.mVertexBuff < right.mVertexBuff) {
        return true;
    } else {
        return false;
    }
}

} /* namespace gles */
} /* namespace ann */
