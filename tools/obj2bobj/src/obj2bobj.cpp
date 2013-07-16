//============================================================================
// Name        : obj2bobj.cpp
// Author      : zerksud
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>

#include <sys/stat.h>

#include "MeshPacker.h"

void printHelp() {
    std::cout << "usage: obj2bobj <obj_filename>" << std::endl;
}

std::string changeExtension(const std::string& filename, const std::string& extension) {
    std::string::size_type dotPos = filename.find_last_of('.');
    if (dotPos == std::string::npos) {
        return filename + "." + extension;
    }

    std::string result = filename.substr(0, dotPos + 1) + extension;

    return result;
}

bool fileExists(const std::string& fileName) {
    struct stat buf;
    return (stat(fileName.c_str(), &buf) != -1);
}

int main(int argc, const char** argv) {
    if (argc != 2) {
        printHelp();
        return 0;
    }

    std::ifstream ifs(argv[1]);
    if (!ifs.good()) {
        std::cout << "Can't read file " << argv[1] << std::endl;
        return 0;
    }

    std::stringstream ss;
    ss << ifs.rdbuf();
    ifs.close();

    ann::tools::mesh_packer::RawBuff buff = ann::tools::mesh_packer::packObj(ss);

    std::cout << "Got buffer with size " << buff.size << std::endl;

    std::string outputFileName = changeExtension(argv[1], "bobj");
    if (fileExists(outputFileName)) {
        std::cerr << "Can't write bobj data to existing file " << outputFileName << std::endl;
    } else {
        std::ofstream ofs(outputFileName.c_str(), std::ios::binary);
        ofs.write(reinterpret_cast<char*>(buff.buff), buff.size);
        ofs.close();

        std::cout << std::endl << "data saved to " << outputFileName << std::endl;
    }

    delete[] buff.buff;

    return 0;
}
