#ifndef CREDENTIALSFILE_H
#define CREDENTIALSFILE_H

#include <inttypes.h>
#include <fstream>
#include <cstdio>

struct CFileHeader
{
    char length;
    std::string signature;
};

struct CFileEntry
{
    char id;
    std::string username;
    std::string purpose;
    std::string password;
};

struct CFile
{
    CFileHeader header;
    int numberOfEntries;
    CFileEntry* entries;
};

void createBinaryFile(std::string path);

#endif // CREDENTIALSFILE_H
