#include "Functions.h"
#include <assert.h>
#include <math.h>

void Func::SetDebugMode(DebugMode mode)
{
	switch (mode)
	{
	case kStandardMode:
		Func::isDebugMode = false;
		break;
	case kDebugMode:
		Func::isDebugMode = true;
	}
}

void Func::ToggleDebugMode()
{

	if (callCnt % 2 == 0)
	{
		Func::SetDebugMode(kDebugMode);
	}
	else
	{
		Func::SetDebugMode(kStandardMode);
	}

	callCnt++;
}

void Func::rotate(int& _x, int& _y, float _theta)
{
	int newX = 0;
	int newY = 0;

	newX = static_cast<int>((_x * cosf(_theta)) - (_y * sinf(_theta)));
	newY = static_cast<int>((_x * sinf(_theta)) + (_y * cosf(_theta)));

	_x = newX;
	_y = newY;
}

void Func::DrawQuadPlus(
	int _x, int _y, 
	int _horizontal, int _vertical, 
	float _scaleX, float _scaleY, 
	float _theta,
	int _srcX, int _srcY,
	int _srcW, int _srcH,
	int _textureHandle,
	unsigned int _hexColor,
	const char _mode[]
)
{
	// scale
	int leftTop[2] = { 0 , 0 };
	int rightTop[2] = { 0 , 0 };
	int leftBottom[2] = { 0 , 0 };
	int rightBottom[2] = { 0 , 0 };

	// translate & draw
	if (strcmp(_mode, "center") == 0)
	{
		leftTop[0] = static_cast<int>(_horizontal * _scaleX / -2);
		leftTop[1] = static_cast<int>(_vertical * _scaleY / -2);
		rightTop[0] = static_cast<int>(_horizontal * _scaleX / 2);
		rightTop[1] = static_cast<int>(_vertical * _scaleY / -2);
		leftBottom[0] = static_cast<int>(_horizontal * _scaleX / -2);
		leftBottom[1] = static_cast<int>(_vertical * _scaleY / 2);
		rightBottom[0] = static_cast<int>(_horizontal * _scaleX / 2);
		rightBottom[1] = static_cast<int>(_vertical * _scaleY / 2);
	}
	else if (strcmp(_mode, "leftTop") == 0)
	{
		leftTop[0] = 0;
		leftTop[1] = 0;
		rightTop[0] = static_cast<int>(_horizontal * _scaleX);
		rightTop[1] = 0;
		leftBottom[0] = 0;
		leftBottom[1] = static_cast<int>(_vertical * _scaleY);
		rightBottom[0] = static_cast<int>(_horizontal * _scaleX);
		rightBottom[1] = static_cast<int>(_vertical * _scaleY);
	}
	else
	{
		assert(true);
	}

	// rotate
	rotate(leftTop[0], leftTop[1], _theta);
	rotate(rightTop[0], rightTop[1], _theta);
	rotate(leftBottom[0], leftBottom[1], _theta);
	rotate(rightBottom[0], rightBottom[1], _theta);



	Novice::DrawQuad(
		_x + leftTop[0], _y + leftTop[1],
		_x + rightTop[0], _y + rightTop[1],
		_x + leftBottom[0], _y + leftBottom[1],
		_x + rightBottom[0], _y + rightBottom[1],
		_srcX, _srcY,
		_srcW, _srcH,
		_textureHandle,
		_hexColor
	);

	// debug
	if (isDebugMode)
	{
		// 4頂点の確認
		Novice::DrawEllipse(_x + leftTop[0], _y + leftTop[1], 5, 5, 0.0f, 0x2f76ffff, kFillModeSolid);
		Novice::DrawEllipse(_x + rightTop[0], _y + rightTop[1], 5, 5, 0.0f, 0x2f76ffff, kFillModeSolid);
		Novice::DrawEllipse(_x + leftBottom[0], _y + leftBottom[1], 5, 5, 0.0f, 0x2f76ffff, kFillModeSolid);
		Novice::DrawEllipse(_x + rightBottom[0], _y + rightBottom[1], 5, 5, 0.0f, 0x2f76ffff, kFillModeSolid);

		// 中心点の確認
		Novice::DrawEllipse(_x, _y, 5, 5, 0.0f, 0xff0000ff, kFillModeSolid);
	}

}
