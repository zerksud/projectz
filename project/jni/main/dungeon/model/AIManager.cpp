/*
 * AIManager.cpp
 *
 *  Created on: Apr 9, 2013
 *      Author: zerksud
 */

#include "AIManager.h"

#include <math.h>

#include <ananas/util/NotificationCenter.h>
#include <ananas/util/ZLogger.h>

#include "Game.h"
#include "Constants.h"
#include "Helpers.h"

#undef LOGD
#define LOGD(...)   ((void)0)

namespace dungeon {
namespace model {

AIManager::AIManager() {
    ann::util::NotificationCenter::getInstance().addObserver(Constants::kEndOfPlayerTurnNotification, this, [this](const ann::util::Dictionary& dict){
        this->update();
    });
}

AIManager::~AIManager() {
    ann::util::NotificationCenter::getInstance().removeObserver(Constants::kEndOfPlayerTurnNotification, this);
}

void alignMonsterToPosition(Monster* monster, const Position& position) {

    Position monsterPosition = monster->getPosition();

    PositionDiff diff = position - monsterPosition;
    double dx = diff.getdX();
    double dy = -diff.getdY();
    double hypotenuse = sqrt(dx * dx + dy * dy);
    double sinAlpha = dy / hypotenuse;
    double cosAlpha = dx / hypotenuse;
    double alpha = acos(cosAlpha);  // FIXME: discrete angle can be calculated without acos
    if (sinAlpha < 0) {
        alpha = M_PI * 2 - alpha;
    }
    alpha = alpha / M_PI * 180.0;

    LOGD("diff = %s", ann::util::dictionaryToString(helpers::makeDictionary(diff)).c_str());
    LOGD("alpha = %f", alpha);

    int rotateAngle = monster->getDirection().align(round(alpha));

    if (rotateAngle != 0) {
        ann::util::Dictionary dict;
        dict["id"] = monster->getId();
        dict["angleDiff"] = rotateAngle;
        ann::util::NotificationCenter::getInstance().postNotification(Constants::kMonsterRotateNotification, dict);
    }
}

void AIManager::update() {
    Game& game = Game::getInstance();
    Dungeon* dungeon = game.getDungeon();
    MonsterDriver* monsterDriver = game.getMonsterDriver();

    Monster* player = game.getPlayer();
    Position playerPosition = player->getPosition();
    IdType playerId = player->getId();

    for (const MonsterPair& monsterPair : dungeon->getMonsters()) {
        IdType monsterId = monsterPair.first;
        if (monsterId == playerId) {
            continue;
        }

        Monster* monster = monsterPair.second;
        Position monsterPosition = monster->getPosition();

        alignMonsterToPosition(monster, playerPosition);

        bool success = monsterDriver->tryToMove(monster, MoveDirection::FORWARD);
        Position expectedPos = monsterDriver->getLastExpectedMovePosition();

        ann::util::Dictionary moveDict;
        moveDict["id"] = monsterId;
        moveDict["success"] = success;
        moveDict["positionDiff"] = helpers::makeDictionary(expectedPos - monsterPosition);
        ann::util::NotificationCenter::getInstance().postNotification(Constants::kMonsterMoveResultNotification, moveDict);

        alignMonsterToPosition(monster, playerPosition);
    }

    ann::util::NotificationCenter::getInstance().postNotification(Constants::kAllRoundActionsDoneNotification);
}

} /* namespace model */
} /* namespace dungeon */
