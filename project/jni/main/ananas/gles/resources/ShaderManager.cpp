/*
 * ShaderManager.cpp
 *
 *  Created on: Mar 13, 2013
 *      Author: zerksud
 */

#include "ShaderManager.h"

#include <ananas/android/AndroidLayer.h>
#include <ananas/Engine.h>

#include <ananas/util/Constants.h>
#include <ananas/util/ZLogger.h>

#define VERTEX_SHADER_EXTENSION ".vsh"
#define FRAGMENT_SHADER_EXTENSION   ".fsh"

namespace ann {
namespace gles {

ShaderManager::ShaderManager() {
}

ShaderManager::~ShaderManager() {
    for (ShaderMapPair& e : mShaderPrograms) {
        ShaderProgram *shader = e.second;
        delete shader;
    }
}

GLuint createProgram(GLuint vertexShaderHandle, GLuint fragmentShaderHandle) {
    GLuint programHandle = glCreateProgram();
    if (programHandle != 0) {
        glAttachShader(programHandle, vertexShaderHandle);
        glAttachShader(programHandle, fragmentShaderHandle);

        glBindAttribLocation(programHandle, 0, "a_Position");   //TODO: unnecessary?
        glBindAttribLocation(programHandle, 1, "a_Color");

        glLinkProgram(programHandle);

        GLint linkStatus;
        glGetProgramiv(programHandle, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint infoLogSize;
            glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &infoLogSize);

            char* infoLog = new char[infoLogSize];
            glGetProgramInfoLog(programHandle, infoLogSize, nullptr, infoLog);

            LOGE("Can't link program: %s", infoLog);
            delete[] infoLog;

            glDeleteProgram(programHandle);
            programHandle = 0;
        }
    }

    if (programHandle == 0) {
        LOGE("Error creating program");
    }

    return programHandle;
}

const char* getShaderSource(std::string fileName) {
    std::string fullFileName = util::Constants::kShadersLocation + fileName;
    android::AssetManager *am = android::AndroidLayer::getInstance().getAssetManager();
    const char* shaderSource = am->loadText(fullFileName.c_str());
    if (shaderSource == NULL) {
        LOGE("Can't read shader source from %s", fileName.c_str());
    }

    return shaderSource;
}

GLuint createShader(std::string shaderName, GLenum shaderType) {
    LOGV("Creating shader from %s...", shaderName.c_str());

    GLuint shaderHandle = glCreateShader(shaderType);
    LOGV("created shader with handle %d", shaderHandle);
    if (shaderType != 0) {
        const char* shaderSource = getShaderSource(shaderName);
        if (shaderSource != nullptr) {
            glShaderSource(shaderHandle, 1, &shaderSource, nullptr);
            glCompileShader(shaderHandle);

            GLint compileStatus;
            glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compileStatus);
            if (compileStatus != GL_TRUE) {
                GLint infoLogSize;
                glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &infoLogSize);

                char* infoLog = new char[infoLogSize];
                glGetShaderInfoLog(shaderHandle, infoLogSize, nullptr, infoLog);

                LOGE("Can't compile shader %s: %s", shaderName.c_str(), infoLog);
                delete[] infoLog;

                glDeleteShader(shaderHandle);
                shaderHandle = 0;
            }
        } else {
            glDeleteShader(shaderHandle);
            shaderHandle = 0;
        }
    }

    if (shaderHandle == 0) {
        LOGE("Error creating shader %s", shaderName.c_str());
    }

    //TODO: glError?

    return shaderHandle;
}

void compileAndLinkGLProgram(ShaderProgram *program, std::string vertexShaderFileName, std::string fragmentShaderFileName) {
    LOGV("Creating shader program from %s and %s", vertexShaderFileName.c_str(), fragmentShaderFileName.c_str());

    GLuint vertexHandle = createShader(vertexShaderFileName, GL_VERTEX_SHADER);
    GLuint fragmentHandle = createShader(fragmentShaderFileName, GL_FRAGMENT_SHADER);
    GLuint programHandle = createProgram(vertexHandle, fragmentHandle);
    program->setHandles(programHandle, vertexHandle, fragmentHandle);

    //TODO: glErrors?
}

ShaderProgram* ShaderManager::createShaderProgram(std::string shaderName) {
    std::string vertexShaderFileName = shaderName + VERTEX_SHADER_EXTENSION;
    std::string fragmentShaderFileName = shaderName + FRAGMENT_SHADER_EXTENSION;

    return createShaderProgram(vertexShaderFileName, fragmentShaderFileName);
}

ShaderProgram* ShaderManager::createShaderProgram(std::string vertexShaderFileName, std::string fragmentShaderFileName) {
    std::string shaderUniqueName = vertexShaderFileName + fragmentShaderFileName;
    ShaderProgram* shader = nullptr;
    ShaderProgramMap::const_iterator i = mShaderPrograms.find(shaderUniqueName);
    if (i != mShaderPrograms.end()) {
        shader = i->second;

    } else {
        shader = new ShaderProgram(vertexShaderFileName, fragmentShaderFileName);

        if (Engine::getInstance().getGLESLayer()->glContextIsActive()) {
            compileAndLinkGLProgram(shader, vertexShaderFileName, fragmentShaderFileName);
        }

        mShaderPrograms.insert(ShaderMapPair(shaderUniqueName, shader));
    }

    return shader;
}

void ShaderManager::glContextRecreated() {
    for (ShaderMapPair& e : mShaderPrograms) {
        ShaderProgram *shader = e.second;
        ann::Engine::getInstance().getGLThread()->addTask([shader](){
            compileAndLinkGLProgram(shader, shader->getVertexShaderFileName(), shader->getFragmentShaderFileName());
        });
    }
}

void ShaderManager::glContextDestroyed() {
    for (ShaderMapPair& e : mShaderPrograms) {
        ShaderProgram *shader = e.second;
        shader->release();
    }
}

} /* namespace gles */
} /* namespace ann */
