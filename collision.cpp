#include "collision.h"
#include "VectorPlus.h"


bool EllipseCollision(Vector2 pos1, float radius1, Vector2 pos2, float radius2)
{
    float length = floatLength(pos1, pos2);

	if (length<=radius1+radius2)
	{
		return true;

	}
	else
	{
		return false;

	}
}
