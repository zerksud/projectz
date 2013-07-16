/*
 * Helpers.cpp
 *
 *  Created on: Mar 14, 2013
 *      Author: zerksud
 */

#include "Helpers.h"

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <vector>

#include <GLES2/gl2.h>

#include <ananas/util/ZLogger.h>
#include <ananas/util/StringHelpers.h>

namespace ann {
namespace gles {
namespace helpers {

std::string glErrorString(GLenum errorFlag) {
    std::string message;
    switch (errorFlag) {
    case GL_INVALID_ENUM:
        message = "Invalid enum";
        break;
    case GL_INVALID_VALUE:
        message = "Invalid value";
        break;
    case GL_INVALID_OPERATION:
        message = "Invalid operation";
        break;
    case GL_OUT_OF_MEMORY:
        message = "Out of memory";
        break;
    default: {
        std::stringstream ss;
        ss << "Unknown GL error flag: " << errorFlag;
        message = ss.str();
    }
        break;
    }

    return message;
}

std::string eglErrorString(EGLint errorFlag) {
    std::string message;
    switch (errorFlag) {
    case EGL_NOT_INITIALIZED:
        message = "EGL_NOT_INITIALIZED: EGL is not initialized, or could not be initialized, for the specified EGL display connection.";
        break;

    case EGL_BAD_ACCESS:
        message = "EGL_BAD_ACCESS: EGL cannot access a requested resource (for example a context is bound in another thread).";
        break;

    case EGL_BAD_ALLOC:
        message = "EGL_BAD_ALLOC: EGL failed to allocate resources for the requested operation.";
        break;
    case EGL_BAD_ATTRIBUTE:
        message = "EGL_BAD_ATTRIBUTE: An unrecognized attribute or attribute value was passed in the attribute list.";
        break;
    case EGL_BAD_CONTEXT:
        message = "EGL_BAD_CONTEXT: An EGLContext argument does not name a valid EGL rendering context.";
        break;
    case EGL_BAD_CONFIG:
        message = "EGL_BAD_CONFIG: An EGLConfig argument does not name a valid EGL frame buffer configuration.";
        break;
    case EGL_BAD_CURRENT_SURFACE:
        message = "EGL_BAD_CURRENT_SURFACE: The current surface of the calling thread is a window, pixel buffer or pixmap that is no longer valid.";
        break;
    case EGL_BAD_DISPLAY:
        message = "EGL_BAD_DISPLAY: An EGLDisplay argument does not name a valid EGL display connection.";
        break;
    case EGL_BAD_SURFACE:
        message = "EGL_BAD_SURFACE: An EGLSurface argument does not name a valid surface (window, pixel buffer or pixmap) configured for GL rendering.";
        break;
    case EGL_BAD_MATCH:
        message = "EGL_BAD_MATCH: Arguments are inconsistent (for example, a valid context requires buffers not supplied by a valid surface).";
        break;
    case EGL_BAD_PARAMETER:
        message = "EGL_BAD_PARAMETER: One or more argument values are invalid.";
        break;
    case EGL_BAD_NATIVE_PIXMAP:
        message = "EGL_BAD_NATIVE_PIXMAP: A NativePixmapType argument does not refer to a valid native pixmap.";
        break;
    case EGL_BAD_NATIVE_WINDOW:
        message = "EGL_BAD_NATIVE_WINDOW: A NativeWindowType argument does not refer to a valid native window.";
        break;
    case EGL_CONTEXT_LOST:
        message = "EGL_CONTEXT_LOST: A power management event has occurred. The application must destroy all contexts and reinitialise OpenGL ES state and objects to continue rendering.";
        break;
    default:
        std::stringstream ss;
        ss << "Unknown EGL error flag: " << errorFlag;
        message = ss.str();
    }
    return message;
}

void handleErrors(ErrorSource source, const char* fileName, int lineNum, const char* funcName, const char* format, ...) {
    va_list args;
    va_start(args, format);
    int messageSize = vsnprintf(nullptr, 0, format, args);
    va_end(args);

    char* buff = new char[messageSize + 1];
    va_start(args, format);
    vsprintf(buff, format, args);
    va_end(args);

    std::string userMessage = buff;
    delete[] buff;

    switch (source) {
    case ErrorSource::EGL:
        handleEGLError(fileName, lineNum, funcName, userMessage);
        break;
    case ErrorSource::GL:
        handleGLErrors(fileName, lineNum, funcName, userMessage);
        break;
    default:
        LOGE("Got uknown ErrorSource %d", source);
        break;
    }
}

void handleGLErrors(const char* fileName, int lineNum, const char* funcName, const std::string& msg) {
    GLenum errorFlag = GL_NO_ERROR;
    while ((errorFlag = glGetError()) != GL_NO_ERROR) {
        std::string fullMessage = msg + ": " + glErrorString(errorFlag);
        util::ZLogger::getInstance().print(ANDROID_LOG_ERROR, fileName, lineNum, funcName, fullMessage.c_str());
    }
}

void handleEGLError(const char* fileName, int lineNum, const char* funcName, const std::string& msg) {
    EGLint errorFlag = eglGetError();
    if (errorFlag != EGL_SUCCESS) {
        std::string fullMessage = msg + ": " + eglErrorString(errorFlag);
        util::ZLogger::getInstance().print(ANDROID_LOG_ERROR, fileName, lineNum, funcName, fullMessage.c_str());
    }
}

std::string getEGLRenderableTypeString(GLint type) {
    std::stringstream ss;
    std::vector<std::string> flags;
    GLint typeCopy = type;
    if (typeCopy & EGL_OPENGL_ES_BIT) {
        flags.push_back("EGL_OPENGL_ES_BIT");
        typeCopy &= ~EGL_OPENGL_ES_BIT;
    }
    if (typeCopy & EGL_OPENVG_BIT) {
        flags.push_back("EGL_OPENVG_BIT");
        typeCopy &= ~EGL_OPENVG_BIT;
    }
    if (typeCopy & EGL_OPENGL_ES2_BIT) {
        flags.push_back("EGL_OPENGL_ES2_BIT");
        typeCopy &= ~EGL_OPENGL_ES2_BIT;
    }
    if (typeCopy & EGL_OPENGL_BIT) {
        flags.push_back("EGL_OPENGL_BIT");
        typeCopy &= ~EGL_OPENGL_BIT;
    }
    if (typeCopy) {
        std::stringstream to_string;    //TODO: itoa?
        to_string << typeCopy;
        flags.push_back(to_string.str());
    }

    for (size_t i = 0; i < flags.size() - 1; ++i) {
        ss << flags[i] << " | ";
    }
    ss << flags[flags.size() - 1];

    ss << " = " << type;

    return ss.str();
}

void printEGLConfig(EGLDisplay display, EGLConfig config) {
    bool success;
    GLint value = 0;
    std::stringstream ss;

    ss << "EGL frame buffer configuration:" << std::endl;

    success = eglGetConfigAttrib(display, config, EGL_RENDERABLE_TYPE, &value);
    if (success == EGL_TRUE) {
        ss << "EGL_RENDERABLE_TYPE = " << getEGLRenderableTypeString(value) << std::endl;
    } else {
        ss << "Can't get EGL_RENDERABLE_TYPE" << std::endl;
    }

    LOGV(ss.str().c_str());
}

void printMatrix(std::string message, const glm::mat4& matrix) {
    std::stringstream ss;
    ss << std::setprecision(2);

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ss << "\t" << matrix[i][j];
        }
        ss << std::endl;
    }

    LOGV("%s\n%s", message.c_str(), ss.str().c_str());
}

void printVector(std::string message, const glm::vec4& vector) {
    std::stringstream ss;
    ss << std::setprecision(2);

    for (int i = 0; i < 4; ++i) {
        ss << "\t" << vector[i];
    }

    LOGV("%s\n%s", message.c_str(), ss.str().c_str());
}

void printBuffer(std::string message, size_t size, float* buffer) {
    std::stringstream ss;
    ss << std::setprecision(2);

    size_t printSize = std::min(size, static_cast<size_t>(100));

    ss << "[";
    for (size_t i = 0; i < printSize - 1; ++i) {
        ss << buffer[i] << ", ";
    }
    if (printSize > 0) {
        ss << buffer[printSize - 1];
    }
    ss << "]";

    LOGV("%s\n%s", message.c_str(), ss.str().c_str());
}

void printBoundingBox(std::string message, const BoundingBox& bb) {
    std::stringstream ss;
    ss << std::setprecision(2);

    const glm::vec3 minP = bb.getMinPoint();
    ss << "[";
    for (int i = 0; i < 3; ++i) {
        ss << minP[i] << ", ";
    }
    ss << "] - [";
    const glm::vec3 maxP = bb.getMaxPoint();
    for (int i = 0; i < 3; ++i) {
        ss << maxP[i] << ", ";
    }
    ss << "]";

    LOGV("%s\n%s", message.c_str(), ss.str().c_str());
}

} /* namespace helpers */
} /* namespace gles */
} /* namespace ann */
