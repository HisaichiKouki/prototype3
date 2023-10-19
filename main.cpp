#include <Novice.h>
#include "Vector2.h"
#include "VectorPlus.h"
#include <corecrt_math.h>
#include "Functions.h"

const char kWindowTitle[] = "LC1A_20_ヒサイチ_コウキ";
typedef struct Player
{
	Vector2 pos;
	Vector2 direction;
	Vector2 velocity;
	Vector2 radius;

	Vector2 joystick;

	Vector2 prepos[3];
	Vector2 screenPos;

	float shotSpeed;
	float moveSpeed;
	bool trigerA;
	float velocityRatio;
	bool aim;
	int count;
	int aimTimer;
	float anchorRadius;

	int flickTimer;
	bool flick;//はじきフラグ
	bool attack;
	float flickLength;
	int flickCT;
}Player;

float clump(float a, float min, float max);


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1920, 1080);

	Player player{};
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
	float dedZone = 35;

	Vector2 scroll{};

	float fieldRadius = 2000;
	Vector2 fieldToPlayer{};
	float centerToPlayerLength = 0;

	float miniMap = 10;//ミニマップの倍率 20で20分の１　10で10分の1　数字が小さくなるほど大きく表示
	float miniMapPlayerSize = 3;//ミニマップに表示されるプレイヤー関係のサイズ　1が等倍　数字が大きくなるほど大きく表示

	int fieldTexture = Novice::LoadTexture("./Resources/images/field_1.png");


	Func Functions;
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
		preJoyStickX = joystickX;
		preJoyStickY = joystickY;

		if (player.velocityRatio > 0)
		{
			player.velocityRatio -= 0.01f;
			if (player.velocityRatio < 0)
			{
				player.velocityRatio = 0;
			}
		}

		if (player.flick && player.flickCT == 0)player.flickCT = 20;

		if (player.flickCT > 0)
		{
			player.flickCT--;
			if (player.flickCT <= 0)
			{
				player.flickCT = 0;
				player.flick = false;

			}
		}
		//ボタン入力ここから
		player.trigerA = false;
		if (Novice::IsTriggerButton(0, kPadButton10))
		{
			player.trigerA = true;
		}



		if (player.aimTimer > 0)
		{
			player.aimTimer--;
		}
		else if (player.aimTimer <= 0)
		{
			player.aimTimer = 0;
			if (player.count == 0)
			{
				for (int i = 0; i < 3; i++)
				{
					player.prepos[i] = { 0,0 };
				}
			}
		}
		if (player.trigerA && player.aimTimer == 0)
		{
			if (player.direction.x!=0|| player.direction.y!=0)
			{
				player.prepos[player.count] = player.pos;
				player.velocityRatio = 1;
				player.aim = true;
				player.count++;
				if (player.count >= 3)
				{
					player.count = 0;
					player.aim = false;
					player.aimTimer = 30;

				}
			}

			
		}
		//ボタン入力ここまで


		///スティック入力ここから
		Novice::GetAnalogInputLeft(0, &joystickX, &joystickY);

		player.flickLength = floatLength({ (float)joystickX, (float)joystickY }, { 0,0 });

		player.joystick.x = (clump(float(joystickX / 32767.0f), -1, 1) * 100);
		player.joystick.y = (clump(float(joystickY / 32767.0f), -1, 1) * 100);

		xy = vectorLength(player.joystick, { 0,0 });
		length = sqrtf(xy.x * xy.x + xy.y * xy.y);

		if (length >= dedZone)
		{
			player.direction = vectorNormalize(player.joystick, { 0,0 });
		}
		else
		{
			player.direction = { 0,0 };
		}

		//はじき判定
		if (player.flickTimer > 0)
		{
			player.flickTimer--;
			if (player.flickLength > 32000&&!player.aim)
			{
				player.flick = true;
				player.velocityRatio = 1;
			}
		}
		if (preJoyStickX<dedZone * 120 && preJoyStickX > -dedZone * 120 && preJoyStickY<dedZone * 120 && preJoyStickY > -dedZone * 120)
		{

			if (!player.flick && player.flickCT == 0)player.flickTimer = 1;
		}
		if (player.trigerA || player.flick && player.flickCT == 0)
		{
			player.velocity.x = player.direction.x * player.shotSpeed;
			player.velocity.y = player.direction.y * player.shotSpeed;

		}


		player.velocity.x *= player.velocityRatio;
		player.velocity.y *= player.velocityRatio;






		if (!player.aim && !player.flick&& player.aimTimer<=15)
		{
			player.velocity.x = player.direction.x * player.moveSpeed;
			player.velocity.y = player.direction.y * player.moveSpeed;
		}

		///スティック入力ここまで


		player.pos.x += player.velocity.x;
		player.pos.y += player.velocity.y;

		//フィールドの外に出ないように
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

		scroll.x = (-player.pos.x + 960);
		scroll.y = (-player.pos.y + 540);


		if (keys[DIK_SPACE])
		{
			player.flick = false;
		}
		///                                                            ///
		/// --------------------↑更新処理ここまで-------------------- ///
		///                                                            ///

		///                                                            ///
		/// --------------------↓描画処理ここから-------------------- ///
		///                                                            ///


		Novice::ScreenPrintf(0, 200, "joystickX=%d joystickY=%d", joystickX, joystickY);
		Novice::ScreenPrintf(0, 220, "Player.joystickX=%f ", player.joystick.x);
		Novice::ScreenPrintf(0, 240, "Player.joystickY=%f", player.joystick.y);
		Novice::ScreenPrintf(0, 260, "player.vel.x=%f player.vel.y=%f", player.velocity.x, player.velocity.y);
		Novice::ScreenPrintf(0, 280, "player.TrigerA=%d", player.trigerA);
		Novice::ScreenPrintf(0, 300, "player.velocityRatio=%f", player.velocityRatio);
		Novice::ScreenPrintf(0, 320, "player.count=%d", player.count);
		Novice::ScreenPrintf(0, 340, "World.X=%f World.Y=%f", player.pos.x, player.pos.y);
		Novice::ScreenPrintf(0, 360, "player.playTime=%d player.flick=%d", player.flickTimer, player.flick);
		Novice::ScreenPrintf(0, 380, "player.flickLength=%f", player.flickLength);
		Novice::ScreenPrintf(0, 400, "length=%f", length);
		Novice::ScreenPrintf(0, 420, "player.flickCT=%d", player.flickCT);


		Novice::DrawBox(0, 0, 1920, 1080, 0, 0x222222ff, kFillModeSolid);
		Functions.DrawQuadPlus(int(scroll.x), int(scroll.y), int(fieldRadius * 2), int(fieldRadius * 2), 1.0f, 1.0f, 0, 0, 0, 4000, 4000, fieldTexture, 0xffffffff);
		if (player.aim)Novice::DrawBox(0, 0, 1920, 1080, 0, 0x88888844, kFillModeSolid);
		if (player.aimTimer > 0)Novice::DrawBox(0, 0, 1920, 1080, 0, 0xaa888844, kFillModeSolid);
		if (player.aimTimer > 0)Novice::DrawTriangle(int(player.prepos[0].x + scroll.x), int(player.prepos[0].y + scroll.y), int(player.prepos[1].x + scroll.x), int(player.prepos[1].y + scroll.y), int(player.prepos[2].x + scroll.x), int(player.prepos[2].y + scroll.y), RED, kFillModeSolid);

		Novice::DrawEllipse(int(100 + player.joystick.x), int(100 + player.joystick.y), 10, 10, 0, GREEN, kFillModeSolid);//直接入力
		Novice::DrawEllipse(int(100 + player.direction.x * 100), int(100 + player.direction.y * 100), 10, 10, 0, RED, kFillModeSolid);//方向
		Novice::DrawEllipse(int(100 + player.velocity.x), int(100 + player.velocity.y), 10, 10, 0, BLUE, kFillModeSolid);//速度
		Novice::DrawEllipse(100, 100, 100, 100, 0, WHITE, kFillModeWireFrame);//最大範囲
		Novice::DrawEllipse(100, 100, (int)dedZone, (int)dedZone, 0, GREEN, kFillModeWireFrame);//デッドゾーン


		Novice::DrawEllipse(int(scroll.x), int(scroll.y), (int)fieldRadius, (int)fieldRadius, 0, GREEN, kFillModeWireFrame);
		for (int i = 0; i < 3; i++)
		{
			if (player.prepos[i].x != 0)Novice::DrawEllipse(int(player.prepos[i].x + scroll.x), int(player.prepos[i].y + scroll.y), int(player.anchorRadius), int(player.anchorRadius), 0, RED, kFillModeSolid);
		}
		Novice::DrawBox(int(-50 + scroll.x), int(-50 + scroll.y), 100, 100, 0, RED, kFillModeWireFrame);//スポーン地点
		Novice::DrawLine(int(player.pos.x + scroll.x), int(player.pos.y + scroll.y), int(player.pos.x + scroll.x + player.direction.x * 150), int(player.pos.y + scroll.y + player.direction.y * 150), WHITE);//プレイヤーの方向表示
		Novice::DrawEllipse(int(player.pos.x + scroll.x), int(player.pos.y + scroll.y), int(player.radius.x), int(player.radius.y), 0, GREEN, kFillModeSolid);//プレイヤー
		if(player.flick)Novice::DrawEllipse(int(player.pos.x + scroll.x), int(player.pos.y + scroll.y), int(player.radius.x), int(player.radius.y), 0, 0x00ffffff, kFillModeSolid);//プレイヤー


		//ミニマップ
		Novice::DrawEllipse(int(30 + fieldRadius / miniMap), int(1050 - fieldRadius / miniMap), int(fieldRadius / miniMap), int(fieldRadius / miniMap), 0, GREEN, kFillModeWireFrame);
		for (int i = 0; i < 3; i++)
		{//玉
			if (player.prepos[i].x != 0)Novice::DrawEllipse(int((30 + fieldRadius / miniMap) + player.prepos[i].x / miniMap), int(int(1050 - fieldRadius / miniMap) + player.prepos[i].y / miniMap), int(player.anchorRadius * miniMapPlayerSize / miniMap), int(player.anchorRadius * miniMapPlayerSize / miniMap), 0, RED, kFillModeSolid);
		}
		//範囲
		if (player.aimTimer > 0)Novice::DrawTriangle(int((30 + fieldRadius / miniMap) + player.prepos[0].x / miniMap), int(int(1050 - fieldRadius / miniMap) + player.prepos[0].y / miniMap), int((30 + fieldRadius / miniMap) + player.prepos[1].x / miniMap), int(int(1050 - fieldRadius / miniMap) + player.prepos[1].y / miniMap), int((30 + fieldRadius / miniMap) + player.prepos[2].x / miniMap), int(int(1050 - fieldRadius / miniMap) + player.prepos[2].y / miniMap), RED, kFillModeSolid);
		//プレイヤー
		Novice::DrawEllipse(int((30 + fieldRadius / miniMap) + player.pos.x / miniMap), int(int(1050 - fieldRadius / miniMap) + player.pos.y / miniMap), int(player.radius.x * miniMapPlayerSize / miniMap), int(player.radius.y * miniMapPlayerSize / miniMap), 0, WHITE, kFillModeSolid);

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
