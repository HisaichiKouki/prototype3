#include <Novice.h>
#include "Vector2.h"
#include "VectorPlus.h"
#include <corecrt_math.h>


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
}Player;

float clump(float a, float min, float max);



// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1920, 1080);

	Player player{};
	player.radius = { 30,30 };
	player.shotSpeed = 50;
	player.moveSpeed = 20;
	player.pos.x = 0;
	player.pos.y = 0;

	int joystickX = 0;
	int joystickY = 0;
	Novice::SetJoystickDeadZone(0, 0, 0);


	Vector2 xy{};
	float length = 0;
	float dedZone = 35;

	Vector2 scroll{};
	
	float fildRadius = 2000;
	Vector2 fildToPlayer{};


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

		if (player.velocityRatio > 0)
		{
			player.velocityRatio -= 0.01f;
			if (player.velocityRatio < 0)
			{
				player.velocityRatio = 0;
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
		//ボタン入力ここまで


		///スティック入力ここから
		Novice::GetAnalogInputLeft(0, &joystickX, &joystickY);

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
		if (player.trigerA)
		{
			player.velocity.x = player.direction.x * player.shotSpeed;
			player.velocity.y = player.direction.y * player.shotSpeed;

		}

		player.velocity.x *= player.velocityRatio;
		player.velocity.y *= player.velocityRatio;


		if (!player.aim)
		{
			player.velocity.x = player.direction.x * player.moveSpeed;
			player.velocity.y = player.direction.y * player.moveSpeed;
		}

		///スティック入力ここまで


		player.pos.x += player.velocity.x;
		player.pos.y += player.velocity.y;

		


		scroll.x = (-player.pos.x + 960);
		scroll.y = (-player.pos.y + 540);


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




		if (player.aim)Novice::DrawBox(0, 0, 1920, 1080, 0, 0x888888ff, kFillModeSolid);
		if (player.aimTimer > 0)Novice::DrawBox(0, 0, 1920, 1080, 0, 0xaa8888ff, kFillModeSolid);
		if (player.aimTimer > 0)Novice::DrawTriangle(int(player.prepos[0].x + scroll.x), int(player.prepos[0].y + scroll.y), int(player.prepos[1].x + scroll.x), int(player.prepos[1].y + scroll.y), int(player.prepos[2].x + scroll.x), int(player.prepos[2].y + scroll.y), RED, kFillModeSolid);

		Novice::DrawEllipse(int(100 + player.joystick.x), int(100 + player.joystick.y), 10, 10, 0, GREEN, kFillModeSolid);//直接入力
		Novice::DrawEllipse(int(100 + player.direction.x * 100), int(100 + player.direction.y * 100), 10, 10, 0, RED, kFillModeSolid);//方向
		Novice::DrawEllipse(int(100 + player.velocity.x), int(100 + player.velocity.y), 10, 10, 0, BLUE, kFillModeSolid);//速度
		Novice::DrawEllipse(100, 100, 100, 100, 0, WHITE, kFillModeWireFrame);//最大範囲
		Novice::DrawEllipse(100, 100, (int)dedZone, (int)dedZone, 0, GREEN, kFillModeWireFrame);//デッドゾーン


		Novice::DrawEllipse(int(scroll.x), int(scroll.y), (int)fildRadius, (int)fildRadius, 0, GREEN, kFillModeWireFrame);
		for (int i = 0; i < 3; i++)
		{
			Novice::DrawEllipse(int(player.prepos[i].x + scroll.x), int(player.prepos[i].y + scroll.y), 10, 10, 0, RED, kFillModeSolid);
		}
		Novice::DrawBox(int(-50 + scroll.x), int(-50 + scroll.y), 100, 100, 0, RED, kFillModeWireFrame);//スポーン地点
		Novice::DrawLine(int(player.pos.x + scroll.x), int(player.pos.y + scroll.y), int(player.pos.x + scroll.x + player.direction.x * 100), int(player.pos.y + scroll.y + player.direction.y * 100), WHITE);
		Novice::DrawEllipse(int(player.pos.x + scroll.x), int(player.pos.y + scroll.y), int(player.radius.x), int(player.radius.y), 0, GREEN, kFillModeSolid);//プレイヤー
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
