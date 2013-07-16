/*
 * Monster.cpp
 *
 *  Created on: Mar 25, 2013
 *      Author: zerksud
 */

#include "Monster.h"

namespace dungeon {
namespace model {

Monster::Monster() {
}

Monster::~Monster() {
}

const Position& Monster::getPosition() const {
    return mPosition;
}

void Monster::setPosition(const Position& position) {
    mPosition = position;
}

Direction& Monster::getDirection() {
    return mDirection;
}

} /* namespace model */
} /* namespace dungeon */
