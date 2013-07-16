/*
 * ShaderProgram.cpp
 *
 *  Created on: Mar 13, 2013
 *      Author: zerksud
 */

#include "ShaderProgram.h"

#include <GLES2/gl2.h>

#define NO_HANDLE   -1

namespace ann {
namespace gles {

ShaderProgram::ShaderProgram(std::string vertexShaderFileName, std::string fragmentShaderFileName) {
    mVertexShaderFileName = vertexShaderFileName;
    mFragmentShaderFileName = fragmentShaderFileName;

    mProgramHandle = 0;
    mVertexShaderHandle = 0;
    mFragmentShaderHandle = 0;

    mMVPMatrixHandle = NO_HANDLE;
    mMVMatrixHandle = NO_HANDLE;
    mLightPosHandle = NO_HANDLE;
    mTextureHandle = NO_HANDLE;

    mPositionHandle = NO_HANDLE;
    mNormalHandle = NO_HANDLE;
    mTexCoordinateHandle = NO_HANDLE;
    mColorHandle = NO_HANDLE;

    mLoaded = false;
}

ShaderProgram::~ShaderProgram() {
}

std::string ShaderProgram::getVertexShaderFileName() const {
    return mVertexShaderFileName;
}

std::string ShaderProgram::getFragmentShaderFileName() const {
    return mFragmentShaderFileName;
}

void ShaderProgram::setHandles(GLuint programHandle, GLuint vertexShaderHandle, GLuint fragmentShaderHandle) {
    mProgramHandle = programHandle;
    mVertexShaderHandle = vertexShaderHandle;
    mFragmentShaderHandle = fragmentShaderHandle;

    mMVPMatrixHandle = glGetUniformLocation(mProgramHandle, "u_MVPMatrix");
    mMVMatrixHandle = glGetUniformLocation(mProgramHandle, "u_MVMatrix");
    mLightPosHandle = glGetUniformLocation(mProgramHandle, "u_LightPos");
    mTextureHandle = glGetUniformLocation(mProgramHandle, "u_Texture");

    mPositionHandle = glGetAttribLocation(mProgramHandle, "a_Position");
    mNormalHandle = glGetAttribLocation(mProgramHandle, "a_Normal");
    mTexCoordinateHandle = glGetAttribLocation(mProgramHandle, "a_TexCoordinate");
    mColorHandle = glGetAttribLocation(mProgramHandle, "a_Color");

    mLoaded = true;
}

void ShaderProgram::glUseProgram() const {
    ::glUseProgram(mProgramHandle);
}

void ShaderProgram::release() {
    if (mLoaded) {
        glDeleteShader(mVertexShaderHandle);
        glDeleteShader(mFragmentShaderHandle);
        glDeleteProgram(mProgramHandle);

        mVertexShaderHandle = 0;
        mFragmentShaderHandle = 0;
        mProgramHandle = 0;

        mLoaded = false;
    }
}

bool ShaderProgram::isLoaded() const {
    return mLoaded;
}

GLuint ShaderProgram::getProgramHandle() const {
    return mProgramHandle;
}

GLuint ShaderProgram::getMVPMatrixHandle() const {
    return mMVPMatrixHandle;
}

GLuint ShaderProgram::getMVMatrixHandle() const {
    return mMVMatrixHandle;
}

GLuint ShaderProgram::getLightPosHandle() const {
    return mLightPosHandle;
}

GLuint ShaderProgram::getTextureHandle() const {
    return mTextureHandle;
}

GLuint ShaderProgram::getPositionHandle() const {
    return mPositionHandle;
}

GLuint ShaderProgram::getNormalHandle() const {
    return mNormalHandle;
}

GLuint ShaderProgram::getTexCoordinateHandle() const {
    return mTexCoordinateHandle;
}

GLuint ShaderProgram::getColorHandle() const {
    return mColorHandle;
}

} /* namespace gles */
} /* namespace ann */
