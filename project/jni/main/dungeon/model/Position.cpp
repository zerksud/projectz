/*
 * Position.cpp
 *
 *  Created on: Mar 25, 2013
 *      Author: zerksud
 */

#include "Position.h"

namespace dungeon {
namespace model {

Position::Position() {
    mX = 0;
    mY = 0;
}

Position::Position(const Position& other) {
    mX = other.mX;
    mY = other.mY;
}

Position::Position(int xValue, int yValue) {
    mX = xValue;
    mY = yValue;
}

Position::~Position() {
}

int Position::getX() const {
    return mX;
}

int Position::getY() const {
    return mY;
}

const Position Position::operator+(const PositionDiff& diff) const {
    int x = mX + diff.getdX();
    int y = mY + diff.getdY();

    return Position(x, y);
}

const Position Position::operator -(const PositionDiff& diff) const {
    int x = mX - diff.getdX();
    int y = mY - diff.getdY();

    return Position(x, y);
}

const PositionDiff Position::operator -(const Position& other) const {
    int dx = mX - other.getX();
    int dy = mY - other.getY();

    return PositionDiff(dx, dy);
}

bool Position::operator ==(const Position& other) const {
    return (mX == other.mX && mY == other.mY);
}

Position& Position::operator=(const Position& other) {
    if (this != &other) {
        mX = other.mX;
        mY = other.mY;
    }

    return *this;
}

PositionDiff::PositionDiff() {
    mdX = 0;
    mdY = 0;
}

PositionDiff::PositionDiff(int dXValue, int dYValue) {
    mdX = dXValue;
    mdY = dYValue;
}

int PositionDiff::getdX() const {
    return mdX;
}

int PositionDiff::getdY() const {
    return mdY;
}

} /* namespace model */
} /* namespace dungeon */
