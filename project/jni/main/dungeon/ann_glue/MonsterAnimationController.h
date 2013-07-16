/*
 * MonsterAnimationController.h
 *
 *  Created on: Apr 9, 2013
 *      Author: zerksud
 */

#ifndef MONSTERANIMATIONCONTROLLER_H_
#define MONSTERANIMATIONCONTROLLER_H_

#include <unordered_map>
#include <list>

#include <ananas/gles/SceneNode.h>
#include <ananas/util/NotificationCenter.h>
#include <dungeon/model/Registrable.h>

namespace dungeon {
namespace ann_glue {

class MonsterAnimationController {
public:
    MonsterAnimationController();
    virtual ~MonsterAnimationController();

    void clearMonsterNodes();
    void addMonsterNode(model::IdType id, ann::gles::SceneNode* node);

private:
    typedef std::unordered_map<model::IdType, ann::gles::SceneNode*> MonsterSceneNodeList;
    typedef std::pair<model::IdType, ann::gles::SceneNode*> MonsterNodePair;
    typedef std::list<ann::util::Dictionary> DictionaryList;


    ann::gles::SceneNode* getMonsterNode(model::IdType id);

    void queueAction(const std::string& action, const ann::util::Dictionary& dict);
    void applyQueuedActions();

    void processMoveResult(const ann::util::Dictionary& dict, float durationScale);
    void processRotate(const ann::util::Dictionary& dict, float durationScale);


    MonsterSceneNodeList mMonsterNodes;
    DictionaryList mMonsterActionQueue;
};

} /* namespace ann_glue */
} /* namespace dungeon */
#endif /* MONSTERANIMATIONCONTROLLER_H_ */
