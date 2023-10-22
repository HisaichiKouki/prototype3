#pragma once
#include "Struct.h"

class ScoreObject
{
public:
	
	ScoreObject();
	
	static void Draw(int _decimalNum);

private:
	static int PowI(int _x, int _y);
	ScoreData score;
};