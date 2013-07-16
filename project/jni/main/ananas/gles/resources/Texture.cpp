/*
 * Texture.cpp
 *
 *  Created on: Mar 14, 2013
 *      Author: zerksud
 */

#include "Texture.h"

namespace ann {
namespace gles {

Texture::Texture(std::string textureFileName) {
    mGLName = 0;
    mFileName = textureFileName;
    mLoaded = false;
}

Texture::~Texture() {
}

std::string Texture::getFileName() const {
    return mFileName;
}

void Texture::setGLName(GLuint glName) {
    mGLName = glName;
    mLoaded = true;
}

GLuint Texture::getGLName() const {
    return mGLName;
}

bool Texture::isLoaded() const {
    return mLoaded;
}

void Texture::release() {
    if (mLoaded) {
        glDeleteTextures(1, &mGLName);
        mGLName = 0;
        mLoaded = false;
    }
}

} /* namespace gles */
} /* namespace ann */
