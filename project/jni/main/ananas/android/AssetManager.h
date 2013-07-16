/*
 * AssetManager.h
 *
 *  Created on: Mar 13, 2013
 *      Author: zerksud
 */

#ifndef ASSETMANAGER_H_
#define ASSETMANAGER_H_

#include <android/asset_manager_jni.h>

#include "RawBuff.h"

namespace ann {
namespace android {

class AssetManager {
public:
    AssetManager();
    virtual ~AssetManager();

    const char* loadText(const char *assetFileName);    // TODO: use std::string?
    const RawBuff* loadData(const char* assetFileName);

private:
    void freeOwnedBuff();
    void loadRawDataIntoOwnedBuff(const char* assetFileName, bool nullTerminated);

    RawBuff* mOwnedBuff;
};

} /* namespace android*/
} /* namespace ann */
#endif /* ASSETMANAGER_H_ */
