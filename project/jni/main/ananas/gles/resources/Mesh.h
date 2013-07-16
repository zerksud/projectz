/*
 * Mesh.h
 *
 *  Created on: Mar 18, 2013
 *      Author: zerksud
 */

#ifndef MESH_H_
#define MESH_H_

#include <string>

#include <GLES2/gl2.h>

#include <ananas/gles/BoundingBox.h>
#include "ShaderProgram.h"

namespace ann {
namespace gles {

typedef float float_t;

class Mesh {
public:
    Mesh(std::string fileName);
    virtual ~Mesh();

    void load(size_t vertexDataSize, const float_t* vertexData, size_t indexCount, const GLushort* indexBuff, const BoundingBox& boundingBox);
    bool isLoaded() const;

    const std::string& getFileName() const;
    void enableAttribArrays(const ShaderProgram& shader) const;
    void drawArrays() const;
    const BoundingBox* getBoundingBox() const;

    void release();

    friend bool less(const Mesh& left, const Mesh& right);

    static void setWireFrame(bool value);

    static const int kPositionDataSize = 3;
    static const int kNormalDataSize = 3;
    static const int kTexCoordinateSize = 2;

    static const int kVertexDataSize = kPositionDataSize + kNormalDataSize + kTexCoordinateSize;
    static const int kNormalDataOffset = kPositionDataSize;
    static const int kTexCoordinateDataOffset = kNormalDataOffset + kNormalDataSize;

private:
    static const int kBytesPerFloat = 4;
    static const int kVertexDataSizeInBytes = kVertexDataSize * kBytesPerFloat;
    static const int kNormalDataOffsetOnBytes = kNormalDataOffset * kBytesPerFloat;
    static const int kTexCoordinateDataOffsetInBytes = kTexCoordinateDataOffset * kBytesPerFloat;

    static const int kBytesPerShort = 2;

    static GLuint makeGLBuffer(GLenum target, GLsizeiptr size, const GLvoid* data);

    static bool sWireFrame;

    std::string mName;

    GLuint mVertexBuff;

    int mIndexCount;
    GLuint mIndexBuff;
    GLushort* mIndices;  // TODO: remove for release to save memory?

    BoundingBox mBoundingBox;

    bool mLoaded;
};

bool less(const Mesh& left, const Mesh& right);

} /* namespace gles */
} /* namespace ann */
#endif /* MESH_H_ */
