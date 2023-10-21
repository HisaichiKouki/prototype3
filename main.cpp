#include <Novice.h>
#include "Headers.h"
#include <corecrt_math.h>
#include "MatrixCalc.h"
#include "collision.h"
#include "Easing.h"
#include "randPlus.h"
const char kWindowTitle[] = "LC1A_20_ヒサイチ_コウキ";

float clump(float a, float min, float max);


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1920, 1080);

	PlayerData player{};
	player.radius = { 32,32 };
	player.shotSpeed = 60;
	player.moveSpeed = 20;
	player.pos.x = 0;
	player.pos.y = 0;
	player.anchorRadius = 10;
	int joystickX = 0;
	int joystickY = 0;
	int preJoyStickX = 0;
	int preJoyStickY = 0;
	Novice::SetJoystickDeadZone(0, 0, 0);

	Vector2 xy{};
	float length = 0;
	float dedZone = 30;//こっちがほんとのデッドゾーン

	Vector2 scroll{};

	float fieldRadius = 2000;//フィールド半径
	Vector2 fieldToPlayer{};//フィールドの中心からプレイヤーまでのxy距離
	float centerToPlayerLength = 0;//フィールドの中心からプレイヤーまでのLength

	float miniMap = 8;//ミニマップの倍率 20で20分の１　10で10分の1　数字が小さくなるほど大きく表示
	float miniMapPlayerSize = 2;//ミニマップに表示されるプレイヤー関係のサイズ　1が等倍　数字が大きくなるほど大きく表示

	int fieldTexture = Novice::LoadTexture("./Resources/images/field_1.png");
	int enemyTexture = Novice::LoadTexture("./Resources/images/enemy_1.png");

	float enemyRadius = 64;//敵全体の半径
	const int kDedTimer = 30;
#pragma region enemy1//小さい三角形

	testEnemy1 enemy1{};
	enemy1.parentIsAlive = false;
	enemy1.centorPos = { 0,0 };
	enemy1.radius = enemyRadius;
	enemy1.posReset[0] = { enemy1.centorPos.x,enemy1.centorPos.y - (enemy1.radius * 1.5f) };
	enemy1.posReset[1] = { enemy1.centorPos.x - (enemy1.radius * 1.5f),enemy1.centorPos.y + (enemy1.radius * 1.5f) };
	enemy1.posReset[2] = { enemy1.centorPos.x + (enemy1.radius * 1.5f),enemy1.centorPos.y + (enemy1.radius * 1.5f) };
#pragma endregion
#pragma region enemy2//シシカク

	testEnemy2 enemy2{};
	enemy2.radius = enemyRadius;
	const int kEnemy2Num = 16;
	enemy2.velocity[0] = { 0,-3 };
	enemy2.velocity[1] = { 3,0 };
	enemy2.velocity[2] = { 0,3 };
	enemy2.velocity[3] = { -3,0 };

	enemy2.posReset[0][0] = { enemy2.centorPos[0].x - enemy2.radius * 4.5f ,enemy2.centorPos[0].y - enemy2.radius * 7.5f };
	enemy2.posReset[0][1] = { enemy2.centorPos[0].x - enemy2.radius * 1.5f ,enemy2.centorPos[0].y - enemy2.radius * 7.5f };
	enemy2.posReset[0][2] = { enemy2.centorPos[0].x + enemy2.radius * 1.5f ,enemy2.centorPos[0].y - enemy2.radius * 7.5f };
	enemy2.posReset[0][3] = { enemy2.centorPos[0].x + enemy2.radius * 4.5f ,enemy2.centorPos[0].y - enemy2.radius * 7.5f };

	enemy2.posReset[0][4] = { enemy2.centorPos[0].x + enemy2.radius * 7.5f ,enemy2.centorPos[0].y - enemy2.radius * 4.5f };
	enemy2.posReset[0][5] = { enemy2.centorPos[0].x + enemy2.radius * 7.5f ,enemy2.centorPos[0].y - enemy2.radius * 1.5f };
	enemy2.posReset[0][6] = { enemy2.centorPos[0].x + enemy2.radius * 7.5f ,enemy2.centorPos[0].y + enemy2.radius * 1.5f };
	enemy2.posReset[0][7] = { enemy2.centorPos[0].x + enemy2.radius * 7.5f ,enemy2.centorPos[0].y + enemy2.radius * 4.5f };

	enemy2.posReset[0][8] = { enemy2.centorPos[0].x + enemy2.radius * 4.5f ,enemy2.centorPos[0].y + enemy2.radius * 7.5f };
	enemy2.posReset[0][9] = { enemy2.centorPos[0].x + enemy2.radius * 1.5f ,enemy2.centorPos[0].y + enemy2.radius * 7.5f };
	enemy2.posReset[0][10] = { enemy2.centorPos[0].x - enemy2.radius * 1.5f ,enemy2.centorPos[0].y + enemy2.radius * 7.5f };
	enemy2.posReset[0][11] = { enemy2.centorPos[0].x - enemy2.radius * 4.5f ,enemy2.centorPos[0].y + enemy2.radius * 7.5f };

	enemy2.posReset[0][12] = { enemy2.centorPos[0].x - enemy2.radius * 7.5f ,enemy2.centorPos[0].y + enemy2.radius * 4.5f };
	enemy2.posReset[0][13] = { enemy2.centorPos[0].x - enemy2.radius * 7.5f ,enemy2.centorPos[0].y + enemy2.radius * 1.5f };
	enemy2.posReset[0][14] = { enemy2.centorPos[0].x - enemy2.radius * 7.5f ,enemy2.centorPos[0].y - enemy2.radius * 1.5f };
	enemy2.posReset[0][15] = { enemy2.centorPos[0].x - enemy2.radius * 7.5f ,enemy2.centorPos[0].y - enemy2.radius * 4.5f };

	for (int j = 1; j < 4; j++)
	{
		for (int i = 0; i < 16; i++)
		{
			enemy2.posReset[j][i] = enemy2.posReset[0][i];
		}
	}
#pragma endregion
#pragma region enemy3//横一列

	testEnemy3 enemy3{};
	enemy3.radius = enemyRadius;

	enemy3.posReset[0] = { enemy3.centorPos.x - enemy3.radius * 4.5f ,enemy3.centorPos.y };
	enemy3.posReset[1] = { enemy3.centorPos.x - enemy3.radius * 1.5f ,enemy3.centorPos.y };
	enemy3.posReset[2] = { enemy3.centorPos.x + enemy3.radius * 1.5f ,enemy3.centorPos.y };
	enemy3.posReset[3] = { enemy3.centorPos.x + enemy3.radius * 4.5f ,enemy3.centorPos.y };
#pragma endregion
#pragma region enemy4//縦一列

	testEnemy3 enemy4{};
	enemy4.radius = enemyRadius;

	enemy4.posReset[0] = { enemy4.centorPos.x,enemy4.centorPos.y - enemy4.radius * 4.5f };
	enemy4.posReset[1] = { enemy4.centorPos.x,enemy4.centorPos.y - enemy4.radius * 1.5f };
	enemy4.posReset[2] = { enemy4.centorPos.x,enemy4.centorPos.y + enemy4.radius * 1.5f };
	enemy4.posReset[3] = { enemy4.centorPos.x,enemy4.centorPos.y + enemy4.radius * 4.5f };
#pragma endregion
#pragma region enemy5//左斜め上

	testEnemy3 enemy5{};
	enemy5.radius = enemyRadius;

	enemy5.posReset[0] = { enemy5.centorPos.x - enemy5.radius * 4.5f * 0.7f,enemy5.centorPos.y - enemy5.radius * 4.5f * 0.7f };
	enemy5.posReset[1] = { enemy5.centorPos.x - enemy5.radius * 1.5f * 0.7f,enemy5.centorPos.y - enemy5.radius * 1.5f * 0.7f };
	enemy5.posReset[2] = { enemy5.centorPos.x + enemy5.radius * 1.5f * 0.7f,enemy5.centorPos.y + enemy5.radius * 1.5f * 0.7f };
	enemy5.posReset[3] = { enemy5.centorPos.x + enemy5.radius * 4.5f * 0.7f,enemy5.centorPos.y + enemy5.radius * 4.5f * 0.7f };
#pragma endregion
#pragma region enemy6//右斜め上

	testEnemy3 enemy6{};
	enemy6.radius = enemyRadius;

	enemy6.posReset[0] = { enemy6.centorPos.x + enemy6.radius * 4.5f * 0.7f,enemy6.centorPos.y - enemy6.radius * 4.5f * 0.7f };
	enemy6.posReset[1] = { enemy6.centorPos.x + enemy6.radius * 1.5f * 0.7f,enemy6.centorPos.y - enemy6.radius * 1.5f * 0.7f };
	enemy6.posReset[2] = { enemy6.centorPos.x - enemy6.radius * 1.5f * 0.7f,enemy6.centorPos.y + enemy6.radius * 1.5f * 0.7f };
	enemy6.posReset[3] = { enemy6.centorPos.x - enemy6.radius * 4.5f * 0.7f,enemy6.centorPos.y + enemy6.radius * 4.5f * 0.7f };
#pragma endregion
#pragma region enemy7//八方向
	testEnemy4 enemy7{};
	enemy7.radius = enemyRadius;
	float enemy7PosRadius = 1400;
	float enemy7PosSpeed = 4;
	enemy7.velocity[0] = { 0,enemy7PosSpeed };
	enemy7.velocity[1] = { -enemy7PosSpeed,0 };
	enemy7.velocity[2] = { 0,-enemy7PosSpeed };
	enemy7.velocity[3] = { enemy7PosSpeed,0 };

	enemy7.velocity[4] = { -enemy7PosSpeed * 0.7f,enemy7PosSpeed * 0.7f };
	enemy7.velocity[5] = { -enemy7PosSpeed * 0.7f,-enemy7PosSpeed * 0.7f };
	enemy7.velocity[6] = { enemy7PosSpeed * 0.7f,-enemy7PosSpeed * 0.7f };
	enemy7.velocity[7] = { enemy7PosSpeed * 0.7f,enemy7PosSpeed * 0.7f };

	enemy7.centorPos[0] = { 0,-enemy7PosRadius };//上
	enemy7.centorPos[1] = { enemy7PosRadius,0 };//右
	enemy7.centorPos[2] = { 0,+enemy7PosRadius };//下
	enemy7.centorPos[3] = { -enemy7PosRadius,0 };//左

	enemy7.centorPos[4] = { enemy7PosRadius * 0.7f,-enemy7PosRadius * 0.7f };//右上
	enemy7.centorPos[5] = { enemy7PosRadius * 0.7f,enemy7PosRadius * 0.7f };//右下
	enemy7.centorPos[6] = { -enemy7PosRadius * 0.7f,enemy7PosRadius * 0.7f };//左下
	enemy7.centorPos[7] = { -enemy7PosRadius * 0.7f,-enemy7PosRadius * 0.7f };//左上

	//上
	enemy7.posReset[0][0] = { enemy7.centorPos[0].x,enemy7.centorPos[0].y - enemy7.radius * 4.5f };
	enemy7.posReset[0][1] = { enemy7.centorPos[0].x,enemy7.centorPos[0].y - enemy7.radius * 1.5f };
	enemy7.posReset[0][2] = { enemy7.centorPos[0].x,enemy7.centorPos[0].y + enemy7.radius * 1.5f };
	enemy7.posReset[0][3] = { enemy7.centorPos[0].x,enemy7.centorPos[0].y + enemy7.radius * 4.5f };

	//右
	enemy7.posReset[1][0] = { enemy7.centorPos[1].x - enemy7.radius * 4.5f ,enemy7.centorPos[1].y };
	enemy7.posReset[1][1] = { enemy7.centorPos[1].x - enemy7.radius * 1.5f ,enemy7.centorPos[1].y };
	enemy7.posReset[1][2] = { enemy7.centorPos[1].x + enemy7.radius * 1.5f ,enemy7.centorPos[1].y };
	enemy7.posReset[1][3] = { enemy7.centorPos[1].x + enemy7.radius * 4.5f ,enemy7.centorPos[1].y };

	//下
	enemy7.posReset[2][0] = { enemy7.centorPos[2].x,enemy7.centorPos[2].y - enemy7.radius * 4.5f };
	enemy7.posReset[2][1] = { enemy7.centorPos[2].x,enemy7.centorPos[2].y - enemy7.radius * 1.5f };
	enemy7.posReset[2][2] = { enemy7.centorPos[2].x,enemy7.centorPos[2].y + enemy7.radius * 1.5f };
	enemy7.posReset[2][3] = { enemy7.centorPos[2].x,enemy7.centorPos[2].y + enemy7.radius * 4.5f };

	//左
	enemy7.posReset[3][0] = { enemy7.centorPos[3].x - enemy7.radius * 4.5f ,enemy7.centorPos[3].y };
	enemy7.posReset[3][1] = { enemy7.centorPos[3].x - enemy7.radius * 1.5f ,enemy7.centorPos[3].y };
	enemy7.posReset[3][2] = { enemy7.centorPos[3].x + enemy7.radius * 1.5f ,enemy7.centorPos[3].y };
	enemy7.posReset[3][3] = { enemy7.centorPos[3].x + enemy7.radius * 4.5f ,enemy7.centorPos[3].y };

	//右上
	enemy7.posReset[4][0] = { enemy7.centorPos[4].x + enemy7.radius * 4.5f * 0.7f,enemy7.centorPos[4].y - enemy7.radius * 4.5f * 0.7f };
	enemy7.posReset[4][1] = { enemy7.centorPos[4].x + enemy7.radius * 1.5f * 0.7f,enemy7.centorPos[4].y - enemy7.radius * 1.5f * 0.7f };
	enemy7.posReset[4][2] = { enemy7.centorPos[4].x - enemy7.radius * 1.5f * 0.7f,enemy7.centorPos[4].y + enemy7.radius * 1.5f * 0.7f };
	enemy7.posReset[4][3] = { enemy7.centorPos[4].x - enemy7.radius * 4.5f * 0.7f,enemy7.centorPos[4].y + enemy7.radius * 4.5f * 0.7f };

	//右下
	enemy7.posReset[5][0] = { enemy7.centorPos[5].x - enemy7.radius * 4.5f * 0.7f,enemy7.centorPos[5].y - enemy7.radius * 4.5f * 0.7f };
	enemy7.posReset[5][1] = { enemy7.centorPos[5].x - enemy7.radius * 1.5f * 0.7f,enemy7.centorPos[5].y - enemy7.radius * 1.5f * 0.7f };
	enemy7.posReset[5][2] = { enemy7.centorPos[5].x + enemy7.radius * 1.5f * 0.7f,enemy7.centorPos[5].y + enemy7.radius * 1.5f * 0.7f };
	enemy7.posReset[5][3] = { enemy7.centorPos[5].x + enemy7.radius * 4.5f * 0.7f,enemy7.centorPos[5].y + enemy7.radius * 4.5f * 0.7f };

	//左下
	enemy7.posReset[6][0] = { enemy7.centorPos[6].x + enemy7.radius * 4.5f * 0.7f,enemy7.centorPos[6].y - enemy7.radius * 4.5f * 0.7f };
	enemy7.posReset[6][1] = { enemy7.centorPos[6].x + enemy7.radius * 1.5f * 0.7f,enemy7.centorPos[6].y - enemy7.radius * 1.5f * 0.7f };
	enemy7.posReset[6][2] = { enemy7.centorPos[6].x - enemy7.radius * 1.5f * 0.7f,enemy7.centorPos[6].y + enemy7.radius * 1.5f * 0.7f };
	enemy7.posReset[6][3] = { enemy7.centorPos[6].x - enemy7.radius * 4.5f * 0.7f,enemy7.centorPos[6].y + enemy7.radius * 4.5f * 0.7f };

	//左上
	enemy7.posReset[7][0] = { enemy7.centorPos[7].x - enemy7.radius * 4.5f * 0.7f,enemy7.centorPos[7].y - enemy7.radius * 4.5f * 0.7f };
	enemy7.posReset[7][1] = { enemy7.centorPos[7].x - enemy7.radius * 1.5f * 0.7f,enemy7.centorPos[7].y - enemy7.radius * 1.5f * 0.7f };
	enemy7.posReset[7][2] = { enemy7.centorPos[7].x + enemy7.radius * 1.5f * 0.7f,enemy7.centorPos[7].y + enemy7.radius * 1.5f * 0.7f };
	enemy7.posReset[7][3] = { enemy7.centorPos[7].x + enemy7.radius * 4.5f * 0.7f,enemy7.centorPos[7].y + enemy7.radius * 4.5f * 0.7f };
#pragma endregion
#pragma region enemy8//ジグザグ
	testEnemy5 enemy8{};
	enemy8.radius = enemyRadius;
	enemy8.velocity[0].x = 4;
	enemy8.velocity[1].x = -4;
	enemy8.velocity[2].x = 4;
	enemy8.velocity[3].x = -4;


	enemy8.posReset[0][0] = { enemy8.centorPos[0].x - enemy8.radius * 4.5f ,enemy8.centorPos[0].y };
	enemy8.posReset[0][1] = { enemy8.centorPos[0].x - enemy8.radius * 1.5f ,enemy8.centorPos[0].y };
	enemy8.posReset[0][2] = { enemy8.centorPos[0].x + enemy8.radius * 1.5f ,enemy8.centorPos[0].y };
	enemy8.posReset[0][3] = { enemy8.centorPos[0].x + enemy8.radius * 4.5f ,enemy8.centorPos[0].y };

	enemy8.posReset[0][4] = { enemy8.centorPos[0].x - enemy8.radius * 12.5f ,enemy8.centorPos[0].y };
	enemy8.posReset[0][5] = { enemy8.centorPos[0].x - enemy8.radius * 15.5f ,enemy8.centorPos[0].y };
	enemy8.posReset[0][6] = { enemy8.centorPos[0].x - enemy8.radius * 18.5f ,enemy8.centorPos[0].y };
	enemy8.posReset[0][7] = { enemy8.centorPos[0].x - enemy8.radius * 21.5f ,enemy8.centorPos[0].y };

	enemy8.posReset[0][8] = { enemy8.centorPos[0].x + enemy8.radius * 12.5f ,enemy8.centorPos[0].y };
	enemy8.posReset[0][9] = { enemy8.centorPos[0].x + enemy8.radius * 15.5f ,enemy8.centorPos[0].y };
	enemy8.posReset[0][10] = { enemy8.centorPos[0].x + enemy8.radius * 18.5f ,enemy8.centorPos[0].y };
	enemy8.posReset[0][11] = { enemy8.centorPos[0].x + enemy8.radius * 21.5f ,enemy8.centorPos[0].y };

	for (int j = 1; j < 4; j++)
	{
		for (int i = 0; i < 12; i++)
		{
			enemy8.posReset[j][i] = enemy8.posReset[0][i];
		}
	}
#pragma endregion
#pragma region enemy9//センターゴー
	testEnemy6 enemy9{};
	enemy9.radius = enemyRadius;

	enemy9.centorPos[0].y = 800;
	enemy9.centorPos[1].y = -800;

	enemy9.posReset[0][0] = { enemy9.centorPos[0].x - enemy9.radius * 11.5f ,enemy9.centorPos[0].y };
	enemy9.posReset[0][1] = { enemy9.centorPos[0].x - enemy9.radius * 8.5f ,enemy9.centorPos[0].y };
	enemy9.posReset[0][2] = { enemy9.centorPos[0].x - enemy9.radius * 5.5f ,enemy9.centorPos[0].y };
	enemy9.posReset[0][3] = { enemy9.centorPos[0].x - enemy9.radius * 2.5f ,enemy9.centorPos[0].y };
	enemy9.posReset[0][4] = { enemy9.centorPos[0].x + enemy9.radius * 11.5f ,enemy9.centorPos[0].y };
	enemy9.posReset[0][5] = { enemy9.centorPos[0].x + enemy9.radius * 8.5f ,enemy9.centorPos[0].y };
	enemy9.posReset[0][6] = { enemy9.centorPos[0].x + enemy9.radius * 5.5f ,enemy9.centorPos[0].y };
	enemy9.posReset[0][7] = { enemy9.centorPos[0].x + enemy9.radius * 2.5f ,enemy9.centorPos[0].y };

	for (int i = 0; i < 8; i++)
	{
		enemy9.posReset[1][i].x = enemy9.posReset[0][i].x;
		enemy9.posReset[1][i].y = enemy9.centorPos[1].y;
	}

	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < 8; i++)
		{
			enemy9.velocity[j][i].x = vectorNormalize(enemy9.posReset[j][i], { 0,0 }).x * 4;
			enemy9.velocity[j][i].y = vectorNormalize(enemy9.posReset[j][i], { 0,0 }).y * 4;
		}
	}
#pragma endregion
#pragma region enemy10//ランダムに横一列の４体が出る
	const int kenemy10Num = 4;
	testEnemy3 enemy10[kenemy10Num]{};//ランダムに横一列の４体が出る
	for (int j = 0; j < kenemy10Num; j++)
	{
		enemy10[j].radius = enemyRadius;

		for (int i = 0; i < 4; i++)
		{
			enemy10[j].posReset[i] = enemy3.posReset[i];

		}
	}
#pragma endregion
#pragma region enemy11//ランダムに三角形が出る
	const int kenemy11Num = 10;
	testEnemy1 enemy11[kenemy11Num]{};

	for (int j = 0; j < kenemy11Num; j++)
	{
		enemy11[j].radius = enemyRadius;
		for (int i = 0; i < 3; i++)
		{
			enemy11[j].posReset[i] = enemy1.posReset[i];
		}

	}
#pragma endregion
#pragma region enemy12//ランダムに縦一列の４体が出る
	const int kenemy12Num = 4;
	testEnemy3 enemy12[kenemy12Num]{};//ランダムに横一列の４体が出る
	for (int j = 0; j < kenemy12Num; j++)
	{
		enemy12[j].radius = enemyRadius;

		for (int i = 0; i < 4; i++)
		{
			enemy12[j].posReset[i] = enemy4.posReset[i];

		}
	}
#pragma endregion

	Count dash{};//ダッシュ攻撃で倒した数
	Count triangle{};//三角攻撃で倒した数
	float count = 0;//全体で倒した数
	float endCount = 0;//フィーバーごとにリセット
	float precount = 0;

	Ennergy ennergy{};
	ennergy.dashRate = 3;//ダッシュは三角の数字倍貯まる
	ennergy.triangleRate = 1;
	ennergy.powerUp = 2.0f;//移動速度が倍になる
	ennergy.max = 100;
	ennergy.fever = false;
	ennergy.feverTime = 300;//フレーム数 実際にはちょっと長くなる
	ennergy.damage = 0.2f;//攻撃していない時に敵に当たった時ダメージの代わりにゲージが減る

	int shakeRadius = 10;
	int shakeGaugeX = shakeRadius;
	int shakeGaugeY = shakeRadius;
	int shakeGaugeSeed = 1;
	Vector2 shakeGaugePos{};

	float screenSize = 1;//フィーバー中画面を小さくする
	float setScreenSize = 2.3f;//数字が大きいほど画面が小さくなる
	float setScreenEaseT = 0;
	float setScreenEaseTChange = 1;

	int gameTimer = 0;
	// クラス変数の宣言
	Func Functions;
	AttackAreaObject attAreaObj;
	EnemyObject enemyObj;

	// 敵の座標を仮に宣言・定義
	Vector2 enemyPos = { 0,0 };

	// 白地テクスチャ
	const int kWhiteTexture = Novice::LoadTexture("white1x1.png");


	// 敵の生存フラグを宣言・定義
	bool isEnemyDead = false;

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///                                                            ///
		/// --------------------↓更新処理ここから-------------------- ///
		///                                                            ///       
		if (keys[DIK_RETURN])
		{
			gameTimer = 0;
		}

#pragma region count
		//倒した数のカウント
		if (!player.dashAttack)
		{
			if (dash.endCount == 0)dash.endCount = dash.count - dash.preCount;
			dash.preCount = dash.count;
		}
		else
		{
			dash.endCount = 0;
			ennergy.dash = false;

		}
		if (!player.triangulAttack)
		{
			if (triangle.endCount == 0)triangle.endCount = triangle.count - triangle.preCount;
			triangle.preCount = triangle.count;
		}
		else
		{
			triangle.endCount = 0;
			ennergy.triangle = false;


		}

		if (!player.dashAttack && !player.triangulAttack)
		{
			if (endCount == 0)endCount = count - precount;
			//precount = count;
		}
		else
		{
			endCount = 0;
		}
#pragma endregion
#pragma region ennegy
		//ゲージ処理
		if (!ennergy.fever)
		{
			setScreenEaseTChange = -1;
			if (screenSize > 1.0f)
			{
				screenSize -= 0.01f;
				if (screenSize < 1.0f)
				{
					screenSize = 1.0f;
				}
			}
			if (!ennergy.dash && dash.endCount != 0)
			{
				ennergy.count += dash.endCount * ennergy.dashRate * (dash.endCount / 50 + 1);//撃破数×倍率×一度に倒した数の倍率
				ennergy.dash = true;
			}
			if (!ennergy.triangle && triangle.endCount != 0)
			{
				ennergy.count += triangle.endCount * ennergy.triangleRate * (triangle.endCount / 50 + 1);//撃破数×倍率×一度に倒した数の倍率
				ennergy.triangle = true;
			}

			if (ennergy.count >= ennergy.max)
			{
				ennergy.count = ennergy.max;
				ennergy.fever = true;
			}

		}
		else
		{
			setScreenEaseTChange = 1;
			ennergy.count -= ennergy.max / ennergy.feverTime;
			;
			if (ennergy.count < 0)
			{
				ennergy.count = 0;
				ennergy.fever = false;
			}

		}

		//スクリーン拡縮処理
		screenSize = easeInOutCubic(setScreenEaseT / 100) * (setScreenSize - 1) + 1;//easeOutBack
		if (setScreenEaseT < 100 || setScreenEaseT > 0)
		{
			setScreenEaseT += setScreenEaseTChange * 2;
			if (setScreenEaseT > 100)setScreenEaseT = 100;
			if (setScreenEaseT < 0)setScreenEaseT = 0;
		}
#pragma endregion
#pragma region enemySpawn
		if (keys[DIK_R])
		{
			if (keys[DIK_1])
			{
				enemy1.parentIsAlive = true;
				enemy1.centorPos = { 0,0 };
				for (int i = 0; i < 3; i++)
				{
					enemy1.childIsAlive[i] = true;
					enemy1.relativePos[i] = { enemy1.posReset[i].x + enemy1.centorPos.x,enemy1.posReset[i].y + enemy1.centorPos.y };
					enemy1.dedTimer[i] = kDedTimer;
					enemy1.isDed[i] = false;
				}
			}
			else if (keys[DIK_2])
			{
				enemy2.radius = 0;
				enemy2.easeT = 0;

				for (int j = 0; j < 4; j++)
				{
					enemy2.parentIsAlive[j] = true;
					enemy2.centorPos[0] = { -400,1200 };
					enemy2.centorPos[1] = { -1200,-400 };
					enemy2.centorPos[2] = { 400,-1200 };
					enemy2.centorPos[3] = { 1200,400 };
					for (int i = 0; i < kEnemy2Num; i++)
					{
						enemy2.childIsAlive[j][i] = true;
						enemy2.relativePos[j][i] = { enemy2.posReset[j][i].x + enemy2.centorPos[j].x,enemy2.posReset[j][i].y + enemy2.centorPos[j].y };
						enemy2.dedTimer[j][i] = kDedTimer;
						enemy2.isDed[j][i] = false;
					}
				}
				enemy2.hostIsAlive = true;

			}
			else if (keys[DIK_3])
			{
				enemy3.parentIsAlive = true;
				enemy3.centorPos = { 0,0 };
				for (int i = 0; i < 4; i++)
				{
					enemy3.childIsAlive[i] = true;
					enemy3.relativePos[i] = { enemy3.posReset[i].x + enemy3.centorPos.x,enemy3.posReset[i].y + enemy3.centorPos.y };
					enemy3.dedTimer[i] = kDedTimer;
					enemy3.isDed[i] = false;
				}

			}
			else if (keys[DIK_4])
			{
				enemy4.parentIsAlive = true;
				enemy4.centorPos = { 0,0 };
				for (int i = 0; i < 4; i++)
				{
					enemy4.childIsAlive[i] = true;
					enemy4.relativePos[i] = { enemy4.posReset[i].x + enemy4.centorPos.x,enemy4.posReset[i].y + enemy4.centorPos.y };
					enemy4.dedTimer[i] = kDedTimer;
					enemy4.isDed[i] = false;
				}

			}
			else if (keys[DIK_5])
			{
				enemy5.parentIsAlive = true;
				enemy5.centorPos = { 0,0 };
				for (int i = 0; i < 4; i++)
				{
					enemy5.childIsAlive[i] = true;
					enemy5.relativePos[i] = { enemy5.posReset[i].x + enemy5.centorPos.x,enemy5.posReset[i].y + enemy5.centorPos.y };
					enemy5.dedTimer[i] = kDedTimer;
					enemy5.isDed[i] = false;
				}

			}
			else if (keys[DIK_6])
			{
				enemy6.parentIsAlive = true;
				enemy6.centorPos = { 0,0 };
				for (int i = 0; i < 4; i++)
				{
					enemy6.childIsAlive[i] = true;
					enemy6.relativePos[i] = { enemy6.posReset[i].x + enemy6.centorPos.x,enemy6.posReset[i].y + enemy6.centorPos.y };
					enemy6.dedTimer[i] = kDedTimer;
					enemy6.isDed[i] = false;
				}

			}
			else if (keys[DIK_7])
			{
				enemy7.radius = 0;
				enemy7.easeT = 0;

				for (int j = 0; j < 8; j++)
				{
					enemy7.parentIsAlive[j] = true;
					for (int i = 0; i < 4; i++)
					{
						enemy7.childIsAlive[j][i] = true;
						enemy7.relativePos[j][i] = { enemy7.posReset[j][i].x ,enemy7.posReset[j][i].y };
						enemy7.dedTimer[j][i] = kDedTimer;
						enemy7.isDed[j][i] = false;
					}

				}
				enemy7.hostIsAlive = true;
			}
			else if (keys[DIK_8])
			{
				enemy8.radius = 0;
				enemy8.easeT = 0;
				enemy8.centorPos[0] = { -600,-450 };
				enemy8.centorPos[1] = { 600,-150 };
				enemy8.centorPos[2] = { -600,150 };
				enemy8.centorPos[3] = { 600,450 };

				for (int j = 0; j < 4; j++)
				{
					enemy8.parentIsAlive[j] = true;
					for (int i = 0; i < 12; i++)
					{
						enemy8.childIsAlive[j][i] = true;
						enemy8.relativePos[j][i] = { enemy8.centorPos[j].x + enemy8.posReset[j][i].x ,enemy8.centorPos[j].y + enemy8.posReset[j][i].y };
						enemy8.dedTimer[j][i] = kDedTimer;
						enemy8.isDed[j][i] = false;
					}
				}
				enemy8.hostIsAlive = true;
			}
			else if (keys[DIK_9])
			{
				enemy9.radius = 0;
				enemy9.easeT = 0;

				for (int j = 0; j < 2; j++)
				{
					enemy9.parentIsAlive[j] = true;
					for (int i = 0; i < 8; i++)
					{
						enemy9.childIsAlive[j][i] = true;
						enemy9.relativePos[j][i] = { enemy9.posReset[j][i].x , enemy9.posReset[j][i].y };
						enemy9.dedTimer[j][i] = kDedTimer;
						enemy9.isDed[j][i] = false;
					}
				}
				enemy9.hostIsAlive = true;
			}
			else if (keys[DIK_0] && !preKeys[DIK_0])
			{
				for (int j = 0; j < kenemy10Num; j++)
				{
					if (!enemy10[j].parentIsAlive)
					{
						enemy10[j].parentIsAlive = true;
						enemy10[j].centorPos = randShake(1000, 1000, j + 1 + j * 3);

						for (int i = 0; i < 4; i++)
						{
							enemy10[j].childIsAlive[i] = true;
							enemy10[j].relativePos[i] = { enemy10[j].posReset[i].x + enemy10[j].centorPos.x,enemy10[j].posReset[i].y + enemy10[j].centorPos.y };
							enemy10[j].dedTimer[i] = kDedTimer;
							enemy10[j].isDed[i] = false;
						}
						break;
					}
				}
			}
		}

		//ランダム敵出現
		//横一列
		if (gameTimer != 0 && gameTimer % 630 == 0|| gameTimer != 0 && gameTimer%2400==0)
		{
			for (int j = 0; j < kenemy10Num; j++)
			{
				if (!enemy10[j].parentIsAlive)
				{
					enemy10[j].radius = 0;
					enemy10[j].easeT = 0;
					enemy10[j].parentIsAlive = true;
					enemy10[j].centorPos = randShake(1000, 1000, gameTimer + 1);
					if (enemy10[j].centorPos.x >= 0)
					{
						enemy10[j].velocity.x = -4;
					}
					else
					{
						enemy10[j].velocity.x = 4;

					}

					for (int i = 0; i < 4; i++)
					{
						enemy10[j].childIsAlive[i] = true;
						enemy10[j].relativePos[i] = { enemy10[j].posReset[i].x + enemy10[j].centorPos.x,enemy10[j].posReset[i].y + enemy10[j].centorPos.y };
						enemy10[j].dedTimer[i] = kDedTimer;
						enemy10[j].isDed[i] = false;
					}
					break;
				}
			}
		}
		//三角形
		if (gameTimer != 0 && gameTimer % 330 == 0 || gameTimer >= 3000 && gameTimer % 730 == 0)
		{
			for (int j = 0; j < kenemy11Num; j++)
			{
				if (!enemy11[j].parentIsAlive)
				{
					enemy11[j].radius = 0;
					enemy11[j].easeT = 0;
					enemy11[j].parentIsAlive = true;
					enemy11[j].centorPos = randShake(1000, 1000, gameTimer + 1);
					if (enemy11[j].centorPos.x >= 0)
					{
						enemy11[j].velocity.x = -4 * 0.7f;
					}
					else
					{
						enemy11[j].velocity.x = 4 * 0.7f;
					}
					if (enemy11[j].centorPos.y >= 0)
					{
						enemy11[j].velocity.y = -4 * 0.7f;
					}
					else
					{
						enemy11[j].velocity.y = 4 * 0.7f;
					}
					for (int i = 0; i < 3; i++)
					{
						enemy11[j].childIsAlive[i] = true;
						enemy11[j].relativePos[i] = { enemy11[j].posReset[i].x + enemy11[j].centorPos.x,enemy11[j].posReset[i].y + enemy11[j].centorPos.y };
						enemy11[j].dedTimer[i] = kDedTimer;
						enemy11[j].isDed[i] = false;
					}
					break;
				}
			}
		}
		//縦一列
		if (gameTimer >= 1800 && gameTimer % 600 == 0|| gameTimer >= 1800 && gameTimer % 900 == 0||gameTimer != 0 && gameTimer % 2400 == 0)
		{
			for (int j = 0; j < kenemy12Num; j++)
			{
				if (!enemy12[j].parentIsAlive)
				{
					enemy12[j].radius = 0;
					enemy12[j].easeT = 0;
					enemy12[j].parentIsAlive = true;
					enemy12[j].centorPos = randShake(1000, 1000, gameTimer + 1);
					if (enemy12[j].centorPos.y >= 0)
					{
						enemy12[j].velocity.y = -4;
					}
					else
					{
						enemy12[j].velocity.y = 4;

					}

					for (int i = 0; i < 4; i++)
					{
						enemy12[j].childIsAlive[i] = true;
						enemy12[j].relativePos[i] = { enemy12[j].posReset[i].x + enemy12[j].centorPos.x,enemy12[j].posReset[i].y + enemy12[j].centorPos.y };
						enemy12[j].dedTimer[i] = kDedTimer;
						enemy12[j].isDed[i] = false;
					}
					break;
				}
			}
		}
		//敵出現のイージング
		if (enemy2.hostIsAlive)
		{
			enemy2.radius = easeOutElastic(enemy2.easeT / 100) * enemyRadius;
			if (enemy2.easeT < 100)enemy2.easeT += 2;
			if (enemy2.easeT > 100)enemy2.easeT = 100;

		}
		if (enemy7.hostIsAlive)
		{
			enemy7.radius = easeOutElastic(enemy7.easeT / 100) * enemyRadius;
			if (enemy7.easeT < 100)enemy7.easeT += 2;
			if (enemy7.easeT > 100)enemy7.easeT = 100;

		}
		if (enemy8.hostIsAlive)
		{
			enemy8.radius = easeOutElastic(enemy8.easeT / 100) * enemyRadius;
			if (enemy8.easeT < 100)enemy8.easeT += 2;
			if (enemy8.easeT > 100)enemy8.easeT = 100;

		}
		if (enemy9.hostIsAlive)
		{
			enemy9.radius = easeOutElastic(enemy9.easeT / 100) * enemyRadius;
			if (enemy9.easeT < 100)enemy9.easeT += 2;
			if (enemy9.easeT > 100)enemy9.easeT = 100;

		}
		for (int j = 0; j < kenemy10Num; j++)
		{
			if (enemy10[j].parentIsAlive)
			{
				enemy10[j].radius = easeOutElastic(enemy10[j].easeT / 100) * enemyRadius;
				if (enemy10[j].easeT < 100)enemy10[j].easeT += 2;
				if (enemy10[j].easeT > 100)enemy10[j].easeT = 100;

			}
		}
		for (int j = 0; j < kenemy11Num; j++)
		{
			if (enemy11[j].parentIsAlive)
			{
				enemy11[j].radius = easeOutElastic(enemy11[j].easeT / 100) * enemyRadius;
				if (enemy11[j].easeT < 100)enemy11[j].easeT += 2;
				if (enemy11[j].easeT > 100)enemy11[j].easeT = 100;

			}
		}
		for (int j = 0; j < kenemy12Num; j++)
		{
			if (enemy12[j].parentIsAlive)
			{
				enemy12[j].radius = easeOutElastic(enemy12[j].easeT / 100) * enemyRadius;
				if (enemy12[j].easeT < 100)enemy12[j].easeT += 2;
				if (enemy12[j].easeT > 100)enemy12[j].easeT = 100;

			}
		}
#pragma endregion
#pragma region player
		//前フレのジョイスティック情報を保存
		preJoyStickX = joystickX;
		preJoyStickY = joystickY;

		//速度減速率が0以上
		if (player.velocityRatio > 0)
		{
			//速度の割合を小さくしていく。
			player.velocityRatio -= 0.01f;
			if (player.velocityRatio < 0)
			{
				player.velocityRatio = 0;
			}
		}

		//フリックを検知したら
		if (player.flick && player.flickCT == 0)player.flickCT = 20;//CTをつけてすぐ減速しないように


		if (player.flickCT > 0)
		{
			player.flickCT--;
			if (player.flickCT <= 0)
			{
				//CTが0になったらフリック移動おわり
				player.flickCT = 0;
				player.flick = false;
				player.dashAttack = false;//突撃攻撃フラグをおろす

			}
		}

		//ボタン入力ここから
		player.trigerA = false;
		if (Novice::IsTriggerButton(0, kPadButton10))
		{
			player.trigerA = true;
		}


		//三角形を出した後の攻撃時間
		if (player.aimTimer > 0)
		{
			player.aimTimer--;
		}
		else if (player.aimTimer <= 0)
		{
			player.aimTimer = 0;
			player.triangulAttack = false;
			if (player.count == 0)
			{
				for (int i = 0; i < 3; i++)
				{
					//三角形の点
					//今は原点に戻してるけど意味はない
					player.prepos[i] = { 0,0 };
				}
			}
		}
		//攻撃終わりにAボタンを押した時
		if (player.trigerA && player.aimTimer == 0 && !player.dashAttack)
		{
			//移動入力がされている時
			if (player.direction.x != 0 || player.direction.y != 0)
			{

				//点がプレイヤーの位置に番号順で設置される
				player.prepos[player.count] = player.pos;
				//点の座標を番号順に記録
				attAreaObj.SetDashPoint(player.prepos[player.count].x, player.prepos[player.count].y, player.count);
				//プレイヤーの速度を等倍に
				player.velocityRatio = 1;
				player.aim = true;
				//点を出したら次の点を出すためにカウントを足す
				player.count++;
				if (player.count >= 3)
				{
					//三点設置したら設置フェーズをおわる
					player.count = 0;
					player.aim = false;
					player.aimTimer = 30;//攻撃時間を代入
					player.triangulAttack = true;//三角形攻撃を有効に

				}
			}


		}
		//ボタン入力ここまで


		///スティック入力ここから
		Novice::GetAnalogInputLeft(0, &joystickX, &joystickY);

		//コントローラーの原点からの距離をはかる
		player.flickLength = floatLength({ (float)joystickX, (float)joystickY }, { 0,0 });

		//ジョイスティック入力を-100から100まで
		player.joystick.x = (clump(float(joystickX / 32767.0f), -1, 1) * 100);
		player.joystick.y = (clump(float(joystickY / 32767.0f), -1, 1) * 100);

		//円の当たり判定的な。
		xy = vectorLength(player.joystick, { 0,0 });
		length = sqrtf(xy.x * xy.x + xy.y * xy.y);

		//長さがデッドゾーンを超えたら方向を代入
		if (length >= dedZone)
		{
			//方向を正規化。速度をかけるだけで使えるようにするため
			player.direction = vectorNormalize(player.joystick, { 0,0 });
		}
		else
		{
			player.direction = { 0,0 };
		}

		//はじき判定
		//フリック判定フレが0以上の時
		if (player.flickTimer > 0)
		{
			player.flickTimer--;
			//スティックが端に行ったら
			if (player.flickLength > 32000 && !player.aim && !player.triangulAttack)
			{
				//フリックをTRUEに
				player.flick = true;
				//突撃フラグを有効に
				player.dashAttack = true;
				//速度を等倍に
				player.velocityRatio = 1;
			}
		}
		//スティックのポジションがデッドゾーンの中の時
		if (preJoyStickX<dedZone * 50 && preJoyStickX > -dedZone * 50 && preJoyStickY<dedZone * 50 && preJoyStickY > -dedZone * 50)
		{
			//フリック判定フレをリセット
			if (!player.flick && player.flickCT == 0)player.flickTimer = 2;
		}
		//Aボタンまたはフリックしたとき
		if (player.trigerA || player.flick && player.flickCT == 0)
		{
			//方向に発射速度をかける
			player.velocity.x = player.direction.x * player.shotSpeed;
			player.velocity.y = player.direction.y * player.shotSpeed;

			if (ennergy.fever)
			{
				//フィーバーの倍率をかける
				player.velocity.x = player.direction.x * player.shotSpeed * ennergy.powerUp;
				player.velocity.y = player.direction.y * player.shotSpeed * ennergy.powerUp;

			}

		}

		//割合をかける事で徐々に減速する
		player.velocity.x *= player.velocityRatio;
		player.velocity.y *= player.velocityRatio;




		//フリックも三角形も作っていないとき
		if (!player.aim && !player.flick && player.aimTimer <= 5)
		{
			if (length >= dedZone)
			{
				//スティックで移動できるように
				player.velocity.x = player.joystick.x / 100 * player.moveSpeed;
				player.velocity.y = player.joystick.y / 100 * player.moveSpeed;
			}
			else
			{
				player.velocity = { 0,0 };
			}

		}

		///スティック入力ここまで
#pragma endregion
		/// キー入力（デバッグ) ここから

		if (keys[DIK_SPACE] && !preKeys[DIK_SPACE])
		{
			Functions.ToggleDebugMode();
		}

		/// キー入力（デバッグ）ここまで

		//ポジション移動
		player.pos.x += player.velocity.x;
		player.pos.y += player.velocity.y;

		//敵の移動
#pragma region enemyMove
		if (enemy1.parentIsAlive)
		{
			enemy1.centorPos.x++;
			for (int i = 0; i < 3; i++)
			{
				enemy1.childIsAlive[i] = EllipseCollision({ 0,0 }, fieldRadius, enemy1.relativePos[i], enemy1.radius);
				if (!enemy1.isDed[i] && enemy1.childIsAlive[i])enemy1.relativePos[i].x += 4;
			}
			if (!enemy1.childIsAlive[0] && !enemy1.childIsAlive[1] && !enemy1.childIsAlive[2])
			{
				enemy1.parentIsAlive = false;
			}
		}
		if (enemy2.hostIsAlive)
		{
			if (enemy2.easeT == 100)
			{
				for (int j = 0; j < 4; j++)
				{
					if (enemy2.parentIsAlive[j])
					{
						for (int i = 0; i < kEnemy2Num; i++)
						{

							enemy2.childIsAlive[j][i] = EllipseCollision({ 0,0 }, fieldRadius, enemy2.relativePos[j][i], enemy2.radius);
							if (!enemy2.isDed[j][i] && enemy2.childIsAlive[j][i])
							{
								enemy2.relativePos[j][i].x += enemy2.velocity[j].x;
								enemy2.relativePos[j][i].y += enemy2.velocity[j].y;
							}
						}
						if (!enemy2.childIsAlive[j][0] && !enemy2.childIsAlive[j][1] && !enemy2.childIsAlive[j][2] && !enemy2.childIsAlive[j][3]
							&& !enemy2.childIsAlive[j][4] && !enemy2.childIsAlive[j][5] && !enemy2.childIsAlive[j][6] && !enemy2.childIsAlive[j][7]
							&& !enemy2.childIsAlive[j][8] && !enemy2.childIsAlive[j][9] && !enemy2.childIsAlive[j][10] && !enemy2.childIsAlive[j][11]
							&& !enemy2.childIsAlive[j][12] && !enemy2.childIsAlive[j][13] && !enemy2.childIsAlive[j][14] && !enemy2.childIsAlive[j][15])
						{
							enemy2.parentIsAlive[j] = false;
						}

					}
				}
			}
		}


		if (enemy3.parentIsAlive)
		{
			enemy3.centorPos.x++;
			for (int i = 0; i < 4; i++)
			{
				enemy3.childIsAlive[i] = EllipseCollision({ 0,0 }, fieldRadius, enemy3.relativePos[i], enemy3.radius);
				if (!enemy3.isDed[i] && enemy3.childIsAlive[i])enemy3.relativePos[i].x++;
			}
			if (!enemy3.childIsAlive[0] && !enemy3.childIsAlive[1] && !enemy3.childIsAlive[2] && !enemy3.childIsAlive[3])
			{
				enemy3.parentIsAlive = false;
			}
		}
		if (enemy4.parentIsAlive)
		{
			enemy4.centorPos.x++;
			for (int i = 0; i < 4; i++)
			{
				enemy4.childIsAlive[i] = EllipseCollision({ 0,0 }, fieldRadius, enemy4.relativePos[i], enemy4.radius);
				if (!enemy4.isDed[i] && enemy4.childIsAlive[i])enemy4.relativePos[i].x++;
			}
			if (!enemy4.childIsAlive[0] && !enemy4.childIsAlive[1] && !enemy4.childIsAlive[2] && !enemy4.childIsAlive[3])
			{
				enemy4.parentIsAlive = false;
			}
		}
		if (enemy5.parentIsAlive)
		{
			enemy5.centorPos.x++;
			for (int i = 0; i < 4; i++)
			{
				enemy5.childIsAlive[i] = EllipseCollision({ 0,0 }, fieldRadius, enemy5.relativePos[i], enemy5.radius);
				if (!enemy5.isDed[i] && enemy5.childIsAlive[i])enemy5.relativePos[i].x++;
			}
			if (!enemy5.childIsAlive[0] && !enemy5.childIsAlive[1] && !enemy5.childIsAlive[2] && !enemy5.childIsAlive[3])
			{
				enemy5.parentIsAlive = false;
			}
		}
		if (enemy6.parentIsAlive)
		{
			enemy6.centorPos.x++;
			for (int i = 0; i < 4; i++)
			{
				enemy6.childIsAlive[i] = EllipseCollision({ 0,0 }, fieldRadius, enemy6.relativePos[i], enemy6.radius);
				if (!enemy6.isDed[i] && enemy6.childIsAlive[i])enemy6.relativePos[i].x++;
			}
			if (!enemy6.childIsAlive[0] && !enemy6.childIsAlive[1] && !enemy6.childIsAlive[2] && !enemy6.childIsAlive[3])
			{
				enemy6.parentIsAlive = false;
			}
		}
		if (enemy7.hostIsAlive)
		{
			if (enemy7.easeT == 100)
			{

				for (int j = 0; j < 8; j++)
				{
					if (enemy7.parentIsAlive[j])
					{
						enemy7.centorPos[j].x += enemy7.velocity[j].x;
						enemy7.centorPos[j].y += enemy7.velocity[j].y;

						for (int i = 0; i < 4; i++)
						{
							enemy7.childIsAlive[j][i] = EllipseCollision({ 0,0 }, fieldRadius, enemy7.relativePos[j][i], enemy7.radius);

							if (!enemy7.isDed[j][i] && enemy7.childIsAlive[j][i])
							{
								enemy7.relativePos[j][i].x += enemy7.velocity[j].x;
								enemy7.relativePos[j][i].y += enemy7.velocity[j].y;
							}
						}
						if (!enemy7.childIsAlive[j][0] && !enemy7.childIsAlive[j][1] && !enemy7.childIsAlive[j][2] && !enemy7.childIsAlive[j][3])
						{
							enemy7.parentIsAlive[j] = false;
						}
					}
				}
			}
			if (enemy7.parentIsAlive[0] == false && enemy7.parentIsAlive[1] == false && enemy7.parentIsAlive[2] == false && enemy7.parentIsAlive[3] == false && enemy7.parentIsAlive[4] == false && enemy7.parentIsAlive[5] == false && enemy7.parentIsAlive[6] == false && enemy7.parentIsAlive[7] == false)enemy7.hostIsAlive = false;
		}
		if (enemy8.hostIsAlive)
		{
			if (enemy8.easeT == 100)
			{
				for (int j = 0; j < 4; j++)
				{
					if (enemy8.parentIsAlive[j])
					{
						enemy8.centorPos[j].x += enemy8.velocity[j].x;

						for (int i = 0; i < 12; i++)
						{
							enemy8.childIsAlive[j][i] = EllipseCollision({ 0,0 }, fieldRadius, enemy8.relativePos[j][i], enemy8.radius);

							if (!enemy8.isDed[j][i] && enemy8.childIsAlive[j][i])
							{
								enemy8.relativePos[j][i].x += enemy8.velocity[j].x;
								enemy8.relativePos[j][i].y += enemy8.velocity[j].y;
							}
						}
						if (!enemy8.childIsAlive[j][0] && !enemy8.childIsAlive[j][1] && !enemy8.childIsAlive[j][2] && !enemy8.childIsAlive[j][3] && !enemy8.childIsAlive[j][4] && !enemy8.childIsAlive[j][5] && !enemy8.childIsAlive[j][6] && !enemy8.childIsAlive[j][7] && !enemy8.childIsAlive[j][8] && !enemy8.childIsAlive[j][9] && !enemy8.childIsAlive[j][10] && !enemy8.childIsAlive[j][11])
						{
							enemy8.parentIsAlive[j] = false;
						}
					}
				}
			}
			if (enemy8.parentIsAlive[0] == false && enemy8.parentIsAlive[1] == false && enemy8.parentIsAlive[2] == false && enemy8.parentIsAlive[3] == false)enemy8.hostIsAlive = false;
		}

		if (enemy9.hostIsAlive)
		{
			if (enemy9.easeT == 100)
			{
				for (int j = 0; j < 2; j++)
				{
					if (enemy9.parentIsAlive[j])
					{
						//enemy9.centorPos[j].x += enemy9.velocity[j][i].x;

						for (int i = 0; i < 8; i++)
						{
							enemy9.childIsAlive[j][i] = EllipseCollision({ 0,0 }, fieldRadius, enemy9.relativePos[j][i], enemy9.radius);

							if (!enemy9.isDed[j][i] && enemy9.childIsAlive[j][i])
							{
								enemy9.relativePos[j][i].x -= enemy9.velocity[j][i].x;
								enemy9.relativePos[j][i].y -= enemy9.velocity[j][i].y;
							}
						}
						if (!enemy9.childIsAlive[j][0] && !enemy9.childIsAlive[j][1] && !enemy9.childIsAlive[j][2] && !enemy9.childIsAlive[j][3] && !enemy9.childIsAlive[j][4] && !enemy9.childIsAlive[j][5] && !enemy9.childIsAlive[j][6] && !enemy9.childIsAlive[j][7])
						{
							enemy9.parentIsAlive[j] = false;
						}
					}
				}
			}
			if (enemy9.parentIsAlive[0] == false && enemy9.parentIsAlive[1] == false)enemy9.hostIsAlive = false;
		}

		for (int j = 0; j < kenemy10Num; j++)
		{
			if (enemy10[j].parentIsAlive)
			{
				if (enemy10[j].easeT == 100)
				{
					enemy10[j].centorPos.x++;
					for (int i = 0; i < 4; i++)
					{
						enemy10[j].childIsAlive[i] = EllipseCollision({ 0,0 }, fieldRadius, enemy10[j].relativePos[i], enemy10[j].radius);
						if (!enemy10[j].isDed[i] && enemy10[j].childIsAlive[i])enemy10[j].relativePos[i].x += enemy10[j].velocity.x;
					}
					if (!enemy10[j].childIsAlive[0] && !enemy10[j].childIsAlive[1] && !enemy10[j].childIsAlive[2] && !enemy10[j].childIsAlive[3])
					{
						enemy10[j].parentIsAlive = false;
					}
				}
			}
		}
		for (int j = 0; j < kenemy11Num; j++)
		{
			if (enemy11[j].parentIsAlive)
			{
				if (enemy11[j].easeT == 100)
				{
					enemy11[j].centorPos.x++;
					for (int i = 0; i < 3; i++)
					{
						enemy11[j].childIsAlive[i] = EllipseCollision({ 0,0 }, fieldRadius, enemy11[j].relativePos[i], enemy11[j].radius);
						if (!enemy11[j].isDed[i] && enemy11[j].childIsAlive[i])
						{
							enemy11[j].relativePos[i].x += enemy11[j].velocity.x;
							enemy11[j].relativePos[i].y += enemy11[j].velocity.y;
						}
					}
					if (!enemy11[j].childIsAlive[0] && !enemy11[j].childIsAlive[1] && !enemy11[j].childIsAlive[2] )
					{
						enemy11[j].parentIsAlive = false;
					}
				}
			}
		}
		for (int j = 0; j < kenemy12Num; j++)
		{
			if (enemy12[j].parentIsAlive)
			{
				if (enemy12[j].easeT == 100)
				{
					enemy12[j].centorPos.x++;
					for (int i = 0; i < 4; i++)
					{
						enemy12[j].childIsAlive[i] = EllipseCollision({ 0,0 }, fieldRadius, enemy12[j].relativePos[i], enemy12[j].radius);
						if (!enemy12[j].isDed[i] && enemy12[j].childIsAlive[i])enemy12[j].relativePos[i].y += enemy12[j].velocity.y;
					}
					if (!enemy12[j].childIsAlive[0] && !enemy12[j].childIsAlive[1] && !enemy12[j].childIsAlive[2] && !enemy12[j].childIsAlive[3])
					{
						enemy12[j].parentIsAlive = false;
					}
				}
			}
		}
#pragma endregion
		//ここで攻撃処理をしたいと考えてる

#pragma region playerAttack
		//ダッシュ攻撃
		if (player.dashAttack)
		{
			if (enemy1.parentIsAlive)
			{
				for (int i = 0; i < 3; i++)
				{
					if (!enemy1.isDed[i])enemy1.isDed[i] = EllipseCollision(player.pos, player.radius.x, enemy1.relativePos[i], enemy1.radius);
				}
			}
			if (enemy2.hostIsAlive)
			{
				for (int j = 0; j < 4; j++)
				{
					if (enemy2.parentIsAlive[j])
					{
						for (int i = 0; i < kEnemy2Num; i++)
						{
							if (!enemy2.isDed[j][i])enemy2.isDed[j][i] = EllipseCollision(player.pos, player.radius.x, enemy2.relativePos[j][i], enemy2.radius);
						}
					}
				}
			}

			if (enemy3.parentIsAlive)
			{
				for (int i = 0; i < 4; i++)
				{
					if (!enemy3.isDed[i])enemy3.isDed[i] = EllipseCollision(player.pos, player.radius.x, enemy3.relativePos[i], enemy3.radius);
				}
			}
			if (enemy4.parentIsAlive)
			{
				for (int i = 0; i < 4; i++)
				{
					if (!enemy4.isDed[i])enemy4.isDed[i] = EllipseCollision(player.pos, player.radius.x, enemy4.relativePos[i], enemy4.radius);
				}
			}
			if (enemy5.parentIsAlive)
			{
				for (int i = 0; i < 4; i++)
				{
					if (!enemy5.isDed[i])enemy5.isDed[i] = EllipseCollision(player.pos, player.radius.x, enemy5.relativePos[i], enemy5.radius);
				}
			}
			if (enemy6.parentIsAlive)
			{
				for (int i = 0; i < 4; i++)
				{
					if (!enemy6.isDed[i])enemy6.isDed[i] = EllipseCollision(player.pos, player.radius.x, enemy6.relativePos[i], enemy6.radius);
				}
			}
			if (enemy7.hostIsAlive)
			{
				for (int j = 0; j < 8; j++)
				{
					if (enemy7.parentIsAlive[j])
					{
						for (int i = 0; i < 4; i++)
						{
							if (!enemy7.isDed[j][i])enemy7.isDed[j][i] = EllipseCollision(player.pos, player.radius.x, enemy7.relativePos[j][i], enemy7.radius);
						}
					}
				}
			}
			if (enemy8.hostIsAlive)
			{
				for (int j = 0; j < 4; j++)
				{
					if (enemy8.parentIsAlive[j])
					{
						for (int i = 0; i < 12; i++)
						{
							if (!enemy8.isDed[j][i])enemy8.isDed[j][i] = EllipseCollision(player.pos, player.radius.x, enemy8.relativePos[j][i], enemy8.radius);
						}
					}
				}
			}
			if (enemy9.hostIsAlive)
			{
				for (int j = 0; j < 2; j++)
				{
					if (enemy9.parentIsAlive[j])
					{
						for (int i = 0; i < 8; i++)
						{
							if (!enemy9.isDed[j][i])enemy9.isDed[j][i] = EllipseCollision(player.pos, player.radius.x, enemy9.relativePos[j][i], enemy9.radius);
						}
					}
				}
			}
			for (int j = 0; j < kenemy10Num; j++)
			{
				if (enemy10[j].parentIsAlive)
				{
					for (int i = 0; i < 4; i++)
					{
						if (!enemy10[j].isDed[i])enemy10[j].isDed[i] = EllipseCollision(player.pos, player.radius.x, enemy10[j].relativePos[i], enemy10[j].radius);
					}
				}
			}
			for (int j = 0; j < kenemy11Num; j++)
			{
				if (enemy11[j].parentIsAlive)
				{
					for (int i = 0; i < 3; i++)
					{
						if (!enemy11[j].isDed[i])enemy11[j].isDed[i] = EllipseCollision(player.pos, player.radius.x, enemy11[j].relativePos[i], enemy11[j].radius);
					}
				}
			}
			for (int j = 0; j < kenemy12Num; j++)
			{
				if (enemy12[j].parentIsAlive)
				{
					for (int i = 0; i < 4; i++)
					{
						if (!enemy12[j].isDed[i])enemy12[j].isDed[i] = EllipseCollision(player.pos, player.radius.x, enemy12[j].relativePos[i], enemy12[j].radius);
					}
				}
			}
		}

		//三角攻撃
		if (player.triangulAttack)
		{

			// 敵の生存フラグを当たり判定によって更新
			isEnemyDead = attAreaObj.TriangleCollision(enemyPos);


			if (enemy1.parentIsAlive)
			{
				for (int i = 0; i < 3; i++)
				{
					if (!enemy1.isDed[i])enemy1.isDed[i] = attAreaObj.TriangleCollision(enemy1.relativePos[i]);
				}
			}
			if (enemy2.hostIsAlive)
			{
				for (int j = 0; j < 4; j++)
				{
					if (enemy2.parentIsAlive[j])
					{
						for (int i = 0; i < kEnemy2Num; i++)
						{
							if (!enemy2.isDed[j][i])enemy2.isDed[j][i] = attAreaObj.TriangleCollision(enemy2.relativePos[j][i]);
						}
					}
				}

			}

			if (enemy3.parentIsAlive)
			{
				for (int i = 0; i < 4; i++)
				{
					if (!enemy3.isDed[i])enemy3.isDed[i] = attAreaObj.TriangleCollision(enemy3.relativePos[i]);
				}
			}
			if (enemy4.parentIsAlive)
			{
				for (int i = 0; i < 4; i++)
				{
					if (!enemy4.isDed[i])enemy4.isDed[i] = attAreaObj.TriangleCollision(enemy4.relativePos[i]);
				}
			}
			if (enemy5.parentIsAlive)
			{
				for (int i = 0; i < 4; i++)
				{
					if (!enemy5.isDed[i])enemy5.isDed[i] = attAreaObj.TriangleCollision(enemy5.relativePos[i]);
				}
			}
			if (enemy6.parentIsAlive)
			{
				for (int i = 0; i < 4; i++)
				{
					if (!enemy6.isDed[i])enemy6.isDed[i] = attAreaObj.TriangleCollision(enemy6.relativePos[i]);
				}
			}
			if (enemy7.hostIsAlive)
			{
				for (int j = 0; j < 8; j++)
				{
					if (enemy7.parentIsAlive[j])
					{
						for (int i = 0; i < 4; i++)
						{
							if (!enemy7.isDed[j][i])enemy7.isDed[j][i] = attAreaObj.TriangleCollision(enemy7.relativePos[j][i]);
						}
					}
				}
			}
			if (enemy8.hostIsAlive)
			{
				for (int j = 0; j < 4; j++)
				{
					if (enemy8.parentIsAlive[j])
					{
						for (int i = 0; i < 12; i++)
						{
							if (!enemy8.isDed[j][i])enemy8.isDed[j][i] = attAreaObj.TriangleCollision(enemy8.relativePos[j][i]);
						}
					}
				}
			}
			if (enemy9.hostIsAlive)
			{
				for (int j = 0; j < 2; j++)
				{
					if (enemy9.parentIsAlive[j])
					{
						for (int i = 0; i < 8; i++)
						{
							if (!enemy9.isDed[j][i])enemy9.isDed[j][i] = attAreaObj.TriangleCollision(enemy9.relativePos[j][i]);
						}
					}
				}
			}
			for (int j = 0; j < kenemy10Num; j++)
			{
				if (enemy10[j].parentIsAlive)
				{
					for (int i = 0; i < 4; i++)
					{
						if (!enemy10[j].isDed[i])enemy10[j].isDed[i] = attAreaObj.TriangleCollision(enemy10[j].relativePos[i]);
					}
				}
			}
			for (int j = 0; j < kenemy11Num; j++)
			{
				if (enemy11[j].parentIsAlive)
				{
					for (int i = 0; i < 3; i++)
					{
						if (!enemy11[j].isDed[i])enemy11[j].isDed[i] = attAreaObj.TriangleCollision(enemy11[j].relativePos[i]);
					}
				}
			}
			for (int j = 0; j < kenemy12Num; j++)
			{
				if (enemy12[j].parentIsAlive)
				{
					for (int i = 0; i < 4; i++)
					{
						if (!enemy12[j].isDed[i])enemy12[j].isDed[i] = attAreaObj.TriangleCollision(enemy12[j].relativePos[i]);
					}
				}
			}
		}
#pragma endregion

#pragma region playerDamage
		if (!player.dashAttack && !player.triangulAttack)
		{
			if (enemy1.parentIsAlive)
			{
				for (int i = 0; i < 3; i++)
				{
					if (enemy1.dedTimer[i] == kDedTimer && EllipseCollision(player.pos, player.radius.x, enemy1.relativePos[i], enemy1.radius))
					{
						if (ennergy.count > 0)
						{
							ennergy.count -= ennergy.damage;
							ennergy.damageFlag = true;
						}
						else
						{
							ennergy.count = 0;
						}
					}
				}
			}
			if (enemy2.hostIsAlive)
			{
				for (int j = 0; j < 4; j++)
				{
					if (enemy2.parentIsAlive[j])
					{
						for (int i = 0; i < kEnemy2Num; i++)
						{
							if (enemy2.dedTimer[j][i] == kDedTimer && EllipseCollision(player.pos, player.radius.x, enemy2.relativePos[j][i], enemy2.radius))
							{
								if (ennergy.count > 0)
								{
									ennergy.count -= ennergy.damage;
									ennergy.damageFlag = true;
								}
								else
								{
									ennergy.count = 0;
								}
							}
						}
					}
				}
			}

			if (enemy3.parentIsAlive)
			{
				for (int i = 0; i < 4; i++)
				{
					if (enemy3.dedTimer[i] == kDedTimer && EllipseCollision(player.pos, player.radius.x, enemy3.relativePos[i], enemy3.radius))
					{
						if (ennergy.count > 0)
						{
							ennergy.count -= ennergy.damage;
							ennergy.damageFlag = true;
						}
						else
						{
							ennergy.count = 0;
						}
					}
				}
			}
			if (enemy4.parentIsAlive)
			{
				for (int i = 0; i < 4; i++)
				{
					if (enemy4.dedTimer[i] == kDedTimer && EllipseCollision(player.pos, player.radius.x, enemy4.relativePos[i], enemy4.radius))
					{
						if (ennergy.count > 0)
						{
							ennergy.count -= ennergy.damage;
							ennergy.damageFlag = true;
						}
						else
						{
							ennergy.count = 0;
						}
					}
				}
			}
			if (enemy5.parentIsAlive)
			{
				for (int i = 0; i < 4; i++)
				{
					if (enemy5.dedTimer[i] == kDedTimer && EllipseCollision(player.pos, player.radius.x, enemy5.relativePos[i], enemy5.radius))
					{
						if (ennergy.count > 0)
						{
							ennergy.count -= ennergy.damage;
							ennergy.damageFlag = true;
						}
						else
						{
							ennergy.count = 0;
						}
					}
				}
			}
			if (enemy6.parentIsAlive)
			{
				for (int i = 0; i < 4; i++)
				{
					if (enemy6.dedTimer[i] == kDedTimer && EllipseCollision(player.pos, player.radius.x, enemy6.relativePos[i], enemy6.radius))
					{
						if (ennergy.count > 0)
						{
							ennergy.count -= ennergy.damage;
							ennergy.damageFlag = true;
						}
						else
						{
							ennergy.count = 0;
						}
					}
				}
			}
			if (enemy7.hostIsAlive)
			{
				for (int j = 0; j < 8; j++)
				{
					if (enemy7.parentIsAlive[j])
					{
						for (int i = 0; i < 4; i++)
						{
							if (enemy7.dedTimer[j][i] == kDedTimer && EllipseCollision(player.pos, player.radius.x, enemy7.relativePos[j][i], enemy7.radius))
							{
								if (ennergy.count > 0)
								{
									ennergy.count -= ennergy.damage;
									ennergy.damageFlag = true;
								}
								else
								{
									ennergy.count = 0;
								}
							}
						}
					}
				}
			}
			if (enemy8.hostIsAlive)
			{
				for (int j = 0; j < 4; j++)
				{
					if (enemy8.parentIsAlive[j])
					{
						for (int i = 0; i < 12; i++)
						{
							if (enemy8.dedTimer[j][i] == kDedTimer && EllipseCollision(player.pos, player.radius.x, enemy8.relativePos[j][i], enemy8.radius))
							{
								if (ennergy.count > 0)
								{
									ennergy.count -= ennergy.damage;
									ennergy.damageFlag = true;
								}
								else
								{
									ennergy.count = 0;
								}
							}
						}
					}
				}
			}
			if (enemy9.hostIsAlive)
			{
				for (int j = 0; j < 2; j++)
				{
					if (enemy9.parentIsAlive[j])
					{
						for (int i = 0; i < 8; i++)
						{
							if (enemy9.dedTimer[j][i] == kDedTimer && EllipseCollision(player.pos, player.radius.x, enemy9.relativePos[j][i], enemy9.radius))
							{
								if (ennergy.count > 0)
								{
									ennergy.count -= ennergy.damage;
									ennergy.damageFlag = true;
								}
								else
								{
									ennergy.count = 0;
								}
							}
						}
					}
				}
			}
			for (int j = 0; j < kenemy10Num; j++)
			{
				if (enemy10[j].parentIsAlive)
				{
					for (int i = 0; i < 4; i++)
					{
						if (enemy10[j].dedTimer[i] == kDedTimer && EllipseCollision(player.pos, player.radius.x, enemy10[j].relativePos[i], enemy10[j].radius))
						{
							if (ennergy.count > 0)
							{
								ennergy.count -= ennergy.damage;
								ennergy.damageFlag = true;
							}
							else
							{
								ennergy.count = 0;
							}
						}
					}
				}
			}
			for (int j = 0; j < kenemy11Num; j++)
			{
				if (enemy11[j].parentIsAlive)
				{
					for (int i = 0; i < 3; i++)
					{
						if (enemy11[j].dedTimer[i] == kDedTimer && EllipseCollision(player.pos, player.radius.x, enemy11[j].relativePos[i], enemy11[j].radius))
						{
							if (ennergy.count > 0)
							{
								ennergy.count -= ennergy.damage;
								ennergy.damageFlag = true;
							}
							else
							{
								ennergy.count = 0;
							}
						}
					}
				}
			}
			for (int j = 0; j < kenemy12Num; j++)
			{
				if (enemy12[j].parentIsAlive)
				{
					for (int i = 0; i < 4; i++)
					{
						if (enemy12[j].dedTimer[i] == kDedTimer && EllipseCollision(player.pos, player.radius.x, enemy12[j].relativePos[i], enemy12[j].radius))
						{
							if (ennergy.count > 0)
							{
								ennergy.count -= ennergy.damage;
								ennergy.damageFlag = true;
							}
							else
							{
								ennergy.count = 0;
							}
						}
					}
				}
			}
		}

#pragma endregion
		//敵のダメージ処理
#pragma region enemyDamage
		if (enemy1.parentIsAlive)
		{
			for (int i = 0; i < 3; i++)
			{
				if (enemy1.isDed[i])
				{
					if (enemy1.dedTimer[i] == kDedTimer)
					{
						count++;
						if (player.dashAttack)dash.count++;
						if (player.triangulAttack)triangle.count++;
					}
					if (enemy1.dedTimer[i] > 0) enemy1.dedTimer[i]--;
					if (enemy1.dedTimer[i] <= 0)enemy1.childIsAlive[i] = false;
				}
				if (enemy1.dedTimer[0] <= 0
					&& enemy1.dedTimer[1] <= 0
					&& enemy1.dedTimer[2] <= 0)
				{
					enemy1.parentIsAlive = false;
				}
			}
		}
		if (enemy2.hostIsAlive)
		{
			for (int j = 0; j < 4; j++)
			{
				if (enemy2.parentIsAlive[j])
				{
					for (int i = 0; i < kEnemy2Num; i++)
					{
						if (enemy2.isDed[j][i])
						{
							if (enemy2.dedTimer[j][i] == kDedTimer)
							{
								count++;
								if (player.dashAttack)dash.count++;
								if (player.triangulAttack)triangle.count++;
							}
							if (enemy2.dedTimer[j][i] > 0)enemy2.dedTimer[j][i]--;
							if (enemy2.dedTimer[j][i] <= 0)enemy2.childIsAlive[j][i] = false;
						}
						if (enemy2.dedTimer[j][0] <= 0 && enemy2.dedTimer[j][1] <= 0 && enemy2.dedTimer[j][2] <= 0 && enemy2.dedTimer[j][3] <= 0
							&& enemy2.dedTimer[j][4] <= 0 && enemy2.dedTimer[j][5] <= 0 && enemy2.dedTimer[j][6] <= 0 && enemy2.dedTimer[j][7] <= 0
							&& enemy2.dedTimer[j][8] <= 0 && enemy2.dedTimer[j][9] <= 0 && enemy2.dedTimer[j][10] <= 0 && enemy2.dedTimer[j][11] <= 0
							&& enemy2.dedTimer[j][12] <= 0 && enemy2.dedTimer[j][13] <= 0 && enemy2.dedTimer[j][14] <= 0 && enemy2.dedTimer[j][15] <= 0
							)
						{
							enemy2.parentIsAlive[j] = false;
						}
						if (enemy2.parentIsAlive[0] == false && enemy2.parentIsAlive[1] == false && enemy2.parentIsAlive[2] == false && enemy2.parentIsAlive[3] == false)
						{
							enemy2.hostIsAlive = false;
						}
					}
				}
			}
		}

		if (enemy3.parentIsAlive)
		{
			for (int i = 0; i < 4; i++)
			{
				if (enemy3.isDed[i])
				{
					if (enemy3.dedTimer[i] == kDedTimer)
					{
						count++;
						if (player.dashAttack)dash.count++;
						if (player.triangulAttack)triangle.count++;
					}
					if (enemy3.dedTimer[i] > 0)enemy3.dedTimer[i]--;
					if (enemy3.dedTimer[i] <= 0)enemy3.childIsAlive[i] = false;
				}
				if (enemy3.dedTimer[0] <= 0 && enemy3.dedTimer[1] <= 0 && enemy3.dedTimer[2] <= 0 && enemy3.dedTimer[3] <= 0)
				{
					enemy3.parentIsAlive = false;
				}
			}
		}
		if (enemy4.parentIsAlive)
		{
			for (int i = 0; i < 4; i++)
			{
				if (enemy4.isDed[i])
				{
					if (enemy4.dedTimer[i] == kDedTimer)
					{
						count++;
						if (player.dashAttack)dash.count++;
						if (player.triangulAttack)triangle.count++;
					}
					if (enemy4.dedTimer[i] > 0)enemy4.dedTimer[i]--;
					if (enemy4.dedTimer[i] <= 0)enemy4.childIsAlive[i] = false;
				}
				if (enemy4.dedTimer[0] <= 0 && enemy4.dedTimer[1] <= 0 && enemy4.dedTimer[2] <= 0 && enemy4.dedTimer[3] <= 0)
				{
					enemy4.parentIsAlive = false;
				}
			}
		}
		if (enemy5.parentIsAlive)
		{
			for (int i = 0; i < 4; i++)
			{
				if (enemy5.isDed[i])
				{
					if (enemy5.dedTimer[i] == kDedTimer)
					{
						count++;
						if (player.dashAttack)dash.count++;
						if (player.triangulAttack)triangle.count++;
					}
					if (enemy5.dedTimer[i] > 0)enemy5.dedTimer[i]--;
					if (enemy5.dedTimer[i] <= 0)enemy5.childIsAlive[i] = false;
				}
				if (enemy5.dedTimer[0] <= 0 && enemy5.dedTimer[1] <= 0 && enemy5.dedTimer[2] <= 0 && enemy5.dedTimer[3] <= 0)
				{
					enemy5.parentIsAlive = false;
				}
			}
		}
		if (enemy6.parentIsAlive)
		{
			for (int i = 0; i < 4; i++)
			{
				if (enemy6.isDed[i])
				{
					if (enemy6.dedTimer[i] == kDedTimer)
					{
						count++;
						if (player.dashAttack)dash.count++;
						if (player.triangulAttack)triangle.count++;
					}
					if (enemy6.dedTimer[i] > 0)enemy6.dedTimer[i]--;
					if (enemy6.dedTimer[i] <= 0)enemy6.childIsAlive[i] = false;
				}
				if (enemy6.dedTimer[0] <= 0 && enemy6.dedTimer[1] <= 0 && enemy6.dedTimer[2] <= 0 && enemy6.dedTimer[3] <= 0)
				{
					enemy6.parentIsAlive = false;
				}
			}
		}
		if (enemy7.hostIsAlive)
		{
			for (int j = 0; j < 8; j++)
			{
				if (enemy7.parentIsAlive[j])
				{
					for (int i = 0; i < 4; i++)
					{
						if (enemy7.isDed[j][i])
						{
							if (enemy7.dedTimer[j][i] == kDedTimer)
							{
								count++;
								if (player.dashAttack)dash.count++;
								if (player.triangulAttack)triangle.count++;
							}
							if (enemy7.dedTimer[j][i] > 0)enemy7.dedTimer[j][i]--;
							if (enemy7.dedTimer[j][i] <= 0)enemy7.childIsAlive[j][i] = false;
						}
						if (enemy7.dedTimer[j][0] <= 0 && enemy7.dedTimer[j][1] <= 0 && enemy7.dedTimer[j][2] <= 0 && enemy7.dedTimer[j][3] <= 0)
						{
							enemy7.parentIsAlive[j] = false;
						}
						if (enemy7.parentIsAlive[0] == false && enemy7.parentIsAlive[1] == false && enemy7.parentIsAlive[2] == false && enemy7.parentIsAlive[3] == false
							&& enemy7.parentIsAlive[4] == false && enemy7.parentIsAlive[5] == false && enemy7.parentIsAlive[6] == false && enemy7.parentIsAlive[7] == false)
						{
							enemy7.hostIsAlive = false;
						}
					}
				}
			}
		}
		if (enemy8.hostIsAlive)
		{
			for (int j = 0; j < 4; j++)
			{
				if (enemy8.parentIsAlive[j])
				{
					for (int i = 0; i < 12; i++)
					{
						if (enemy8.isDed[j][i])
						{
							if (enemy8.dedTimer[j][i] == kDedTimer)
							{
								count++;
								if (player.dashAttack)dash.count++;
								if (player.triangulAttack)triangle.count++;
							}
							if (enemy8.dedTimer[j][i] > 0)enemy8.dedTimer[j][i]--;
							if (enemy8.dedTimer[j][i] <= 0)enemy8.childIsAlive[j][i] = false;
						}
						if (enemy8.dedTimer[j][0] <= 0 && enemy8.dedTimer[j][1] <= 0 && enemy8.dedTimer[j][2] <= 0 && enemy8.dedTimer[j][3] <= 0
							&& enemy8.dedTimer[j][4] <= 0 && enemy8.dedTimer[j][5] <= 0 && enemy8.dedTimer[j][6] <= 0 && enemy8.dedTimer[j][7] <= 0
							&& enemy8.dedTimer[j][8] <= 0 && enemy8.dedTimer[j][9] <= 0 && enemy8.dedTimer[j][10] <= 0 && enemy8.dedTimer[j][11] <= 0)
						{
							enemy8.parentIsAlive[j] = false;
						}
						if (enemy8.parentIsAlive[0] == false && enemy8.parentIsAlive[1] == false && enemy8.parentIsAlive[2] == false && enemy8.parentIsAlive[3] == false)
						{
							enemy8.hostIsAlive = false;
						}
					}
				}
			}
		}
		if (enemy9.hostIsAlive)
		{
			for (int j = 0; j < 2; j++)
			{
				if (enemy9.parentIsAlive[j])
				{
					for (int i = 0; i < 8; i++)
					{
						if (enemy9.isDed[j][i])
						{
							if (enemy9.dedTimer[j][i] == kDedTimer)
							{
								count++;
								if (player.dashAttack)dash.count++;
								if (player.triangulAttack)triangle.count++;
							}
							if (enemy9.dedTimer[j][i] > 0)enemy9.dedTimer[j][i]--;
							if (enemy9.dedTimer[j][i] <= 0)enemy9.childIsAlive[j][i] = false;
						}
						if (enemy9.dedTimer[j][0] <= 0 && enemy9.dedTimer[j][1] <= 0 && enemy9.dedTimer[j][2] <= 0 && enemy9.dedTimer[j][3] <= 0
							&& enemy9.dedTimer[j][4] <= 0 && enemy9.dedTimer[j][5] <= 0 && enemy9.dedTimer[j][6] <= 0 && enemy9.dedTimer[j][7] <= 0)
						{
							enemy9.parentIsAlive[j] = false;
						}
						if (enemy9.parentIsAlive[0] == false && enemy9.parentIsAlive[1] == false)
						{
							enemy9.hostIsAlive = false;
						}
					}
				}
			}
		}
		for (int j = 0; j < kenemy10Num; j++)
		{
			if (enemy10[j].parentIsAlive)
			{
				for (int i = 0; i < 4; i++)
				{
					if (enemy10[j].isDed[i])
					{
						if (enemy10[j].dedTimer[i] == kDedTimer)
						{
							count++;
							if (player.dashAttack)dash.count++;
							if (player.triangulAttack)triangle.count++;
						}
						if (enemy10[j].dedTimer[i] > 0)enemy10[j].dedTimer[i]--;
						if (enemy10[j].dedTimer[i] <= 0)enemy10[j].childIsAlive[i] = false;
					}
					if (enemy10[j].dedTimer[0] <= 0 && enemy10[j].dedTimer[1] <= 0 && enemy10[j].dedTimer[2] <= 0 && enemy10[j].dedTimer[3] <= 0)
					{
						enemy10[j].parentIsAlive = false;
					}
				}
			}
		}
		for (int j = 0; j < kenemy11Num; j++)
		{
			if (enemy11[j].parentIsAlive)
			{
				for (int i = 0; i < 3; i++)
				{
					if (enemy11[j].isDed[i])
					{
						if (enemy11[j].dedTimer[i] == kDedTimer)
						{
							count++;
							if (player.dashAttack)dash.count++;
							if (player.triangulAttack)triangle.count++;
						}
						if (enemy11[j].dedTimer[i] > 0)enemy11[j].dedTimer[i]--;
						if (enemy11[j].dedTimer[i] <= 0)enemy11[j].childIsAlive[i] = false;
					}
					if (enemy11[j].dedTimer[0] <= 0 && enemy11[j].dedTimer[1] <= 0 && enemy11[j].dedTimer[2] <= 0 )
					{
						enemy11[j].parentIsAlive = false;
					}
				}
			}
		}
		for (int j = 0; j < kenemy12Num; j++)
		{
			if (enemy12[j].parentIsAlive)
			{
				for (int i = 0; i < 4; i++)
				{
					if (enemy12[j].isDed[i])
					{
						if (enemy12[j].dedTimer[i] == kDedTimer)
						{
							count++;
							if (player.dashAttack)dash.count++;
							if (player.triangulAttack)triangle.count++;
						}
						if (enemy12[j].dedTimer[i] > 0)enemy12[j].dedTimer[i]--;
						if (enemy12[j].dedTimer[i] <= 0)enemy12[j].childIsAlive[i] = false;
					}
					if (enemy12[j].dedTimer[0] <= 0 && enemy12[j].dedTimer[1] <= 0 && enemy12[j].dedTimer[2] <= 0 && enemy12[j].dedTimer[3] <= 0)
					{
						enemy12[j].parentIsAlive = false;
					}
				}
			}
		}
#pragma endregion
		//フィールドの外に出ないように


		//ダメージのシェイク
		if (ennergy.damageFlag)
		{
			if (shakeGaugeSeed % 1 == 0)
			{
				shakeGaugeX--;
				shakeGaugeY--;
			}
			shakeGaugePos = randShake(shakeGaugeX, shakeGaugeY, shakeGaugeSeed);

			shakeGaugeSeed++;
			if (shakeGaugeX <= 0)
			{
				shakeGaugePos = { 0,0 };
				shakeGaugeSeed = 1;
				shakeGaugeX = shakeRadius;
				shakeGaugeY = shakeRadius;
				ennergy.damageFlag = false;
			}
		}

		//ここはいじらなくてOK
		if (player.pos.x != 0 || player.pos.y != 0) {
			fieldToPlayer.x = (fieldRadius - player.radius.x) * vectorNormalize(player.pos, { 0,0 }).x;
			fieldToPlayer.y = (fieldRadius - player.radius.y) * vectorNormalize(player.pos, { 0,0 }).y;

			centerToPlayerLength = floatLength(player.pos, { 0,0 });
		}
		if (centerToPlayerLength >= fieldRadius - player.radius.x)
		{
			player.pos.x = fieldToPlayer.x;
			player.pos.y = fieldToPlayer.y;
		}
		//ここまでいじらなくてOK

		//スクロールの値を代入
		scroll.x = (-player.pos.x / screenSize + 960);
		scroll.y = (-player.pos.y / screenSize + 540);



		///                                                            ///
		/// --------------------↑更新処理ここまで-------------------- ///
		///                                                            ///

		///                                                            ///
		/// --------------------↓描画処理ここから-------------------- ///
		///                                                            ///


		Novice::ScreenPrintf(0, 200, "joystickX=%d joystickY=%d", joystickX, joystickY);
		Novice::ScreenPrintf(0, 220, "Player.joystickX=%f Player.joystickY=%f", player.joystick.x, player.joystick.y);
		Novice::ScreenPrintf(0, 240, "Player.direction.x=%f Player.direction.y=%f", player.direction.x, player.direction.y);
		Novice::ScreenPrintf(0, 260, "player.vel.x=%f player.vel.y=%f", player.velocity.x, player.velocity.y);
		Novice::ScreenPrintf(0, 280, "player.TrigerA=%d", player.trigerA);
		Novice::ScreenPrintf(0, 300, "player.velocityRatio=%f", player.velocityRatio);
		Novice::ScreenPrintf(0, 320, "player.count=%d", player.count);
		Novice::ScreenPrintf(0, 340, "World.X=%f World.Y=%f", player.pos.x, player.pos.y);
		Novice::ScreenPrintf(0, 360, "player.playTime=%d player.flick=%d", player.flickTimer, player.flick);
		Novice::ScreenPrintf(0, 380, "player.flickLength=%f", player.flickLength);
		Novice::ScreenPrintf(0, 400, "length=%f", length);
		Novice::ScreenPrintf(0, 420, "player.flickCT=%d", player.flickCT);
		Novice::ScreenPrintf(0, 440, "count=%f endCount=%f", count, endCount);
		Novice::ScreenPrintf(0, 460, " dash.endcount=%f dash.endCount=%f", dash.count, dash.endCount);
		Novice::ScreenPrintf(0, 480, "trian.endcount=%f tria.endCount=%f", triangle.count, triangle.endCount);
		Novice::ScreenPrintf(0, 500, "ennergy.count=%f", ennergy.count);
		Novice::ScreenPrintf(0, 520, "screenSize=%f", screenSize);
		Novice::ScreenPrintf(0, 540, "gameTimer=%d seconds=%d", gameTimer, gameTimer / 60);


		//一番後ろの背景
		Novice::DrawBox(0, 0, 1920, 1080, 0, 0x222222ff, kFillModeSolid);
		//フィールドの画像表示
		Functions.DrawQuadPlus(int(scroll.x), int(scroll.y), int(fieldRadius * 2 / screenSize), int(fieldRadius * 2 / screenSize), 1.0f, 1.0f, 0, 0, 0, 4000, 4000, fieldTexture, 0xffffffff);
		//三角形を作った時
		if (player.aim)Novice::DrawBox(0, 0, 1920, 1080, 0, 0x88888844, kFillModeSolid);
		//三角形で攻撃時間
		if (player.aimTimer > 0)Novice::DrawBox(0, 0, 1920, 1080, 0, 0xaa888844, kFillModeSolid);
		if (player.aimTimer > 0)Novice::DrawTriangle(int(player.prepos[0].x / screenSize + scroll.x), int(player.prepos[0].y / screenSize + scroll.y), int(player.prepos[1].x / screenSize + scroll.x), int(player.prepos[1].y / screenSize + scroll.y), int(player.prepos[2].x / screenSize + scroll.x), int(player.prepos[2].y / screenSize + scroll.y), RED, kFillModeSolid);

		//デバッグ用
		Novice::DrawEllipse(int(100 + player.joystick.x), int(100 + player.joystick.y), 10, 10, 0, GREEN, kFillModeSolid);//直接入力
		Novice::DrawEllipse(int(100 + player.direction.x * 100), int(100 + player.direction.y * 100), 10, 10, 0, RED, kFillModeSolid);//方向
		Novice::DrawEllipse(int(100 + player.velocity.x), int(100 + player.velocity.y), 10, 10, 0, BLUE, kFillModeSolid);//速度
		Novice::DrawEllipse(100, 100, 100, 100, 0, WHITE, kFillModeWireFrame);//最大範囲
		Novice::DrawEllipse(100, 100, (int)dedZone, (int)dedZone, 0, GREEN, kFillModeWireFrame);//デッドゾーン

		//フィールドの円周
		Novice::DrawEllipse(int(scroll.x), int(scroll.y), (int)(fieldRadius / screenSize), (int)(fieldRadius / screenSize), 0, GREEN, kFillModeWireFrame);

		//三角形の点
		for (int i = 0; i < 3; i++)
		{
			if (player.prepos[i].x != 0)Novice::DrawEllipse(int(player.prepos[i].x / screenSize + scroll.x), int(player.prepos[i].y / screenSize + scroll.y), int(player.anchorRadius / screenSize), int(player.anchorRadius / screenSize), 0, RED, kFillModeSolid);
		}
		//スポーン地点
		Novice::DrawBox(int(-50 / screenSize + scroll.x), int(-50 / screenSize + scroll.y), 100, 100, 0, RED, kFillModeWireFrame);
		//プレイヤーの方向表示
		Novice::DrawLine(int(player.pos.x / screenSize + scroll.x), int(player.pos.y / screenSize + scroll.y), int(player.pos.x / screenSize + scroll.x + player.direction.x / screenSize * 150), int(player.pos.y / screenSize + scroll.y + player.direction.y / screenSize * 150), WHITE);
		//プレイヤー
		Novice::DrawEllipse(int(player.pos.x / screenSize + scroll.x), int(player.pos.y / screenSize + scroll.y), int(player.radius.x / screenSize), int(player.radius.y / screenSize), 0, GREEN, kFillModeSolid);
		//プレイヤーフリック時
		if (player.flick)Novice::DrawEllipse(int(player.pos.x / screenSize + scroll.x), int(player.pos.y / screenSize + scroll.y), int(player.radius.x / screenSize), int(player.radius.y / screenSize), 0, 0x00ffffff, kFillModeSolid);

		//フィーバーゲージ仮
		Novice::DrawBox(1400 + int(shakeGaugePos.x), 950 + int(shakeGaugePos.y), 450, 100, 0, 0x333333ff, kFillModeSolid);//j仮ゲージ
		Novice::DrawBox(1400 + int(shakeGaugePos.x), 950 + int(shakeGaugePos.y), int(450 * (ennergy.count / ennergy.max)), 100, 0, 0xffff00ff, kFillModeSolid);



		//ミニマップ
		Novice::DrawEllipse(int(50 + fieldRadius / miniMap), int(1020 - fieldRadius / miniMap), int(fieldRadius / miniMap), int(fieldRadius / miniMap), 0, GREEN, kFillModeWireFrame);
		for (int i = 0; i < 3; i++)
		{//ミニマップ三角形の点
			if (player.prepos[i].x != 0)Novice::DrawEllipse(int((50 + fieldRadius / miniMap) + player.prepos[i].x / miniMap), int(int(1020 - fieldRadius / miniMap) + player.prepos[i].y / miniMap), int(player.anchorRadius * miniMapPlayerSize / miniMap), int(player.anchorRadius * miniMapPlayerSize / miniMap), 0, RED, kFillModeSolid);
		}
		//ミニマップ範囲
		if (player.aimTimer > 0)Novice::DrawTriangle(int((50 + fieldRadius / miniMap) + player.prepos[0].x / miniMap), int(int(1020 - fieldRadius / miniMap) + player.prepos[0].y / miniMap), int((50 + fieldRadius / miniMap) + player.prepos[1].x / miniMap), int(int(1020 - fieldRadius / miniMap) + player.prepos[1].y / miniMap), int((50 + fieldRadius / miniMap) + player.prepos[2].x / miniMap), int(int(1020 - fieldRadius / miniMap) + player.prepos[2].y / miniMap), RED, kFillModeSolid);
		//ミニマッププレイヤー
		Novice::DrawEllipse(int((50 + fieldRadius / miniMap) + player.pos.x / miniMap), int(int(1020 - fieldRadius / miniMap) + player.pos.y / miniMap), int(player.radius.x * miniMapPlayerSize / miniMap), int(player.radius.y * miniMapPlayerSize / miniMap), 0, GREEN, kFillModeSolid);

		//ミニマップシシカク
		if (enemy2.hostIsAlive)
		{
			for (int j = 0; j < 4; j++)
			{
				for (int i = 0; i < kEnemy2Num; i++)
				{
					if (enemy2.childIsAlive[j][i])
					{
						if (!enemy2.isDed[j][i])Novice::DrawEllipse(int((50 + fieldRadius / miniMap) + enemy2.relativePos[j][i].x / miniMap), int(int(1020 - fieldRadius / miniMap) + enemy2.relativePos[j][i].y / miniMap), int(enemy2.radius * miniMapPlayerSize / miniMap), int(enemy2.radius * miniMapPlayerSize / miniMap), 0, 0xff00ffff, kFillModeSolid);
						if (enemy2.isDed[j][i] && enemy2.dedTimer[j][i] > 0)Novice::DrawEllipse(int((50 + fieldRadius / miniMap) + enemy2.relativePos[j][i].x / miniMap), int(int(1020 - fieldRadius / miniMap) + enemy2.relativePos[j][i].y / miniMap), int(enemy2.radius * miniMapPlayerSize / miniMap), int(enemy2.radius * miniMapPlayerSize / miniMap), 0, BLUE, kFillModeSolid);
					}
				}
			}
		}
		//ミニマップ８方向エネミー
		if (enemy7.hostIsAlive)
		{
			for (int j = 0; j < 8; j++)
			{
				for (int i = 0; i < 4; i++)
				{
					if (enemy7.childIsAlive[j][i])
					{
						if (!enemy7.isDed[j][i])Novice::DrawEllipse(int((50 + fieldRadius / miniMap) + enemy7.relativePos[j][i].x / miniMap), int(int(1020 - fieldRadius / miniMap) + enemy7.relativePos[j][i].y / miniMap), int(enemy7.radius * miniMapPlayerSize / miniMap), int(enemy7.radius * miniMapPlayerSize / miniMap), 0, 0xff00ffff, kFillModeSolid);
						if (enemy7.isDed[j][i] && enemy7.dedTimer[j][i] > 0)Novice::DrawEllipse(int((50 + fieldRadius / miniMap) + enemy7.relativePos[j][i].x / miniMap), int(int(1020 - fieldRadius / miniMap) + enemy7.relativePos[j][i].y / miniMap), int(enemy7.radius * miniMapPlayerSize / miniMap), int(enemy7.radius * miniMapPlayerSize / miniMap), 0, BLUE, kFillModeSolid);
					}
				}
			}
		}
		//ミニマップジグザグ
		if (enemy8.hostIsAlive)
		{
			for (int j = 0; j < 4; j++)
			{
				for (int i = 0; i < 12; i++)
				{
					if (enemy8.childIsAlive[j][i])
					{
						if (!enemy8.isDed[j][i])Novice::DrawEllipse(int((50 + fieldRadius / miniMap) + enemy8.relativePos[j][i].x / miniMap), int(int(1020 - fieldRadius / miniMap) + enemy8.relativePos[j][i].y / miniMap), int(enemy8.radius * miniMapPlayerSize / miniMap), int(enemy8.radius * miniMapPlayerSize / miniMap), 0, 0xff00ffff, kFillModeSolid);
						if (enemy8.isDed[j][i] && enemy8.dedTimer[j][i] > 0)Novice::DrawEllipse(int((50 + fieldRadius / miniMap) + enemy8.relativePos[j][i].x / miniMap), int(int(1020 - fieldRadius / miniMap) + enemy8.relativePos[j][i].y / miniMap), int(enemy8.radius * miniMapPlayerSize / miniMap), int(enemy8.radius * miniMapPlayerSize / miniMap), 0, BLUE, kFillModeSolid);

					}
				}
			}
		}
		//ミニマップセンターゴー
		if (enemy9.hostIsAlive)
		{
			for (int j = 0; j < 2; j++)
			{
				for (int i = 0; i < 8; i++)
				{
					if (enemy9.childIsAlive[j][i])
					{
						if (!enemy9.isDed[j][i])Novice::DrawEllipse(int((50 + fieldRadius / miniMap) + enemy9.relativePos[j][i].x / miniMap), int(int(1020 - fieldRadius / miniMap) + enemy9.relativePos[j][i].y / miniMap), int(enemy9.radius * miniMapPlayerSize / miniMap), int(enemy9.radius * miniMapPlayerSize / miniMap), 0, 0xff00ffff, kFillModeSolid);
						if (enemy9.isDed[j][i] && enemy9.dedTimer[j][i] > 0)Novice::DrawEllipse(int((50 + fieldRadius / miniMap) + enemy9.relativePos[j][i].x / miniMap), int(int(1020 - fieldRadius / miniMap) + enemy9.relativePos[j][i].y / miniMap), int(enemy9.radius * miniMapPlayerSize / miniMap), int(enemy9.radius * miniMapPlayerSize / miniMap), 0, BLUE, kFillModeSolid);

					}
				}
			}
		}

		//ミニマップランダム横一列
		for (int j = 0; j < kenemy10Num; j++)
		{
			if (enemy10[j].parentIsAlive)
			{
				for (int i = 0; i < 4; i++)
				{
					if (enemy10[j].childIsAlive[i])
					{
						if (!enemy10[j].isDed[i])Functions.DrawQuadPlus(int((50 + fieldRadius / miniMap) + enemy10[j].relativePos[i].x / miniMap), int((1020 - fieldRadius / miniMap) + enemy10[j].relativePos[i].y / miniMap), int(enemy10[j].radius * miniMapPlayerSize / miniMap), int(enemy10[j].radius * miniMapPlayerSize / miniMap), 1, 1, 0, 0, 0, 100, 100, enemyTexture, 0xffffffff);
						if (enemy10[j].isDed[i] && enemy10[j].dedTimer[i] > 0)Functions.DrawQuadPlus(int((50 + fieldRadius / miniMap) + enemy10[j].relativePos[i].x / miniMap), int((1020 - fieldRadius / miniMap) + enemy10[j].relativePos[i].y / miniMap), int(enemy10[j].radius * miniMapPlayerSize / miniMap), int(enemy10[j].radius * miniMapPlayerSize / miniMap), 1, 1, 0, 0, 0, 100, 100, enemyTexture, BLUE);
					}
					//Novice::ScreenPrintf(int(enemy10[j].relativePos[i].x + scroll.x), int(enemy10[j].relativePos[i].y + scroll.y), "enemy10[j].isDed[%d]=%d", i, enemy10[j].isDed[i]);
					//Novice::ScreenPrintf(int(enemy10[j].relativePos[i].x + scroll.x), int(enemy10[j].relativePos[i].y + scroll.y + 20), "enemy10[j].dedTimer[%d]=%d", i, enemy10[j].dedTimer[i]);
				}
			}
		}
		//ミニマップランダム三角形
		for (int j = 0; j < kenemy11Num; j++)
		{
			if (enemy11[j].parentIsAlive)
			{
				for (int i = 0; i < 3; i++)
				{
					if (enemy11[j].childIsAlive[i])
					{
						if (!enemy11[j].isDed[i])Functions.DrawQuadPlus(int((50 + fieldRadius / miniMap) + enemy11[j].relativePos[i].x / miniMap), int((1020 - fieldRadius / miniMap) + enemy11[j].relativePos[i].y / miniMap), int(enemy11[j].radius * miniMapPlayerSize / miniMap), int(enemy11[j].radius * miniMapPlayerSize / miniMap), 1, 1, 0, 0, 0, 100, 100, enemyTexture, 0xffffffff);
						if (enemy11[j].isDed[i] && enemy11[j].dedTimer[i] > 0)Functions.DrawQuadPlus(int((50 + fieldRadius / miniMap) + enemy11[j].relativePos[i].x / miniMap), int((1020 - fieldRadius / miniMap) + enemy11[j].relativePos[i].y / miniMap), int(enemy11[j].radius * miniMapPlayerSize / miniMap), int(enemy11[j].radius * miniMapPlayerSize / miniMap), 1, 1, 0, 0, 0, 100, 100, enemyTexture, BLUE);
					}
					//Novice::ScreenPrintf(int(enemy11[j].relativePos[i].x + scroll.x), int(enemy11[j].relativePos[i].y + scroll.y), "enemy11[j].isDed[%d]=%d", i, enemy11[j].isDed[i]);
					//Novice::ScreenPrintf(int(enemy11[j].relativePos[i].x + scroll.x), int(enemy11[j].relativePos[i].y + scroll.y + 20), "enemy11[j].dedTimer[%d]=%d", i, enemy11[j].dedTimer[i]);
				}
			}
		}
		//ミニマップランダム縦一列
		for (int j = 0; j < kenemy12Num; j++)
		{
			if (enemy12[j].parentIsAlive)
			{
				for (int i = 0; i < 4; i++)
				{
					if (enemy12[j].childIsAlive[i])
					{
						if (!enemy12[j].isDed[i])Functions.DrawQuadPlus(int((50 + fieldRadius / miniMap) + enemy12[j].relativePos[i].x / miniMap), int((1020 - fieldRadius / miniMap) + enemy12[j].relativePos[i].y / miniMap), int(enemy12[j].radius * miniMapPlayerSize / miniMap), int(enemy12[j].radius * miniMapPlayerSize / miniMap), 1, 1, 0, 0, 0, 100, 100, enemyTexture, 0xffffffff);
						if (enemy12[j].isDed[i] && enemy12[j].dedTimer[i] > 0)Functions.DrawQuadPlus(int((50 + fieldRadius / miniMap) + enemy12[j].relativePos[i].x / miniMap), int((1020 - fieldRadius / miniMap) + enemy12[j].relativePos[i].y / miniMap), int(enemy12[j].radius * miniMapPlayerSize / miniMap), int(enemy12[j].radius * miniMapPlayerSize / miniMap), 1, 1, 0, 0, 0, 100, 100, enemyTexture, BLUE);
					}
					//Novice::ScreenPrintf(int(enemy10[j].relativePos[i].x + scroll.x), int(enemy10[j].relativePos[i].y + scroll.y), "enemy10[j].isDed[%d]=%d", i, enemy10[j].isDed[i]);
					//Novice::ScreenPrintf(int(enemy10[j].relativePos[i].x + scroll.x), int(enemy10[j].relativePos[i].y + scroll.y + 20), "enemy10[j].dedTimer[%d]=%d", i, enemy10[j].dedTimer[i]);
				}
			}
		}
		//攻撃有効時間を表示
		if (player.triangulAttack)
		{
			Novice::DrawBox(1820, 0, 100, 100, 0, RED, kFillModeSolid);
		}
		if (player.dashAttack)
		{
			Novice::DrawBox(1720, 0, 100, 100, 0, BLUE, kFillModeSolid);
		}

		if (!isEnemyDead)
		{
			Functions.DrawQuadPlus(
				static_cast<int>(enemyPos.x + scroll.x), static_cast<int>(enemyPos.y + scroll.y),
				30, 30,
				1.0f, 1.0f,
				0.0f,
				0, 0, 1, 1,
				kWhiteTexture,
				0xffff00ff
			);
		}


		//仮敵描画
		//Novice::ScreenPrintf(int(enemy1.centorPos.x + scroll.x), int(enemy1.centorPos.y + scroll.y), "enemy1.parentIsAlive=%d", enemy1.parentIsAlive);
		for (int i = 0; i < 3; i++)
		{
			if (enemy1.childIsAlive[i])
			{
				if (!enemy1.isDed[i])Functions.DrawQuadPlus(int(enemy1.relativePos[i].x + scroll.x), int(enemy1.relativePos[i].y + scroll.y), int(enemy1.radius * 2), int(enemy1.radius * 2), 1, 1, 0, 0, 0, 100, 100, enemyTexture, 0xffffffff);

				if (enemy1.isDed[i] && enemy1.dedTimer[i] > 0)Functions.DrawQuadPlus(int(enemy1.relativePos[i].x + scroll.x), int(enemy1.relativePos[i].y + scroll.y), int(enemy1.radius * 2), int(enemy1.radius * 2), 1, 1, 0, 0, 0, 100, 100, enemyTexture, BLUE);

			}

			//Novice::ScreenPrintf(int(enemy1.relativePos[i].x + scroll.x), int(enemy1.relativePos[i].y + scroll.y), "enemy1.isDed[%d]=%d", i, enemy1.isDed[i]);
			//Novice::ScreenPrintf(int(enemy1.relativePos[i].x + scroll.x), int(enemy1.relativePos[i].y + scroll.y + 20), "enemy1.dedTimer[%d]=%d", i, enemy1.dedTimer[i]);
		}

		if (enemy2.hostIsAlive)
		{
			for (int j = 0; j < 4; j++)
			{
				//Novice::ScreenPrintf(int(enemy2.centorPos.x + scroll.x), int(enemy2.centorPos.y + scroll.y), "enemy2.parentIsAlive=%d", enemy2.parentIsAlive);
				for (int i = 0; i < kEnemy2Num; i++)
				{
					if (enemy2.childIsAlive[j][i])
					{
						if (!enemy2.isDed[j][i])Functions.DrawQuadPlus(int(enemy2.relativePos[j][i].x / screenSize + scroll.x), int(enemy2.relativePos[j][i].y / screenSize + scroll.y), int(enemy2.radius * 2 / screenSize), int(enemy2.radius * 2 / screenSize), 1, 1, 0, 0, 0, 100, 100, enemyTexture, 0xffffffff);
						if (enemy2.isDed[j][i] && enemy2.dedTimer[j][i] > 0)Functions.DrawQuadPlus(int(enemy2.relativePos[j][i].x / screenSize + scroll.x), int(enemy2.relativePos[j][i].y / screenSize + scroll.y), int(enemy2.radius * 2 / screenSize), int(enemy2.radius * 2 / screenSize), 1, 1, 0, 0, 0, 100, 100, enemyTexture, BLUE);
					}
					//Novice::ScreenPrintf(int(enemy2.relativePos[i].x + scroll.x), int(enemy2.relativePos[i].y + scroll.y), "enemy2.isDed[%d]=%d", i, enemy2.isDed[i]);
					//Novice::ScreenPrintf(int(enemy2.relativePos[i].x + scroll.x), int(enemy2.relativePos[i].y + scroll.y + 20), "enemy2.dedTimer[%d]=%d", i, enemy2.dedTimer[i]);
				}
			}
		}


		//Novice::ScreenPrintf(int(enemy3.centorPos.x + scroll.x), int(enemy3.centorPos.y + scroll.y), "enemy3.parentIsAlive=%d", enemy3.parentIsAlive);
		for (int i = 0; i < 4; i++)
		{
			if (enemy3.childIsAlive[i])
			{
				if (!enemy3.isDed[i])Functions.DrawQuadPlus(int(enemy3.relativePos[i].x + scroll.x), int(enemy3.relativePos[i].y + scroll.y), int(enemy3.radius * 2), int(enemy3.radius * 2), 1, 1, 0, 0, 0, 100, 100, enemyTexture, 0xffffffff);
				if (enemy3.isDed[i] && enemy3.dedTimer[i] > 0)Functions.DrawQuadPlus(int(enemy3.relativePos[i].x + scroll.x), int(enemy3.relativePos[i].y + scroll.y), int(enemy3.radius * 2), int(enemy3.radius * 2), 1, 1, 0, 0, 0, 100, 100, enemyTexture, BLUE);
			}
			//Novice::ScreenPrintf(int(enemy3.relativePos[i].x + scroll.x), int(enemy3.relativePos[i].y + scroll.y), "enemy3.isDed[%d]=%d", i, enemy3.isDed[i]);
			//Novice::ScreenPrintf(int(enemy3.relativePos[i].x + scroll.x), int(enemy3.relativePos[i].y + scroll.y + 20), "enemy3.dedTimer[%d]=%d", i, enemy3.dedTimer[i]);
		}

		//Novice::ScreenPrintf(int(enemy4.centorPos.x + scroll.x), int(enemy4.centorPos.y + scroll.y), "enemy4.parentIsAlive=%d", enemy4.parentIsAlive);
		for (int i = 0; i < 4; i++)
		{
			if (enemy4.childIsAlive[i])
			{
				if (!enemy4.isDed[i])Functions.DrawQuadPlus(int(enemy4.relativePos[i].x + scroll.x), int(enemy4.relativePos[i].y + scroll.y), int(enemy4.radius * 2), int(enemy4.radius * 2), 1, 1, 0, 0, 0, 100, 100, enemyTexture, 0xffffffff);
				if (enemy4.isDed[i] && enemy4.dedTimer[i] > 0)Functions.DrawQuadPlus(int(enemy4.relativePos[i].x + scroll.x), int(enemy4.relativePos[i].y + scroll.y), int(enemy4.radius * 2), int(enemy4.radius * 2), 1, 1, 0, 0, 0, 100, 100, enemyTexture, BLUE);
			}
			//Novice::ScreenPrintf(int(enemy4.relativePos[i].x + scroll.x), int(enemy4.relativePos[i].y + scroll.y), "enemy4.isDed[%d]=%d", i, enemy4.isDed[i]);
			//Novice::ScreenPrintf(int(enemy4.relativePos[i].x + scroll.x), int(enemy4.relativePos[i].y + scroll.y + 20), "enemy4.dedTimer[%d]=%d", i, enemy4.dedTimer[i]);
		}

		//Novice::ScreenPrintf(int(enemy5.centorPos.x + scroll.x), int(enemy5.centorPos.y + scroll.y), "enemy5.parentIsAlive=%d", enemy5.parentIsAlive);
		for (int i = 0; i < 4; i++)
		{
			if (enemy5.childIsAlive[i])
			{
				if (!enemy5.isDed[i])Functions.DrawQuadPlus(int(enemy5.relativePos[i].x + scroll.x), int(enemy5.relativePos[i].y + scroll.y), int(enemy5.radius * 2), int(enemy5.radius * 2), 1, 1, 0, 0, 0, 100, 100, enemyTexture, 0xffffffff);
				if (enemy5.isDed[i] && enemy5.dedTimer[i] > 0)Functions.DrawQuadPlus(int(enemy5.relativePos[i].x + scroll.x), int(enemy5.relativePos[i].y + scroll.y), int(enemy5.radius * 2), int(enemy5.radius * 2), 1, 1, 0, 0, 0, 100, 100, enemyTexture, BLUE);
			}
			//Novice::ScreenPrintf(int(enemy5.relativePos[i].x + scroll.x), int(enemy5.relativePos[i].y + scroll.y), "enemy5.isDed[%d]=%d", i, enemy5.isDed[i]);
			//Novice::ScreenPrintf(int(enemy5.relativePos[i].x + scroll.x), int(enemy5.relativePos[i].y + scroll.y + 20), "enemy5.dedTimer[%d]=%d", i, enemy5.dedTimer[i]);
		}

		//Novice::ScreenPrintf(int(enemy6.centorPos.x + scroll.x), int(enemy6.centorPos.y + scroll.y), "enemy6.parentIsAlive=%d", enemy6.parentIsAlive);
		for (int i = 0; i < 4; i++)
		{
			if (enemy6.childIsAlive[i])
			{
				if (!enemy6.isDed[i])Functions.DrawQuadPlus(int(enemy6.relativePos[i].x + scroll.x), int(enemy6.relativePos[i].y + scroll.y), int(enemy6.radius * 2), int(enemy6.radius * 2), 1, 1, 0, 0, 0, 100, 100, enemyTexture, 0xffffffff);
				if (enemy6.isDed[i] && enemy6.dedTimer[i] > 0)Functions.DrawQuadPlus(int(enemy6.relativePos[i].x + scroll.x), int(enemy6.relativePos[i].y + scroll.y), int(enemy6.radius * 2), int(enemy6.radius * 2), 1, 1, 0, 0, 0, 100, 100, enemyTexture, BLUE);
			}
			//Novice::ScreenPrintf(int(enemy6.relativePos[i].x + scroll.x), int(enemy6.relativePos[i].y + scroll.y), "enemy6.isDed[%d]=%d", i, enemy6.isDed[i]);
			//Novice::ScreenPrintf(int(enemy6.relativePos[i].x + scroll.x), int(enemy6.relativePos[i].y + scroll.y + 20), "enemy6.dedTimer[%d]=%d", i, enemy6.dedTimer[i]);
		}

		if (enemy7.hostIsAlive)
		{
			for (int j = 0; j < 8; j++)
			{

				//Novice::ScreenPrintf(int(enemy7.centorPos[j].x + scroll.x), int(enemy7.centorPos[j].y + scroll.y), "enemy7[%d].parentIsAlive=%d", j, enemy7.parentIsAlive);
				for (int i = 0; i < 4; i++)
				{
					if (enemy7.childIsAlive[j][i])
					{
						if (!enemy7.isDed[j][i])Functions.DrawQuadPlus(int(enemy7.relativePos[j][i].x / screenSize + scroll.x), int(enemy7.relativePos[j][i].y / screenSize + scroll.y), int(enemy7.radius * 2 / screenSize), int(enemy7.radius * 2 / screenSize), 1, 1, 0, 0, 0, 100, 100, enemyTexture, 0xffffffff);
						if (enemy7.isDed[j][i] && enemy7.dedTimer[j][i] > 0)Functions.DrawQuadPlus(int(enemy7.relativePos[j][i].x / screenSize + scroll.x), int(enemy7.relativePos[j][i].y / screenSize + scroll.y), int(enemy7.radius * 2 / screenSize), int(enemy7.radius * 2 / screenSize), 1, 1, 0, 0, 0, 100, 100, enemyTexture, BLUE);
					}
					//Novice::ScreenPrintf(int(enemy7.relativePos[j][i].x + scroll.x), int(enemy7.relativePos[j][i].y + scroll.y), "enemy7.isDed[%d][%d]=%d", j, i, enemy7.isDed[i]);
					//Novice::ScreenPrintf(int(enemy7.relativePos[j][i].x + scroll.x), int(enemy7.relativePos[j][i].y + scroll.y + 20), "enemy7.dedTimer[%d][%d]=%d", j, i, enemy7.dedTimer[i]);
				}

			}
		}
		if (enemy8.hostIsAlive)
		{
			for (int j = 0; j < 4; j++)
			{

				//Novice::ScreenPrintf(int(enemy7.centorPos[j].x + scroll.x), int(enemy7.centorPos[j].y + scroll.y), "enemy7[%d].parentIsAlive=%d", j, enemy7.parentIsAlive);
				for (int i = 0; i < 12; i++)
				{
					if (enemy8.childIsAlive[j][i])
					{
						if (!enemy8.isDed[j][i])Functions.DrawQuadPlus(int(enemy8.relativePos[j][i].x / screenSize + scroll.x), int(enemy8.relativePos[j][i].y / screenSize + scroll.y), int(enemy8.radius * 2 / screenSize), int(enemy8.radius * 2 / screenSize), 1, 1, 0, 0, 0, 100, 100, enemyTexture, 0xffffffff);
						if (enemy8.isDed[j][i] && enemy8.dedTimer[j][i] > 0)Functions.DrawQuadPlus(int(enemy8.relativePos[j][i].x / screenSize + scroll.x), int(enemy8.relativePos[j][i].y / screenSize + scroll.y), int(enemy8.radius * 2 / screenSize), int(enemy8.radius * 2 / screenSize), 1, 1, 0, 0, 0, 100, 100, enemyTexture, BLUE);

					}
					//Novice::ScreenPrintf(int(enemy8.relativePos[j][i].x + scroll.x), int(enemy8.relativePos[j][i].y + scroll.y), "enemy8.isDed[%d][%d]=%d", j, i, enemy8.isDed[i]);
					//Novice::ScreenPrintf(int(enemy8.relativePos[j][i].x + scroll.x), int(enemy8.relativePos[j][i].y + scroll.y + 20), "enemy8.dedTimer[%d][%d]=%d", j, i, enemy8.dedTimer[i]);
				}

			}
		}
		if (enemy9.hostIsAlive)
		{
			for (int j = 0; j < 2; j++)
			{

				//Novice::ScreenPrintf(int(enemy7.centorPos[j].x + scroll.x), int(enemy7.centorPos[j].y + scroll.y), "enemy7[%d].parentIsAlive=%d", j, enemy7.parentIsAlive);
				for (int i = 0; i < 8; i++)
				{
					if (enemy9.childIsAlive[j][i])
					{
						if (!enemy9.isDed[j][i])Functions.DrawQuadPlus(int(enemy9.relativePos[j][i].x / screenSize + scroll.x), int(enemy9.relativePos[j][i].y / screenSize + scroll.y), int(enemy9.radius * 2 / screenSize), int(enemy9.radius * 2 / screenSize), 1, 1, 0, 0, 0, 100, 100, enemyTexture, 0xffffffff);
						if (enemy9.isDed[j][i] && enemy9.dedTimer[j][i] > 0)Functions.DrawQuadPlus(int(enemy9.relativePos[j][i].x / screenSize + scroll.x), int(enemy9.relativePos[j][i].y / screenSize + scroll.y), int(enemy9.radius * 2 / screenSize), int(enemy9.radius * 2 / screenSize), 1, 1, 0, 0, 0, 100, 100, enemyTexture, BLUE);

					}
					//Novice::ScreenPrintf(int(enemy9.relativePos[j][i].x + scroll.x), int(enemy9.relativePos[j][i].y + scroll.y), "enemy9.isDed[%d][%d]=%d", j, i, enemy9.isDed[i]);
					//Novice::ScreenPrintf(int(enemy9.relativePos[j][i].x + scroll.x), int(enemy9.relativePos[j][i].y + scroll.y + 20), "enemy9.dedTimer[%d][%d]=%d", j, i, enemy9.dedTimer[i]);
				}

			}
		}

		for (int j = 0; j < kenemy10Num; j++)
		{
			if (enemy10[j].parentIsAlive)
			{
				for (int i = 0; i < 4; i++)
				{
					if (enemy10[j].childIsAlive[i])
					{
						if (!enemy10[j].isDed[i])Functions.DrawQuadPlus(int(enemy10[j].relativePos[i].x / screenSize + scroll.x), int(enemy10[j].relativePos[i].y / screenSize + scroll.y), int(enemy10[j].radius * 2 / screenSize), int(enemy10[j].radius * 2 / screenSize), 1, 1, 0, 0, 0, 100, 100, enemyTexture, 0xffffffff);
						if (enemy10[j].isDed[i] && enemy10[j].dedTimer[i] > 0)Functions.DrawQuadPlus(int(enemy10[j].relativePos[i].x / screenSize + scroll.x), int(enemy10[j].relativePos[i].y / screenSize + scroll.y), int(enemy10[j].radius * 2 / screenSize), int(enemy10[j].radius * 2 / screenSize), 1, 1, 0, 0, 0, 100, 100, enemyTexture, BLUE);
					}
					//Novice::ScreenPrintf(int(enemy10[j].relativePos[i].x + scroll.x), int(enemy10[j].relativePos[i].y + scroll.y), "enemy10[j].isDed[%d]=%d", i, enemy10[j].isDed[i]);
					//Novice::ScreenPrintf(int(enemy10[j].relativePos[i].x + scroll.x), int(enemy10[j].relativePos[i].y + scroll.y + 20), "enemy10[j].dedTimer[%d]=%d", i, enemy10[j].dedTimer[i]);
				}
			}
		}
		for (int j = 0; j < kenemy11Num; j++)
		{
			if (enemy11[j].parentIsAlive)
			{
				for (int i = 0; i < 3; i++)
				{
					if (enemy11[j].childIsAlive[i])
					{
						if (!enemy11[j].isDed[i])Functions.DrawQuadPlus(int(enemy11[j].relativePos[i].x / screenSize + scroll.x), int(enemy11[j].relativePos[i].y / screenSize + scroll.y), int(enemy11[j].radius * 2 / screenSize), int(enemy11[j].radius * 2 / screenSize), 1, 1, 0, 0, 0, 100, 100, enemyTexture, 0xffffffff);
						if (enemy11[j].isDed[i] && enemy11[j].dedTimer[i] > 0)Functions.DrawQuadPlus(int(enemy11[j].relativePos[i].x / screenSize + scroll.x), int(enemy11[j].relativePos[i].y / screenSize + scroll.y), int(enemy11[j].radius * 2 / screenSize), int(enemy11[j].radius * 2 / screenSize), 1, 1, 0, 0, 0, 100, 100, enemyTexture, BLUE);
					}
					//Novice::ScreenPrintf(int(enemy11[j].relativePos[i].x + scroll.x), int(enemy11[j].relativePos[i].y + scroll.y), "enemy11[j].isDed[%d]=%d", i, enemy11[j].isDed[i]);
					//Novice::ScreenPrintf(int(enemy11[j].relativePos[i].x + scroll.x), int(enemy11[j].relativePos[i].y + scroll.y + 20), "enemy11[j].dedTimer[%d]=%d", i, enemy11[j].dedTimer[i]);
				}
			}
		}
		for (int j = 0; j < kenemy12Num; j++)
		{
			if (enemy12[j].parentIsAlive)
			{
				for (int i = 0; i < 4; i++)
				{
					if (enemy12[j].childIsAlive[i])
					{
						if (!enemy12[j].isDed[i])Functions.DrawQuadPlus(int(enemy12[j].relativePos[i].x / screenSize + scroll.x), int(enemy12[j].relativePos[i].y / screenSize + scroll.y), int(enemy12[j].radius * 2 / screenSize), int(enemy12[j].radius * 2 / screenSize), 1, 1, 0, 0, 0, 100, 100, enemyTexture, 0xffffffff);
						if (enemy12[j].isDed[i] && enemy12[j].dedTimer[i] > 0)Functions.DrawQuadPlus(int(enemy12[j].relativePos[i].x / screenSize + scroll.x), int(enemy12[j].relativePos[i].y / screenSize + scroll.y), int(enemy12[j].radius * 2 / screenSize), int(enemy12[j].radius * 2 / screenSize), 1, 1, 0, 0, 0, 100, 100, enemyTexture, BLUE);
					}
					//Novice::ScreenPrintf(int(enemy10[j].relativePos[i].x + scroll.x), int(enemy10[j].relativePos[i].y + scroll.y), "enemy10[j].isDed[%d]=%d", i, enemy10[j].isDed[i]);
					//Novice::ScreenPrintf(int(enemy10[j].relativePos[i].x + scroll.x), int(enemy10[j].relativePos[i].y + scroll.y + 20), "enemy10[j].dedTimer[%d]=%d", i, enemy10[j].dedTimer[i]);
				}
			}
		}
		///                                                            ///
		/// --------------------↑描画処理ここまで-------------------- ///
		///                                                            ///       

		gameTimer++;
		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

float clump(float a, float min, float max)
{
	if (a <= min)
	{
		return min;
	}
	else if (a >= max)
	{
		return max;
	}
	else
	{
		return a;
	}

}
