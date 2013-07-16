/*
 * ShaderProgram.h
 *
 *  Created on: Mar 13, 2013
 *      Author: zerksud
 */

#ifndef SHADERPROGRAM_H_
#define SHADERPROGRAM_H_

#include <string>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

namespace ann {
namespace gles {

class ShaderProgram {
public:
    ShaderProgram(std::string vertexShaderFileName, std::string fragmentShaderFileName);
    virtual ~ShaderProgram();

    std::string getVertexShaderFileName() const;
    std::string getFragmentShaderFileName() const;

    void setHandles(GLuint programHandle, GLuint vertexShaderHandle, GLuint fragmentShaderHandle);
    void glUseProgram() const;
    void release();

    bool isLoaded() const;

    GLuint getProgramHandle() const;
    GLuint getMVPMatrixHandle() const;
    GLuint getMVMatrixHandle() const;
    GLuint getLightPosHandle() const;
    GLuint getTextureHandle() const;
    GLuint getPositionHandle() const;
    GLuint getNormalHandle() const;
    GLuint getTexCoordinateHandle() const;
    GLuint getColorHandle() const;

private:
    std::string mVertexShaderFileName;
    std::string mFragmentShaderFileName;

    GLuint mProgramHandle;
    GLuint mVertexShaderHandle;
    GLuint mFragmentShaderHandle;

    GLint mMVPMatrixHandle;
    GLint mMVMatrixHandle;
    GLint mLightPosHandle;
    GLint mTextureHandle;

    GLint mPositionHandle;
    GLint mNormalHandle;
    GLint mTexCoordinateHandle;
    GLint mColorHandle;

    bool mLoaded;
};

} /* namespace gles */
} /* namespace ann */
#endif /* SHADERPROGRAM_H_ */
