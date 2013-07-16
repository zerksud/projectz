/*
 * UpdateManager.cpp
 *
 *  Created on: Mar 26, 2013
 *      Author: zerksud
 */

#include "UpdateManager.h"

#include <errno.h>

#include <algorithm>

#include <ananas/util/ZLogger.h>

namespace ann {

UpdateManager::UpdateManager() {
    mLastUpdateTime = 0;
}

UpdateManager::~UpdateManager() {
}

void UpdateManager::update() {
    timespec curTime;
    errno = 0;
    int success = clock_gettime(CLOCK_MONOTONIC, &curTime);
    if (success != 0) {
        LOGE("Can't get current monotonic time: errno = ", errno);
        return;
    }

    time_t currTime = curTime.tv_sec * 1000 + curTime.tv_nsec / 1e6;

    if (mLastUpdateTime == 0) {
        mLastUpdateTime = currTime;
        return;
    }

    time_t dt = currTime - mLastUpdateTime;
    if (dt > 0) {
        for (Updatable* object : mUpdatables) {
            object->update(dt);
        }

        mLastUpdateTime = currTime;
    }
}

void UpdateManager::addUpdatable(Updatable* object) {
    UpdatableList::const_iterator pos = std::find(mUpdatables.begin(), mUpdatables.end(), object);
    if (pos != mUpdatables.end()) {
        LOGE("Can't add already added updatable");
        return;
    }

    mUpdatables.push_back(object);
}

void UpdateManager::removeUpdatable(Updatable* object) {
    UpdatableList::iterator pos = std::find(mUpdatables.begin(), mUpdatables.end(), object);
    if (pos == mUpdatables.end()) {
        LOGE("Can't remove not added updatable");
        return;
    }

    mUpdatables.erase(pos);
}

} /* namespace ann */
