#include "randPlus.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

//int RandomMT(int min, int max) {
//    std::uniform_int_distribution<> setRand(min, max);
//    return setRand(mt);
//}
Vector2 randShake(int radiusX, int radiusY,int seed)
{ 
   unsigned int currentTime = int(time(nullptr));
    srand(currentTime*seed+ currentTime % seed-seed);
    seed *= rand();
    srand(seed);
    return Vector2(float(rand() % (radiusX * 2 + 1) - radiusX), float(rand() % (radiusY * 2 + 1) - radiusY));
}

int RandomNum(int seed)
{
    unsigned int currentTime = int(time(nullptr));
    srand(currentTime * seed + currentTime % seed - seed);
    seed *= rand();
    srand(seed);
    return(rand());
}
