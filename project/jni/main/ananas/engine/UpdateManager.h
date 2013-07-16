/*
 * UpdateManager.h
 *
 *  Created on: Mar 26, 2013
 *      Author: zerksud
 */

#ifndef UPDATEMANAGER_H_
#define UPDATEMANAGER_H_

#include <vector>

#include "Updatable.h"

namespace ann {

class UpdateManager {
public:
    UpdateManager();
    virtual ~UpdateManager();

    void update();

    void addUpdatable(Updatable* object);
    void removeUpdatable(Updatable* object);

private:
    typedef std::vector<Updatable*> UpdatableList;

    UpdatableList mUpdatables;
    time_t mLastUpdateTime; //TODO: use difftime for portable code?
};

} /* namespace ann */
#endif /* UPDATEMANAGER_H_ */
