#pragma once

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
	Vector2 preDirection;

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
	bool parentIsAlive;
};

struct AttackAreaData
{
	Vector2 position[3];
};

struct Count
{
	float count;
	float preCount;
	float endCount;
};
struct Ennergy
{
	float count;
	float max;
	float dashRate;
	float triangleRate;
	float powerUp;
	bool fever;
	float feverTime;
	bool dash;
	bool triangle;
	float damage;
	bool damageFlag;
};

struct testEnemy1//三角に配置される敵
{
	Vector2 centorPos;
	Vector2 relativePos[3];
	Vector2 velocity;
	float radius;
	bool parentIsAlive;
	bool childIsAlive[3];
	bool isDed[3];
	int dedTimer[3];
	Vector2 posReset[3];
	float easeT;

};

struct testEnemy2//四角に配置される敵
{
	Vector2 centorPos[4];
	Vector2 relativePos[4][16];
	Vector2 velocity[4];
	float radius;
	bool parentIsAlive[4];
	bool childIsAlive[4][16];
	bool isDed[4][16];
	int dedTimer[4][16];
	Vector2 posReset[4][16];
	bool hostIsAlive;
	float easeT;
	int count;
	bool countFlag[4][16];
	int timer;

};

struct testEnemy3//１列に配置される敵
{
	Vector2 centorPos;
	Vector2 relativePos[4];
	Vector2 velocity;
	float radius;
	bool parentIsAlive;
	bool childIsAlive[4];
	bool isDed[4];
	int dedTimer[4];
	Vector2 posReset[4];
	float easeT;
	int timer;

};
struct testEnemy4//八方向１列に配置される敵
{
	Vector2 centorPos[8];
	Vector2 relativePos[8][4];
	Vector2 velocity[8];
	float radius;
	bool parentIsAlive[8];
	bool childIsAlive[8][4];
	bool isDed[8][4];
	int dedTimer[8][4];
	Vector2 posReset[8][4];
	bool hostIsAlive;
	float easeT;
	int count;
	bool countFlag[8][4];
	int timer;

};

struct testEnemy5//１列に配置される敵
{
	Vector2 centorPos[4];
	Vector2 relativePos[4][12];
	Vector2 velocity[4];
	float radius;
	bool parentIsAlive[4];
	bool childIsAlive[4][12];
	bool isDed[4][12];
	int dedTimer[4][12];
	Vector2 posReset[4][12];
	bool hostIsAlive;
	float easeT;
	int count;
	bool countFlag[4][12];
	int timer;

};

struct testEnemy6  //列が上下に出て中心に向かって進む
{
	Vector2 centorPos[2];
	Vector2 relativePos[2][8];
	Vector2 velocity[2][8];
	float radius;
	bool parentIsAlive[2];
	bool childIsAlive[2][8];
	bool isDed[2][8];
	int dedTimer[2][8];
	Vector2 posReset[2][8];
	bool hostIsAlive;
	float easeT;
	int count;
	bool countFlag[2][8];
	int timer;

};

struct testEnemy7
{
	Vector2 centorPos;
	Vector2 relativePos[60];
	Vector2 velocity[60];
	float radius[60];
	bool parentIsAlive;
	bool childIsAlive[60];
	bool isDed[60];
	int dedTimer[60];
	Vector2 posReset[60];
	bool hostIsAlive;
	float easeT[60];
	float thta[60];
	float length;
	float z[60];
	int count;
	bool countFlag[60];
	int timer;

};


