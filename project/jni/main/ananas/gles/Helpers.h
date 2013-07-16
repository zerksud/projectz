/*
 * Helpers.h
 *
 *  Created on: Mar 14, 2013
 *      Author: zerksud
 */

#ifndef HELPERS_H_
#define HELPERS_H_

#include <string>

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include <glm/glm.hpp>

#include "BoundingBox.h"

#define HANDLE_GL_ERRORS(...)   (ann::gles::helpers::handleErrors(ann::gles::helpers::ErrorSource::GL, __FILE__, __LINE__, __func__, __VA_ARGS__))
#define HANDLE_EGL_ERROR(...)  (ann::gles::helpers::handleErrors(ann::gles::helpers::ErrorSource::EGL, __FILE__, __LINE__, __func__, __VA_ARGS__))

namespace ann {
namespace gles {
namespace helpers {

std::string glErrorString(GLenum errorFlag);

enum ErrorSource {
    EGL,
    GL
};

void handleErrors(ErrorSource source, const char* fileName, int lineNum, const char* funcName, const char* format, ...);

void handleGLErrors(const char* fileName, int lineNum, const char* funcName, const std::string& msg);
void handleEGLError(const char* fileName, int lineNum, const char* funcName, const std::string& msg);

void printEGLConfig(EGLDisplay display, EGLConfig config);

void printMatrix(std::string message, const glm::mat4& matrix);

void printVector(std::string message, const glm::vec4& vector);

void printBuffer(std::string message, size_t size, float* buffer);

void printBoundingBox(std::string message, const BoundingBox& bb);

} /* namespace helpers */
} /* namespace gles */
} /* namespace ann */
#endif /* HELPERS_H_ */
