#include "VectorPlus.h"
#include <corecrt_math.h>

Vector2 vectorLength(Vector2 pos1, Vector2 pos2)
{
    return Vector2(pos1.x-pos2.x,pos1.y-pos2.y);
}



Vector2 vectorNormalize(Vector2 pos1, Vector2 pos2)
{
    Vector2 xy = vectorLength(pos1, pos2);
    float length = sqrtf(xy.x*xy.x+xy.y*xy.y);

    if (length==0)
    {
        return Vector2{ 0,0 };
    }

    return Vector2(xy.x/length,xy.y/length);
}

float floatLength(Vector2 pos1, Vector2 pos2)
{
    Vector2 xy = vectorLength(pos1, pos2);
    return sqrtf(xy.x * xy.x + xy.y * xy.y);
}


