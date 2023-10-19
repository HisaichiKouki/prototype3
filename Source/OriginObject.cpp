#include "././Header/OriginObject.h"

void Object::GetPosition(int* _x, int* _y)
{
	if (_x == nullptr)
	{
		return;
	}
	if (_y == nullptr)
	{
		return;
	}

	// 座標を渡す
	*_x = 0;
	*_y = 0;
}

void Object::GetPosition(float* _x, float* _y)
{
	if (_x == nullptr)
	{
		return;
	}
	if (_y == nullptr)
	{
		return;
	}

	// 座標を渡す
	*_x = 0;
	*_y = 0;
}

void Object::SetPosition(int _x, int _y)
{
	[[maybe_unused]] int privatePosX = _x;
	[[maybe_unused]] int privatePosY = _y;
}

void Object::SetPosition(float _x, float _y)
{
	[[maybe_unused]] float privatePosX = _x;
	[[maybe_unused]] float privatePosY = _y;
}

void Object::UpdatePosition()
{
	// なんやかんやして
	// 座標更新
	[[maybe_unused]] int privatePosX = 0;
	[[maybe_unused]] int privatePosY = 0;
}

