/*
 * Registrable.h
 *
 *  Created on: Apr 8, 2013
 *      Author: zerksud
 */

#ifndef REGISTRABLE_H_
#define REGISTRABLE_H_

namespace dungeon {
namespace model {

typedef unsigned int IdType;

class Registrable {
public:
    Registrable();
    Registrable(const Registrable& other);
    virtual ~Registrable();

    IdType getId() const;
    void setId(IdType id);
    bool isRegistered() const;

    static const IdType kNoId = 0;

private:
    IdType mId;
};

} /* namespace model */
} /* namespace dungeon */
#endif /* REGISTRABLE_H_ */
