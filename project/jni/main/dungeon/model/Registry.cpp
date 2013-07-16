/*
 * Registry.cpp
 *
 *  Created on: Apr 8, 2013
 *      Author: zerksud
 */

#include "Registry.h"

#include <ananas/util/ZLogger.h>

namespace dungeon {
namespace model {

Registry::Registry() {
    mNextFreeId = Registrable::kNoId + 1;
}

Registry::~Registry() {
}

Registry& Registry::getInstance() {
    static Registry instance;

    return instance;
}

void Registry::assignUniqueId(Registrable* object) {
    if (object->getId() != Registrable::kNoId) {
        LOGE("Can't register one object twice");
        return;
    }

    if (!mReleasedIds.empty()) {
        IdType id = *mReleasedIds.begin();
        mReleasedIds.erase(id);
        object->setId(id);
    } else {
        object->setId(mNextFreeId++);
    }
}

void Registry::releaseUniqueId(Registrable* object) {
    IdType id = object->getId();

    if (id == Registrable::kNoId) {
        LOGE("Can't relese empty id");
        return;
    }

    if (id >= mNextFreeId) {
        LOGE("Can't release not assigned id");
        return;
    }

    if (mReleasedIds.count(id) > 0) {
        LOGE("Can't release already released id");
        return;
    }

    object->setId(Registrable::kNoId);
    mReleasedIds.insert(id);
}

} /* namespace model */
} /* namespace dungeon */
