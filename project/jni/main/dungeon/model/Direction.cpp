/*
 * Direction.cpp
 *
 *  Created on: Mar 25, 2013
 *      Author: zerksud
 */

#include "Direction.h"

#include "math.h"

#include <algorithm>

#include <ananas/util/ZLogger.h>

#undef LOGD
#define LOGD(...)   ((void)0)

namespace dungeon {
namespace model {

Direction::PredictedMovesMap Direction::kPredictedMoves = {
        { 0, PositionDiff(1.0f, 0.0f) },
        { 45, PositionDiff(1.0f, -1.0f) },
        { 90, PositionDiff(0.0f, -1.0f) },
        { 135, PositionDiff(-1.0f, -1.0f) },
        { 180, PositionDiff(-1.0f, 0.0f) },
        { 225, PositionDiff(-1.0f, 1.0f) },
        { 270, PositionDiff(0.0f, 1.0f) },
        { 315, PositionDiff(1.0f, 1.0f) } };

Direction::Direction() {
    mAngle = 0;
}

Direction::~Direction() {
}

void Direction::rotateLeft() {
    rotate(90);
}

void Direction::rotateRight() {
    rotate(-90);
}

void Direction::rotateBack() {
    rotate(180);
}

void Direction::rotateHalfLeft() {
    rotate(Constants::kMonsterRotateStep);
}

void Direction::rotateHalfRight() {
    rotate(-Constants::kMonsterRotateStep);
}

PositionDiff Direction::getPositionDiff(int angle) {
    PredictedMovesMap::const_iterator pos = kPredictedMoves.find(angle);

    PositionDiff diff;
    if (pos != kPredictedMoves.end()) {
        diff = pos->second;
    } else {
        LOGE("Got unsupported angle %d", angle);
    }
    LOGD("angle = %d diff = [%d; %d]", angle, diff.getdX(), diff.getdY());

    return diff;
}

PositionDiff Direction::predictMove() const {
    return getPositionDiff(mAngle);
}

int Direction::align(int angle) {
    LOGD("mAngle = %d, angle = %d", mAngle, angle);
    int discreteDiff = round((1.0 * angle - mAngle) / Constants::kMonsterRotateStep) * Constants::kMonsterRotateStep;

    if (discreteDiff < 0) { // FIXME: ugly angle conversions
        discreteDiff += 360;
    }

    if (discreteDiff > 180) {
        discreteDiff -= 360;
    }

    LOGD("diff = %d", discreteDiff);

    rotate(discreteDiff);

    LOGD("mAngle = %d", mAngle);

    return discreteDiff;
}

void Direction::rotate(int angle) {
    int newAngle = mAngle + angle;
    while (newAngle < 0) {
        newAngle += 360;
    }
    newAngle %= 360;

    mAngle = newAngle;
}

} /* namespace model */
} /* namespace dungeon */
