/*
 * Monster.h
 *
 *  Created on: Mar 25, 2013
 *      Author: zerksud
 */

#ifndef MONSTER_H_
#define MONSTER_H_

#include "Direction.h"
#include "Position.h"
#include "Registrable.h"

namespace dungeon {
namespace model {

class Monster: public Registrable {
public:
    Monster();
    virtual ~Monster();

    const Position& getPosition() const;
    void setPosition(const Position& position);

    Direction& getDirection();

private:
    Position mPosition;
    Direction mDirection;


};

} /* namespace model */
} /* namespace dungeon */
#endif /* MONSTER_H_ */
