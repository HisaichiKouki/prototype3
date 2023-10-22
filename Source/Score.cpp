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
	maxDigit = 0;
}

void ScoreObject::SetScoreTextureHandle(int _textureHandle[10])
{
	for (int i = 0; i < 10; i++)
	{
		textureHandles[i] = _textureHandle[i];
	}
	dPattern = DPATTERN_FILLED_BY_ZERO;
}

void ScoreObject::SetSize(int _width, int _height)
{
	score.width = _width;
	score.height = _height;
}

void ScoreObject::SetMaxDigit(int _maxDigit)
{
	maxDigit = _maxDigit;
}

void ScoreObject::SetPattern(DrawPattern _dPattern)
{
	dPattern = _dPattern;
}

void ScoreObject::SetScale(float _scaleX, float _scaleY)
{
	score.scale.x = _scaleX;
	score.scale.y = _scaleY;
}

void ScoreObject::SetColor(unsigned int _color)
{
	score.color = _color;
}

void ScoreObject::Draw(int _decimalNum, int _posX, int _posY, int _margin)
{
	int digit = 0;
	int intervalCnt = 0;
	int decNumCpy = _decimalNum;

	int result[16] = { 0 };

	// 桁数を求める
	do
	{
		decNumCpy /= 10;
		digit++;
	} while (decNumCpy != 0);

	// パターン分岐
	switch (dPattern)
	{
	case ScoreObject::DPATTERN_FILLED_BY_ZERO:
		intervalCnt = maxDigit;
		break;
	case ScoreObject::DPATTERN_ONLY_DIGIT:
		intervalCnt = digit;
		break;
	default:
		break;
	}

	for (int i = 0; i < intervalCnt; i++)
	{
		result[intervalCnt - 1 - i] = _decimalNum / PowI(10, intervalCnt - 1 - i);
		_decimalNum %= PowI(10, intervalCnt - 1 - i);

		// 描画
		func.DrawQuadPlus(
			_posX + static_cast<int>(score.width * score.scale.x) * i + _margin, _posY,
			static_cast<int>(score.width * score.scale.x), static_cast<int>(score.height * score.scale.y),
			1.0f, 1.0f, 0.0f,
			0, 0, score.width, score.height,
			textureHandles[result[intervalCnt - 1 - i]],
			score.color,
			"leftTop"
		);

		if (func.isDebug())
		{
			Novice::DrawBox(
				_posX + static_cast<int>(score.width * score.scale.x) * i + _margin,
				_posY,
				static_cast<int>(score.width * score.scale.x),
				static_cast<int>(score.height * score.scale.y),
				0.0f,
				0xdf0000ff,
				kFillModeWireFrame
			);
		}

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
