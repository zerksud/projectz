/*
 * Dungeon.h
 *
 *  Created on: Mar 24, 2013
 *      Author: zerksud
 */

#ifndef DUNGEON_H_
#define DUNGEON_H_

#include <unordered_map>

#include "Monster.h"

namespace dungeon {
namespace model {

enum CellType {
    WALL,
    FLOOR
};

typedef std::unordered_map<IdType, Monster*> MonsterList;
typedef std::pair<IdType, Monster*> MonsterPair;

class Dungeon {
public:
    Dungeon(int width, int height, CellType* terrain, const Position& startPosition);
    virtual ~Dungeon();

    int getWidth() const;
    int getHeight() const;

    bool emptyCell(int x, int y) const;
    bool emptyCell(const Position& pos) const;

    bool floorCell(int x, int y) const;
    bool floorCell(const Position& pos) const;

    Position getStartPosition() const;  //TODO: getEntranceList?

    void placeMonster(Monster* monster);
    Monster* getMonster(IdType id);
    const MonsterList& getMonsters();

private:
    Dungeon(const Dungeon& other);
    Dungeon& operator=(const Dungeon& other);

    bool validIndices(int x, int y) const;
    int calcIndex(int x, int y) const;
    bool floorCellImpl(int x, int y) const;
    bool emptyCellImpl(int x, int y) const;

    int mWidth;
    int mHeight;
    CellType* mTerrain;
    Position mStartPosition;

    MonsterList mMonsters;
};

} /* namespace model */
} /* namespace dungeon */
#endif /* DUNGEON_H_ */
