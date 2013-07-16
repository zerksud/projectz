/*
 * GameController.cpp
 *
 *  Created on: Mar 25, 2013
 *      Author: zerksud
 */

#include "GameController.h"

#include <ananas/Engine.h>
#include <ananas/util/ZLogger.h>

#include <dungeon/model/Game.h>
#include <dungeon/model/Monster.h>

#include "DungeonNodeBuilder.h"
#include "Helpers.h"

namespace dungeon {
namespace ann_glue {

GameController::GameController() {
    mMonsterController = new MonsterAnimationController();
}

GameController::~GameController() {
    delete mMonsterController;
}

void GameController::resetGame() {
    model::Game& game = model::Game::getInstance();
    game.newGame();

    mMonsterController->clearMonsterNodes();

    ann::gles::SceneManager* sm = ann::Engine::getInstance().getGLESLayer()->getSceneManager();
    sm->clear();

    model::Dungeon* dungeon = game.getDungeon();
    ann::gles::SceneNode* dungeonNode = DungeonNodeBuilder::buildDungeonNode(*dungeon);
    sm->getRootSceneNode()->attachNode(dungeonNode);

    ann::gles::SceneNode* playerNode = new ann::gles::SceneNode();
    model::Monster* player = game.getPlayer();
    model::Position playerPos = player->getPosition();
    playerNode->translate(helpers::glPoint(playerPos));
    sm->getRootSceneNode()->attachNode(playerNode);

    ann::gles::SceneNode* cameraNode = new ann::gles::SceneNode();
    cameraNode->translate(0.0f, 0.0f, 0.5f);
    playerNode->attachNode(cameraNode);
    sm->attachCamera(cameraNode);

    mMonsterController->addMonsterNode(player->getId(), playerNode);

    ann::gles::Entity skull("skull.bobj", "perfragment", "skull.png");
    ann::gles::SceneNode* skullNode = new ann::gles::SceneNode();
    skullNode->attachEntity(skull);
    skullNode->scale(0.5f);
    skullNode->rotate(180.0f, 0.0f, 1.0f, 0.0f);

    model::IdType playerId = player->getId();
    for (const model::MonsterPair& monsterPair : dungeon->getMonsters()) {
        model::IdType monsterId = monsterPair.first;
        if (monsterId == playerId) {
            continue;
        }

        model::Monster* monster = monsterPair.second;
        model::Position monsterPos = monster->getPosition();

        ann::gles::SceneNode* monsterNode = new ann::gles::SceneNode(*skullNode);
        monsterNode->translate(helpers::glPoint(monsterPos));
        sm->getRootSceneNode()->attachNode(monsterNode);

        mMonsterController->addMonsterNode(monsterId, monsterNode);
    }
    delete skullNode;

    ann::gles::Entity sword("sword.bobj", "perfragment", "sword.png");
    ann::gles::SceneNode* swordNode = new ann::gles::SceneNode();
    swordNode->attachEntity(sword);
    swordNode->translate(0.2f, -0.1f, -1.0f);
    swordNode->rotate(-90.0f, 1.0f, 0.0f, 0.0f);
    swordNode->rotate(-45.0f, 0.0f, 1.0f, 0.0f);
    swordNode->rotate(-45.0f, 1.0f, 0.0f, 0.0f);

    sm->getRootHUDNode()->attachNode(swordNode);
}

} /* namespace ann_glue */
} /* namespace dungeon */
