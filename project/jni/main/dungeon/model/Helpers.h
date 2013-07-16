/*
 * DungeonHelpers.h
 *
 *  Created on: Apr 1, 2013
 *      Author: zerksud
 */

#ifndef DUNGEONHELPERS_H_
#define DUNGEONHELPERS_H_

#include <ananas/util/NotificationCenter.h>

#include "Position.h"
#include "MonsterDriver.h"

namespace dungeon {
namespace model {
namespace helpers {

ann::util::Dictionary makeDictionary(const Position& pos);
ann::util::Dictionary makeDictionary(const PositionDiff& pos);

Position parsePosition(const ann::util::Dictionary& dict);
PositionDiff parsePositionDiff(const ann::util::Dictionary& dict);

MoveDirection parseDirection(std::string direction);

} /* namespace helpers */
} /* namespace model */
} /* namespace dungeon */
#endif /* DUNGEONHELPERS_H_ */
