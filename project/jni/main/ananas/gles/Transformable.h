/*
 * Transformable.h
 *
 *  Created on: Mar 26, 2013
 *      Author: zerksud
 */

#ifndef TRANSFORMABLE_H_
#define TRANSFORMABLE_H_

#include <glm/glm.hpp>

namespace ann {
namespace gles {

class Transformable {
public:
    Transformable();
    virtual ~Transformable();

    virtual void translate(float dx, float dy, float dz) = 0;
    virtual void translate(glm::vec3 diffVector) = 0;
    virtual void rotate(float angle, float x, float y, float z) = 0;
    virtual void scale(float s) = 0;
};

} /* namespace gles */
} /* namespace ann */
#endif /* TRANSFORMABLE_H_ */
