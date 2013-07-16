/*
 * Constants.cpp
 *
 *  Created on: Apr 10, 2013
 *      Author: zerksud
 */

#include "Constants.h"

namespace dungeon {
namespace model {

const char* Constants::kMonsterMoveResultNotification = "MonsterMoveResultNotification";
const char* Constants::kMonsterRotateNotification = "MonsterRotateNotification";
const char* Constants::kEndOfPlayerTurnNotification = "EndOfPlayerTurnNotification";
const char* Constants::kAllRoundActionsDoneNotification = "EndOfRoundNotification";

const char* Constants::kPlayerMoveCommand = "PlayerMoveCommand";
const char* Constants::kPlayerRotateCommand = "PlayerRotateCommand";

const char* Constants::kMoveDirectionForward = "Forward";
const char* Constants::kMoveDirectionBackward = "Backward";
const char* Constants::kMoveDirectionLeft = "Left";
const char* Constants::kMoveDirectionRight = "Right";

const char* Constants::kRotateDirectionHalfLeft = "HalfLeft";
const char* Constants::kRotateDirectionHalfRight = "HalfRight";

Constants::~Constants() {
}

Constants::Constants() {
}

} /* namespace model */
} /* namespace dungeon */
