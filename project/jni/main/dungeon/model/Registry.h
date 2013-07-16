/*
 * Registry.h
 *
 *  Created on: Apr 8, 2013
 *      Author: zerksud
 */

#ifndef REGISTRY_H_
#define REGISTRY_H_

#include "Registrable.h"

#include <unordered_set>

namespace dungeon {
namespace model {

class Registry {
public:
    static Registry& getInstance();

    void assignUniqueId(Registrable* object);
    void releaseUniqueId(Registrable* object);

private:
    Registry();
    Registry(const Registry& other);
    virtual ~Registry();
    const Registry& operator=(const Registry& other);

    typedef std::unordered_set<IdType> ReleasedIdSet;

    IdType mNextFreeId;
    ReleasedIdSet mReleasedIds;
};

} /* namespace model */
} /* namespace dungeon */
#endif /* REGISTRY_H_ */
