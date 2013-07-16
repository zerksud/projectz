/*
 * ShaderManager.h
 *
 *  Created on: Mar 13, 2013
 *      Author: zerksud
 */

#ifndef SHADERMANAGER_H_
#define SHADERMANAGER_H_

#include <string>
#include <unordered_map>

#include "ShaderProgram.h"

namespace ann {
namespace gles {

class ShaderManager {
public:
    ShaderManager();
    virtual ~ShaderManager();

    ShaderProgram* createShaderProgram(std::string shaderName);
    ShaderProgram* createShaderProgram(std::string vertexShaderFileName, std::string fragmentShaderFileName);
    void glContextRecreated();
    void glContextDestroyed();

private:
    typedef std::string ShaderName;
    typedef std::pair<const ShaderName, ShaderProgram*> ShaderMapPair;
    typedef std::unordered_map<ShaderName, ShaderProgram*> ShaderProgramMap;

    ShaderProgramMap mShaderPrograms;
};

} /* namespace gles */
} /* namespace ann */
#endif /* SHADERMANAGER_H_ */
