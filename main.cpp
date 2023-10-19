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
	float dedZone = 35;//こっちがほんとのデッドゾーン

	Vector2 scroll{};

	float fieldRadius = 2000;//フィールド半径
	Vector2 fieldToPlayer{};//フィールドの中心からプレイヤーまでのxy距離
	float centerToPlayerLength = 0;//フィールドの中心からプレイヤーまでのLength

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
			player.triangulAttack = true;//三角形攻撃を有効に
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
		if (player.trigerA && player.aimTimer == 0)
		{
			//移動入力がされている時
			if (player.direction.x!=0|| player.direction.y!=0)
			{
				//点がプレイヤーの位置に番号順で設置される
				player.prepos[player.count] = player.pos;
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
			if (player.flickLength > 32000&&!player.aim)
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
		if (preJoyStickX<dedZone * 120 && preJoyStickX > -dedZone * 120 && preJoyStickY<dedZone * 120 && preJoyStickY > -dedZone * 120)
		{
			//フリック判定フレをリセット
			if (!player.flick && player.flickCT == 0)player.flickTimer = 1;
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
		if (!player.aim && !player.flick&& player.aimTimer<=15)
		{
			//スティックで移動できるように
			player.velocity.x = player.direction.x * player.moveSpeed;
			player.velocity.y = player.direction.y * player.moveSpeed;
		}

		///スティック入力ここまで

		//ポジション移動
		player.pos.x += player.velocity.x;
		player.pos.y += player.velocity.y;


		//ここで攻撃処理をしたいと考えてる

		if (player.dashAttack)
		{

		}
		if (player.triangulAttack)
		{

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
		if(player.flick)Novice::DrawEllipse(int(player.pos.x + scroll.x), int(player.pos.y + scroll.y), int(player.radius.x), int(player.radius.y), 0, 0x00ffffff, kFillModeSolid);


		//ミニマップ
		Novice::DrawEllipse(int(30 + fieldRadius / miniMap), int(1050 - fieldRadius / miniMap), int(fieldRadius / miniMap), int(fieldRadius / miniMap), 0, GREEN, kFillModeWireFrame);
		for (int i = 0; i < 3; i++)
		{//ミニマップ三角形の点
			if (player.prepos[i].x != 0)Novice::DrawEllipse(int((30 + fieldRadius / miniMap) + player.prepos[i].x / miniMap), int(int(1050 - fieldRadius / miniMap) + player.prepos[i].y / miniMap), int(player.anchorRadius * miniMapPlayerSize / miniMap), int(player.anchorRadius * miniMapPlayerSize / miniMap), 0, RED, kFillModeSolid);
		}
		//ミニマップ範囲
		if (player.aimTimer > 0)Novice::DrawTriangle(int((30 + fieldRadius / miniMap) + player.prepos[0].x / miniMap), int(int(1050 - fieldRadius / miniMap) + player.prepos[0].y / miniMap), int((30 + fieldRadius / miniMap) + player.prepos[1].x / miniMap), int(int(1050 - fieldRadius / miniMap) + player.prepos[1].y / miniMap), int((30 + fieldRadius / miniMap) + player.prepos[2].x / miniMap), int(int(1050 - fieldRadius / miniMap) + player.prepos[2].y / miniMap), RED, kFillModeSolid);
		//ミニマッププレイヤー
		Novice::DrawEllipse(int((30 + fieldRadius / miniMap) + player.pos.x / miniMap), int(int(1050 - fieldRadius / miniMap) + player.pos.y / miniMap), int(player.radius.x * miniMapPlayerSize / miniMap), int(player.radius.y * miniMapPlayerSize / miniMap), 0, WHITE, kFillModeSolid);


		//攻撃有効時間を表示
		if (player.triangulAttack)
		{
			Novice::DrawBox(1820, 0, 100, 100, 0, RED, kFillModeSolid);
		}
		if (player.dashAttack)
		{
			Novice::DrawBox(1720, 0, 100, 100, 0, BLUE, kFillModeSolid);
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
