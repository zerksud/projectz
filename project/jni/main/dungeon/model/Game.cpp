/*
 * Game.cpp
 *
 *  Created on: Mar 25, 2013
 *      Author: zerksud
 */

#include "Game.h"

#include <ananas/util/ZLogger.h>

#include "Constants.h"
#include "Helpers.h"
#include "Registry.h"

namespace dungeon {
namespace model {

Game& Game::getInstance() {
    static Game instance;

    return instance;
}

Game::Game() {
    mDungeon = nullptr;
    mPlayer = nullptr;
}

Game::~Game() {
    releaseDynamicObjects();
}

void Game::releaseDynamicObjects() {
    if (mDungeon != nullptr) {
        delete mDungeon;
    }
    if (mPlayer != nullptr) {
        delete mPlayer;
    }
}

Dungeon* Game::getDungeon() {
    return mDungeon;
}

MonsterDriver* Game::getMonsterDriver() {
    return &mMonsterDriver;
}

Monster* Game::getPlayer() {
    return mPlayer;
}

void Game::newGame() {
    releaseDynamicObjects();

    mDungeon = mDungeonGenerator.generateDungeon();
    mMonsterDriver.setCurrentDungeon(mDungeon);

    mPlayer = new Monster();
    Registry::getInstance().assignUniqueId(mPlayer);
    mPlayer->setPosition(mDungeon->getStartPosition());
    mDungeon->placeMonster(mPlayer);
}

} /* namespace model */
} /* namespace dungeon */
