/*
 * ananas_startup.cpp
 *
 *  Created on: Mar 22, 2013
 *      Author: zerksud
 */

#include <ananas/Engine.h>

#include "GameController.h"

void ananas_startup() {
    dungeon::ann_glue::GameController* gc = new dungeon::ann_glue::GameController();
    gc->resetGame();
    ann::Engine::getInstance().setUserData(gc);
}
