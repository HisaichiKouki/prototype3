#include "././Header/Score.h"

ScoreObject::ScoreObject()
{
	// 初期化
	score.position.x = 0;
	score.position.y = 0;
	score.width = 0;
	score.height = 0;
	score.color = 0;
	handleCnt = 0;
	for (int i = 0; i < 10; i++) textureHandles[i] = 0;
}

void ScoreObject::SetScoreTextureHandle(int _textureHandle[10])
{
	for (int i = 0; i < 10; i++)
	{
		textureHandles[i] = _textureHandle[i];
	}
}

void ScoreObject::SetSize(int _width, int _height)
{
	score.width = _width;
	score.height = _height;
}

void ScoreObject::SetColor(unsigned int _color)
{
	score.color = _color;
}

void ScoreObject::Draw(int _decimalNum, int _posX, int _posY, int _margin)
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

		// 描画
		func.DrawQuadPlus(
			_posX + score.width * i + _margin, _posY,
			score.width, score.height,
			1.0f, 1.0f, 0.0f,
			0, 0, score.width, score.height,
			textureHandles[result[digit - 1 - i]],
			score.color,
			"leftTop"
		);
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
