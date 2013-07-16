/*
 * DungeonNodeBuilder.cpp
 *
 *  Created on: Mar 25, 2013
 *      Author: zerksud
 */

#include "DungeonNodeBuilder.h"

#include <string>

#include <ananas/gles/resources/Entity.h>
#include <ananas/util/ZLogger.h>

#include "Helpers.h"


#define QUAD_OBJ_FILE_NAME  "quad.bobj"
#define DEFAULT_SHADER_NAME "perfragment"
#define CELL_SIZE           1.0f

#define FLOOR_TEXTURE    "stone_gray0.png"
#define ROOF_TEXTURE     "stone2_gray0.png"
#define WALL_TEXTURE     "stone_brick1.png"

namespace dungeon {
namespace ann_glue {
namespace DungeonNodeBuilder {

ann::gles::SceneNode* createQuad(std::string textureFileName) {
    ann::gles::Entity quadEntity(QUAD_OBJ_FILE_NAME, DEFAULT_SHADER_NAME, textureFileName);

    ann::gles::SceneNode* node = new ann::gles::SceneNode();
    node->attachEntity(quadEntity);
    //TODO: set cell size?

    return node;
}

ann::gles::SceneNode* createFloor() {
    ann::gles::SceneNode* node = createQuad(FLOOR_TEXTURE);

    node->translate(0.0f, -CELL_SIZE / 2.0f, 0.0f);

    return node;
}

ann::gles::SceneNode* createRoof() {
    ann::gles::SceneNode* node = createQuad(ROOF_TEXTURE);

    node->translate(0.0f, CELL_SIZE / 2.0f, 0.0f);
    node->rotate(180.0f, 1.0f, 0.0f, 0.0f);

    return node;
}

ann::gles::SceneNode* createFarWall() {
    ann::gles::SceneNode* node = createQuad(WALL_TEXTURE);

    node->translate(0.0f, 0.0f, -CELL_SIZE / 2.0f);
    node->rotate(90.0f, 1.0f, 0.0f, 0.0f);


    return node;
}

ann::gles::SceneNode* createBackWall() {
    ann::gles::SceneNode* node = createQuad(WALL_TEXTURE);

    node->translate(0.0f, 0.0f, CELL_SIZE / 2.0f);
    node->rotate(90.0f, 1.0f, 0.0f, 0.0f);
    node->rotate(180.0f, 0.0f, 1.0f, 0.0f);

    return node;
}

ann::gles::SceneNode* createLeftWall() {
    ann::gles::SceneNode* node = createQuad(WALL_TEXTURE);

    node->translate(-CELL_SIZE / 2.0f, 0.0f, 0.0f);
    node->rotate(90.0f, 0.0f, 1.0f, 0.0f);
    node->rotate(-90.0f, 0.0f, 0.0f, 1.0f);

    return node;
}

ann::gles::SceneNode* createRightWall() {
    ann::gles::SceneNode* node = createQuad(WALL_TEXTURE);

    node->translate(CELL_SIZE / 2.0f, 0.0f, 0.0f);
    node->rotate(-90.0f, 0.0f, 1.0f, 0.0f);
    node->rotate(90.0f, 0.0f, 0.0f, 1.0f);

    return node;
}

ann::gles::SceneNode* buildDungeonNode(const model::Dungeon& dungeon) {
    ann::gles::SceneNode* dungeonNode = new ann::gles::SceneNode();

    ann::gles::SceneNode* floorNode = createFloor();
    ann::gles::SceneNode* roofNode = createRoof();
    ann::gles::SceneNode* farWallNode = createFarWall();
    ann::gles::SceneNode* backWallNode = createBackWall();
    ann::gles::SceneNode* leftWallNode = createLeftWall();
    ann::gles::SceneNode* rightWallNode = createRightWall();

    int width = dungeon.getWidth();
    int height = dungeon.getHeight();

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (!dungeon.floorCell(x, y)) {
                continue;
            }

            ann::gles::SceneNode* cellNode = new ann::gles::SceneNode();
            cellNode->translate(helpers::glPoint(x, y));
            dungeonNode->attachNode(cellNode);

            ann::gles::SceneNode* floor = new ann::gles::SceneNode(*floorNode);
            cellNode->attachNode(floor);

            ann::gles::SceneNode* roof = new ann::gles::SceneNode(*roofNode);
            cellNode->attachNode(roof);

            if (y == 0 || (y > 0 && !dungeon.floorCell(x, y - 1))) {
                ann::gles::SceneNode* leftWall = new ann::gles::SceneNode(*leftWallNode);
                cellNode->attachNode(leftWall);
            }

            if (y == height - 1 || (y < height - 1 && !dungeon.floorCell(x, y + 1))) {
                ann::gles::SceneNode* rightWall = new ann::gles::SceneNode(*rightWallNode);
                cellNode->attachNode(rightWall);
            }

            if (x == 0 || (x > 0 && !dungeon.floorCell(x - 1, y))) {
                ann::gles::SceneNode* backWall = new ann::gles::SceneNode(*backWallNode);
                cellNode->attachNode(backWall);
            }

            if (x == width - 1 || (x < width - 1 && !dungeon.floorCell(x + 1, y))) {
                ann::gles::SceneNode* farWall = new ann::gles::SceneNode(*farWallNode);
                cellNode->attachNode(farWall);
            }
        }
    }

    delete floorNode;
    delete roofNode;
    delete farWallNode;
    delete backWallNode;
    delete leftWallNode;
    delete rightWallNode;

    return dungeonNode;
}

} /* namespace DungeonNodeBuilder */
} /* namespace ann_glue */
} /* namespace dungeon */
