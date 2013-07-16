/*
 * Updatable.h
 *
 *  Created on: Mar 26, 2013
 *      Author: zerksud
 */

#ifndef UPDATABLE_H_
#define UPDATABLE_H_

#include <ctime>

namespace ann {

class Updatable {
public:
    Updatable();
    virtual ~Updatable();
    virtual void update(time_t dt) = 0;
};

} /* namespace ann */
#endif /* UPDATABLE_H_ */
