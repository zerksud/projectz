/*
 * Helpers.h
 *
 *  Created on: Mar 26, 2013
 *      Author: zerksud
 */

#ifndef GAMEHELPERS_H_
#define GAMEHELPERS_H_

#include <glm/glm.hpp>

#include <dungeon/model/Position.h>

namespace dungeon {
namespace ann_glue {
namespace helpers {

glm::vec3 glPoint(const model::Position& pos);
glm::vec3 glPoint(const model::PositionDiff& posDiff);
glm::vec3 glPoint(int x, int y);

} /* namespace game_helpers */
} /* namespace ann_glue */
} /* namespace dungeon */
#endif /* HELPERS_H_ */
