/*
 * MeshPacker.h
 *
 *  Created on: Apr 6, 2013
 *      Author: zerksud
 */

#ifndef MESHPACKER_H_
#define MESHPACKER_H_

#include <sstream>

namespace ann {
namespace tools {
namespace mesh_packer {

struct RawBuff {
    unsigned char* buff;
    size_t size;

    RawBuff() {
        buff = 0;
        size = 0;
    }
};

RawBuff packObj(std::stringstream& ss);

} /* namespace mesh_packer */
} /* namespace tools */
} /* namespace ann */
#endif /* MESHPARSER_H_ */
