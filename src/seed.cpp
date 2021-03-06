#include "includes.h"

int randint(int minvalue, int maxvalue)
{
    return rand() % (maxvalue+1-minvalue) + minvalue;
}

unsigned long getSeed(std::string username)
{
    //It's kind of pointless to be honest, but standard issue random doesn't cut it for me,
    //and I didn't want to implement a more complex engine. YMMV whether it's better than
    //rand() or not.

    char *ptr;
    unsigned int hash = strtoul(username.c_str(), &ptr, 36);
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
