#include "includes.h"

int randint(int minvalue, int maxvalue)
{
    return rand() % (maxvalue+1-minvalue) + minvalue;
}

unsigned long GetSeed()
{
    //All this is just to guarantee better seed variety without going through an implementation of an already existing RNG engine
    //I know it is a fair bit asinine, but I thought I'd try

    char* username;
    #if (defined (_WIN32) || defined (_WIN64))
        username = (char *)malloc(sizeof(char) * strlen(getenv("USERNAME")));
    	strcpy(username, getenv("USERNAME"));
    #elif (defined (LINUX) || defined (__linux__) || defined(__APPLE__))
        username = (char *)malloc(sizeof(char) * strlen(getenv("USER")));
	    strcpy(username, getenv("USER"));
    #endif

    char *ptr;
    unsigned int hash = strtoul(username, &ptr, 36);
    unsigned int programStart = time(NULL);
    unsigned int programNow;
    unsigned long seed = time(NULL);

    //Stage 1
    if (seed % 3 == 0)
        seed += (time(NULL) * hash);
    else
        seed -= (time(NULL) * hash);

    //Stage 2
    unsigned int nbuffer, sum = 0, remainder;
    nbuffer = seed;

    while (nbuffer != 0)
    {
        remainder = nbuffer % 10;
        sum += remainder;
        nbuffer /= 10;
    }
    programNow = time(NULL);

    if (sum % 2 == 0)
    {
        if (seed -= sum*sum > 0)
            seed -= sum*sum;
        else
            seed += sum*sum;
        seed += 7;  //There is no "Why 7 specifically", I just thought, why not ?
    }
    else
    {
        if ((programNow - programStart) % 3 != 0)
        {
            seed += (hash / (programNow - programStart)) * (hash / (programNow - programStart));
        }
        else
        {
            seed -= hash;
        }
    }

    return seed;
}
