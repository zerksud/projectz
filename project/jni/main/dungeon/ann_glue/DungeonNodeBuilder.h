/*
 * DungeonNodeBuilder.h
 *
 *  Created on: Mar 25, 2013
 *      Author: zerksud
 */

#ifndef DUNGEONNODEBUILDER_H_
#define DUNGEONNODEBUILDER_H_

#include <ananas/gles/SceneNode.h>

#include <dungeon/model/Dungeon.h>

namespace dungeon {
namespace ann_glue {
namespace DungeonNodeBuilder {

ann::gles::SceneNode* buildDungeonNode(const model::Dungeon& dungeon);

} /* namespace DungeonNodeBuilder */
} /* namespace ann_glue */
} /* namespace dungeon */
#endif /* DUNGEONNODEBUILDER_H_ */
