#pragma once
#include <assert.h>

// もととなるクラス（基底クラス）
class Object {
public:

	virtual void GetPosition(int* _x, int* _y);
	virtual void GetPosition(float* _x, float* _y);

	virtual void SetPosition(int _x, int _y);
	virtual void SetPosition(float _x, float _y);

	virtual void UpdatePosition();

};