/*
 * RawBuff.cpp
 *
 *  Created on: Mar 26, 2013
 *      Author: zerksud
 */

#include "RawBuff.h"

#include <ananas/util/ZLogger.h>

namespace ann {
namespace android {

RawBuff::RawBuff() {
    mBuff = nullptr;
    mSize = 0;
}

RawBuff::~RawBuff() {
    freeMemory();
}

void RawBuff::setBuff(BuffPtr buff, size_t size) {
    freeMemory();

    mBuff = buff;
    mSize = size;
}

ConstBuffPtr RawBuff::getBuff() const {
    return mBuff;
}

size_t RawBuff::getSize() const {
    return mSize;
}

void RawBuff::freeMemory() {
    if (mBuff != nullptr) {
        delete[] mBuff;
        mBuff = nullptr;
    }
    mSize = 0;
}

} /* namespace android */
} /* namespace ann */
