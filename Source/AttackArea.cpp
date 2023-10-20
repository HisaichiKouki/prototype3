#include <assert.h>
#include "././Header/Various.h"
#include "././Header/AttackArea.h"

void AttackAreaObject::SetDashPoint(float _x, float _y, int _pointCnt)
{

	assert(_pointCnt <= 3);

	switch (_pointCnt)
	{
	case 0:
		attArea.position[0].x = _x;
		attArea.position[0].y = _y;
		break;
	case 1:
		attArea.position[1].x = _x;
		attArea.position[1].y = _y;
		break;
	case 2:
		attArea.position[2].x = _x;
		attArea.position[2].y = _y;
		break;
	}
	if (nowPointCnt >= 2)
	{
		nowPointCnt = 0;
	}
	nowPointCnt++;
}

bool AttackAreaObject::TriangleCollision(Vector2 _targetPosition)
{
	
	{
		Vector2 ap = { attArea.position[0].x - _targetPosition.x, attArea.position[0].y - _targetPosition.y };
		Vector2 bp = { attArea.position[1].x - _targetPosition.x, attArea.position[1].y - _targetPosition.y };
		Vector2 cp = { attArea.position[2].x - _targetPosition.x, attArea.position[2].y - _targetPosition.y };
		
		// それぞれ外積を計算する
		float cross01 = CrossProduct(ap, bp);
		float cross12 = CrossProduct(bp, cp);
		float cross20 = CrossProduct(cp, ap);

		// 内外判定
		if ((cross01 >= 0 && cross12 >= 0 && cross20 >= 0) ||
			(cross01 <= 0 && cross12 <= 0 && cross20 <= 0))
		{
			// 内側ァ！！
			return true;
		}
		else
		{
			// 外側ァ！！
			return false;
		}
	}
}

void AttackAreaObject::GetAreaPoint(Vector2* _p0, Vector2* _p1, Vector2* _p2)
{
	_p0->x = attArea.position[0].x;
	_p0->y = attArea.position[0].y;

	_p1->x = attArea.position[1].x;
	_p1->y = attArea.position[1].y;

	_p2->x = attArea.position[2].x;
	_p2->y = attArea.position[2].y;
}
