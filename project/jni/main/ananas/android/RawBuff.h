/*
 * RawBuff.h
 *
 *  Created on: Mar 26, 2013
 *      Author: zerksud
 */

#ifndef RAWBUFF_H_
#define RAWBUFF_H_

#include <stdint.h>

namespace ann {
namespace android {

typedef uint8_t* BuffPtr;
typedef const uint8_t* ConstBuffPtr;

class RawBuff {
public:
    RawBuff();
    virtual ~RawBuff();

    void setBuff(BuffPtr buff, size_t size);
    ConstBuffPtr getBuff() const;

    size_t getSize() const;

private:
    void freeMemory();

    uint8_t* mBuff;
    size_t mSize;
};

} /* namespace android */
} /* namespace ann */
#endif /* RAWBUFF_H_ */
