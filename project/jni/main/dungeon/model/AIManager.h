/*
 * AIManager.h
 *
 *  Created on: Apr 9, 2013
 *      Author: zerksud
 */

#ifndef AIMANAGER_H_
#define AIMANAGER_H_

#include "Dungeon.h"
#include "MonsterDriver.h"

namespace dungeon {
namespace model {

class AIManager {
public:
    AIManager();
    virtual ~AIManager();

    void update();
};

} /* namespace model */
} /* namespace dungeon */
#endif /* AIMANAGER_H_ */
