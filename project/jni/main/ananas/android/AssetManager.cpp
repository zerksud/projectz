/*
 * AssetManager.cpp
 *
 *  Created on: Mar 13, 2013
 *      Author: zerksud
 */

#include "AssetManager.h"

#include "AndroidLayer.h"
#include <ananas/util/ZLogger.h>

#define READ_CHUNK_SIZE  1024

namespace ann {
namespace android {

AssetManager::AssetManager() {
    mOwnedBuff = new RawBuff();
}

AssetManager::~AssetManager() {
    delete mOwnedBuff;
}

void AssetManager::loadRawDataIntoOwnedBuff(const char* assetFileName, bool nullTerminated) {
    mOwnedBuff->setBuff(nullptr, 0);
    AAssetManager* assetManager = AndroidLayer::getInstance().getAAssetManager();    //TODO: lock local mutex for thread-safe assets loading?

    AAsset *asset = AAssetManager_open(assetManager, assetFileName, AASSET_MODE_STREAMING);
    if (asset == nullptr) {
        LOGE("Can't open file %s", assetFileName);
        return;
    }
    off_t fileLength = AAsset_getLength(asset);
    size_t buffSize = nullTerminated ? fileLength + 1 : fileLength;

    uint8_t *buff = new uint8_t[buffSize];
    off_t readBytes = 0;
    off_t stepReadButes;
    while ((stepReadButes = AAsset_read(asset, buff + readBytes, READ_CHUNK_SIZE)) > 0) {
        readBytes += stepReadButes;
    }
    if (nullTerminated) {
        buff[readBytes] = 0;
    }

    mOwnedBuff->setBuff(buff, buffSize);
}

const char* AssetManager::loadText(const char *assetFileName) {
    loadRawDataIntoOwnedBuff(assetFileName, true);
    const char* text = reinterpret_cast<const char*>(mOwnedBuff->getBuff());

    return text;
}

const RawBuff* AssetManager::loadData(const char* assetFileName) {
    loadRawDataIntoOwnedBuff(assetFileName, false);
    return mOwnedBuff;
}

} /* namespace android*/
} /* namespace ann */
