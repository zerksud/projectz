/*
 * DungeonGenerator.h
 *
 *  Created on: Mar 25, 2013
 *      Author: zerksud
 */

#ifndef DUNGEONGENERATOR_H_
#define DUNGEONGENERATOR_H_

#include "Dungeon.h"

namespace dungeon {
namespace model {

class DungeonGenerator {
public:
    DungeonGenerator();
    virtual ~DungeonGenerator();

    Dungeon* generateDungeon() const;   //TODO: width, height, seed?

private:
    static const char* kDefaultDungeon;
    static int kDefaultDungeonWidth;
    static int kDefaultDungeonHeight;
};

} /* namespace model */
} /* namespace dungeon */
#endif /* DUNGEONGENERATOR_H_ */
