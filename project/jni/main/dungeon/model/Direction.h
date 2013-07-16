/*
 * Direction.h
 *
 *  Created on: Mar 25, 2013
 *      Author: zerksud
 */

#ifndef DIRECTION_H_
#define DIRECTION_H_

#include "Position.h"

#include <map>

#include "Constants.h"

namespace dungeon {
namespace model {

class Direction {
public:
    Direction();
    virtual ~Direction();

    void rotateLeft();
    void rotateRight();
    void rotateBack();
    void rotateHalfLeft();
    void rotateHalfRight();

    PositionDiff predictMove() const;
    int align(int angle);

private:
    void rotate(int angle);

    typedef std::map<int, PositionDiff> PredictedMovesMap;
    static PredictedMovesMap kPredictedMoves;
    static PositionDiff getPositionDiff(int angle);

    int mAngle;
};

} /* namespace model */
} /* namespace dungeon */
#endif /* DIRECTION_H_ */
