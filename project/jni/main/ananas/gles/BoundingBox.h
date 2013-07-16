/*
 * BoundingBox.h
 *
 *  Created on: Mar 18, 2013
 *      Author: zerksud
 */

#ifndef BOUNDINGBOX_H_
#define BOUNDINGBOX_H_

#include <glm/glm.hpp>

namespace ann {
namespace gles {

class BoundingBox {
public:
    BoundingBox();
    BoundingBox(BoundingBox& other);
    BoundingBox(glm::vec3 minVertex, glm::vec3 maxVertex);
    virtual ~BoundingBox();

    bool isEmpty() const;
    const glm::vec3& getMinPoint() const;
    const glm::vec3& getMaxPoint() const;
    void transform(glm::mat4 matrix);
    void merge(const BoundingBox* other);

private:
    enum Extent {
        EMPTY,
        FINITE
    };

    glm::vec3 mMinVertex;
    glm::vec3 mMaxVertex;
    Extent mExtent;
};

} /* namespace gles */
} /* namespace ann */
#endif /* BOUNDINGBOX_H_ */
