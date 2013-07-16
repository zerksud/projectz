/*
 * PlayerController.cpp
 *
 *  Created on: Apr 12, 2013
 *      Author: zerksud
 */

#include "PlayerController.h"

#include <ananas/util/NotificationCenter.h>
#include <ananas/util/ZLogger.h>

#include "Game.h"
#include "Helpers.h"

namespace dungeon {
namespace model {

PlayerController::PlayerController() {
    ann::util::NotificationCenter::getInstance().addObserver(Constants::kPlayerMoveCommand, this, [this](const ann::util::Dictionary& dict) {
        this->handlePlayerMoveCommand(dict);
    });
    ann::util::NotificationCenter::getInstance().addObserver(Constants::kPlayerRotateCommand, this, [this](const ann::util::Dictionary& dict) {
        this->handlePlayerRotateCommand(dict);
    });
}

PlayerController::~PlayerController() {
    ann::util::NotificationCenter::getInstance().removeObserver(Constants::kPlayerMoveCommand, this);
    ann::util::NotificationCenter::getInstance().removeObserver(Constants::kPlayerRotateCommand, this);
}

void PlayerController::handlePlayerMoveCommand(const ann::util::Dictionary& dict) {
    std::string directionCommand = dict["direction"].asString();
    MoveDirection moveDirection = helpers::parseDirection(directionCommand);

    Game& game = Game::getInstance();
    Monster* player = game.getPlayer();

    Position oldPos = player->getPosition();
    bool success = game.getMonsterDriver()->tryToMove(player, moveDirection);
    Position expectedPos = game.getMonsterDriver()->getLastExpectedMovePosition();

    ann::util::Dictionary responce;

    responce["id"] = player->getId();
    responce["success"] = success;
    responce["positionDiff"] = helpers::makeDictionary(expectedPos - oldPos);

    ann::util::NotificationCenter::getInstance().postNotification(Constants::kMonsterMoveResultNotification, responce);

    if (success) {
        ann::util::NotificationCenter::getInstance().postNotification(Constants::kEndOfPlayerTurnNotification);
    } else {
        ann::util::NotificationCenter::getInstance().postNotification(Constants::kAllRoundActionsDoneNotification);
    }
}

void PlayerController::handlePlayerRotateCommand(const ann::util::Dictionary& dict) {
    std::string direction = dict["direction"].asString();

    int angleDiff = 0;
    Game& game = Game::getInstance();
    MonsterDriver* monsterDriver = game.getMonsterDriver();
    Monster* player = game.getPlayer();

    if (direction == Constants::kRotateDirectionHalfLeft) {
        monsterDriver->rotateHalfLeft(player);
        angleDiff = Constants::kMonsterRotateStep;
    } else if (direction == Constants::kRotateDirectionHalfRight) {
        monsterDriver->rotateHalfRight(player);
        angleDiff = -Constants::kMonsterRotateStep;
    } else {
        LOGE("Uknown rotation direction %s", direction.c_str());
    }

    ann::util::Dictionary responce;
    responce["id"] = player->getId();
    responce["angleDiff"] = angleDiff;

    ann::util::NotificationCenter::getInstance().postNotification(Constants::kMonsterRotateNotification, responce);

    ann::util::NotificationCenter::getInstance().postNotification(Constants::kAllRoundActionsDoneNotification);
}

} /* namespace model */
} /* namespace dungeon */
