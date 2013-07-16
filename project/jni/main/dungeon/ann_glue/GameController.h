/*
 * GameController.h
 *
 *  Created on: Mar 25, 2013
 *      Author: zerksud
 */

#ifndef GAMECONTROLLER_H_
#define GAMECONTROLLER_H_

#include <unordered_map>

#include <ananas/engine/UserData.h>

#include "UIProxy.h"
#include "MonsterAnimationController.h"

namespace dungeon {
namespace ann_glue {

class GameController: public ann::UserData {
public:
    GameController();
    virtual ~GameController();

    void resetGame();

private:
    MonsterAnimationController* mMonsterController;
    UIProxy mUIProxy;
};

} /* namespace ann_glue */
} /* namespace dungeon */
#endif /* GAMECONTROLLER_H_ */
