﻿#pragma once

struct Vector2
{
	float x;
	float y;
};

struct XY
{
	int x;
	int y;
};

typedef struct PlayerData
{
	Vector2 pos;
	Vector2 acceleration;
	Vector2 direction;
	Vector2 velocity;
	Vector2 radius;

	Vector2 joystick;

	Vector2 prepos[3];//三角形の点
	Vector2 screenPos;

	float shotSpeed;//ダッシュ時の速度
	float moveSpeed;//スティック移動の速度
	bool trigerA;//Aボタンを押したか
	float velocityRatio;//速度の割合。1が等倍で0.5fが半分0で速度0
	bool aim;//Aボタンを押した時のアクション
	int count;//点を何個出したかカウント
	int aimTimer;//三角形を作った後の時間
	float anchorRadius;//点の半径

	int flickTimer;//はじき判定フレ
	bool flick;//はじきフラグ
	float flickLength;//フリックで端まで行ったか確認するため
	int flickCT;//フリックした直後に減速しないように

	bool dashAttack;//フリックでフラグをたてたい
	bool triangulAttack;//三角形の攻撃

}PlayerData;

struct EnemyData
{
	Vector2 position;
	Vector2 acceleration;
	Vector2 velocity;
	bool isAlive;
};

struct AttackAreaData
{
	Vector2 position[3];
};