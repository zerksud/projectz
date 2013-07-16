/*
 * Dungeon.cpp
 *
 *  Created on: Mar 24, 2013
 *      Author: zerksud
 */

#include "Dungeon.h"

#include <algorithm>

#include <ananas/util/ZLogger.h>

#include "Registry.h"

namespace dungeon {
namespace model {

Dungeon::Dungeon(int width, int height, CellType* terrain, const Position& startPosition) {
    mWidth = width;
    mHeight = height;
    mTerrain = terrain;
    mStartPosition = startPosition;
}

Dungeon::~Dungeon() {
    delete mTerrain;
    for (MonsterPair monsterPair : mMonsters) {
        Monster* monster = monsterPair.second;
        Registry::getInstance().releaseUniqueId(monster);
        delete monster;
    }
}

int Dungeon::getWidth() const {
    return mWidth;
}

int Dungeon::getHeight() const {
    return mHeight;
}

bool Dungeon::emptyCell(int x, int y) const {
    if (!validIndices(x, y)) {
        LOGE("cell [%d; %d] is out of bounds", x, y);
        return false;
    }

    return emptyCellImpl(x, y);
}

bool Dungeon::emptyCell(const Position& pos) const {
    return emptyCell(pos.getX(), pos.getY());
}

bool Dungeon::floorCell(int x, int y) const {
    if (!validIndices(x, y)) {
        LOGE("cell [%d; %d] is out of bounds", x, y);
        return false;
    }

    return floorCellImpl(x, y);
}

bool Dungeon::floorCell(const Position& pos) const {
    return floorCell(pos.getX(), pos.getY());
}

bool Dungeon::validIndices(int x, int y) const {
    return (x >= 0 || x < mWidth || y >= 0 || y < mHeight);
}

int Dungeon::calcIndex(int x, int y) const {
    return (y * mWidth + x);
}

Position Dungeon::getStartPosition() const {
    return mStartPosition;
}

void Dungeon::placeMonster(Monster* monster) {
    if (!monster->isRegistered()) {
        LOGE("Can't place non registered monster");
        return;
    }

    MonsterList::const_iterator pos = mMonsters.find(monster->getId());;
    if (pos != mMonsters.end()) {
        LOGE("Can't place already placed monster");
        return;
    }

    mMonsters.insert(MonsterPair(monster->getId(), monster));
}

Monster* Dungeon::getMonster(IdType id) {
    Monster* monster = nullptr;

    MonsterList::const_iterator pos = mMonsters.find(id);;
    if (pos != mMonsters.end()) {
        monster = pos->second;
    } else {
        LOGE("Can't return not places monster with id %lu", id);
    }

    return monster;
}

const MonsterList& Dungeon::getMonsters() {
    return mMonsters;
}

bool Dungeon::floorCellImpl(int x, int y) const {
    bool floor = false;
    int index = calcIndex(x, y);
    switch (mTerrain[index]) {
    case WALL:
        floor = false;
        break;
    case FLOOR:
        floor = true;
        break;
    default:
        LOGE("Unknown terrain type %d", mTerrain[index]);
        floor = false;
        break;
    }

    return floor;
}

bool Dungeon::emptyCellImpl(int x, int y) const {
    bool empty = floorCellImpl(x, y);

    if (empty) {
        Position position(x, y);
        for (const MonsterPair& monsterPair : mMonsters) {
            Monster* monster = monsterPair.second;
            if (monster->getPosition() == position) {
                empty = false;
                break;
            }
        }
    }

    return empty;
}

} /* namespace model */
} /* namespace dungeon */
