/*
 * Game.h
 *
 *  Created on: Mar 25, 2013
 *      Author: zerksud
 */

#ifndef GAME_H_
#define GAME_H_

#include <ananas/util/NotificationCenter.h>

#include "AIManager.h"
#include "DungeonGenerator.h"
#include "MonsterDriver.h"
#include "PlayerController.h"

namespace dungeon {
namespace model {

class Game {
public:
    static Game& getInstance();

    void newGame();

    Dungeon* getDungeon();
    Monster* getPlayer();

    MonsterDriver* getMonsterDriver();

private:
    Game();
    Game(const Game& other);
    virtual ~Game();
    Game& operator=(const Game& other);

    void releaseDynamicObjects();

    AIManager mAIManager;
    DungeonGenerator mDungeonGenerator;
    MonsterDriver mMonsterDriver;
    PlayerController mPlayerController;

    Dungeon* mDungeon;
    Monster* mPlayer;
};

} /* namespace model */
} /* namespace dungeon */
#endif /* GAME_H_ */
