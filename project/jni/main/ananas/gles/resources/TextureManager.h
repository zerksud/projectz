/*
 * TextureManager.h
 *
 *  Created on: Mar 14, 2013
 *      Author: zerksud
 */

#ifndef TEXTUREMANAGER_H_
#define TEXTUREMANAGER_H_

#include <string>
#include <unordered_map>

#include "Texture.h"

namespace ann {
namespace gles {

class TextureManager {
public:
    TextureManager();
    virtual ~TextureManager();

    Texture* createTexture(std::string textureFileName);
    void glContextRecreated();
    void glContextDestroyed();

private:
    typedef std::string TextureName;
    typedef std::pair<const TextureName, Texture*> TextureMapPair;
    typedef std::unordered_map<TextureName, Texture*> TextureMap;

    TextureMap mTextures;
};

} /* namespace gles */
} /* namespace ann */
#endif /* TEXTUREMANAGER_H_ */
