/*
 * Constants.h
 *
 *  Created on: Apr 10, 2013
 *      Author: zerksud
 */

#ifndef MODELCONSTANTS_H_
#define MODELCONSTANTS_H_

namespace dungeon {
namespace model {

class Constants {
public:
    static const char* kMonsterMoveResultNotification;
    static const char* kMonsterRotateNotification;
    static const char* kEndOfPlayerTurnNotification;
    static const char* kAllRoundActionsDoneNotification;

    static const char* kPlayerMoveCommand;
    static const char* kPlayerRotateCommand;

    static const char* kMoveDirectionForward;
    static const char* kMoveDirectionBackward;
    static const char* kMoveDirectionLeft;
    static const char* kMoveDirectionRight;

    static const char* kRotateDirectionHalfLeft;
    static const char* kRotateDirectionHalfRight;

    static const int kMonsterRotateStep = 45;

private:
    Constants();
    Constants(Constants& other);
    virtual ~Constants();
    Constants& operator=(const Constants& other);
};

} /* namespace model */
} /* namespace dungeon */
#endif /* MODELCONSTANTS_H_ */
