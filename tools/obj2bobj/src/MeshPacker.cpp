/*
 * MeshPacker.cpp
 *
 *  Created on: Apr 6, 2013
 *      Author: zerksud
 */

#include "MeshPacker.h"

#include <stdio.h>
#include <string.h>

#include <stdint.h>
#include <string>
#include <map>
#include <vector>

#include <glm/glm.hpp>

#include "StringHelpers.h"

#define OBJECT_PREFIX       "o"
#define VERTEX_PREFIX       "v"
#define NORMAL_PREFIX       "vn"
#define TEXTURE_UV_PREFIX   "vt"
#define FACE_PREFIX         "f"

#define VECTOR_3D_COMPONENT_COUNT   3
#define UV_COMPONENT_COUNT          2

#define FACE_POINT_COUNT            3
#define POINT_INDEX_COUNT           3

#define VERTEX_INDEX_POSITION       0
#define TEXTURE_INDEX_POSITION      1
#define NORMAL_INDEX_POSITION       2

#define VERTEX_DATA_SIZE            8   // 3 + 3 + 2 (position + normal + texture)
#define NORMAL_OFFSET               3
#define TEXTURE_COORDINATE_OFFSET   6   // 3 + 3
namespace ann {
namespace tools {
namespace mesh_packer {

struct Vertex {
    int v;
    int n;
    int uv;

    std::string toString() const {
        std::stringstream ss;
        ss << v << '/' << uv << '/' << n;
        return ss.str();
    }
};

struct Face {
    Vertex vertices[3];
};

typedef uint32_t ann_size_t;
typedef unsigned short GLushort;
typedef std::map<std::string, GLushort> VertexIndexMap;

typedef std::vector<glm::vec3> VertexList;
typedef std::vector<glm::vec3> NormalList;
typedef std::vector<glm::vec2> TextureCoordinatesList;
typedef std::vector<Face> FaceList;

void memcpyAdvance(unsigned char** dstPtr, const void* src, size_t count) {
//    printf("write %lu bytes to %lu\n", count, reinterpret_cast<size_t>(*dstPtr));
    fflush(stdout);
    memcpy(*dstPtr, src, count);
    *dstPtr += count;
}

RawBuff makeBobjBuff(ann_size_t vertexBuffSize, const float* vertexBuff, ann_size_t indexBuffSize, const GLushort* indexBuff, const glm::vec3& minPosition, const glm::vec3& maxPosition) {
    const ann_size_t vertexBuffSizeInBytes = vertexBuffSize * sizeof(float);
    const ann_size_t indexBuffSizeInBytes = indexBuffSize * sizeof(GLushort);

    size_t buffSize = 4;    // header size
    buffSize += 2 * VECTOR_3D_COMPONENT_COUNT * sizeof(float);
    buffSize += sizeof(ann_size_t) + vertexBuffSizeInBytes;
    buffSize += sizeof(ann_size_t) + indexBuffSizeInBytes;

    unsigned char* buff = new unsigned char[buffSize];  // TODO: fill with zeroes?
    unsigned char* buffCurrentPtr = buff;

    memcpyAdvance(&buffCurrentPtr, "BOBJ", 4);

    printf("minPosition = [%f; %f; %f]\n", minPosition[0], minPosition[1], minPosition[2]);
    for (int i = 0; i < 3; ++i) {
        memcpyAdvance(&buffCurrentPtr, &(minPosition[i]), sizeof(float));
    }

    printf("maxPosition = [%f; %f; %f]\n", maxPosition[0], maxPosition[1], maxPosition[2]);
    for (int i = 0; i < 3; ++i) {
        memcpyAdvance(&buffCurrentPtr, &(maxPosition[i]), sizeof(float));
    }

    printf("vertexBuffSizeInBytes = %u\n", vertexBuffSizeInBytes);
    memcpyAdvance(&buffCurrentPtr, &vertexBuffSizeInBytes, sizeof(vertexBuffSizeInBytes));
    memcpyAdvance(&buffCurrentPtr, vertexBuff, vertexBuffSizeInBytes);

    printf("indexBuffSizeInBytes = %u\n", indexBuffSizeInBytes);
    memcpyAdvance(&buffCurrentPtr, &indexBuffSizeInBytes, sizeof(indexBuffSizeInBytes));
    memcpyAdvance(&buffCurrentPtr, indexBuff, indexBuffSizeInBytes);

    RawBuff rawBuff;
    rawBuff.buff = buff;
    rawBuff.size = buffSize;

    return rawBuff;
}

RawBuff packMeshData(const VertexList& vertices, const NormalList& normals, const TextureCoordinatesList& uvs, const FaceList& faces) {
    float_t minValue, maxValue;
    minValue = std::numeric_limits<float_t>::min();
    maxValue = std::numeric_limits<float_t>::max();
    glm::vec3 minPosition = glm::vec3(maxValue, maxValue, maxValue);
    glm::vec3 maxPosition = glm::vec3(minValue, minValue, minValue);

    VertexIndexMap indexMap;

    int pointCount = faces.size() * FACE_POINT_COUNT;
    GLushort* indicesBuff = new GLushort[pointCount];
    float* vertexBuff = new float[pointCount * VERTEX_DATA_SIZE];

    int verticesCreated = 0;
    int uniqueVerticesCreated = 0;
    for (const Face& face : faces) {
        for (int i = 0; i < FACE_POINT_COUNT; ++i) {
            std::string vertexKey = face.vertices[i].toString();
            VertexIndexMap::const_iterator pos = indexMap.find(vertexKey);
            if (pos != indexMap.end()) {
                indicesBuff[verticesCreated] = pos->second;
            } else {
                int baseOffset = uniqueVerticesCreated * VERTEX_DATA_SIZE;
                const glm::vec3& v = vertices[face.vertices[i].v - 1];
                const glm::vec3& n = normals[face.vertices[i].n - 1];

                for (int j = 0; j < VECTOR_3D_COMPONENT_COUNT; ++j) {
                    vertexBuff[baseOffset + j] = v[j];
                    vertexBuff[baseOffset + NORMAL_OFFSET + j] = n[j];

                    if (v[j] > maxPosition[j]) {
                        maxPosition[j] = v[j];
                    }
                    if (v[j] < minPosition[j]) {
                        minPosition[j] = v[j];
                    }
                }

                const glm::vec2& uv = uvs[face.vertices[i].uv - 1];
                vertexBuff[baseOffset + TEXTURE_COORDINATE_OFFSET] = uv[0];
                vertexBuff[baseOffset + TEXTURE_COORDINATE_OFFSET + 1] = uv[1];

                indexMap[vertexKey] = uniqueVerticesCreated;
                indicesBuff[verticesCreated] = uniqueVerticesCreated;

                ++uniqueVerticesCreated;
            }

            ++verticesCreated;
        }
    }

    printf("Constructed %d distinct points for mesh with %d points overall\n", uniqueVerticesCreated, verticesCreated);

    ann_size_t vertexBuffActualSize = uniqueVerticesCreated * VERTEX_DATA_SIZE;
    ann_size_t indicesBuffActualSize = verticesCreated;

    RawBuff bobjBuff = makeBobjBuff(vertexBuffActualSize, vertexBuff, indicesBuffActualSize, indicesBuff, minPosition, maxPosition);

    delete[] indicesBuff;
    delete[] vertexBuff;

    return bobjBuff;
}

int readFloatValues(std::stringstream& ss, int count, float* buff) {
    int read_count = 0;
    while (!ss.eof() && read_count < count) {
        ss >> buff[read_count++];
    }

    return read_count;
}

bool readFace(const std::string& line, Face* face) {
    std::vector<std::string> points = util::split(line, ' ');   //TODO: move delimeter to macro?
    if (points.size() != FACE_POINT_COUNT) {
        printf("Can't split face '%s' into %d components ", line.c_str(), FACE_POINT_COUNT);
        return false;
    }

    for (int i = 0; i < FACE_POINT_COUNT; ++i) {
        std::vector<std::string> point = util::split(points[i], '/');  //TODO: move delimeter to macro?
        if (point.size() != POINT_INDEX_COUNT) {
            printf("Expected %d indices in face point #%d from %s", POINT_INDEX_COUNT, i, line.c_str());
            return false;
        }

        int indices[POINT_INDEX_COUNT] = { 0, 0, 0 };
        for (int j = 0; j < POINT_INDEX_COUNT; ++j) {
            int value;
            if (util::parseInt(point[j], &value)) {
                indices[j] = value;
            } else {
                printf("Can't parse index #%d from point %s", j, point[i].c_str());
                return false;
            }
        }

        face->vertices[i].v = indices[VERTEX_INDEX_POSITION];
        face->vertices[i].uv = indices[TEXTURE_INDEX_POSITION];
        face->vertices[i].n = indices[NORMAL_INDEX_POSITION];
    }

    return true;
}

bool parseObj(std::stringstream& ss, VertexList* vertices, NormalList* normals, TextureCoordinatesList* textureCoordinates, FaceList* faces) {
    float* readBuff = new float[std::max(VECTOR_3D_COMPONENT_COUNT, UV_COMPONENT_COUNT)];
    std::string line;
    while (std::getline(ss, line)) {
        std::stringstream ssline(line);
        std::string prefix;
        ssline >> prefix;

        if (prefix == OBJECT_PREFIX) {  //TODO: remove clears for multi-object files?
            vertices->clear();
            textureCoordinates->clear();
            normals->clear();
        } else if (prefix == VERTEX_PREFIX) {
            int readValuesCount = readFloatValues(ssline, VECTOR_3D_COMPONENT_COUNT, readBuff);
            if (readValuesCount == VECTOR_3D_COMPONENT_COUNT) {
                glm::vec3 p(readBuff[0], readBuff[1], readBuff[2]);
                vertices->push_back(p);
            } else {
                printf("Can't read vertex from %s", line.c_str());
                return false;
            }
        } else if (prefix == NORMAL_PREFIX) {
            int readValuesCount = readFloatValues(ssline, VECTOR_3D_COMPONENT_COUNT, readBuff);
            if (readValuesCount == VECTOR_3D_COMPONENT_COUNT) {
                glm::vec3 n(readBuff[0], readBuff[1], readBuff[2]);
                n = glm::normalize(n);
                normals->push_back(n);
            } else {
                printf("Can't read normal from %s", line.c_str());
                return false;
            }
        } else if (prefix == TEXTURE_UV_PREFIX) {
            int readValuesCount = readFloatValues(ssline, UV_COMPONENT_COUNT, readBuff);
            if (readValuesCount == UV_COMPONENT_COUNT) {
                glm::vec2 uv(readBuff[0], readBuff[1]);
                textureCoordinates->push_back(uv);
            } else {
                printf("Can't read uv coordinates from %s", line.c_str());
                return false;
            }
        } else if (prefix == FACE_PREFIX) {
            Face face;
            std::string remainder;
            std::getline(ssline, remainder);
            util::trim(&remainder);
            if (readFace(remainder, &face)) {
                faces->push_back(face);
            } else {
                printf("Can't read face from %s", line.c_str());
                return false;
            }
        }
    }

    return true;
}

RawBuff packObj(std::stringstream& ss) {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texturePoints;
    std::vector<Face> faces;

    bool success = parseObj(ss, &vertices, &normals, &texturePoints, &faces);
    if (!success) {
        printf("Can't parse obj data");
        return RawBuff();
    }

    RawBuff buff = packMeshData(vertices, normals, texturePoints, faces);

    return buff;
}

} /* namespace mesh_packer */
} /* namespace tools */
} /* namespace ann */
