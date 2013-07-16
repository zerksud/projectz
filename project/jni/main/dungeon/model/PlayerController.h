/*
 * PlayerController.h
 *
 *  Created on: Apr 12, 2013
 *      Author: zerksud
 */

#ifndef PLAYERCONTROLLER_H_
#define PLAYERCONTROLLER_H_

#include <ananas/util/Dictionary.h>

#include "Monster.h"

namespace dungeon {
namespace model {

class PlayerController {
public:
    PlayerController();
    virtual ~PlayerController();

    void setPlayer(Monster* player);

private:
    void handlePlayerMoveCommand(const ann::util::Dictionary& dict);
    void handlePlayerRotateCommand(const ann::util::Dictionary& dict);
};

} /* namespace model */
} /* namespace dungeon */
#endif /* PLAYERCONTROLLER_H_ */
