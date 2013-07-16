/*
 * TextureManager.cpp
 *
 *  Created on: Mar 14, 2013
 *      Author: zerksud
 */

#include "TextureManager.h"

#include <libpng/png.h>

#include <ananas/gles/Helpers.h>
#include <ananas/Engine.h>
#include <ananas/android/AndroidLayer.h>

#include <ananas/util/Constants.h>
#include <ananas/util/ZLogger.h>

namespace ann {
namespace gles {

TextureManager::TextureManager() {
}

TextureManager::~TextureManager() {
    for (TextureMapPair& e : mTextures) {
        Texture* texture = e.second;
        delete texture;
    }
}

struct memory_read_buff {
    const uint8_t* dataBuff;
    size_t readBytes;
    size_t buffSize;
};

void png_memory_read(png_structp png_ptr, png_bytep outBuff, png_size_t bytesToRead) {
    memory_read_buff* data = static_cast<memory_read_buff*>(png_get_io_ptr(png_ptr));
    size_t bytesToCopy = std::min(bytesToRead, data->buffSize - data->readBytes);

    memcpy(outBuff, data->dataBuff + data->readBytes, bytesToCopy);

    data->readBytes += bytesToCopy;
}

GLint chooseGLFormat(png_structp png_ptr, png_infop info_ptr, int colorType) {
    bool transparency = false;
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(png_ptr);
        transparency = true;
    }

    GLint glFormat = 0;
    switch (colorType) {
    case PNG_COLOR_TYPE_PALETTE:
        png_set_palette_to_rgb(png_ptr);
        glFormat = transparency ? GL_RGBA : GL_RGB;
        break;
    case PNG_COLOR_TYPE_RGB:
        glFormat = transparency ? GL_RGBA : GL_RGB;
        break;
    case PNG_COLOR_TYPE_RGBA:
        glFormat = GL_RGBA;
        break;
    case PNG_COLOR_TYPE_GRAY:
        png_set_expand_gray_1_2_4_to_8(png_ptr);
        glFormat = transparency ? GL_LUMINANCE_ALPHA : GL_LUMINANCE;
        break;
    case PNG_COLOR_TYPE_GA:
        png_set_expand_gray_1_2_4_to_8(png_ptr);
        glFormat = GL_LUMINANCE_ALPHA;
        break;
    default:
        LOGE("Got unknown png color type: %d", colorType);  //TODO: GL_ALPHA?
        break;
    }

    return glFormat;
}

struct gl_texture_data {
    uint8_t* data;
    GLint internalFormat;
    GLsizei width;
    GLsizei height;
};

void makeGLTextureDataFromPng(png_structp png_ptr, png_infop info_ptr, const android::RawBuff *pngRawData, gl_texture_data *textureData) {
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        LOGE("Can't create png read struct");
        return;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr) {
        LOGE("Can't create png info struct");
        return;
    }

    memory_read_buff pngReadBuff = { pngRawData->getBuff(), 0, pngRawData->getSize() };
    png_set_read_fn(png_ptr, &pngReadBuff, png_memory_read);

    png_read_info(png_ptr, info_ptr);

    png_uint_32 width, height;
    int depth, colorType;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &depth, &colorType, nullptr, nullptr, nullptr);
    textureData->width = width;
    textureData->height = height;
    LOGV("Read png header: %dx%d; depth = %d", width, height, depth);

    if (depth < 8) {    //TODO: unnecessary check according to the implementation?
        png_set_packing(png_ptr);
    }

    textureData->internalFormat = chooseGLFormat(png_ptr, info_ptr, colorType);

    png_read_update_info(png_ptr, info_ptr);

    png_size_t rowSize = png_get_rowbytes(png_ptr, info_ptr);
    if (rowSize <= 0) {
        return;
    }

    png_byte* bitmapBuffer = new png_byte[rowSize * height];

    png_bytep *rowPtrs = new png_bytep[height];
    for (png_uint_32 i = 0; i < height; ++i) {
        rowPtrs[height - 1 - i] = bitmapBuffer + i * rowSize;
    }

    png_read_image(png_ptr, rowPtrs);

    textureData->data = bitmapBuffer;

    delete[] rowPtrs;
}

gl_texture_data getTextureBitmap(std::string textureFileName) {
    gl_texture_data textureData;
    memset(&textureData, 0, sizeof textureData);

    std::string textureFullName = util::Constants::kTexturesLocation + textureFileName;
    android::AssetManager *am = android::AndroidLayer::getInstance().getAssetManager();
    const android::RawBuff *pngRawData = am->loadData(textureFullName.c_str());
    if (pngRawData->getBuff() == nullptr) {
        LOGE("Can't get raw data for texture %s", textureFullName.c_str());
        return textureData;
    }

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    makeGLTextureDataFromPng(png_ptr, info_ptr, pngRawData, &textureData);

    png_structpp png_ptr_to_delete = png_ptr ? &png_ptr : nullptr;
    png_infopp info_ptr_to_delete = info_ptr ? &info_ptr : nullptr;
    png_destroy_read_struct(png_ptr_to_delete, info_ptr_to_delete, nullptr);

    return textureData;
}

GLuint loadGLTextureFromFile(std::string textureFileName) {
    GLuint textureName;
    glGenTextures(1, &textureName);

    if (textureName != 0) {
        gl_texture_data bitmap = getTextureBitmap(textureFileName);
        if (bitmap.data != nullptr) {
            glBindTexture(GL_TEXTURE_2D, textureName);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glTexImage2D(GL_TEXTURE_2D, 0, bitmap.internalFormat, bitmap.width, bitmap.height, 0, bitmap.internalFormat, GL_UNSIGNED_BYTE, static_cast<void*>(bitmap.data));
            glGenerateMipmap(GL_TEXTURE_2D);

            delete[] bitmap.data;

            GLenum glError = glGetError();
            if (glError != GL_NO_ERROR) {
                std::string errMsg = helpers::glErrorString(glError);
                LOGE("Can't create texture %s: %s", textureFileName.c_str(), errMsg.c_str());

                glDeleteTextures(1, &textureName);
            }
        } else {
            glDeleteTextures(1, &textureName);
            textureName = 0;
            LOGE("Can't get bitmap for texture %s", textureFileName.c_str());
        }
    }

    if (textureName == 0) {
        LOGE("Error creating texture %s", textureFileName.c_str());
    }

    return textureName;
}

Texture* TextureManager::createTexture(std::string textureFileName) {
    Texture* texture = nullptr;

    TextureMap::const_iterator i = mTextures.find(textureFileName);
    if (i != mTextures.end()) {
        texture = i->second;
    } else {
        texture = new Texture(textureFileName);

        if (Engine::getInstance().getGLESLayer()->glContextIsActive()) {
            GLuint glName = loadGLTextureFromFile(textureFileName);
            texture->setGLName(glName);
        }

        mTextures.insert(TextureMapPair(textureFileName, texture));
    }

    return texture;
}

void TextureManager::glContextRecreated() {
    for (TextureMapPair& e : mTextures) {
        Texture* texture = e.second;
        ann::Engine::getInstance().getGLThread()->addTask([texture]() {
            GLuint glName = loadGLTextureFromFile(texture->getFileName());
            texture->setGLName(glName);
        });
    }
}

void TextureManager::glContextDestroyed() {
    for (TextureMapPair& e : mTextures) {
        Texture* texture = e.second;
        texture->release();
    }
}

} /* namespace gles */
} /* namespace ann */
