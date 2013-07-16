/*
 * MonsterDriver.h
 *
 *  Created on: Mar 25, 2013
 *      Author: zerksud
 */

#ifndef MONSTERDRIVER_H_
#define MONSTERDRIVER_H_

#include "Dungeon.h"
#include "Monster.h"

namespace dungeon {
namespace model {

enum MoveDirection {
    NO_DIRECTION = -1,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class MonsterDriver {
public:
    MonsterDriver();
    virtual ~MonsterDriver();

    void setCurrentDungeon(Dungeon* dungeon);

    static void rotateHalfLeft(Monster* monster);
    static void rotateHalfRight(Monster* monster);

    bool tryToMove(Monster* monster, MoveDirection moveDirection);
    Position getLastExpectedMovePosition();

private:
    bool diagonalMovementAroundTheCorner(const Position& origin, const Position& destination) const;

    Dungeon* mCurrentDungeon;
    Position mLastExpectedMovePosition;
};

} /* namespace model */
} /* namespace dungeon */
#endif /* MONSTERDRIVER_H_ */
