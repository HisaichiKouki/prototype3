#include "././Header/Enemy.h"

// インストラクタ
EnemyObject::EnemyObject()
{
	enemy.position.x = 0;
	enemy.position.y = 0;
	enemy.acceleration.x = 0;
	enemy.acceleration.y = 0;
	enemy.velocity.x = 0;
	enemy.velocity.y = 0;
	enemy.parentIsAlive = false;
}

EnemyObject::~EnemyObject()
{
	;
}

void EnemyObject::GetPosition(int* _x, int* _y)
{
	*_x = static_cast<int>(enemy.position.x);
	*_y = static_cast<int>(enemy.position.y);
}
void EnemyObject::GetPosition(float* _x, float* _y)
{
	*_x = enemy.position.x;
	*_y = enemy.position.y;
}

void EnemyObject::SetPosition(int _x, int _y)
{
	enemy.position.x = static_cast<float>(_x);
	enemy.position.y = static_cast<float>(_y);
}
void EnemyObject::SetPosition(float _x, float _y)
{
	enemy.position.x = _x;
	enemy.position.y = _y;
}


void EnemyObject::UpdatePosition()
{
	;
}

void EnemyObject::SetIsAlive(bool _b)
{
	enemy.parentIsAlive = _b;
}