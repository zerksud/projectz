/*
 * UIProxy.cpp
 *
 *  Created on: Mar 26, 2013
 *      Author: zerksud
 */

#include "UIProxy.h"

#include <ananas/util/NotificationCenter.h>

#include <dungeon/model/Constants.h>

#include "Constants.h"

namespace dungeon {
namespace ann_glue {

void postPlayerRotate(const std::string& direction) {
    ann::util::Dictionary dict;
    dict["direction"] = direction;

    ann::util::NotificationCenter::getInstance().postNotification(model::Constants::kPlayerRotateCommand, dict);
}

void postPlayerMove(const std::string& action) {
    ann::util::Dictionary dict;
    dict["direction"] = action;

    ann::util::NotificationCenter::getInstance().postNotification(model::Constants::kPlayerMoveCommand, dict);
}

void rotateLeft() {
    postPlayerRotate(model::Constants::kRotateDirectionHalfLeft);
}

void rotateRight() {
    postPlayerRotate(model::Constants::kRotateDirectionHalfRight);
}

void moveForward() {
    postPlayerMove(model::Constants::kMoveDirectionForward);
}

void moveBackward() {
    postPlayerMove(model::Constants::kMoveDirectionBackward);
}

void moveLeft() {
    postPlayerMove(model::Constants::kMoveDirectionLeft);
}

void moveRight() {
    postPlayerMove(model::Constants::kMoveDirectionRight);
}

UIProxy::UIProxy() {
    ann::util::NotificationCenter& nc = ann::util::NotificationCenter::getInstance();

    nc.addObserver(Constants::kUIActionPlayerRotateLeft, this, [](const ann::util::Dictionary& dict) {rotateLeft();});
    nc.addObserver(Constants::kUIActionPlayerRotateRight, this, [](const ann::util::Dictionary& dict) {rotateRight();});
    nc.addObserver(Constants::kUIActionPlayerMoveForward, this, [](const ann::util::Dictionary& dict) {moveForward();});
    nc.addObserver(Constants::kUIActionPlayerMoveBackward, this, [](const ann::util::Dictionary& dict) {moveBackward();});
    nc.addObserver(Constants::kUIActionPlayerMoveLeft, this, [](const ann::util::Dictionary& dict) {moveLeft();});
    nc.addObserver(Constants::kUIActionPlayerMoveRight, this, [](const ann::util::Dictionary& dict) {moveRight();});
}

UIProxy::~UIProxy() {
    ann::util::NotificationCenter& nc = ann::util::NotificationCenter::getInstance();

    nc.removeObserver(Constants::kUIActionPlayerRotateLeft, this);
    nc.removeObserver(Constants::kUIActionPlayerRotateRight, this);
    nc.removeObserver(Constants::kUIActionPlayerMoveForward, this);
    nc.removeObserver(Constants::kUIActionPlayerMoveBackward, this);
    nc.removeObserver(Constants::kUIActionPlayerMoveLeft, this);
    nc.removeObserver(Constants::kUIActionPlayerMoveRight, this);
}

} /* namespace ann_glue*/
} /* namespace dungeon */
