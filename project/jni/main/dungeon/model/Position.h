/*
 * Position.h
 *
 *  Created on: Mar 25, 2013
 *      Author: zerksud
 */

#ifndef POSITION_H_
#define POSITION_H_

namespace dungeon {
namespace model {

class PositionDiff {
public:
    PositionDiff();
    PositionDiff(int dXValue, int dYValue);

    int getdX() const;
    int getdY() const;

private:
    int mdX;
    int mdY;
};

class Position {
public:
    Position();
    Position(const Position& other);
    Position(int xValue, int yValue);
    virtual ~Position();

    int getX() const;
    int getY() const;

    const Position operator+(const PositionDiff& diff) const;
    const Position operator-(const PositionDiff& diff) const;
    const PositionDiff operator-(const Position& other) const;
    bool operator==(const Position& other) const;
    Position& operator=(const Position& other);

private:
    int mX;
    int mY;
};

} /* namespace model */
} /* namespace dungeon */
#endif /* POSITION_H_ */
