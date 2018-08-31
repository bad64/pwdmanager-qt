#include "credentialsfile.h"

void createBinaryFile(std::string path)
{
    std::ofstream file(path.c_str(), std::ios::out | std::ios::binary);

    file << '\x001' << "CRDNTLS" << '\x002';

    file << EOF;
    file.close();
}
