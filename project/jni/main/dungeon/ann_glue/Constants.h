/*
 * Constants.h
 *
 *  Created on: Apr 10, 2013
 *      Author: zerksud
 */

#ifndef ANANASGLUECONSTANTS_H_
#define ANANASGLUECONSTANTS_H_

namespace dungeon {
namespace ann_glue {

class Constants {
public:
    static const char* kUIActionPlayerRotateLeft;
    static const char* kUIActionPlayerRotateRight;
    static const char* kUIActionPlayerMoveForward;
    static const char* kUIActionPlayerMoveBackward;
    static const char* kUIActionPlayerMoveLeft;
    static const char* kUIActionPlayerMoveRight;

    static const int kDefaultAllMonstersAnimationDuration = 1000;
    static const int kDefaultAnimationDuration = 500;
    static constexpr float kFailedMoveDistanceFactor = 0.20f;

private:
    Constants();
    Constants(Constants& other);
    virtual ~Constants();
    Constants& operator=(const Constants& other);
};

} /* namespace ann_glue */
} /* namespace dungeon */
#endif /* ANANASGLUECONSTANTS_H_ */
