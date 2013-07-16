/*
 * MonsterAnimationController.cpp
 *
 *  Created on: Apr 9, 2013
 *      Author: zerksud
 */

#include "MonsterAnimationController.h"

#include <ananas/Engine.h>
#include <ananas/util/ZLogger.h>

#include <dungeon/model/Constants.h>
#include <dungeon/model/Game.h>
#include <dungeon/model/Helpers.h>
#include <dungeon/model/Position.h>

#include "Constants.h"
#include "Helpers.h"

#undef LOGD
#define LOGD(...)   ((void)0)

namespace dungeon {
namespace ann_glue {

MonsterAnimationController::MonsterAnimationController() {
    ann::util::NotificationCenter& nc = ann::util::NotificationCenter::getInstance();
    nc.addObserver(model::Constants::kMonsterMoveResultNotification, this, [this](const ann::util::Dictionary& dict) {
        queueAction("move", dict);
    });

    nc.addObserver(model::Constants::kMonsterRotateNotification, this, [this](const ann::util::Dictionary& dict) {
        queueAction("rotate", dict);
    });

    nc.addObserver(model::Constants::kAllRoundActionsDoneNotification, this, [this](const ann::util::Dictionary& dict) {
        applyQueuedActions();
    });
}

MonsterAnimationController::~MonsterAnimationController() {
    ann::util::NotificationCenter::getInstance().removeObserver(model::Constants::kMonsterMoveResultNotification, this);
    ann::util::NotificationCenter::getInstance().removeObserver(model::Constants::kMonsterRotateNotification, this);
    ann::util::NotificationCenter::getInstance().removeObserver(model::Constants::kAllRoundActionsDoneNotification, this);
}

void MonsterAnimationController::clearMonsterNodes() {
    mMonsterNodes.clear();
}

void MonsterAnimationController::addMonsterNode(model::IdType id, ann::gles::SceneNode* node) {
    mMonsterNodes.insert(MonsterNodePair(id, node));
}

ann::gles::SceneNode* MonsterAnimationController::getMonsterNode(model::IdType id) {
    MonsterSceneNodeList::const_iterator pos = mMonsterNodes.find(id);
    if (pos == mMonsterNodes.end()) {
        LOGE("Can't find monster node with id %lu", id);
        return nullptr;
    }

    return pos->second;
}

void MonsterAnimationController::queueAction(const std::string& action, const ann::util::Dictionary& dict) {
    ann::util::Dictionary dictCopy = dict;
    dictCopy["action"] = action;
    mMonsterActionQueue.push_back(dictCopy);
}

void MonsterAnimationController::applyQueuedActions() {
    int animationLength = 0;
    model::IdType playerId = model::Game::getInstance().getPlayer()->getId();
    for (ann::util::Dictionary& dict : mMonsterActionQueue) {
        model::IdType monsterId = dict["id"].asUInt();
        if (monsterId == playerId) {
            continue;
        }

        animationLength += Constants::kDefaultAnimationDuration;
    }

    float animationScale = 1.0f;
    if (animationLength > Constants::kDefaultAllMonstersAnimationDuration) {
        animationScale = (float)Constants::kDefaultAllMonstersAnimationDuration / animationLength;
    }

    for (ann::util::Dictionary& dict : mMonsterActionQueue) {
        model::IdType monsterId = dict["id"].asUInt();
        float scale = (monsterId == playerId) ? 1.0f : animationScale;

        std::string action = dict["action"].asString();
        if (action == "move") {
            processMoveResult(dict, scale);
        } else if (action == "rotate") {
            processRotate(dict, scale);
        }
    }

    mMonsterActionQueue.clear();
}

void MonsterAnimationController::processMoveResult(const ann::util::Dictionary& dict, float durationScale) {
    model::IdType monsterId = dict["id"].asUInt();
    bool success = dict["success"].asBool();
    model::PositionDiff diff = model::helpers::parsePositionDiff(dict["positionDiff"]);

    ann::gles::SceneNode* monsterNode = getMonsterNode(monsterId);
    if (monsterNode == nullptr) {
        return;
    }

    ann::anim::AnimationManager* am = ann::Engine::getInstance().getAnimationManager();

    if (success) {
        am->translate(monsterNode, helpers::glPoint(diff), Constants::kDefaultAnimationDuration * durationScale);
    } else {
        glm::vec3 moveAnimationDiff = Constants::kFailedMoveDistanceFactor * helpers::glPoint(diff);
        am->translate(monsterNode, moveAnimationDiff, Constants::kDefaultAnimationDuration / 2 * durationScale);
        am->translate(monsterNode, -moveAnimationDiff, Constants::kDefaultAnimationDuration / 2 * durationScale);
    }
}

void MonsterAnimationController::processRotate(const ann::util::Dictionary& dict, float durationScale) {
    model::IdType monsterId = dict["id"].asUInt();
    int rotateAngle = dict["angleDiff"].asInt();

    ann::gles::SceneNode* monsterNode = getMonsterNode(monsterId);
    if (monsterNode == nullptr) {
        return;
    }

    ann::anim::AnimationManager* am = ann::Engine::getInstance().getAnimationManager();
    am->rotate(monsterNode, rotateAngle, 0.0f, 1.0f, 0.0f, Constants::kDefaultAnimationDuration * durationScale);
}

} /* namespace ann_glue */
} /* namespace dungeon */
