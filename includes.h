#ifndef INCLUDES_H
#define INCLUDES_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>

#define VERSION "2.0"

//Qt
#include <QtWidgets>

#if (defined (_WIN32) || defined (_WIN64))
    #include <conio.h>
    #include <io.h>
    #include <windows.h>
#elif (defined (LINUX) || defined (__linux__) || defined(__APPLE__))
    #include <unistd.h>
#endif

typedef struct User User;
struct User
{
    char* username;
    char* path;
};

typedef struct DBRow DBRow;
struct DBRow
{
    int id;
    char* username;
    char* purpose;
    char* password;
};

//seed.cpp
int randint(int minvalue, int maxvalue);
unsigned long getSeed(char *username);

//password.cpp
const char* generate(unsigned int requiredLength, bool requireUppercase, bool requireNumerals, bool requireSpecials);

//misc.cpp
int IsDir();

#endif // INCLUDES_H
