/*
 * Texture.h
 *
 *  Created on: Mar 14, 2013
 *      Author: zerksud
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <string>

#include <GLES2/gl2.h>

namespace ann {
namespace gles {

class Texture {
public:
    Texture(std::string textureFileName);
    virtual ~Texture();

    std::string getFileName() const;

    void setGLName(GLuint glName);
    GLuint getGLName() const;

    bool isLoaded() const;

    void release();

private:
    std::string mFileName;
    GLuint mGLName;

    bool mLoaded;
};

} /* namespace gles */
} /* namespace ann */
#endif /* TEXTURE_H_ */
