#include "././Header/Score.h"

ScoreObject::ScoreObject()
{
	// 初期化
	score.position.x = 0;
	score.position.y = 0;
	score.width = 0;
	score.height = 0;
	score.textureHandle = 0;
	score.color = 0;
}

void ScoreObject::Draw(int _decimalNum)
{
	int digit = 0;
	int decNumCpy = _decimalNum;

	int result[16] = { 0 };

	// 桁数を求める
	do
	{
		decNumCpy /= 10;
		digit++;
	} while (decNumCpy != 0);

	for (int i = 0; i < digit; i++)
	{
		
		result[digit - 1 - i] = _decimalNum / PowI(10, digit - 1 - i);
		_decimalNum %= PowI(10, digit - 1 - i);
		printf("%d", result[digit - 1 - i]);
	}
}

int ScoreObject::PowI(int _x, int _y)
{
	int result = 1;

	for (int i = 0; i < _y; i++)
	{
		result *= _x;
	}

	return result;
}
