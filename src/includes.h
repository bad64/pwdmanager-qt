#ifndef INCLUDES_H
#define INCLUDES_H

#include <iostream>
#include <sstream>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>

#define VERSION "2.2.0"

//Qt
#include <QtWidgets>

#if (defined (_WIN32) || defined (_WIN64))
    #include <windows.h>
    #include <winbase.h>
#elif (defined (LINUX) || defined (__linux__) || defined(__APPLE__))
    #include <unistd.h>
#endif

#include "credentialsfile.h"

struct User
{
    std::string username;
    std::string path;
};

struct DBRow
{
    std::string username;
    std::string purpose;
    std::string password;

    DBRow()
        : username("<none>"), purpose("<none>"), password("<none>")
    {}

    DBRow(std::string nusername, std::string npurpose, std::string npassword)
        : username(nusername), purpose(npurpose), password(npassword)
    {}
};

//seed.cpp
int randint(int minvalue, int maxvalue);
unsigned long getSeed(std::string username);

//password.cpp
std::string generate(unsigned int requiredLength, bool requireUppercase, bool requireNumerals, bool requireSpecials);

//misc.cpp
int IsDir();
std::string xorCrypt(std::string msg, std::string key = "");

#endif // INCLUDES_H
