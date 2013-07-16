/*
 * DungeonHelpers.cpp
 *
 *  Created on: Apr 1, 2013
 *      Author: zerksud
 */

#include "Helpers.h"

#include <string>

#include <ananas/util/ZLogger.h>

namespace dungeon {
namespace model {
namespace helpers {

ann::util::Dictionary makeDictionary(const Position& pos) {
    ann::util::Dictionary dict;
    dict["x"] = pos.getX();
    dict["y"] = pos.getY();

    return dict;
}

ann::util::Dictionary makeDictionary(const PositionDiff& posDiff) {
    ann::util::Dictionary dict;
    dict["dx"] = posDiff.getdX();
    dict["dy"] = posDiff.getdY();

    return dict;
}

void printParseError(const char* messageFormat, const ann::util::Dictionary& value) {
    Json::FastWriter fw;
    std::string json = fw.write(value);

    LOGE(messageFormat, json.c_str());
}

Position parsePosition(const ann::util::Dictionary& dict) {
    if (dict.type() != Json::objectValue) {
        printParseError("Can't parse Position from non-dictionary %s", dict);
    }

    int x = dict["x"].asInt();
    int y = dict["y"].asInt();

    return Position(x, y);
}

PositionDiff parsePositionDiff(const ann::util::Dictionary& dict) {
    if (dict.type() != Json::objectValue) {
        printParseError("Can't parse PositionDiff from non-dictionary %s", dict);
    }

    int x = dict["dx"].asInt();
    int y = dict["dy"].asInt();

    return PositionDiff(x, y);
}

MoveDirection parseDirection(std::string direction) {
    MoveDirection moveDirection = NO_DIRECTION;
    if (direction == Constants::kMoveDirectionForward) {
        moveDirection = FORWARD;
    } else if (direction == Constants::kMoveDirectionLeft) {
        moveDirection = LEFT;
    } else if (direction == Constants::kMoveDirectionRight) {
        moveDirection = RIGHT;
    } else if (direction == Constants::kMoveDirectionBackward) {
        moveDirection = BACKWARD;
    } else {
        LOGE("Can't move player in unknown direction %s", direction.c_str());
    }

    return moveDirection;
}

} /* namespace helpers */
} /* namespace model */
} /* namespace dungeon */
