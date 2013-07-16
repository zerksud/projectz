/*
 * Helpers.cpp
 *
 *  Created on: Mar 26, 2013
 *      Author: zerksud
 */

#include "Helpers.h"

namespace dungeon {
namespace ann_glue {
namespace helpers {

glm::vec3 glPoint(const model::Position& pos) {
    return glm::vec3(pos.getY(), 0.0f, -pos.getX());
}

glm::vec3 glPoint(const model::PositionDiff& posDiff) {
    return glm::vec3(posDiff.getdY(), 0.0f, -posDiff.getdX());
}

glm::vec3 glPoint(int x, int y) {
    return glm::vec3(y, 0.0f, -x);
}

} /* namespace game_helpers */
} /* namespace ann_glue */
} /* namespace dungeon */
