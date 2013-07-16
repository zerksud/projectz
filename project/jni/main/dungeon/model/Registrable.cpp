/*
 * Registrable.cpp
 *
 *  Created on: Apr 8, 2013
 *      Author: zerksud
 */

#include "Registrable.h"

namespace dungeon {
namespace model {

Registrable::Registrable() {
    mId = kNoId;
}

Registrable::Registrable(const Registrable& other) {
    mId = kNoId;
}

Registrable::~Registrable() {
}

IdType Registrable::getId() const {
    return mId;
}

void Registrable::setId(IdType id) {
    mId = id;
}

bool Registrable::isRegistered() const {
    return mId != kNoId;
}

} /* namespace model */
} /* namespace dungeon */
