/*
 * MonsterDriver.cpp
 *
 *  Created on: Mar 25, 2013
 *      Author: zerksud
 */

#include "MonsterDriver.h"

#include <stdlib.h>

#include <ananas/util/ZLogger.h>

namespace dungeon {
namespace model {

MonsterDriver::MonsterDriver() {
    mCurrentDungeon = nullptr;
}

MonsterDriver::~MonsterDriver() {
}

void MonsterDriver::setCurrentDungeon(Dungeon* dungeon) {
    mCurrentDungeon = dungeon;
}

void MonsterDriver::rotateHalfLeft(Monster* monster) {
    monster->getDirection().rotateHalfLeft();
}

void MonsterDriver::rotateHalfRight(Monster* monster) {
    monster->getDirection().rotateHalfRight();
}

Position MonsterDriver::getLastExpectedMovePosition() {
    return mLastExpectedMovePosition;
}

Direction alignDirection(Direction direction, MoveDirection moveDirection) {
    Direction alignedDirection = direction;

    switch (moveDirection) {
    case FORWARD:
        break;
    case BACKWARD:
        alignedDirection.rotateBack();
        break;
    case LEFT:
        alignedDirection.rotateLeft();
        break;
    case RIGHT:
        alignedDirection.rotateRight();
        break;
    default:
        LOGE("Unknown move direction %d", moveDirection);
        break;
    }

    return alignedDirection;
}

Position expectedMovePosition(Monster& monster, MoveDirection moveDirection) {
    Direction alignedDirection = alignDirection(monster.getDirection(), moveDirection);
    PositionDiff diff = alignedDirection.predictMove();
    Position newPosition = monster.getPosition() + diff;

    LOGV("diff = [%d; %d]", diff.getdX(), diff.getdY());

    return newPosition;
}

bool MonsterDriver::diagonalMovementAroundTheCorner(const Position& origin, const Position& destination) const {
    PositionDiff diff = destination - origin;
    int sum = abs(diff.getdX()) + abs(diff.getdY());
    if (sum <= 1) {
        return false;
    }

    return !mCurrentDungeon->floorCell(origin.getX(), origin.getY() + diff.getdY()) ||
            !mCurrentDungeon->floorCell(origin.getX() + diff.getdX(), origin.getY());
}

bool MonsterDriver::tryToMove(Monster* monster, MoveDirection moveDirection) {
    mLastExpectedMovePosition = expectedMovePosition(*monster, moveDirection);

    bool movementIsPossible = mCurrentDungeon->emptyCell(mLastExpectedMovePosition) &&
            !diagonalMovementAroundTheCorner(monster->getPosition(), mLastExpectedMovePosition);

    if (movementIsPossible) {
        monster->setPosition(mLastExpectedMovePosition);
    }

    return movementIsPossible;
}

} /* namespace model */
} /* namespace dungeon */
