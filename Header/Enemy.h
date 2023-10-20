#pragma once
#include "Struct.h"
#include "OriginObject.h"

class EnemyObject : public Object
{
public:

	EnemyObject();
	~EnemyObject();

	/// @brief 敵が生きているかを設定する。
	/// @param _b true or false
	void SetIsAlive(bool _b);

	/// @brief 座標を取得
	/// @param _x x座標
	/// @param _y y座標
	void GetPosition(int* _x, int* _y) override;

	/// @brief 座標を取得
	/// @param _x x座標
	/// @param _y y座標
	void GetPosition(float* _x, float* _y) override;

	/// @brief 座標を設定
	/// @param _x x座標
	/// @param _y y座標
	void SetPosition(int _x, int _y) override;

	/// @brief 座標を設定
	/// @param _x x座標
	/// @param _y y座標
	void SetPosition(float _x, float _y) override;

	/// @brief 座標を更新 (Not Available)
	void UpdatePosition() override;

private:
	EnemyData enemy;

};