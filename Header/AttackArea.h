#pragma once
#include "Struct.h"
#include "OriginObject.h"

/// @brief 範囲攻撃クラス (三角形)
class AttackAreaObject : public Object
{
public:

	/// @brief ３つの点を１点ずつ設定する。
	/// @param _x x座標 (通常、プレイヤーの座標を渡す)
	/// @param _y y座標 (通常、プレイヤーの座標を渡す)
	/// @param _pointCnt カウント。0から2までの整数値
	void SetDashPoint(float _x, float _y, int _pointCnt);

	/// @brief 三角形の当たり判定
	/// @param _targetPosition ターゲットになるオブジェクトの座標 (敵など)
	bool TriangleCollision(Vector2 _targetPosition);

	/// @brief エリアを表示するための３点を取得する。
	/// @param _p0 0番目の座標
	/// @param _p1 1番目の座標
	/// @param _p2 2番目の座標
	void GetAreaPoint(Vector2* _p0, Vector2* _p1, Vector2* _p2);

private:

	/// @brief アタックエリア（三角形）の実体
	AttackAreaData attArea = { 0 };
	/// @brief ダッシュ回数をカウント
	int nowPointCnt = 0;
};

