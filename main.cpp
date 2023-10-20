#include <Novice.h>
#include "Headers.h"
#include <corecrt_math.h>
#include "MatrixCalc.h"
#include "collision.h"

const char kWindowTitle[] = "LC1A_20_ヒサイチ_コウキ";

float clump(float a, float min, float max);


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1920, 1080);

	PlayerData player{};
	player.radius = { 30,30 };
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
	float dedZone = 35;//こっちがほんとのデッドゾーン

	Vector2 scroll{};

	float fieldRadius = 2000;//フィールド半径
	Vector2 fieldToPlayer{};//フィールドの中心からプレイヤーまでのxy距離
	float centerToPlayerLength = 0;//フィールドの中心からプレイヤーまでのLength

	float miniMap = 8;//ミニマップの倍率 20で20分の１　10で10分の1　数字が小さくなるほど大きく表示
	float miniMapPlayerSize = 2;//ミニマップに表示されるプレイヤー関係のサイズ　1が等倍　数字が大きくなるほど大きく表示

	int fieldTexture = Novice::LoadTexture("./Resources/images/field_1.png");


	float enemyRadius = 50;//敵全体の半径
#pragma region enemy1

	testEnemy1 enemy1{};
	enemy1.parentIsAlive = false;
	enemy1.centorPos = { 0,0 };
	enemy1.radius = enemyRadius;
	enemy1.posReset[0] = { enemy1.centorPos.x,enemy1.centorPos.y - (enemy1.radius * 3) };
	enemy1.posReset[1] = { enemy1.centorPos.x - (enemy1.radius * 3),enemy1.centorPos.y + (enemy1.radius * 3) };
	enemy1.posReset[2] = { enemy1.centorPos.x + (enemy1.radius * 3),enemy1.centorPos.y + (enemy1.radius * 3) };
#pragma endregion
#pragma region enemy2

	testEnemy2 enemy2{};
	enemy2.radius = enemyRadius;
	const int kEnemy2Num = 16;


	enemy2.posReset[14] = { enemy2.centorPos.x - 2 * enemy2.radius * 3 ,enemy2.centorPos.y + 3 * enemy2.radius * 3 };

	enemy2.posReset[0] = { enemy2.centorPos.x - enemy2.radius * 4.5f ,enemy2.centorPos.y - enemy2.radius * 7.5f };
	enemy2.posReset[1] = { enemy2.centorPos.x - enemy2.radius * 1.5f ,enemy2.centorPos.y - enemy2.radius * 7.5f };
	enemy2.posReset[2] = { enemy2.centorPos.x + enemy2.radius * 1.5f ,enemy2.centorPos.y - enemy2.radius * 7.5f };
	enemy2.posReset[3] = { enemy2.centorPos.x + enemy2.radius * 4.5f ,enemy2.centorPos.y - enemy2.radius * 7.5f };

	enemy2.posReset[4] = { enemy2.centorPos.x + enemy2.radius * 7.5f ,enemy2.centorPos.y - enemy2.radius * 4.5f };
	enemy2.posReset[5] = { enemy2.centorPos.x + enemy2.radius * 7.5f ,enemy2.centorPos.y - enemy2.radius * 1.5f };
	enemy2.posReset[6] = { enemy2.centorPos.x + enemy2.radius * 7.5f ,enemy2.centorPos.y + enemy2.radius * 1.5f };
	enemy2.posReset[7] = { enemy2.centorPos.x + enemy2.radius * 7.5f ,enemy2.centorPos.y + enemy2.radius * 4.5f };

	enemy2.posReset[8] = { enemy2.centorPos.x + enemy2.radius * 4.5f ,enemy2.centorPos.y + enemy2.radius * 7.5f };
	enemy2.posReset[9] = { enemy2.centorPos.x + enemy2.radius * 1.5f ,enemy2.centorPos.y + enemy2.radius * 7.5f };
	enemy2.posReset[10] = { enemy2.centorPos.x - enemy2.radius * 1.5f ,enemy2.centorPos.y + enemy2.radius * 7.5f };
	enemy2.posReset[11] = { enemy2.centorPos.x - enemy2.radius * 4.5f ,enemy2.centorPos.y + enemy2.radius * 7.5f };

	enemy2.posReset[12] = { enemy2.centorPos.x - enemy2.radius * 7.5f ,enemy2.centorPos.y + enemy2.radius * 4.5f };
	enemy2.posReset[13] = { enemy2.centorPos.x - enemy2.radius * 7.5f ,enemy2.centorPos.y + enemy2.radius * 1.5f };
	enemy2.posReset[14] = { enemy2.centorPos.x - enemy2.radius * 7.5f ,enemy2.centorPos.y - enemy2.radius * 1.5f };
	enemy2.posReset[15] = { enemy2.centorPos.x - enemy2.radius * 7.5f ,enemy2.centorPos.y - enemy2.radius * 4.5f };
#pragma endregion
#pragma region enemy3

	testEnemy3 enemy3{};//横一列
	enemy3.radius = enemyRadius;

	enemy3.posReset[0] = { enemy3.centorPos.x - enemy3.radius * 4.5f ,enemy3.centorPos.y };
	enemy3.posReset[1] = { enemy3.centorPos.x - enemy3.radius * 1.5f ,enemy3.centorPos.y };
	enemy3.posReset[2] = { enemy3.centorPos.x + enemy3.radius * 1.5f ,enemy3.centorPos.y };
	enemy3.posReset[3] = { enemy3.centorPos.x + enemy3.radius * 4.5f ,enemy3.centorPos.y };
#pragma endregion
#pragma region enemy4

	testEnemy3 enemy4{};//縦一列
	enemy4.radius = enemyRadius;

	enemy4.posReset[0] = { enemy4.centorPos.x,enemy4.centorPos.y - enemy4.radius * 4.5f };
	enemy4.posReset[1] = { enemy4.centorPos.x,enemy4.centorPos.y - enemy4.radius * 1.5f };
	enemy4.posReset[2] = { enemy4.centorPos.x,enemy4.centorPos.y + enemy4.radius * 1.5f };
	enemy4.posReset[3] = { enemy4.centorPos.x,enemy4.centorPos.y + enemy4.radius * 4.5f };
#pragma endregion
#pragma region enemy5

	testEnemy3 enemy5{};//左斜め上
	enemy5.radius = enemyRadius;

	enemy5.posReset[0] = { enemy5.centorPos.x - enemy5.radius * 4.5f * 0.7f,enemy5.centorPos.y - enemy5.radius * 4.5f * 0.7f };
	enemy5.posReset[1] = { enemy5.centorPos.x - enemy5.radius * 1.5f * 0.7f,enemy5.centorPos.y - enemy5.radius * 1.5f * 0.7f };
	enemy5.posReset[2] = { enemy5.centorPos.x + enemy5.radius * 1.5f * 0.7f,enemy5.centorPos.y + enemy5.radius * 1.5f * 0.7f };
	enemy5.posReset[3] = { enemy5.centorPos.x + enemy5.radius * 4.5f * 0.7f,enemy5.centorPos.y + enemy5.radius * 4.5f * 0.7f };
#pragma endregion
#pragma region enemy6

	testEnemy3 enemy6{};//右斜め上
	enemy6.radius = enemyRadius;

	enemy6.posReset[0] = { enemy6.centorPos.x + enemy6.radius * 4.5f * 0.7f,enemy6.centorPos.y - enemy6.radius * 4.5f * 0.7f };
	enemy6.posReset[1] = { enemy6.centorPos.x + enemy6.radius * 1.5f * 0.7f,enemy6.centorPos.y - enemy6.radius * 1.5f * 0.7f };
	enemy6.posReset[2] = { enemy6.centorPos.x - enemy6.radius * 1.5f * 0.7f,enemy6.centorPos.y + enemy6.radius * 1.5f * 0.7f };
	enemy6.posReset[3] = { enemy6.centorPos.x - enemy6.radius * 4.5f * 0.7f,enemy6.centorPos.y + enemy6.radius * 4.5f * 0.7f };
#pragma endregion
#pragma region enemy7
	testEnemy4 enemy7;
	enemy7.radius = enemyRadius;
	float enemy7PosRadius = 1800;
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
					enemy1.dedTimer[i] = 30;
					enemy1.isDed[i] = false;
				}
			}
			else if (keys[DIK_2])
			{
				enemy2.parentIsAlive = true;
				enemy2.centorPos = { 500,0 };
				for (int i = 0; i < kEnemy2Num; i++)
				{
					enemy2.childIsAlive[i] = true;
					enemy2.relativePos[i] = { enemy2.posReset[i].x + enemy2.centorPos.x,enemy2.posReset[i].y + enemy2.centorPos.y };
					enemy2.dedTimer[i] = 30;
					enemy2.isDed[i] = false;
				}

			}
			else if (keys[DIK_3])
			{
				enemy3.parentIsAlive = true;
				enemy3.centorPos = { 0,0 };
				for (int i = 0; i < 4; i++)
				{
					enemy3.childIsAlive[i] = true;
					enemy3.relativePos[i] = { enemy3.posReset[i].x + enemy3.centorPos.x,enemy3.posReset[i].y + enemy3.centorPos.y };
					enemy3.dedTimer[i] = 30;
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
					enemy4.dedTimer[i] = 30;
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
					enemy5.dedTimer[i] = 30;
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
					enemy6.dedTimer[i] = 30;
					enemy6.isDed[i] = false;
				}

			}
			else if (keys[DIK_7])
			{

				for (int j = 0; j < 8; j++)
				{
					enemy7.parentIsAlive[j] = true;
					for (int i = 0; i < 4; i++)
					{
						enemy7.childIsAlive[j][i] = true;
						enemy7.relativePos[j][i] = { enemy7.posReset[j][i].x ,enemy7.posReset[j][i].y };
						enemy7.dedTimer[j][i] = 30;
						enemy7.isDed[j][i] = false;
					}

				}
				enemy7.hostIsAlive = true;
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
		if (preJoyStickX<dedZone * 60 && preJoyStickX > -dedZone * 60 && preJoyStickY<dedZone * 60 && preJoyStickY > -dedZone * 60)
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
		if (enemy2.parentIsAlive)
		{
			enemy2.centorPos.x++;
			for (int i = 0; i < kEnemy2Num; i++)
			{
				enemy2.childIsAlive[i] = EllipseCollision({ 0,0 }, fieldRadius, enemy2.relativePos[i], enemy2.radius);
				if (!enemy2.isDed[i] && enemy2.childIsAlive[i])enemy2.relativePos[i].x++;
			}
			if (!enemy2.childIsAlive[0] && !enemy2.childIsAlive[1] && !enemy2.childIsAlive[2] && !enemy2.childIsAlive[3]
				&& !enemy2.childIsAlive[4] && !enemy2.childIsAlive[5] && !enemy2.childIsAlive[6] && !enemy2.childIsAlive[7]
				&& !enemy2.childIsAlive[8] && !enemy2.childIsAlive[9] && !enemy2.childIsAlive[10] && !enemy2.childIsAlive[11]
				&& !enemy2.childIsAlive[12] && !enemy2.childIsAlive[13] && !enemy2.childIsAlive[14] && !enemy2.childIsAlive[15])
			{
				enemy2.parentIsAlive = false;
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
			if (enemy7.parentIsAlive[0] == false && enemy7.parentIsAlive[1] == false && enemy7.parentIsAlive[2] == false && enemy7.parentIsAlive[3] == false && enemy7.parentIsAlive[4] == false && enemy7.parentIsAlive[5] == false && enemy7.parentIsAlive[6] == false && enemy7.parentIsAlive[7] == false)enemy7.hostIsAlive = false;
		}

#pragma endregion
		//ここで攻撃処理をしたいと考えてる

		if (player.dashAttack)
		{
			if (enemy1.parentIsAlive)
			{
				for (int i = 0; i < 3; i++)
				{
					if (!enemy1.isDed[i])enemy1.isDed[i] = EllipseCollision(player.pos, player.radius.x, enemy1.relativePos[i], enemy1.radius);
				}
			}
			if (enemy2.parentIsAlive)
			{
				for (int i = 0; i < kEnemy2Num; i++)
				{
					if (!enemy2.isDed[i])enemy2.isDed[i] = EllipseCollision(player.pos, player.radius.x, enemy2.relativePos[i], enemy2.radius);
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
			if (enemy2.parentIsAlive)
			{
				for (int i = 0; i < kEnemy2Num; i++)
				{
					if (!enemy2.isDed[i])enemy2.isDed[i] = attAreaObj.TriangleCollision(enemy2.relativePos[i]);
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


		//敵のダメージ処理
		if (enemy1.parentIsAlive)
		{
			for (int i = 0; i < 3; i++)
			{
				if (enemy1.isDed[i])
				{
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
		if (enemy2.parentIsAlive)
		{
			for (int i = 0; i < kEnemy2Num; i++)
			{
				if (enemy2.isDed[i])
				{
					if (enemy2.dedTimer[i] > 0)enemy2.dedTimer[i]--;
					if (enemy2.dedTimer[i] <= 0)enemy2.childIsAlive[i] = false;
				}
				if (enemy2.dedTimer[0] <= 0 && enemy2.dedTimer[1] <= 0 && enemy2.dedTimer[2] <= 0 && enemy2.dedTimer[3] <= 0 && enemy2.dedTimer[4] <= 0
					&& enemy2.dedTimer[5] <= 0 && enemy2.dedTimer[6] <= 0 && enemy2.dedTimer[7] <= 0 && enemy2.dedTimer[8] <= 0 && enemy2.dedTimer[9] <= 0
					&& enemy2.dedTimer[10] <= 0 && enemy2.dedTimer[11] <= 0 && enemy2.dedTimer[12] <= 0 && enemy2.dedTimer[13] <= 0 && enemy2.dedTimer[14] <= 0
					&& enemy2.dedTimer[15] <= 0
					)//&& enemy2.dedTimer[16] <= 0 && enemy2.dedTimer[17] <= 0 && enemy2.dedTimer[18] <= 0 && enemy2.dedTimer[19] <= 0
				{
					enemy2.parentIsAlive = false;

				}
			}

		}
		if (enemy3.parentIsAlive)
		{
			for (int i = 0; i < 4; i++)
			{
				if (enemy3.isDed[i])
				{
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
					if (enemy6.dedTimer[i] > 0)enemy6.dedTimer[i]--;
					if (enemy6.dedTimer[i] <= 0)enemy6.childIsAlive[i] = false;
				}
				if (enemy6.dedTimer[0] <= 0 && enemy6.dedTimer[1] <= 0 && enemy6.dedTimer[2] <= 0 && enemy6.dedTimer[3] <= 0)
				{
					enemy6.parentIsAlive = false;
				}
			}
		}
		for (int j = 0; j < 8; j++)
		{
			if (enemy7.parentIsAlive[j])
			{
				for (int i = 0; i < 4; i++)
				{
					if (enemy7.isDed[j][i])
					{
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
		//フィールドの外に出ないように
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
		scroll.x = (-player.pos.x + 960);
		scroll.y = (-player.pos.y + 540);



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

		//一番後ろの背景
		Novice::DrawBox(0, 0, 1920, 1080, 0, 0x222222ff, kFillModeSolid);
		//フィールドの画像表示
		Functions.DrawQuadPlus(int(scroll.x), int(scroll.y), int(fieldRadius * 2), int(fieldRadius * 2), 1.0f, 1.0f, 0, 0, 0, 4000, 4000, fieldTexture, 0xffffffff);
		//三角形を作った時
		if (player.aim)Novice::DrawBox(0, 0, 1920, 1080, 0, 0x88888844, kFillModeSolid);
		//三角形で攻撃時間
		if (player.aimTimer > 0)Novice::DrawBox(0, 0, 1920, 1080, 0, 0xaa888844, kFillModeSolid);
		if (player.aimTimer > 0)Novice::DrawTriangle(int(player.prepos[0].x + scroll.x), int(player.prepos[0].y + scroll.y), int(player.prepos[1].x + scroll.x), int(player.prepos[1].y + scroll.y), int(player.prepos[2].x + scroll.x), int(player.prepos[2].y + scroll.y), RED, kFillModeSolid);

		//デバッグ用
		Novice::DrawEllipse(int(100 + player.joystick.x), int(100 + player.joystick.y), 10, 10, 0, GREEN, kFillModeSolid);//直接入力
		Novice::DrawEllipse(int(100 + player.direction.x * 100), int(100 + player.direction.y * 100), 10, 10, 0, RED, kFillModeSolid);//方向
		Novice::DrawEllipse(int(100 + player.velocity.x), int(100 + player.velocity.y), 10, 10, 0, BLUE, kFillModeSolid);//速度
		Novice::DrawEllipse(100, 100, 100, 100, 0, WHITE, kFillModeWireFrame);//最大範囲
		Novice::DrawEllipse(100, 100, (int)dedZone, (int)dedZone, 0, GREEN, kFillModeWireFrame);//デッドゾーン

		//フィールドの円周
		Novice::DrawEllipse(int(scroll.x), int(scroll.y), (int)fieldRadius, (int)fieldRadius, 0, GREEN, kFillModeWireFrame);

		//三角形の点
		for (int i = 0; i < 3; i++)
		{
			if (player.prepos[i].x != 0)Novice::DrawEllipse(int(player.prepos[i].x + scroll.x), int(player.prepos[i].y + scroll.y), int(player.anchorRadius), int(player.anchorRadius), 0, RED, kFillModeSolid);
		}
		//スポーン地点
		Novice::DrawBox(int(-50 + scroll.x), int(-50 + scroll.y), 100, 100, 0, RED, kFillModeWireFrame);
		//プレイヤーの方向表示
		Novice::DrawLine(int(player.pos.x + scroll.x), int(player.pos.y + scroll.y), int(player.pos.x + scroll.x + player.direction.x * 150), int(player.pos.y + scroll.y + player.direction.y * 150), WHITE);
		//プレイヤー
		Novice::DrawEllipse(int(player.pos.x + scroll.x), int(player.pos.y + scroll.y), int(player.radius.x), int(player.radius.y), 0, GREEN, kFillModeSolid);
		//プレイヤーフリック時
		if (player.flick)Novice::DrawEllipse(int(player.pos.x + scroll.x), int(player.pos.y + scroll.y), int(player.radius.x), int(player.radius.y), 0, 0x00ffffff, kFillModeSolid);


		//ミニマップ
		Novice::DrawEllipse(int(30 + fieldRadius / miniMap), int(1050 - fieldRadius / miniMap), int(fieldRadius / miniMap), int(fieldRadius / miniMap), 0, GREEN, kFillModeWireFrame);
		for (int i = 0; i < 3; i++)
		{//ミニマップ三角形の点
			if (player.prepos[i].x != 0)Novice::DrawEllipse(int((30 + fieldRadius / miniMap) + player.prepos[i].x / miniMap), int(int(1050 - fieldRadius / miniMap) + player.prepos[i].y / miniMap), int(player.anchorRadius * miniMapPlayerSize / miniMap), int(player.anchorRadius * miniMapPlayerSize / miniMap), 0, RED, kFillModeSolid);
		}
		//ミニマップ範囲
		if (player.aimTimer > 0)Novice::DrawTriangle(int((30 + fieldRadius / miniMap) + player.prepos[0].x / miniMap), int(int(1050 - fieldRadius / miniMap) + player.prepos[0].y / miniMap), int((30 + fieldRadius / miniMap) + player.prepos[1].x / miniMap), int(int(1050 - fieldRadius / miniMap) + player.prepos[1].y / miniMap), int((30 + fieldRadius / miniMap) + player.prepos[2].x / miniMap), int(int(1050 - fieldRadius / miniMap) + player.prepos[2].y / miniMap), RED, kFillModeSolid);
		//ミニマッププレイヤー
		Novice::DrawEllipse(int((30 + fieldRadius / miniMap) + player.pos.x / miniMap), int(int(1050 - fieldRadius / miniMap) + player.pos.y / miniMap), int(player.radius.x * miniMapPlayerSize / miniMap), int(player.radius.y * miniMapPlayerSize / miniMap), 0, GREEN, kFillModeSolid);


		//ミニマップ８方向エネミー
		for (int j = 0; j < 8; j++)
		{
			for (int i = 0; i < 4; i++)
			{
				if (enemy7.childIsAlive[j][i])
				{
					if (!enemy7.isDed[j][i])Novice::DrawEllipse(int((30 + fieldRadius / miniMap) + enemy7.relativePos[j][i].x / miniMap), int(int(1050 - fieldRadius / miniMap) + enemy7.relativePos[j][i].y / miniMap), int(enemy7.radius * miniMapPlayerSize / miniMap), int(enemy7.radius * miniMapPlayerSize / miniMap), 0, 0xff00ffff, kFillModeSolid);
					if (enemy7.isDed[j][i] && enemy7.dedTimer[j][i] > 0)Novice::DrawEllipse(int((30 + fieldRadius / miniMap) + enemy7.relativePos[j][i].x / miniMap), int(int(1050 - fieldRadius / miniMap) + enemy7.relativePos[j][i].y / miniMap), int(enemy7.radius * miniMapPlayerSize / miniMap), int(enemy7.radius * miniMapPlayerSize / miniMap), 0, BLUE, kFillModeSolid);
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
				if (!enemy1.isDed[i])Novice::DrawEllipse(int(enemy1.relativePos[i].x + scroll.x), int(enemy1.relativePos[i].y + scroll.y), int(enemy1.radius), int(enemy1.radius), 0, 0xff00ffff, kFillModeSolid);
				if (enemy1.isDed[i] && enemy1.dedTimer[i] > 0)
				{
					Novice::DrawEllipse(int(enemy1.relativePos[i].x + scroll.x), int(enemy1.relativePos[i].y + scroll.y), int(enemy1.radius), int(enemy1.radius), 0, BLUE, kFillModeSolid);
				}
			}

			//Novice::ScreenPrintf(int(enemy1.relativePos[i].x + scroll.x), int(enemy1.relativePos[i].y + scroll.y), "enemy1.isDed[%d]=%d", i, enemy1.isDed[i]);
			//Novice::ScreenPrintf(int(enemy1.relativePos[i].x + scroll.x), int(enemy1.relativePos[i].y + scroll.y + 20), "enemy1.dedTimer[%d]=%d", i, enemy1.dedTimer[i]);
		}


		//Novice::ScreenPrintf(int(enemy2.centorPos.x + scroll.x), int(enemy2.centorPos.y + scroll.y), "enemy2.parentIsAlive=%d", enemy2.parentIsAlive);
		for (int i = 0; i < kEnemy2Num; i++)
		{
			if (enemy2.childIsAlive[i])
			{
				if (!enemy2.isDed[i])Novice::DrawEllipse(int(enemy2.relativePos[i].x + scroll.x), int(enemy2.relativePos[i].y + scroll.y), int(enemy2.radius), int(enemy2.radius), 0, 0xff00ffff, kFillModeSolid);
				if (enemy2.isDed[i] && enemy2.dedTimer[i] > 0)
				{
					Novice::DrawEllipse(int(enemy2.relativePos[i].x + scroll.x), int(enemy2.relativePos[i].y + scroll.y), int(enemy2.radius), int(enemy2.radius), 0, BLUE, kFillModeSolid);
				}
			}
			//Novice::ScreenPrintf(int(enemy2.relativePos[i].x + scroll.x), int(enemy2.relativePos[i].y + scroll.y), "enemy2.isDed[%d]=%d", i, enemy2.isDed[i]);
			//Novice::ScreenPrintf(int(enemy2.relativePos[i].x + scroll.x), int(enemy2.relativePos[i].y + scroll.y + 20), "enemy2.dedTimer[%d]=%d", i, enemy2.dedTimer[i]);
		}

		//Novice::ScreenPrintf(int(enemy3.centorPos.x + scroll.x), int(enemy3.centorPos.y + scroll.y), "enemy3.parentIsAlive=%d", enemy3.parentIsAlive);
		for (int i = 0; i < 4; i++)
		{
			if (enemy3.childIsAlive[i])
			{
				if (!enemy3.isDed[i])Novice::DrawEllipse(int(enemy3.relativePos[i].x + scroll.x), int(enemy3.relativePos[i].y + scroll.y), int(enemy3.radius), int(enemy3.radius), 0, 0xff00ffff, kFillModeSolid);
				if (enemy3.isDed[i] && enemy3.dedTimer[i] > 0)
				{
					Novice::DrawEllipse(int(enemy3.relativePos[i].x + scroll.x), int(enemy3.relativePos[i].y + scroll.y), int(enemy3.radius), int(enemy3.radius), 0, BLUE, kFillModeSolid);
				}
			}
			//Novice::ScreenPrintf(int(enemy3.relativePos[i].x + scroll.x), int(enemy3.relativePos[i].y + scroll.y), "enemy3.isDed[%d]=%d", i, enemy3.isDed[i]);
			//Novice::ScreenPrintf(int(enemy3.relativePos[i].x + scroll.x), int(enemy3.relativePos[i].y + scroll.y + 20), "enemy3.dedTimer[%d]=%d", i, enemy3.dedTimer[i]);
		}

		//Novice::ScreenPrintf(int(enemy4.centorPos.x + scroll.x), int(enemy4.centorPos.y + scroll.y), "enemy4.parentIsAlive=%d", enemy4.parentIsAlive);
		for (int i = 0; i < 4; i++)
		{
			if (enemy4.childIsAlive[i])
			{
				if (!enemy4.isDed[i])Novice::DrawEllipse(int(enemy4.relativePos[i].x + scroll.x), int(enemy4.relativePos[i].y + scroll.y), int(enemy4.radius), int(enemy4.radius), 0, 0xff00ffff, kFillModeSolid);
				if (enemy4.isDed[i] && enemy4.dedTimer[i] > 0)
				{
					Novice::DrawEllipse(int(enemy4.relativePos[i].x + scroll.x), int(enemy4.relativePos[i].y + scroll.y), int(enemy4.radius), int(enemy4.radius), 0, BLUE, kFillModeSolid);
				}
			}
			//Novice::ScreenPrintf(int(enemy4.relativePos[i].x + scroll.x), int(enemy4.relativePos[i].y + scroll.y), "enemy4.isDed[%d]=%d", i, enemy4.isDed[i]);
			//Novice::ScreenPrintf(int(enemy4.relativePos[i].x + scroll.x), int(enemy4.relativePos[i].y + scroll.y + 20), "enemy4.dedTimer[%d]=%d", i, enemy4.dedTimer[i]);
		}

		//Novice::ScreenPrintf(int(enemy5.centorPos.x + scroll.x), int(enemy5.centorPos.y + scroll.y), "enemy5.parentIsAlive=%d", enemy5.parentIsAlive);
		for (int i = 0; i < 4; i++)
		{
			if (enemy5.childIsAlive[i])
			{
				if (!enemy5.isDed[i])Novice::DrawEllipse(int(enemy5.relativePos[i].x + scroll.x), int(enemy5.relativePos[i].y + scroll.y), int(enemy5.radius), int(enemy5.radius), 0, 0xff00ffff, kFillModeSolid);
				if (enemy5.isDed[i] && enemy5.dedTimer[i] > 0)
				{
					Novice::DrawEllipse(int(enemy5.relativePos[i].x + scroll.x), int(enemy5.relativePos[i].y + scroll.y), int(enemy5.radius), int(enemy5.radius), 0, BLUE, kFillModeSolid);
				}
			}
			//Novice::ScreenPrintf(int(enemy5.relativePos[i].x + scroll.x), int(enemy5.relativePos[i].y + scroll.y), "enemy5.isDed[%d]=%d", i, enemy5.isDed[i]);
			//Novice::ScreenPrintf(int(enemy5.relativePos[i].x + scroll.x), int(enemy5.relativePos[i].y + scroll.y + 20), "enemy5.dedTimer[%d]=%d", i, enemy5.dedTimer[i]);
		}

		//Novice::ScreenPrintf(int(enemy6.centorPos.x + scroll.x), int(enemy6.centorPos.y + scroll.y), "enemy6.parentIsAlive=%d", enemy6.parentIsAlive);
		for (int i = 0; i < 4; i++)
		{
			if (enemy6.childIsAlive[i])
			{
				if (!enemy6.isDed[i])Novice::DrawEllipse(int(enemy6.relativePos[i].x + scroll.x), int(enemy6.relativePos[i].y + scroll.y), int(enemy6.radius), int(enemy6.radius), 0, 0xff00ffff, kFillModeSolid);
				if (enemy6.isDed[i] && enemy6.dedTimer[i] > 0)
				{
					Novice::DrawEllipse(int(enemy6.relativePos[i].x + scroll.x), int(enemy6.relativePos[i].y + scroll.y), int(enemy6.radius), int(enemy6.radius), 0, BLUE, kFillModeSolid);
				}
			}
			//Novice::ScreenPrintf(int(enemy6.relativePos[i].x + scroll.x), int(enemy6.relativePos[i].y + scroll.y), "enemy6.isDed[%d]=%d", i, enemy6.isDed[i]);
			//Novice::ScreenPrintf(int(enemy6.relativePos[i].x + scroll.x), int(enemy6.relativePos[i].y + scroll.y + 20), "enemy6.dedTimer[%d]=%d", i, enemy6.dedTimer[i]);
		}

		for (int j = 0; j < 8; j++)
		{

			//Novice::ScreenPrintf(int(enemy7.centorPos[j].x + scroll.x), int(enemy7.centorPos[j].y + scroll.y), "enemy7[%d].parentIsAlive=%d", j, enemy7.parentIsAlive);
			for (int i = 0; i < 4; i++)
			{
				if (enemy7.childIsAlive[j][i])
				{
					if (!enemy7.isDed[j][i])Novice::DrawEllipse(int(enemy7.relativePos[j][i].x + scroll.x), int(enemy7.relativePos[j][i].y + scroll.y), int(enemy7.radius), int(enemy7.radius), 0, 0xff00ffff, kFillModeSolid);
					if (enemy7.isDed[j][i] && enemy7.dedTimer[j][i] > 0)
					{
						Novice::DrawEllipse(int(enemy7.relativePos[j][i].x + scroll.x), int(enemy7.relativePos[j][i].y + scroll.y), int(enemy7.radius), int(enemy7.radius), 0, BLUE, kFillModeSolid);
					}
				}
				//Novice::ScreenPrintf(int(enemy7.relativePos[j][i].x + scroll.x), int(enemy7.relativePos[j][i].y + scroll.y), "enemy7.isDed[%d][%d]=%d", j, i, enemy7.isDed[i]);
				//Novice::ScreenPrintf(int(enemy7.relativePos[j][i].x + scroll.x), int(enemy7.relativePos[j][i].y + scroll.y + 20), "enemy7.dedTimer[%d][%d]=%d", j, i, enemy7.dedTimer[i]);
			}

		}
		
		///                                                            ///
		/// --------------------↑描画処理ここまで-------------------- ///
		///                                                            ///       

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
