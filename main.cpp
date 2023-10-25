#include <Novice.h>
#include "Headers.h"
#include <corecrt_math.h>
#include "MatrixCalc.h"
#include "collision.h"
#include "Easing.h"
#include "randPlus.h"
#include<Xinput.h>


const char kWindowTitle[] = "トライダッシュ";

float clump(float a, float min, float max);


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1920, 1080);
	//Novice::SetWindowMode(kFullscreen);

	WORD vibeVolume = 0;
	PlayerData player{};
	player.radius = { 40,40 };
	player.shotSpeed = 60;
	player.moveSpeed = 20;
	player.pos.x = 0;
	player.pos.y = 0;
	player.anchorRadius = 10;
	int directionMove = 0;
	bool triFlag = false;

	//BGM
	int audioHandletitleBGM = Novice::LoadAudio("./Resources/Sounds/BGM/title.mp3");
	int soundHandle = -1;
	float setTitleBGMValume = 0.05f;//設定した音量になるまで音量が上がっていく
	float titleBGMValume = 0;

	int audioHandleResultBGM = Novice::LoadAudio("./Resources/Sounds/BGM/result.mp3");
	int ResultsoundHandle = -1;
	float setResultBGMValume = 0.05f;//設定した音量になるまで音量が上がっていく
	float ResultBGMValume = 0;

	int gameBGM = Novice::LoadAudio("./Resources/Sounds/BGM/gameBGM.mp3");
	//	bool gameBGMFlag = false;

		//SE
		//敵死亡
	int audioHandleDead = Novice::LoadAudio("./Resources/Sounds/boom.wav");
	bool deadAudioFlag = false;
	//敵リスポーン
	int audioHandleSpown = Novice::LoadAudio("./Resources/Sounds/spown.mp3");
	bool SpownAudioFlag = false;
	//ダッシュ
	int audioHandleDash = Novice::LoadAudio("./Resources/Sounds/dash.wav");
	bool dashAudioFlag = false;
	//三角形の点設置
	int audioHandlePointSet = Novice::LoadAudio("./Resources/Sounds/pointSet.mp3");
	bool pointsetAudioFlag = false;
	//三角攻撃時
	int audioHandleTriangle = Novice::LoadAudio("./Resources/Sounds/triangle.wav");
	bool TriangleAudioFlag = false;
	//エクセレント出した時
	int audioHandlesuccess = Novice::LoadAudio("./Resources/Sounds/success.wav");
	bool successAudioFlag = false;
	//チュートリアルのイラスト表示
	int audioHandlTaskClear = Novice::LoadAudio("./Resources/Sounds/success.wav");
	//bool TaskClearAudioFlag = false;
	//ゲームスタート
	int audioHandlGameStart = Novice::LoadAudio("./Resources/Sounds/success.wav");
	//bool TaskClearAudioFlag = false;
	//スコア(ゲージ)増えた時
	int audioHandlScoreUP = Novice::LoadAudio("./Resources/Sounds/success.wav");
	//bool TaskClearAudioFlag = false;
	//カウントダウン
	int audioHandlCountDown = Novice::LoadAudio("./Resources/Sounds/success.wav");
	//	bool CountDownAudioFlag = false;
	//フィニッシュの時
	int audioHandlFinish = Novice::LoadAudio("./Resources/Sounds/success.wav");
	//	bool CountDownAudioFlag = false;
	//リザルトで文字を表示する時
	int audioHandlScoreDisplay = Novice::LoadAudio("./Resources/Sounds/success.wav");
	//	bool CountDownAudioFlag = false;
	//リザルトでYESNO選択してる時
	int audioHandlYESNO = Novice::LoadAudio("./Resources/Sounds/success.wav");
	//	bool CountDownAudioFlag = false;
	//YESNO確定したとき
	int audioHandlSelect = Novice::LoadAudio("./Resources/Sounds/success.wav");
	bool SelectAudioFlag = false;
	//フィーバー始まり
	int audioHandlFeverStart = Novice::LoadAudio("./Resources/Sounds/success.wav");
	//	bool FeverStartAudioFlag = false;
		//フィーバー終わり
	int audioHandlFeverEnd = Novice::LoadAudio("./Resources/Sounds/success.wav");
	bool FeverEndAudioFlag = false;


	Original::LoadAudio("./Resources/Sounds/boom.wav", "enemyDead", 0.03f);
	Original::LoadAudio("./Resources/Sounds/pi.wav", "pi", 0.3f);
	Original::LoadAudio("./Resources/Sounds/spown.mp3", "enemySpawn", 0.1f);
	Original::LoadAudio("./Resources/Sounds/dash.wav", "playerDash", 0.1f);


	int excellentTexture[5]{};

	excellentTexture[0] = Novice::LoadTexture("./Resources/Images/excellent_1.png");
	excellentTexture[1] = Novice::LoadTexture("./Resources/Images/excellent_2.png");
	excellentTexture[2] = Novice::LoadTexture("./Resources/Images/excellent_3.png");
	excellentTexture[3] = Novice::LoadTexture("./Resources/Images/excellent_4.png");
	excellentTexture[4] = Novice::LoadTexture("./Resources/Images/excellent_5.png");


	int scoreNumsHandle[10] = {
		Novice::LoadTexture("./Resources/Images/number/num_0.png"),
		Novice::LoadTexture("./Resources/Images/number/num_1.png"),
		Novice::LoadTexture("./Resources/Images/number/num_2.png"),
		Novice::LoadTexture("./Resources/Images/number/num_3.png"),
		Novice::LoadTexture("./Resources/Images/number/num_4.png"),
		Novice::LoadTexture("./Resources/Images/number/num_5.png"),
		Novice::LoadTexture("./Resources/Images/number/num_6.png"),
		Novice::LoadTexture("./Resources/Images/number/num_7.png"),
		Novice::LoadTexture("./Resources/Images/number/num_8.png"),
		Novice::LoadTexture("./Resources/Images/number/num_9.png")
	};

	// スコア用のクラス変数 ### ScoreClass
	ScoreObject scoreObj;

	// 色設定
	scoreObj.SetColor(0xffffffff);
	// サイズ設定
	scoreObj.SetSize(108, 168);
	// テクスチャハンドルセット
	scoreObj.SetScoreTextureHandle(scoreNumsHandle);
	// 最大桁数を指定
	scoreObj.SetMaxDigit(6);

	int preScore = 0;
	Vector2 cameraEasePos{};
	float cameraEaseT = 0.35f;
	const int kPreNum = 15;
	Vector2 playerPrePos[kPreNum]{};
	unsigned int prePosColor[kPreNum]{};
	for (int i = 0; i < kPreNum; i++)
	{
		prePosColor[i] = WHITE;
	}
	int joystickX = 0;
	int joystickY = 0;
	int preJoyStickX = 0;
	int preJoyStickY = 0;
	int flickRength = 70;

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
	//int enemyTexture = Novice::LoadTexture("./Resources/images/enemy_1.png");
	//int enemyTexture2 = Novice::LoadTexture("./Resources/images/enemy_2.png");

	int enemyTexture[6]{};
	enemyTexture[0] = Novice::LoadTexture("./Resources/enemy/enemy_6.png");
	enemyTexture[1] = Novice::LoadTexture("./Resources/enemy/enemy_5.png");
	enemyTexture[2] = Novice::LoadTexture("./Resources/enemy/enemy_4.png");
	enemyTexture[3] = Novice::LoadTexture("./Resources/enemy/enemy_3.png");
	enemyTexture[4] = Novice::LoadTexture("./Resources/enemy/enemy_2.png");
	enemyTexture[5] = Novice::LoadTexture("./Resources/enemy/enemy_1.png");

	int quickNum = -1;

	int enemyTextureSize = 128;

	float enemyRadius = 64;//敵全体の半径
	const int kDedTimer = 30;
#pragma region enemy1//小さい三角形

	testEnemy1 enemy1{};
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
	enemy7.count = 0;
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
	const int kenemy10Num = 8;
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
	const int kenemy11Num = 15;
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
	const int kenemy12Num = 8;
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
	testEnemy7 enemy13{};
	enemy13.length = 1360;

	int enemyDeadTexture = Novice::LoadTexture("./Resources/Images/enemyDead.png");

	Count dash{};//ダッシュ攻撃で倒した数
	Count triangle{};//三角攻撃で倒した数
	float count = 0;//全体で倒した数
	float endCount = 0;//フィーバーごとにリセット
	float precount = 0;

	int dashCount = 0;
	int trieCount = 0;

	Ennergy ennergy{};
	ennergy.dashRate = 3;//ダッシュは三角の数字倍貯まる
	ennergy.triangleRate = 1.0f;
	ennergy.powerUp = 2.0f;//移動速度が倍になる
	ennergy.max = 100;
	ennergy.fever = false;
	ennergy.feverTime = 300;//フレーム数 実際にはちょっと長くなる
	ennergy.damage = 0.2f;//攻撃していない時に敵に当たった時ダメージの代わりにゲージが減る

	Vector2 plth{};
	float Playerth = 0;

	int shakeRadius = 10;
	int shakeGaugeX = shakeRadius;
	int shakeGaugeY = shakeRadius;
	int shakeGaugeSeed = 1;
	Vector2 shakeGaugePos{};

	float screenSize = 1;//フィーバー中画面を小さくする
	float setScreenSize = 2.3f;//数字が大きいほど画面が小さくなる
	float setScreenEaseT = 0;
	float setScreenEaseTChange = 1;


	bool allReset = true;

	int randSpown = 0;
	int preRandSpown = 0;
	int prepreRandSpown = 0;
	int pre3RandSpown = 0;
	const int variation = 5;
	bool patterrnIsAlive[variation]{};
	bool quickFlag = false;//範囲外に行く前に全員倒したら次の出現が早くなる
	int quickTimer = 0;

	int gameTimer = 0;




	int score = 0;//これがスコア
	int plusScore = 10;//1体何点か
	bool scoreAdd = false;

	float bigNumEaseT = 0;
	float bigNumSize = 0;
	int bigNumTexture[5]{};
	int bigNumCount = 0;
	unsigned int bigNumColor[5]{};
	bigNumColor[0] = 0xffffff66;
	bigNumColor[1] = 0xffffff22;
	bigNumColor[2] = 0xffffff33;
	bigNumColor[3] = 0xffffff44;
	bigNumColor[4] = 0xffffff77;

	bigNumTexture[0] = Novice::LoadTexture("./Resources/font/bignum_5.png");
	bigNumTexture[1] = Novice::LoadTexture("./Resources/font/bignum_4.png");
	bigNumTexture[2] = Novice::LoadTexture("./Resources/font/bignum_3.png");
	bigNumTexture[3] = Novice::LoadTexture("./Resources/font/bignum_2.png");
	bigNumTexture[4] = Novice::LoadTexture("./Resources/font/bignum_1.png");

	float finishEaseT = 0;
	float finishSize = 0;
	float finishEaseT2 = 0;
	float finishSize2 = 0;
	float finishEase = 0;
	float finishEase2 = 0;
	int finishTexture = Novice::LoadTexture("./Resources/font/finish.png");

	int speedUpTexture = Novice::LoadTexture("./Resources/Images/speedup.png");

	float blackEaseT = 0;
	float blackEaseT2 = 0;
	unsigned int blackColor = 0;
	unsigned int blackColor2 = 0;

	int clearTimer = 0;

	int game = 0;

	enum TypeGame
	{
		kTypeGameTitle,
		kTypeGameGame,
		kTypeGameResult,
	};

	// デバッグ用 ### MustDelete
	game = kTypeGameTitle;

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


	bool titleWalkFlag = false;
	bool titleDashFlag = false;
	bool titleTriangleFlag = false;
	bool titleNewGameFlag = false;
	bool startNewGame = false;

	int titleCount = 0;

	bool whileFlag = false;
	int walkTimer = 0;
	int whileTime = 60;//最初のタスクの間と間の時間

	int gamestartTexture = Novice::LoadTexture("./Resources/font/gamestart.png");
	float gamestartEaseT = 0;
	float gamestartEaseT2 = 0;
	float gamestartPos = 0;
	float gamestartPos2 = 0;
	bool titleBackFlag = false;

	int directionTexture = Novice::LoadTexture("./Resources/images/direction.png");
	float directionTheta = 0;

	int gaugeBackTexture = Novice::LoadTexture("./Resources/images/gauge_back.png");
	int gaugeTexture = Novice::LoadTexture("./Resources/images/gauge.png");

	int gaugex = 1330;
	int gaugey = 730 + 600;
	int gaugeWidth = 512;
	int gaugeHeidth = 288;
	float gaugeEaseT = 0;
	float gaugeEasePos = 0;
	int playerTexture = Novice::LoadTexture("./Resources/images/player.png");

	int informationTexture[3]{};
	informationTexture[0] = Novice::LoadTexture("./Resources/images/information_1.png");
	informationTexture[1] = Novice::LoadTexture("./Resources/images/information_2.png");
	informationTexture[2] = Novice::LoadTexture("./Resources/images/information_3.png");

	int infoFont[5]{};
	infoFont[0] = Novice::LoadTexture("./Resources/font/info_font_1.png");
	infoFont[1] = Novice::LoadTexture("./Resources/font/info_font_2.png");
	infoFont[2] = Novice::LoadTexture("./Resources/font/info_font_3.png");
	infoFont[3] = Novice::LoadTexture("./Resources/font/info_font_4.png");
	infoFont[4] = Novice::LoadTexture("./Resources/font/info_font_5.png");

	float informationEaseT = 0;
	float informationEase = 0;

	float infoFontEaseT = 0;
	float infoFontEase = 0;

	float UIPosy = 0;
	bool transitionFlag = false;//リザルト画面でタイトルに戻るかゲームスタートかを一度のみ判定をとるため
	float UIBackEaseT = 0;
	float UIBackPos = 0;

	int choice = 0;//0ならリスタート1ならタイトルに戻る
	int prechoice = 0;//0ならリスタート1ならタイトルに戻る
	int choiceTimer = 0;//スティックの跳ね返りで反対に行かないように


	int scoreTexture = Novice::LoadTexture("./Resources/font/SCORE.png");
	int killTexture = Novice::LoadTexture("./Resources/font/KILL.png");
	int continueTexture = Novice::LoadTexture("./Resources/font/CONTINUE.png");
	int yesTexture = Novice::LoadTexture("./Resources/font/YES.png");
	int noTexture = Novice::LoadTexture("./Resources/font/NO.png");

	float resultEaseT[4]{};//スコア　キル数　	CONTENUE　YESNO	
	float resultEasePos[4]{};

	int resultBackTexture = Novice::LoadTexture("./Resources/images/resultBack.png");
	int resultBackPos[2] = { 0,-1080 };

	float yesnoEaseT = 0;
	float yesnoEaseSize = 0;

	float continueEaseT = 0;
	float continueEasePos = 0;
	float continueEaseChange = 1;

	float scoreEaseT = 0;
	float scorePos = 0;
	float scoreEaseChange = -1;

	int contorollerTexture = Novice::LoadTexture("./Resources/Images/contoroller.png");
	int connectTexture = Novice::LoadTexture("./Resources/Images/connect.png");

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
		/// 

		//Novice::ScreenPrintf(0, 0, "%d", );
		if (Novice::GetNumberOfJoysticks() == 0)
		{
			Novice::DrawBox(0, 0, 1920, 1080, 0, 0x3333aaff, kFillModeSolid);
			Functions.DrawQuadPlus(960, 500, 794, 635, 1, 1, 0, 0, 0, 794, 635, contorollerTexture, WHITE, "center");
			Functions.DrawQuadPlus(960, 800, 1004, 99, 1, 1, 0, 0, 0, 1004, 99, connectTexture, WHITE, "center");
		}
		else
		{

			XINPUT_VIBRATION vibration;
			vibeVolume = 0;
			preScore = score;

			switch (game)
			{

			case kTypeGameTitle:
				if (titleBackFlag)
				{
					if (blackEaseT2 > 0)
					{
						blackEaseT2 -= 1.0f;
						blackColor2 = int(easeInExpo(blackEaseT2 / 100) * 255);


					}
					if (blackEaseT2 <= 0)
					{
						blackEaseT2 = 0;
						allReset = true;
						titleBackFlag = false;
					}
				}
				else
				{


					if (allReset)
					{
						choice = 0;
						prechoice = 0;
						for (int i = 0; i < 4; i++)
						{
							resultEasePos[i] = 0;
							resultEaseT[i] = 0;
						}
						SelectAudioFlag = false;
						preScore = 0;
						directionMove = 0;
						gaugeEaseT = 0;
						gaugeEasePos = 0;
						UIPosy = 0;
						Novice::SetJoystickDeadZone(0, 0, 0);
						gaugey = 730 + 600;
						gamestartEaseT = 0;
						gamestartEaseT2 = 0;
						gamestartPos = 0;
						gamestartPos2 = 0;
						titleCount = 0;
						titleNewGameFlag = false;
						startNewGame = false;
						whileFlag = false;
						titleWalkFlag = true;
						titleDashFlag = false;
						titleTriangleFlag = false;

						walkTimer = 0;
						whileTime = 60;//最初のタスクの間と間の時間

						finishSize2 = 0;
						finishEaseT2 = 0;
						clearTimer = 0;
						fieldToPlayer = { 0,0 };
						cameraEasePos = { 0,0 };
						for (int i = 0; i < kPreNum; i++)
						{
							playerPrePos[i] = { 0,0 };
						}
						player.preDirection = { 0,0 };
						enemy2.timer = 0;
						enemy7.timer = 0;
						enemy8.timer = 0;
						enemy9.timer = 0;
						quickFlag = false;
						quickTimer = 0;
						finishEaseT = 0;
						finishSize = 0;
						bigNumEaseT = 0;
						bigNumSize = 0;
						bigNumCount = 0;
						score = 0;
						plusScore = 10;
						scoreAdd = false;
						for (int i = 0; i < variation; i++)
						{
							patterrnIsAlive[i] = false;
						}
						shakeGaugePos = { 0,0 };
						shakeGaugeX = shakeRadius;
						shakeGaugeY = shakeRadius;
						shakeGaugeSeed = 1;
						screenSize = 1.0f;
						setScreenEaseT = 0;
						setScreenEaseTChange = 1;
						ennergy.count = 0;
						ennergy.damageFlag = false;
						ennergy.dash = false;
						ennergy.triangle = false;
						ennergy.fever = false;
						dash.count = 0;
						dash.endCount = 0;
						dash.preCount = 0;
						count = 0;
						precount = 0;
						endCount = 0;
						triangle.count = 0;
						triangle.endCount = 0;
						triangle.preCount = 0;
#pragma region enemyReset

						enemy1.radius = 0;
						enemy1.easeT = 0;
						enemy1.parentIsAlive = false;

						for (int i = 0; i < 3; i++)
						{
							enemy1.childIsAlive[i] = false;
							enemy1.dedTimer[i] = 0;
							enemy1.isDed[i] = false;
						}

						enemy2.radius = 0;
						enemy2.easeT = 0;
						enemy2.hostIsAlive = false;
						enemy2.count = 0;

						for (int j = 0; j < 4; j++)
						{
							enemy2.parentIsAlive[j] = false;
							for (int i = 0; i < kEnemy2Num; i++)
							{
								enemy2.countFlag[j][i] = false;
								enemy2.childIsAlive[j][i] = false;
								enemy2.dedTimer[j][i] = 0;
								enemy2.isDed[j][i] = false;
							}
						}
						enemy5.easeT = 0;
						enemy5.radius = 0;
						enemy5.parentIsAlive = false;
						for (int i = 0; i < 4; i++)
						{
							enemy5.childIsAlive[i] = false;
							enemy5.dedTimer[i] = 0;
							enemy5.isDed[i] = false;
						}
						enemy6.easeT = 0;
						enemy6.radius = 0;
						enemy6.parentIsAlive = false;
						for (int i = 0; i < 4; i++)
						{
							enemy6.childIsAlive[i] = false;
							enemy6.dedTimer[i] = 0;
							enemy6.isDed[i] = false;
						}
						enemy7.radius = 0;
						enemy7.easeT = 0;
						enemy7.count = 0;

						for (int j = 0; j < 8; j++)
						{
							enemy7.parentIsAlive[j] = false;
							for (int i = 0; i < 4; i++)
							{
								enemy7.countFlag[j][i] = false;
								enemy7.childIsAlive[j][i] = false;
								enemy7.dedTimer[j][i] = 0;
								enemy7.isDed[j][i] = false;
							}

						}
						enemy7.hostIsAlive = false;
						enemy8.radius = 0;
						enemy8.easeT = 0;
						enemy8.count = 0;
						enemy8.timer = 0;
						for (int j = 0; j < 4; j++)
						{
							enemy8.parentIsAlive[j] = false;
							for (int i = 0; i < 12; i++)
							{
								enemy8.countFlag[j][i] = false;
								enemy8.childIsAlive[j][i] = false;
								enemy8.dedTimer[j][i] = 0;
								enemy8.isDed[j][i] = false;
							}
						}
						enemy8.hostIsAlive = false;

						enemy9.radius = 0;
						enemy9.easeT = 0;
						enemy9.count = 0;
						for (int j = 0; j < 2; j++)
						{
							enemy9.parentIsAlive[j] = false;
							for (int i = 0; i < 8; i++)
							{
								enemy9.countFlag[j][i] = false;
								enemy9.childIsAlive[j][i] = false;
								enemy9.dedTimer[j][i] = 0;
								enemy9.isDed[j][i] = false;
							}
						}
						enemy9.hostIsAlive = false;
						for (int j = 0; j < kenemy10Num; j++)
						{

							enemy10[j].parentIsAlive = false;

							for (int i = 0; i < 4; i++)
							{
								enemy10[j].childIsAlive[i] = false;
								enemy10[j].dedTimer[i] = 0;
								enemy10[j].isDed[i] = false;
							}

						}
						for (int j = 0; j < kenemy11Num; j++)
						{

							enemy11[j].radius = 0;
							enemy11[j].easeT = 0;
							enemy11[j].parentIsAlive = false;

							for (int i = 0; i < 3; i++)
							{
								enemy11[j].childIsAlive[i] = false;
								enemy11[j].dedTimer[i] = 0;
								enemy11[j].isDed[i] = false;
							}

						}
						for (int j = 0; j < kenemy12Num; j++)
						{

							enemy12[j].radius = 0;
							enemy12[j].easeT = 0;
							enemy12[j].parentIsAlive = false;
							for (int i = 0; i < 4; i++)
							{
								enemy12[j].childIsAlive[i] = false;
								enemy12[j].dedTimer[i] = 0;
								enemy12[j].isDed[i] = false;
							}

						}
						enemy13.hostIsAlive = false;
						enemy13.count = 0;
						for (int i = 0; i < 60; i++)
						{
							enemy13.countFlag[i] = false;
							enemy13.easeT[i] = 0;
							enemy13.radius[i] = 0;
							enemy13.dedTimer[i] = 0;
							enemy13.childIsAlive[i] = false;
							enemy13.dedTimer[i] = 0;
							enemy13.isDed[i] = false;
							enemy13.z[i] = i * 6.0f;
							enemy13.thta[i] = (enemy13.z[i] / 180) * 3.1415f;
							enemy13.relativePos[i].x = float(cosf(enemy13.thta[i]) - sinf(enemy13.thta[i])) * enemy13.length;
							enemy13.relativePos[i].y = float(cosf(enemy13.thta[i]) + sinf(enemy13.thta[i])) * enemy13.length;
						}
#pragma endregion

						player.pos = { 0,0 };
						player.velocity = { 0,0 };
						player.aim = false;
						player.aimTimer = 0;
						player.count = 0;
						player.dashAttack = false;
						player.flick = false;
						player.flickCT = 0;
						player.flickLength = 0;
						player.flickTimer = 0;
						for (int i = 0; i < 3; i++)
						{
							player.prepos[i] = { 0,0 };
						}
						player.triangulAttack = 0;
						player.trigerA = 0;
						player.velocityRatio = 0;


						gameTimer = 0;


						allReset = false;
					}

#pragma region Player
					for (int i = kPreNum - 1; i > 0; i--)
					{
						prePosColor[i] = prePosColor[i - 1];
						playerPrePos[i] = playerPrePos[i - 1];
					}

					if (player.dashAttack)
					{
						prePosColor[0] = 0x00ffff99;

					}
					else if (player.triangulAttack || player.aim)
					{
						prePosColor[0] = 0x55ff5577;

					}
					else
					{
						prePosColor[0] = 0xffffff99;

					}
					playerPrePos[0] = player.pos;
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

						if (player.aim && player.count >= 2 && player.velocityRatio <= 0.80f)
						{
							triFlag = true;
						}
						if (player.aim && player.count >= 1 && player.velocityRatio <= 0.78f)
						{
							player.prepos[player.count] = player.pos;
						}
					}

					//Novice::ScreenPrintf(0, 0, "player.velocityRatio=%f", player.velocityRatio);
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





					//攻撃終わりにAボタンを押した時
					if (player.trigerA && player.aimTimer == 0 && !player.dashAttack || triFlag)
					{
						//移動入力がされている時
						pointsetAudioFlag = true;
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
							TriangleAudioFlag = true;
							//三点設置したら設置フェーズをおわる
							player.count = 0;
							player.aim = false;
							player.aimTimer = 30;//攻撃時間を代入
							player.triangulAttack = true;//三角形攻撃を有効に
							triFlag = false;

						}



					}

					//三角形を出した後の攻撃時間
					if (player.aimTimer > 0)
					{
						player.aimTimer--;
						if (player.aimTimer <= 0)
						{
							player.aimTimer = 0;
							player.triangulAttack = false;

							for (int i = 0; i < 3; i++)
							{
								//三角形の点
								//今は原点に戻してるけど意味はない
								player.prepos[i] = { 0,0 };
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
					//length = sqrtf(xy.x * xy.x + xy.y * xy.y);
					length = clump(sqrtf(xy.x * xy.x + xy.y * xy.y), 0, 100);

					//長さがデッドゾーンを超えたら方向を代入
					if (length >= dedZone && !player.flick)
					{
						//方向を正規化。速度をかけるだけで使えるようにするため
						player.direction = vectorNormalize(player.joystick, { 0,0 });
						player.preDirection = player.direction;
					}
					else
					{
						player.direction = { 0,0 };
					}


					//スティックのポジションがデッドゾーンの中の時
					if (length <= 17)//preJoyStickX<dedZone * 50 && preJoyStickX > -dedZone * 50 && preJoyStickY<dedZone * 50 && preJoyStickY > -dedZone * 50
					{
						//フリック判定フレをリセット
						if (!player.flick && player.flickCT == 0)player.flickTimer = 3;
					}


					//はじき判定
					//フリック判定フレが0以上の時
					if (player.flickTimer > 0)
					{
						player.flickTimer--;
						//スティックが端に行ったら
						if (length >= flickRength && !player.aim && !player.triangulAttack)
						{
							//フリックをTRUEに
							player.flick = true;
							//突撃フラグを有効に
							player.dashAttack = true;
							//速度を等倍に
							player.velocityRatio = 1;
							dashAudioFlag = true;
						}
					}



					//Aボタンまたはフリックしたとき
					if (player.trigerA && player.aim || player.flick && player.flickCT == 0)
					{
						//方向に発射速度をかける
						player.velocity.x = player.preDirection.x * player.shotSpeed;
						player.velocity.y = player.preDirection.y * player.shotSpeed;

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
					if (!player.aim && !player.flick)//&& player.aimTimer <= 5
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

					if (player.preDirection.x != 0 || player.preDirection.y != 0)
					{
						directionTheta = atan2f(float(player.preDirection.x), float(-player.preDirection.y));

					}

					///スティック入力ここまで
#pragma endregion


			//動きの確認
			//移動
					if (titleWalkFlag)
					{
						if (length >= dedZone)
						{
							walkTimer++;
							if (walkTimer >= 30)
							{
								whileFlag = true;
							}
						}
					}

					//フラグがたったら敵を出す
					if (titleDashFlag || titleTriangleFlag || titleNewGameFlag)
					{
						if (!enemy1.parentIsAlive)
						{
							enemy1.radius = 0;
							enemy1.easeT = 0;
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

						if (enemy1.parentIsAlive)
						{
							enemy1.radius = easeOutElastic(enemy1.easeT / 100) * enemyRadius;
							if (enemy1.easeT < 100)enemy1.easeT += 2;
							if (enemy1.easeT > 100)
							{
								enemy1.easeT = 100;
								titleDashFlag = false;
								titleTriangleFlag = false;
							}

						}

					}


					//ダッシュ攻撃
					if (titleCount == 1)
					{
						if (enemy1.parentIsAlive && player.dashAttack && !player.triangulAttack && enemy1.easeT > 2)
						{
							for (int i = 0; i < 3; i++)
							{
								if (!enemy1.isDed[i])enemy1.isDed[i] = EllipseCollision(player.pos, player.radius.x, enemy1.relativePos[i], enemy1.radius);

							}
						}
					}//三角攻撃
					else if (titleCount == 2)
					{
						if (enemy1.parentIsAlive && player.triangulAttack && !player.dashAttack && enemy1.easeT > 2)
						{
							for (int i = 0; i < 3; i++)
							{
								if (!enemy1.isDed[i])enemy1.isDed[i] = attAreaObj.TriangleCollision(enemy1.relativePos[i]);
							}

						}
					}//NEWGAME
					else if (titleCount == 3)
					{
						if (enemy1.parentIsAlive && player.dashAttack)
						{
							for (int i = 0; i < 3; i++)
							{
								if (!enemy1.isDed[i])enemy1.isDed[i] = EllipseCollision(player.pos, player.radius.x, enemy1.relativePos[i], enemy1.radius);
							}
						}
						else if (enemy1.parentIsAlive && player.triangulAttack)
						{
							for (int i = 0; i < 3; i++)
							{
								if (!enemy1.isDed[i])enemy1.isDed[i] = attAreaObj.TriangleCollision(enemy1.relativePos[i]);
							}

						}

					}

					//敵攻撃用
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
								if (titleCount != 3)whileFlag = true;
								if (titleCount == 1)titleDashFlag = false;
								if (titleCount == 2)titleTriangleFlag = false;
								if (titleCount == 3)
								{
									titleNewGameFlag = false;
									startNewGame = true;
								}

							}
						}
					}



					//フェーズが終わった時すぐに次が出ないように１秒時間を開ける
					if (whileFlag)
					{
						whileTime--;
						if (whileTime == 0)
						{
							whileTime = 60;
							whileFlag = false;
							titleCount++;
							informationEaseT = 0;
							informationEase = 0;
							infoFontEase = 0;
							infoFontEaseT = 0;
							if (titleCount == 1)
							{
								titleDashFlag = true;
								titleWalkFlag = false;

							}
							else if (titleCount == 2)
							{
								titleTriangleFlag = true;
							}
							else if (titleCount == 3)
							{
								titleNewGameFlag = true;
							}

						}
					}


					//ポジション移動
					player.pos.x += player.velocity.x;
					player.pos.y += player.velocity.y;


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


					cameraEasePos.x = (1.0f - cameraEaseT) * cameraEasePos.x + float(player.pos.x) * cameraEaseT;
					cameraEasePos.y = (1.0f - cameraEaseT) * cameraEasePos.y + float(player.pos.y) * cameraEaseT;


					//スクロールの値を代入
					scroll.x = (-cameraEasePos.x / screenSize + 960);
					scroll.y = (-cameraEasePos.y / screenSize + 540);

					if (startNewGame)
					{
						if (gamestartEaseT >= 100)

						{
							Playerth = 0;

							game = 1;
							setScreenEaseTChange = -1;
							player.preDirection = { 0,0 };
							player.pos = { 0,0 };
						}
						//スクリーン拡縮処理
						screenSize = easeOutCubic(setScreenEaseT / 100) * (70 - 1) + 1;//easeOutBack
						if (setScreenEaseT < 100 || setScreenEaseT > 0)
						{
							setScreenEaseT += setScreenEaseTChange * 2;
							if (setScreenEaseT > 100)setScreenEaseT = 100;
							if (setScreenEaseT < 0)setScreenEaseT = 0;
						}
						gamestartPos = easeOutQuart(gamestartEaseT / 100) * 2192;
						if (gamestartEaseT < 100)gamestartEaseT += 2;

					}




					if (gamestartEaseT == 100)
					{
						gamestartPos2 = easeInQuart(gamestartEaseT2 / 100) * 2192;

						if (gamestartEaseT2 < 100)gamestartEaseT2 += 2;

					}

					if (startNewGame)
					{
						gameTimer = 0;

					}
					else
					{
						gameTimer++;
						//directionMove++;
					}
				}
				break;

			case kTypeGameGame:


				if (startNewGame)
				{
					if (gamestartEaseT2 >= 100)

					{
						allReset = true;
						startNewGame = false;

					}
					//スクリーン拡縮処理
					screenSize = easeOutCubic(setScreenEaseT / 100) * (70 - 1) + 1;//easeOutBack
					if (setScreenEaseT < 100 || setScreenEaseT > 0)
					{
						setScreenEaseT += setScreenEaseTChange * 2;
						if (setScreenEaseT > 100)setScreenEaseT = 100;
						if (setScreenEaseT < 0)setScreenEaseT = 0;
					}
					gamestartPos2 = easeOutQuart(gamestartEaseT2 / 100) * 2192;
					if (gamestartEaseT2 < 100)gamestartEaseT2 += 2;


					gaugeEasePos = easeOutQuint(gaugeEaseT / 100) * 600;
					UIPosy = easeOutQuint(gaugeEaseT / 100) * 200;
					if (gaugeEaseT < 100)gaugeEaseT += 2;
				}
				else
				{



#pragma region Game
					if (allReset)
					{
						scoreEaseT = 0;
						scorePos = 0;
						scoreEaseChange = -1;
						yesnoEaseT = 0;
						yesnoEaseSize = 0;

						continueEaseT = 0;
						continueEasePos = 0;
						continueEaseChange = 1;
						startNewGame = false;
						titleBackFlag = false;
						choice = 0;
						prechoice = 0;
						SelectAudioFlag = false;
						preScore = 0;
						directionMove = 0;
						resultBackPos[0] = 0;
						resultBackPos[1] = -1080;
						for (int i = 0; i < 4; i++)
						{
							resultEasePos[i] = 0;
							resultEaseT[i] = 0;
						}
						dashCount = 0;
						trieCount = 0;
						choice = 0;
						transitionFlag = false;
						Novice::SetJoystickDeadZone(0, 0, 0);
						UIBackEaseT = 0;
						UIBackPos = 0;
						gamestartEaseT = 0;
						gamestartEaseT2 = 0;
						gamestartPos = 0;
						gamestartPos2 = 0;
						titleCount = 0;
						titleNewGameFlag = false;
						startNewGame = false;
						whileFlag = false;
						titleWalkFlag = true;
						titleDashFlag = false;
						titleTriangleFlag = false;

						walkTimer = 0;
						whileTime = 60;//最初のタスクの間と間の時間

						finishSize2 = 0;
						finishEaseT2 = 0;
						finishEase = 0;
						finishEase2 = 0;
						clearTimer = 0;
						fieldToPlayer = { 0,0 };
						cameraEasePos = { 0,0 };
						for (int i = 0; i < kPreNum; i++)
						{
							playerPrePos[i] = { 0,0 };
						}
						player.preDirection = { 0,0 };
						enemy2.timer = 0;
						enemy7.timer = 0;
						enemy8.timer = 0;
						enemy9.timer = 0;
						quickFlag = false;
						quickTimer = 0;
						finishEaseT = 0;
						finishSize = 0;
						bigNumEaseT = 0;
						bigNumSize = 0;
						bigNumCount = 0;
						score = 0;
						plusScore = 10;
						scoreAdd = false;
						for (int i = 0; i < variation; i++)
						{
							patterrnIsAlive[i] = false;
						}
						shakeGaugePos = { 0,0 };
						shakeGaugeX = shakeRadius;
						shakeGaugeY = shakeRadius;
						shakeGaugeSeed = 1;
						screenSize = 1.0f;
						setScreenEaseT = 0;
						setScreenEaseTChange = 1;
						ennergy.count = 0;
						ennergy.damageFlag = false;
						ennergy.dash = false;
						ennergy.triangle = false;
						ennergy.fever = false;
						dash.count = 0;
						dash.endCount = 0;
						dash.preCount = 0;
						count = 0;
						precount = 0;
						endCount = 0;
						triangle.count = 0;
						triangle.endCount = 0;
						triangle.preCount = 0;
#pragma region enemyReset
						enemy2.radius = 0;
						enemy2.easeT = 0;
						enemy2.hostIsAlive = false;
						enemy2.count = 0;

						for (int j = 0; j < 4; j++)
						{
							enemy2.parentIsAlive[j] = false;
							for (int i = 0; i < kEnemy2Num; i++)
							{
								enemy2.countFlag[j][i] = false;
								enemy2.childIsAlive[j][i] = false;
								enemy2.dedTimer[j][i] = 0;
								enemy2.isDed[j][i] = false;
							}
						}
						enemy5.easeT = 0;
						enemy5.radius = 0;
						enemy5.parentIsAlive = false;
						for (int i = 0; i < 4; i++)
						{
							enemy5.childIsAlive[i] = false;
							enemy5.dedTimer[i] = 0;
							enemy5.isDed[i] = false;
						}
						enemy6.easeT = 0;
						enemy6.radius = 0;
						enemy6.parentIsAlive = false;
						for (int i = 0; i < 4; i++)
						{
							enemy6.childIsAlive[i] = false;
							enemy6.dedTimer[i] = 0;
							enemy6.isDed[i] = false;
						}
						enemy7.radius = 0;
						enemy7.easeT = 0;
						enemy7.count = 0;

						for (int j = 0; j < 8; j++)
						{
							enemy7.parentIsAlive[j] = false;
							for (int i = 0; i < 4; i++)
							{
								enemy7.countFlag[j][i] = false;
								enemy7.childIsAlive[j][i] = false;
								enemy7.dedTimer[j][i] = 0;
								enemy7.isDed[j][i] = false;
							}

						}
						enemy7.hostIsAlive = false;
						enemy8.radius = 0;
						enemy8.easeT = 0;
						enemy8.count = 0;
						enemy8.timer = 0;
						for (int j = 0; j < 4; j++)
						{
							enemy8.parentIsAlive[j] = false;
							for (int i = 0; i < 12; i++)
							{
								enemy8.countFlag[j][i] = false;
								enemy8.childIsAlive[j][i] = false;
								enemy8.dedTimer[j][i] = 0;
								enemy8.isDed[j][i] = false;
							}
						}
						enemy8.hostIsAlive = false;

						enemy9.radius = 0;
						enemy9.easeT = 0;
						enemy9.count = 0;
						for (int j = 0; j < 2; j++)
						{
							enemy9.parentIsAlive[j] = false;
							for (int i = 0; i < 8; i++)
							{
								enemy9.countFlag[j][i] = false;
								enemy9.childIsAlive[j][i] = false;
								enemy9.dedTimer[j][i] = 0;
								enemy9.isDed[j][i] = false;
							}
						}
						enemy9.hostIsAlive = false;
						for (int j = 0; j < kenemy10Num; j++)
						{

							enemy10[j].parentIsAlive = false;

							for (int i = 0; i < 4; i++)
							{
								enemy10[j].childIsAlive[i] = false;
								enemy10[j].dedTimer[i] = 0;
								enemy10[j].isDed[i] = false;
							}

						}
						for (int j = 0; j < kenemy11Num; j++)
						{

							enemy11[j].radius = 0;
							enemy11[j].easeT = 0;
							enemy11[j].parentIsAlive = false;

							for (int i = 0; i < 3; i++)
							{
								enemy11[j].childIsAlive[i] = false;
								enemy11[j].dedTimer[i] = 0;
								enemy11[j].isDed[i] = false;
							}

						}
						for (int j = 0; j < kenemy12Num; j++)
						{

							enemy12[j].radius = 0;
							enemy12[j].easeT = 0;
							enemy12[j].parentIsAlive = false;
							for (int i = 0; i < 4; i++)
							{
								enemy12[j].childIsAlive[i] = false;
								enemy12[j].dedTimer[i] = 0;
								enemy12[j].isDed[i] = false;
							}

						}
						enemy13.hostIsAlive = false;
						enemy13.count = 0;
						for (int i = 0; i < 60; i++)
						{
							enemy13.countFlag[i] = false;
							enemy13.easeT[i] = 0;
							enemy13.radius[i] = 0;
							enemy13.dedTimer[i] = 0;
							enemy13.childIsAlive[i] = false;
							enemy13.dedTimer[i] = 0;
							enemy13.isDed[i] = false;
							enemy13.z[i] = i * 6.0f;
							enemy13.thta[i] = (enemy13.z[i] / 180) * 3.1415f;
							enemy13.relativePos[i].x = float(cosf(enemy13.thta[i]) - sinf(enemy13.thta[i])) * enemy13.length;
							enemy13.relativePos[i].y = float(cosf(enemy13.thta[i]) + sinf(enemy13.thta[i])) * enemy13.length;
						}
#pragma endregion

						player.pos = { 0,0 };
						player.velocity = { 0,0 };
						player.aim = false;
						player.aimTimer = 0;
						player.count = 0;
						player.dashAttack = false;
						player.flick = false;
						player.flickCT = 0;
						player.flickLength = 0;
						player.flickTimer = 0;
						for (int i = 0; i < 3; i++)
						{
							player.prepos[i] = { 0,0 };
						}
						player.triangulAttack = 0;
						player.trigerA = 0;
						player.velocityRatio = 0;


						gameTimer = 0;


						allReset = false;
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
						if (endCount == 0)
						{
							endCount = count - precount;
							scoreAdd = true;//攻撃が終わったら１フレだけ起動するフラグを立てる
						}
						precount = count;
					}
					else
					{
						endCount = 0;
					}
#pragma endregion

					//スコア加算
					if (ennergy.fever)
					{
						plusScore = 20;
					}
					else
					{
						plusScore = 10;
					}

					if (scoreAdd)
					{
						score += int((endCount * plusScore) * (endCount / 10 + 1));//一回に倒した数×スコアの倍率をかける
						scoreAdd = false;
					}

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
						if (gameTimer < 6000)
						{
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
							FeverEndAudioFlag = true;

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

					//ランダム敵出現


					//大グループ
					if (quickFlag)
					{
						if (quickTimer == 0)
						{
							successAudioFlag = true;
							quickTimer = 352;
						}

					}
					if (quickTimer > 0)
					{
						quickTimer--;
						if (quickTimer == 0 && quickFlag)
						{
							quickFlag = false;

							for (int i = 0; i < 10; i++)
							{
								randSpown = RandomNum(i + 1 + i * 3) % variation;
								if (randSpown != preRandSpown && randSpown != prepreRandSpown && randSpown != pre3RandSpown && !patterrnIsAlive[randSpown])
								{
									patterrnIsAlive[randSpown] = true;
									pre3RandSpown = prepreRandSpown;
									prepreRandSpown = preRandSpown;
									preRandSpown = randSpown;
									break;
								}
							}
						}
					}
					if (gameTimer == 120 || gameTimer == 1400 || gameTimer == 2700 || gameTimer == 3800 || gameTimer == 4700 || gameTimer == 5400 && quickTimer == 0 || gameTimer == 5640)
					{
						for (int i = 0; i < 10; i++)
						{
							randSpown = RandomNum(i + 1 + i * 3) % variation;
							if (randSpown != preRandSpown && randSpown != prepreRandSpown && randSpown != pre3RandSpown && !patterrnIsAlive[randSpown])
							{

								patterrnIsAlive[randSpown] = true;
								pre3RandSpown = prepreRandSpown;
								prepreRandSpown = preRandSpown;
								preRandSpown = randSpown;
								break;
							}
						}
					}

					if (!patterrnIsAlive[0] && !patterrnIsAlive[1] && !patterrnIsAlive[2] && !patterrnIsAlive[3] && !patterrnIsAlive[4] && quickTimer > 200)
					{
						for (int i = 0; i < 10; i++)
						{
							if (ennergy.fever)
							{
								randSpown = RandomNum(i + 1 + i * 3) % variation;
								if (randSpown != preRandSpown && randSpown != prepreRandSpown && randSpown != pre3RandSpown && !patterrnIsAlive[randSpown])
								{

									patterrnIsAlive[randSpown] = true;
									pre3RandSpown = prepreRandSpown;
									prepreRandSpown = preRandSpown;
									preRandSpown = randSpown;
									break;
								}
							}
						}

					}

					if (patterrnIsAlive[0] && !enemy2.hostIsAlive)
					{
						enemy2.timer = 0;
						enemy2.radius = 0;
						enemy2.easeT = 0;
						enemy2.count = 0;
						for (int j = 0; j < 4; j++)
						{
							enemy2.parentIsAlive[j] = true;
							enemy2.centorPos[0] = { -350,1200 };
							enemy2.centorPos[1] = { -1200,-350 };
							enemy2.centorPos[2] = { 350,-1200 };
							enemy2.centorPos[3] = { 1200,350 };
							for (int i = 0; i < kEnemy2Num; i++)
							{
								enemy2.countFlag[j][i] = false;
								enemy2.childIsAlive[j][i] = true;
								enemy2.relativePos[j][i] = { enemy2.posReset[j][i].x + enemy2.centorPos[j].x,enemy2.posReset[j][i].y + enemy2.centorPos[j].y };
								enemy2.dedTimer[j][i] = kDedTimer;
								enemy2.isDed[j][i] = false;
							}
						}
						enemy2.hostIsAlive = true;
					}
					else if (patterrnIsAlive[1] && !enemy7.hostIsAlive)
					{
						enemy7.timer = 0;
						enemy7.radius = 0;
						enemy7.easeT = 0;
						enemy7.count = 0;
						for (int j = 0; j < 8; j++)
						{
							enemy7.parentIsAlive[j] = true;
							for (int i = 0; i < 4; i++)
							{
								enemy7.countFlag[j][i] = false;
								enemy7.childIsAlive[j][i] = true;
								enemy7.relativePos[j][i] = { enemy7.posReset[j][i].x ,enemy7.posReset[j][i].y };
								enemy7.dedTimer[j][i] = kDedTimer;
								enemy7.isDed[j][i] = false;
							}
						}
						enemy7.hostIsAlive = true;
					}
					else if (patterrnIsAlive[2] && !enemy8.hostIsAlive)
					{
						enemy8.timer = 0;

						enemy8.count = 0;
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
								enemy8.countFlag[j][i] = false;
								enemy8.childIsAlive[j][i] = true;
								enemy8.relativePos[j][i] = { enemy8.centorPos[j].x + enemy8.posReset[j][i].x ,enemy8.centorPos[j].y + enemy8.posReset[j][i].y };
								enemy8.dedTimer[j][i] = kDedTimer;
								enemy8.isDed[j][i] = false;
							}
						}
						enemy8.hostIsAlive = true;
					}
					else if (patterrnIsAlive[3] && !enemy9.hostIsAlive)
					{
						enemy9.timer = 0;
						enemy9.radius = 0;
						enemy9.easeT = 0;
						enemy9.count = 0;
						for (int j = 0; j < 2; j++)
						{
							enemy9.parentIsAlive[j] = true;
							for (int i = 0; i < 8; i++)
							{
								enemy9.countFlag[j][i] = false;
								enemy9.childIsAlive[j][i] = true;
								enemy9.relativePos[j][i] = { enemy9.posReset[j][i].x , enemy9.posReset[j][i].y };
								enemy9.dedTimer[j][i] = kDedTimer;
								enemy9.isDed[j][i] = false;
							}
						}
						enemy9.hostIsAlive = true;
					}
					else if (patterrnIsAlive[4] && !enemy13.hostIsAlive)
					{
						enemy13.count = 0;
						enemy13.hostIsAlive = true;
						for (int i = 0; i < 60; i++)
						{
							enemy13.countFlag[i] = false;
							enemy13.easeT[i] = 0;
							enemy13.radius[i] = 0;
							enemy13.dedTimer[i] = kDedTimer;
							enemy13.childIsAlive[i] = true;
							enemy13.isDed[i] = false;
							enemy13.z[i] = i * 6.0f;
							enemy13.thta[i] = (enemy13.z[i] / 180) * 3.1415f;
							enemy13.relativePos[i].x = float(cosf(enemy13.thta[i]) - sinf(enemy13.thta[i])) * enemy13.length;
							enemy13.relativePos[i].y = float(cosf(enemy13.thta[i]) + sinf(enemy13.thta[i])) * enemy13.length;
						}
					}

					//横一列
					if (gameTimer != 0 && gameTimer % 530 == 0 || gameTimer >= 1900 && gameTimer % 320 == 0 || gameTimer != 0 && gameTimer % 2400 == 0)
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
					if (gameTimer != 0 && gameTimer % 240 == 0 || gameTimer >= 1900 && gameTimer % 700 == 0 || gameTimer >= 3500 && gameTimer % 300 == 0)
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
					if (gameTimer >= 1800 && gameTimer % 430 == 0 || gameTimer >= 1800 && gameTimer % 900 == 0 || gameTimer != 0 && gameTimer % 2400 == 0)
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

					//左上	//右上
					if (gameTimer >= 1380 && gameTimer % 1380 == 0 || gameTimer >= 4130 && gameTimer % 300 == 0)//

					{
						if (!enemy5.parentIsAlive && !enemy6.parentIsAlive)
						{
							enemy5.easeT = 0;
							enemy5.radius = 0;
							enemy5.parentIsAlive = true;
							enemy5.centorPos = { 1500,0 };
							for (int i = 0; i < 4; i++)
							{
								enemy5.childIsAlive[i] = true;
								enemy5.relativePos[i] = { enemy5.posReset[i].x + enemy5.centorPos.x,enemy5.posReset[i].y + enemy5.centorPos.y };
								enemy5.dedTimer[i] = kDedTimer;
								enemy5.isDed[i] = false;
							}
							enemy6.easeT = 0;
							enemy6.radius = 0;
							enemy6.parentIsAlive = true;
							enemy6.centorPos = { -1500,0 };
							for (int i = 0; i < 4; i++)
							{
								enemy6.childIsAlive[i] = true;
								enemy6.relativePos[i] = { enemy6.posReset[i].x + enemy6.centorPos.x,enemy6.posReset[i].y + enemy6.centorPos.y };
								enemy6.dedTimer[i] = kDedTimer;
								enemy6.isDed[i] = false;
							}
						}


					}


					//敵出現のイージング
					if (gameTimer < 6000)
					{

						if (enemy2.hostIsAlive)
						{
							enemy2.radius = easeOutElastic(enemy2.easeT / 100) * enemyRadius;
							if (enemy2.easeT < 100)enemy2.easeT += 2;
							if (enemy2.easeT > 100)enemy2.easeT = 100;

						}
						if (enemy5.parentIsAlive)
						{
							enemy5.radius = easeOutElastic(enemy5.easeT / 100) * enemyRadius;
							if (enemy5.easeT < 100)enemy5.easeT += 2;
							if (enemy5.easeT > 100)enemy5.easeT = 100;
						}
						if (enemy6.parentIsAlive)
						{
							enemy6.radius = easeOutElastic(enemy6.easeT / 100) * enemyRadius;
							if (enemy6.easeT < 100)enemy6.easeT += 2;
							if (enemy6.easeT > 100)enemy6.easeT = 100;
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
						if (enemy13.hostIsAlive)
						{
							for (int i = 59; i > 0; i--)
							{
								if (enemy13.easeT[i - 1] != 0)
								{
									if (enemy13.easeT[i] < 100)enemy13.easeT[i] += 2;
									if (enemy13.easeT[i] > 100)enemy13.easeT[i] = 100;

								}
							}

							if (enemy13.easeT[0] < 100)enemy13.easeT[0] += 2;
							if (enemy13.easeT[0] > 100)enemy13.easeT[0] = 100;

							for (int i = 0; i < 60; i++)
							{
								enemy13.radius[i] = easeOutElastic(enemy13.easeT[i] / 100) * enemyRadius;
							}
						}
					}
					else
					{

						enemy2.radius = easeOutElastic(enemy2.easeT / 100) * enemyRadius;
						if (enemy2.easeT > 0)enemy2.easeT -= 1;
						if (enemy2.easeT < 0)enemy2.easeT = 0;



						enemy5.radius = easeOutElastic(enemy5.easeT / 100) * enemyRadius;
						if (enemy5.easeT > 0)enemy5.easeT -= 1;
						if (enemy5.easeT < 0)enemy5.easeT = 0;


						enemy6.radius = easeOutElastic(enemy6.easeT / 100) * enemyRadius;
						if (enemy6.easeT > 0)enemy6.easeT -= 1;
						if (enemy6.easeT < 0)enemy6.easeT = 0;


						enemy7.radius = easeOutElastic(enemy7.easeT / 100) * enemyRadius;
						if (enemy7.easeT > 0)enemy7.easeT -= 1;
						if (enemy7.easeT < 0)enemy7.easeT = 0;



						enemy8.radius = easeOutElastic(enemy8.easeT / 100) * enemyRadius;
						if (enemy8.easeT > 0)enemy8.easeT -= 1;
						if (enemy8.easeT < 0)enemy8.easeT = 0;



						enemy9.radius = easeOutElastic(enemy9.easeT / 100) * enemyRadius;
						if (enemy9.easeT > 0)enemy9.easeT -= 1;
						if (enemy9.easeT < 0)enemy9.easeT = 0;

						for (int j = 0; j < kenemy10Num; j++)
						{

							enemy10[j].radius = easeOutElastic(enemy10[j].easeT / 100) * enemyRadius;
							if (enemy10[j].easeT > 0)enemy10[j].easeT -= 1;
							if (enemy10[j].easeT < 0)enemy10[j].easeT = 0;


						}
						for (int j = 0; j < kenemy11Num; j++)
						{

							enemy11[j].radius = easeOutElastic(enemy11[j].easeT / 100) * enemyRadius;
							if (enemy11[j].easeT > 0)enemy11[j].easeT -= 1;
							if (enemy11[j].easeT < 0)enemy11[j].easeT = 0;


						}
						for (int j = 0; j < kenemy12Num; j++)
						{

							enemy12[j].radius = easeOutElastic(enemy12[j].easeT / 100) * enemyRadius;
							if (enemy12[j].easeT > 0)enemy12[j].easeT -= 1;
							if (enemy12[j].easeT < 0)enemy12[j].easeT = 0;


						}
						if (enemy13.hostIsAlive)
						{
							for (int i = 59; i > 0; i--)
							{
								if (enemy13.easeT[i - 1] != 100)
								{
									if (enemy13.easeT[i] > 0)enemy13.easeT[i] -= 1;
									if (enemy13.easeT[i] < 0)enemy13.easeT[i] = 0;

								}
							}

							if (enemy13.easeT[0] > 0)enemy13.easeT[0] -= 1;
							if (enemy13.easeT[0] < 0)enemy13.easeT[0] = 0;

							for (int i = 0; i < 60; i++)
							{
								enemy13.radius[i] = easeOutElastic(enemy13.easeT[i] / 100) * enemyRadius;
							}
						}
					}


#pragma endregion
#pragma region player

					for (int i = kPreNum - 1; i > 0; i--)
					{
						prePosColor[i] = prePosColor[i - 1];
						playerPrePos[i] = playerPrePos[i - 1];
					}

					if (player.dashAttack)
					{
						prePosColor[0] = 0x00ffff99;

					}
					else if (player.triangulAttack || player.aim)
					{
						prePosColor[0] = 0x55ff5577;

					}
					else
					{
						prePosColor[0] = 0xffffff99;

					}
					playerPrePos[0] = player.pos;
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
						if (player.aim && player.count >= 2 && player.velocityRatio <= 0.80f)
						{
							triFlag = true;
						}
						if (player.aim && player.count >= 1 && player.velocityRatio <= 0.78f)
						{
							player.prepos[player.count] = player.pos;
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



					if (gameTimer < 6000)
					{


						//攻撃終わりにAボタンを押した時
						if (player.trigerA && player.aimTimer == 0 && !player.dashAttack || triFlag)
						{
							//移動入力がされている時
							pointsetAudioFlag = true;
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
								TriangleAudioFlag = true;
								//三点設置したら設置フェーズをおわる
								player.count = 0;
								player.aim = false;
								player.aimTimer = 30;//攻撃時間を代入
								player.triangulAttack = true;//三角形攻撃を有効に
								triFlag = false;
							}



						}
					}
					//三角形を出した後の攻撃時間
					if (player.aimTimer > 0)
					{
						player.aimTimer--;
						if (player.aimTimer <= 0)
						{
							player.aimTimer = 0;
							player.triangulAttack = false;

							for (int i = 0; i < 3; i++)
							{
								//三角形の点
								//今は原点に戻してるけど意味はない
								player.prepos[i] = { 0,0 };
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
					//length = sqrtf(xy.x * xy.x + xy.y * xy.y);
					length = clump(sqrtf(xy.x * xy.x + xy.y * xy.y), 0, 100);

					//長さがデッドゾーンを超えたら方向を代入
					if (length >= dedZone && !player.flick)
					{
						//方向を正規化。速度をかけるだけで使えるようにするため
						player.direction = vectorNormalize(player.joystick, { 0,0 });
						player.preDirection = player.direction;
					}
					else
					{
						player.direction = { 0,0 };
					}

					if (gameTimer > 0 && gameTimer < 6000)
					{
						//スティックのポジションがデッドゾーンの中の時
						if (length <= 17)//preJoyStickX<dedZone * 50 && preJoyStickX > -dedZone * 50 && preJoyStickY<dedZone * 50 && preJoyStickY > -dedZone * 50
						{
							//フリック判定フレをリセット
							if (!player.flick && player.flickCT == 0)player.flickTimer = 3;
						}


						//はじき判定
						//フリック判定フレが0以上の時
						if (player.flickTimer > 0)
						{
							player.flickTimer--;
							//スティックが端に行ったら
							if (length >= flickRength && !player.aim && !player.triangulAttack)
							{
								//フリックをTRUEに
								player.flick = true;
								//突撃フラグを有効に
								player.dashAttack = true;
								//速度を等倍に
								player.velocityRatio = 1;
								dashAudioFlag = true;
							}
						}
					}
					if (gameTimer < 6000)
					{


						//Aボタンまたはフリックしたとき
						if (player.trigerA && player.aim || player.flick && player.flickCT == 0)
						{
							//方向に発射速度をかける
							player.velocity.x = player.preDirection.x * player.shotSpeed;
							player.velocity.y = player.preDirection.y * player.shotSpeed;

							if (ennergy.fever)
							{
								//フィーバーの倍率をかける
								player.velocity.x = player.preDirection.x * player.shotSpeed * ennergy.powerUp;
								player.velocity.y = player.preDirection.y * player.shotSpeed * ennergy.powerUp;

							}

						}

						//割合をかける事で徐々に減速する
						player.velocity.x *= player.velocityRatio;
						player.velocity.y *= player.velocityRatio;




						//フリックも三角形も作っていないとき
						if (!player.aim && !player.flick)//&& player.aimTimer <= 5
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
					}
					else
					{
						player.velocity = { 0,0 };
					}

					if (player.preDirection.x != 0 || player.preDirection.y != 0)
					{
						directionTheta = atan2f(float(player.preDirection.x), float(-player.preDirection.y));

					}
					///スティック入力ここまで
#pragma endregion
				// キー入力（デバッグ) ここから ### MustDelete

					if (keys[DIK_SPACE] && !preKeys[DIK_SPACE])
					{
						Functions.ToggleDebugMode();
						Original::SoundPlay("pi");
					}

					// キー入力（デバッグ）ここまで

					//ポジション移動
					player.pos.x += player.velocity.x;
					player.pos.y += player.velocity.y;

					//敵の移動
#pragma region enemyMove
		/*if (enemy1.parentIsAlive)
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
		}*/
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
							if (!enemy2.parentIsAlive[0] && !enemy2.parentIsAlive[1] && !enemy2.parentIsAlive[2] && !enemy2.parentIsAlive[3])
							{
								enemy2.hostIsAlive = false;
							}
						}
					}


					/*if (enemy3.parentIsAlive)
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
					}*/
					if (enemy5.parentIsAlive)
					{
						if (enemy5.easeT == 100)
						{
							enemy5.centorPos.x -= 4;
							for (int i = 0; i < 4; i++)
							{
								enemy5.childIsAlive[i] = EllipseCollision({ 0,0 }, fieldRadius, enemy5.relativePos[i], enemy5.radius);
								if (!enemy5.isDed[i] && enemy5.childIsAlive[i])enemy5.relativePos[i].x -= 4;
							}
							if (!enemy5.childIsAlive[0] && !enemy5.childIsAlive[1] && !enemy5.childIsAlive[2] && !enemy5.childIsAlive[3])
							{
								enemy5.parentIsAlive = false;
							}
						}
					}
					if (enemy6.parentIsAlive)
					{
						if (enemy6.easeT == 100)
						{
							enemy6.centorPos.x -= 4;
							for (int i = 0; i < 4; i++)
							{
								enemy6.childIsAlive[i] = EllipseCollision({ 0,0 }, fieldRadius, enemy6.relativePos[i], enemy6.radius);
								if (!enemy6.isDed[i] && enemy6.childIsAlive[i])enemy6.relativePos[i].x += 4;
							}
							if (!enemy6.childIsAlive[0] && !enemy6.childIsAlive[1] && !enemy6.childIsAlive[2] && !enemy6.childIsAlive[3])
							{
								enemy6.parentIsAlive = false;
							}
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
						if (enemy7.parentIsAlive[0] == false && enemy7.parentIsAlive[1] == false && enemy7.parentIsAlive[2] == false && enemy7.parentIsAlive[3] == false && enemy7.parentIsAlive[4] == false && enemy7.parentIsAlive[5] == false && enemy7.parentIsAlive[6] == false && enemy7.parentIsAlive[7] == false)
						{
							enemy7.hostIsAlive = false;
						}
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
								if (!enemy11[j].childIsAlive[0] && !enemy11[j].childIsAlive[1] && !enemy11[j].childIsAlive[2])
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

					if (enemy13.easeT[59] == 100 && enemy13.hostIsAlive)
					{
						for (int i = 0; i < 60; i++)
						{
							if (!enemy13.isDed[i] && enemy13.childIsAlive[i]) {
								enemy13.relativePos[i].x = float(cosf(enemy13.thta[i]) - sinf(enemy13.thta[i])) * enemy13.length;
								enemy13.relativePos[i].y = float(cosf(enemy13.thta[i]) + sinf(enemy13.thta[i])) * enemy13.length;

								enemy13.z[i] += 0.2f;
								if (enemy13.z[i] >= 360)enemy13.z[i] = 0;
								enemy13.thta[i] = (enemy13.z[i] / 180) * 3.1415f;

							}
						}
					}
#pragma endregion
					//ここで攻撃処理をしたいと考えてる

#pragma region playerAttack
		//ダッシュ攻撃
					if (player.dashAttack)
					{

						if (enemy2.hostIsAlive && enemy2.easeT > 2)
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


						if (enemy5.parentIsAlive && enemy5.easeT > 2)
						{
							for (int i = 0; i < 4; i++)
							{
								if (!enemy5.isDed[i])enemy5.isDed[i] = EllipseCollision(player.pos, player.radius.x, enemy5.relativePos[i], enemy5.radius);
							}
						}
						if (enemy6.parentIsAlive && enemy6.easeT > 2)
						{
							for (int i = 0; i < 4; i++)
							{
								if (!enemy6.isDed[i])enemy6.isDed[i] = EllipseCollision(player.pos, player.radius.x, enemy6.relativePos[i], enemy6.radius);
							}
						}
						if (enemy7.hostIsAlive && enemy7.easeT > 2)
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
						if (enemy8.hostIsAlive && enemy8.easeT > 2)
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
						if (enemy9.hostIsAlive && enemy9.easeT > 2)
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
							if (enemy10[j].parentIsAlive && enemy10[j].easeT > 2)
							{
								for (int i = 0; i < 4; i++)
								{
									if (!enemy10[j].isDed[i])enemy10[j].isDed[i] = EllipseCollision(player.pos, player.radius.x, enemy10[j].relativePos[i], enemy10[j].radius);
								}
							}
						}
						for (int j = 0; j < kenemy11Num; j++)
						{
							if (enemy11[j].parentIsAlive && enemy11[j].easeT > 2)
							{
								for (int i = 0; i < 3; i++)
								{
									if (!enemy11[j].isDed[i])enemy11[j].isDed[i] = EllipseCollision(player.pos, player.radius.x, enemy11[j].relativePos[i], enemy11[j].radius);
								}
							}
						}
						for (int j = 0; j < kenemy12Num; j++)
						{
							if (enemy12[j].parentIsAlive && enemy12[j].easeT > 2)
							{
								for (int i = 0; i < 4; i++)
								{
									if (!enemy12[j].isDed[i])enemy12[j].isDed[i] = EllipseCollision(player.pos, player.radius.x, enemy12[j].relativePos[i], enemy12[j].radius);
								}
							}
						}
						for (int i = 0; i < 60; i++)
						{
							if (!enemy13.isDed[i] && enemy13.easeT[i] > 2)enemy13.isDed[i] = EllipseCollision(player.pos, player.radius.x, enemy13.relativePos[i], enemy13.radius[i]);
						}
					}

					//三角攻撃
					if (player.triangulAttack)
					{

						// 敵の生存フラグを当たり判定によって更新
						isEnemyDead = attAreaObj.TriangleCollision(enemyPos);


						/*if (enemy1.parentIsAlive)
						{
							for (int i = 0; i < 3; i++)
							{
								if (!enemy1.isDed[i])enemy1.isDed[i] = attAreaObj.TriangleCollision(enemy1.relativePos[i]);
							}
						}*/
						if (enemy2.hostIsAlive)
						{
							for (int j = 0; j < 4; j++)
							{
								if (enemy2.parentIsAlive[j] && enemy2.easeT > 2)
								{
									for (int i = 0; i < kEnemy2Num; i++)
									{
										if (!enemy2.isDed[j][i])enemy2.isDed[j][i] = attAreaObj.TriangleCollision(enemy2.relativePos[j][i]);
									}
								}
							}

						}

						if (enemy5.parentIsAlive && enemy5.easeT > 2)
						{
							for (int i = 0; i < 4; i++)
							{
								if (!enemy5.isDed[i])enemy5.isDed[i] = attAreaObj.TriangleCollision(enemy5.relativePos[i]);
							}
						}
						if (enemy6.parentIsAlive && enemy6.easeT > 2)
						{
							for (int i = 0; i < 4; i++)
							{
								if (!enemy6.isDed[i])enemy6.isDed[i] = attAreaObj.TriangleCollision(enemy6.relativePos[i]);
							}
						}
						if (enemy7.hostIsAlive && enemy7.easeT > 2)
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
						if (enemy8.hostIsAlive && enemy8.easeT > 2)
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
						if (enemy9.hostIsAlive && enemy9.easeT > 2)
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
							if (enemy10[j].parentIsAlive && enemy10[j].easeT > 2)
							{
								for (int i = 0; i < 4; i++)
								{
									if (!enemy10[j].isDed[i])enemy10[j].isDed[i] = attAreaObj.TriangleCollision(enemy10[j].relativePos[i]);
								}
							}
						}
						for (int j = 0; j < kenemy11Num; j++)
						{
							if (enemy11[j].parentIsAlive && enemy11[j].easeT > 2)
							{
								for (int i = 0; i < 3; i++)
								{
									if (!enemy11[j].isDed[i])enemy11[j].isDed[i] = attAreaObj.TriangleCollision(enemy11[j].relativePos[i]);
								}
							}
						}
						for (int j = 0; j < kenemy12Num; j++)
						{
							if (enemy12[j].parentIsAlive && enemy12[j].easeT > 2)
							{
								for (int i = 0; i < 4; i++)
								{
									if (!enemy12[j].isDed[i])enemy12[j].isDed[i] = attAreaObj.TriangleCollision(enemy12[j].relativePos[i]);
								}
							}
						}
						for (int i = 0; i < 60; i++)
						{
							if (!enemy13.isDed[i] && enemy13.easeT[i] > 2)enemy13.isDed[i] = attAreaObj.TriangleCollision(enemy13.relativePos[i]);
						}
					}
#pragma endregion

#pragma region playerDamage
					if (!player.dashAttack && !player.triangulAttack)
					{
						/*if (enemy1.parentIsAlive)
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
						}*/
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
												if (!ennergy.fever) {
													ennergy.count -= ennergy.damage;
													ennergy.damageFlag = true;
												}
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


						if (enemy5.parentIsAlive)
						{
							for (int i = 0; i < 4; i++)
							{
								if (enemy5.dedTimer[i] == kDedTimer && EllipseCollision(player.pos, player.radius.x, enemy5.relativePos[i], enemy5.radius))
								{
									if (ennergy.count > 0)
									{
										if (!ennergy.fever) {
											ennergy.count -= ennergy.damage;
											ennergy.damageFlag = true;
										}
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
										if (!ennergy.fever) {
											ennergy.count -= ennergy.damage;
											ennergy.damageFlag = true;
										}
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
												if (!ennergy.fever) {
													ennergy.count -= ennergy.damage;
													ennergy.damageFlag = true;
												}
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
												if (!ennergy.fever) {
													ennergy.count -= ennergy.damage;
													ennergy.damageFlag = true;
												}
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
												if (!ennergy.fever) {
													ennergy.count -= ennergy.damage;
													ennergy.damageFlag = true;
												}
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
											if (!ennergy.fever) {
												ennergy.count -= ennergy.damage;
												ennergy.damageFlag = true;
											}
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
											if (!ennergy.fever) {
												ennergy.count -= ennergy.damage;
												ennergy.damageFlag = true;
											}
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
											if (!ennergy.fever) {
												ennergy.count -= ennergy.damage;
												ennergy.damageFlag = true;
											}
										}
										else
										{
											ennergy.count = 0;
										}
									}
								}
							}
						}
						for (int i = 0; i < 60; i++)
						{
							if (enemy13.dedTimer[i] == kDedTimer && EllipseCollision(player.pos, player.radius.x, enemy13.relativePos[i], enemy13.radius[i]))
							{
								if (ennergy.count > 0)
								{
									if (!ennergy.fever) {
										ennergy.count -= ennergy.damage;
										ennergy.damageFlag = true;
									}
								}
								else
								{
									ennergy.count = 0;
								}
							}
						}
					}

#pragma endregion
					//敵のダメージ処理
#pragma region enemyDamage
		/*if (enemy1.parentIsAlive)
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
		}*/
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
										quickFlag = true;
										quickNum = 4;

									}
								}
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
							if (!enemy5.childIsAlive[0] && !enemy5.childIsAlive[1] && !enemy5.childIsAlive[2] && !enemy5.childIsAlive[3])
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
							if (!enemy6.childIsAlive[0] && !enemy6.childIsAlive[1] && !enemy6.childIsAlive[2] && !enemy6.childIsAlive[3])
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
										quickFlag = true;
										quickNum = 2;
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

								}
							}
						}
						if (enemy8.parentIsAlive[0] == false && enemy8.parentIsAlive[1] == false && enemy8.parentIsAlive[2] == false && enemy8.parentIsAlive[3] == false)
						{
							enemy8.hostIsAlive = false;
							quickFlag = true;
							quickNum = 3;
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
										quickFlag = true;
										quickNum = 1;
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
								if (!enemy11[j].childIsAlive[0] && !enemy11[j].childIsAlive[1] && !enemy11[j].childIsAlive[2])
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
								if (!enemy12[j].childIsAlive[0] && !enemy12[j].childIsAlive[1] && !enemy12[j].childIsAlive[2] && !enemy12[j].childIsAlive[3])
								{
									enemy12[j].parentIsAlive = false;
								}
							}
						}
					}
					for (int i = 0; i < 60; i++)
					{
						if (enemy13.isDed[i])
						{
							if (enemy13.dedTimer[i] == kDedTimer)
							{
								count++;
								if (player.dashAttack)dash.count++;
								if (player.triangulAttack)triangle.count++;
							}
							if (enemy13.dedTimer[i] > 0)enemy13.dedTimer[i]--;
							if (enemy13.dedTimer[i] <= 0)enemy13.childIsAlive[i] = false;
						}
					}



#pragma endregion

					//敵がちゃんと死んでいるか確認
					if (enemy2.hostIsAlive)
					{

						for (int j = 0; j < 4; j++)
						{
							for (int i = 0; i < 16; i++)
							{
								if (!enemy2.childIsAlive[j][i] && !enemy2.countFlag[j][i])
								{
									enemy2.count++;
									enemy2.countFlag[j][i] = true;
								}
							}
						}
						if (enemy2.count == 64)enemy2.hostIsAlive = false;
					}
					if (enemy7.hostIsAlive)
					{
						for (int j = 0; j < 8; j++)
						{
							for (int i = 0; i < 4; i++)
							{
								if (!enemy7.childIsAlive[j][i] && !enemy7.countFlag[j][i])
								{
									enemy7.count++;
									enemy7.countFlag[j][i] = true;
								}
							}
						}
						if (enemy7.count == 32)enemy7.hostIsAlive = false;
					}
					if (enemy8.hostIsAlive)
					{
						for (int j = 0; j < 4; j++)
						{
							for (int i = 0; i < 12; i++)
							{
								if (!enemy8.childIsAlive[j][i] && !enemy8.countFlag[j][i])
								{
									enemy8.count++;
									enemy8.countFlag[j][i] = true;
								}
							}
						}
						if (enemy8.count == 48)enemy8.hostIsAlive = false;
					}
					if (enemy9.hostIsAlive)
					{

						for (int j = 0; j < 2; j++)
						{
							for (int i = 0; i < 8; i++)
							{
								if (!enemy9.childIsAlive[j][i] && !enemy9.countFlag[j][i])
								{
									enemy9.count++;
									enemy9.countFlag[j][i] = true;
								}
							}
						}
						if (enemy9.count == 16)enemy9.hostIsAlive = false;
					}
					if (enemy13.hostIsAlive)
					{

						for (int i = 0; i < 60; i++)
						{
							if (!enemy13.childIsAlive[i] && !enemy13.countFlag[i])
							{
								enemy13.count++;
								enemy13.countFlag[i] = true;
							}
						}

						if (enemy13.count == 60)
						{
							enemy13.hostIsAlive = false;
							quickFlag = true;
							quickNum = 0;
						}
					}
					if (!enemy2.hostIsAlive)
					{
						patterrnIsAlive[0] = false;
					}
					if (!enemy7.hostIsAlive)
					{
						patterrnIsAlive[1] = false;
					}
					if (!enemy8.hostIsAlive)
					{
						patterrnIsAlive[2] = false;
					}
					if (!enemy9.hostIsAlive)
					{
						patterrnIsAlive[3] = false;
					}
					if (!enemy13.hostIsAlive)
					{
						patterrnIsAlive[4] = false;

					}

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

					/*if (keys[DIK_W] && !preKeys[DIK_W])
					{
						if (cameraEaseT < 1.0f)cameraEaseT += 0.1f;
						if (cameraEaseT > 1.0f)cameraEaseT = 1.0f;
					}
					else if (keys[DIK_S] && !preKeys[DIK_S])
					{
						if (cameraEaseT > 0.1f)cameraEaseT -= 0.1f;
						if (cameraEaseT < 0.1f)cameraEaseT = 0.1f;

					}*/
					cameraEasePos.x = (1.0f - cameraEaseT) * cameraEasePos.x + float(player.pos.x) * cameraEaseT;
					cameraEasePos.y = (1.0f - cameraEaseT) * cameraEasePos.y + float(player.pos.y) * cameraEaseT;


					//スクロールの値を代入
					scroll.x = (-cameraEasePos.x / screenSize + 960);
					scroll.y = (-cameraEasePos.y / screenSize + 540);


					if (gameTimer > 6000)
					{
						if (finishEaseT < 100)
						{
							finishSize = easeOutCirc(finishEaseT / 100);
							finishEase = easeOutQuad(finishEaseT / 100);

							finishEaseT += 2;
						}
						if (finishEaseT == 100)
						{
							clearTimer++;
							if (clearTimer > 30)
							{
								finishSize2 = easeInCirc(finishEaseT2 / 100);
								finishEase2 = easeOutQuad(finishEaseT2 / 100);
								if (finishEaseT2 < 100)finishEaseT2 += 2;
								if (blackEaseT < 100)
								{
									blackEaseT += 2;
									blackColor = int(easeInQuart((float)blackEaseT / 100) * 255);

								}
								else if (blackEaseT >= 100)
								{
									game = 2;
									allReset = true;
								}
							}

						}
					}
					else if (gameTimer >= 5700)
					{
						bigNumSize = easeOutCirc(bigNumEaseT / 120);

						if (bigNumEaseT < 120 && bigNumEaseT>-1 && bigNumCount < 5)
						{
							bigNumEaseT += 2;

							if (bigNumEaseT >= 120)
							{
								bigNumEaseT = 0;
								bigNumSize = 0;
								bigNumCount++;
								if (bigNumCount >= 5)
								{
									bigNumCount = 4;
									bigNumEaseT = -1;
								}
							}
						}
					}

					if (keys[DIK_T] && !preKeys[DIK_T])
					{
						gameTimer += 600;
					}

					if (gameTimer > 5700 && UIBackEaseT < 100)
					{
						UIBackPos = easeInBack(UIBackEaseT / 100) * 200;
						UIBackEaseT += 2;

					}

					if (gameTimer <= 6000)gameTimer++;
					//directionMove++;
#pragma endregion
				}
				break;
			case kTypeGameResult:


#pragma region enemy
				enemy2.radius = easeOutElastic(enemy2.easeT / 100) * enemyRadius;
				if (enemy2.easeT > 0)enemy2.easeT -= 1;
				if (enemy2.easeT < 0)enemy2.easeT = 0;



				enemy5.radius = easeOutElastic(enemy5.easeT / 100) * enemyRadius;
				if (enemy5.easeT > 0)enemy5.easeT -= 1;
				if (enemy5.easeT < 0)enemy5.easeT = 0;


				enemy6.radius = easeOutElastic(enemy6.easeT / 100) * enemyRadius;
				if (enemy6.easeT > 0)enemy6.easeT -= 1;
				if (enemy6.easeT < 0)enemy6.easeT = 0;


				enemy7.radius = easeOutElastic(enemy7.easeT / 100) * enemyRadius;
				if (enemy7.easeT > 0)enemy7.easeT -= 1;
				if (enemy7.easeT < 0)enemy7.easeT = 0;



				enemy8.radius = easeOutElastic(enemy8.easeT / 100) * enemyRadius;
				if (enemy8.easeT > 0)enemy8.easeT -= 1;
				if (enemy8.easeT < 0)enemy8.easeT = 0;



				enemy9.radius = easeOutElastic(enemy9.easeT / 100) * enemyRadius;
				if (enemy9.easeT > 0)enemy9.easeT -= 1;
				if (enemy9.easeT < 0)enemy9.easeT = 0;

				for (int j = 0; j < kenemy10Num; j++)
				{

					enemy10[j].radius = easeOutElastic(enemy10[j].easeT / 100) * enemyRadius;
					if (enemy10[j].easeT > 0)enemy10[j].easeT -= 1;
					if (enemy10[j].easeT < 0)enemy10[j].easeT = 0;


				}
				for (int j = 0; j < kenemy11Num; j++)
				{

					enemy11[j].radius = easeOutElastic(enemy11[j].easeT / 100) * enemyRadius;
					if (enemy11[j].easeT > 0)enemy11[j].easeT -= 1;
					if (enemy11[j].easeT < 0)enemy11[j].easeT = 0;


				}
				for (int j = 0; j < kenemy12Num; j++)
				{

					enemy12[j].radius = easeOutElastic(enemy12[j].easeT / 100) * enemyRadius;
					if (enemy12[j].easeT > 0)enemy12[j].easeT -= 1;
					if (enemy12[j].easeT < 0)enemy12[j].easeT = 0;


				}
				if (enemy13.hostIsAlive)
				{
					for (int i = 59; i > 0; i--)
					{
						if (enemy13.easeT[i - 1] != 100)
						{
							if (enemy13.easeT[i] > 0)enemy13.easeT[i] -= 1;
							if (enemy13.easeT[i] < 0)enemy13.easeT[i] = 0;

						}
					}

					if (enemy13.easeT[0] > 0)enemy13.easeT[0] -= 1;
					if (enemy13.easeT[0] < 0)enemy13.easeT[0] = 0;

					for (int i = 0; i < 60; i++)
					{
						enemy13.radius[i] = easeOutElastic(enemy13.easeT[i] / 100) * enemyRadius;
					}
				}
#pragma endregion

				if (allReset)
				{
					scorePos = 0;
					scoreEaseT = 0;
					scoreEaseChange = -1;
					titleBackFlag = false;
					directionMove = 0;
					triFlag = false;
					Playerth = 0;
					gamestartEaseT = 0;
					gamestartEaseT2 = 0;
					gamestartPos = 0;
					gamestartPos2 = 0;
					titleCount = 0;
					titleNewGameFlag = false;
					startNewGame = false;
					whileFlag = false;
					titleWalkFlag = true;
					titleDashFlag = false;
					titleTriangleFlag = false;

					walkTimer = 0;
					whileTime = 60;//最初のタスクの間と間の時間

					finishSize2 = 0;
					finishEaseT2 = 0;
					clearTimer = 0;
					fieldToPlayer = { 0,0 };
					cameraEasePos = { 0,0 };
					for (int i = 0; i < kPreNum; i++)
					{
						playerPrePos[i] = { 0,0 };
					}
					player.preDirection = { 0,0 };
					enemy2.timer = 0;
					enemy7.timer = 0;
					enemy8.timer = 0;
					enemy9.timer = 0;
					quickFlag = false;
					quickTimer = 0;
					finishEaseT = 0;
					finishSize = 0;
					bigNumEaseT = 0;
					bigNumSize = 0;
					bigNumCount = 0;
					//score = 0;
					plusScore = 10;
					scoreAdd = false;
					for (int i = 0; i < variation; i++)
					{
						patterrnIsAlive[i] = false;
					}
					shakeGaugePos = { 0,0 };
					shakeGaugeX = shakeRadius;
					shakeGaugeY = shakeRadius;
					shakeGaugeSeed = 1;
					screenSize = 1.0f;
					setScreenEaseT = 0;
					setScreenEaseTChange = 1;
					ennergy.count = 0;
					ennergy.damageFlag = false;
					ennergy.dash = false;
					ennergy.triangle = false;
					ennergy.fever = false;
					//dash.count = 0;
					//dash.endCount = 0;
					dash.preCount = 0;
					//count = 0;
					precount = 0;
					//endCount = 0;
					//triangle.count = 0;
					//triangle.endCount = 0;
					triangle.preCount = 0;
#pragma region enemyReset

					enemy1.radius = 0;
					enemy1.easeT = 0;
					enemy1.parentIsAlive = false;

					for (int i = 0; i < 3; i++)
					{
						enemy1.childIsAlive[i] = false;
						enemy1.dedTimer[i] = 0;
						enemy1.isDed[i] = false;
					}

					enemy2.radius = 0;
					enemy2.easeT = 0;
					enemy2.hostIsAlive = false;
					enemy2.count = 0;

					for (int j = 0; j < 4; j++)
					{
						enemy2.parentIsAlive[j] = false;
						for (int i = 0; i < kEnemy2Num; i++)
						{
							enemy2.countFlag[j][i] = false;
							enemy2.childIsAlive[j][i] = false;
							enemy2.dedTimer[j][i] = 0;
							enemy2.isDed[j][i] = false;
						}
					}
					enemy5.easeT = 0;
					enemy5.radius = 0;
					enemy5.parentIsAlive = false;
					for (int i = 0; i < 4; i++)
					{
						enemy5.childIsAlive[i] = false;
						enemy5.dedTimer[i] = 0;
						enemy5.isDed[i] = false;
					}
					enemy6.easeT = 0;
					enemy6.radius = 0;
					enemy6.parentIsAlive = false;
					for (int i = 0; i < 4; i++)
					{
						enemy6.childIsAlive[i] = false;
						enemy6.dedTimer[i] = 0;
						enemy6.isDed[i] = false;
					}
					enemy7.radius = 0;
					enemy7.easeT = 0;
					enemy7.count = 0;

					for (int j = 0; j < 8; j++)
					{
						enemy7.parentIsAlive[j] = false;
						for (int i = 0; i < 4; i++)
						{
							enemy7.countFlag[j][i] = false;
							enemy7.childIsAlive[j][i] = false;
							enemy7.dedTimer[j][i] = 0;
							enemy7.isDed[j][i] = false;
						}

					}
					enemy7.hostIsAlive = false;
					enemy8.radius = 0;
					enemy8.easeT = 0;
					enemy8.count = 0;
					enemy8.timer = 0;
					for (int j = 0; j < 4; j++)
					{
						enemy8.parentIsAlive[j] = false;
						for (int i = 0; i < 12; i++)
						{
							enemy8.countFlag[j][i] = false;
							enemy8.childIsAlive[j][i] = false;
							enemy8.dedTimer[j][i] = 0;
							enemy8.isDed[j][i] = false;
						}
					}
					enemy8.hostIsAlive = false;

					enemy9.radius = 0;
					enemy9.easeT = 0;
					enemy9.count = 0;
					for (int j = 0; j < 2; j++)
					{
						enemy9.parentIsAlive[j] = false;
						for (int i = 0; i < 8; i++)
						{
							enemy9.countFlag[j][i] = false;
							enemy9.childIsAlive[j][i] = false;
							enemy9.dedTimer[j][i] = 0;
							enemy9.isDed[j][i] = false;
						}
					}
					enemy9.hostIsAlive = false;
					for (int j = 0; j < kenemy10Num; j++)
					{

						enemy10[j].parentIsAlive = false;

						for (int i = 0; i < 4; i++)
						{
							enemy10[j].childIsAlive[i] = false;
							enemy10[j].dedTimer[i] = 0;
							enemy10[j].isDed[i] = false;
						}

					}
					for (int j = 0; j < kenemy11Num; j++)
					{

						enemy11[j].radius = 0;
						enemy11[j].easeT = 0;
						enemy11[j].parentIsAlive = false;

						for (int i = 0; i < 3; i++)
						{
							enemy11[j].childIsAlive[i] = false;
							enemy11[j].dedTimer[i] = 0;
							enemy11[j].isDed[i] = false;
						}

					}
					for (int j = 0; j < kenemy12Num; j++)
					{

						enemy12[j].radius = 0;
						enemy12[j].easeT = 0;
						enemy12[j].parentIsAlive = false;
						for (int i = 0; i < 4; i++)
						{
							enemy12[j].childIsAlive[i] = false;
							enemy12[j].dedTimer[i] = 0;
							enemy12[j].isDed[i] = false;
						}

					}
					enemy13.hostIsAlive = false;
					enemy13.count = 0;
					for (int i = 0; i < 60; i++)
					{
						enemy13.countFlag[i] = false;
						enemy13.easeT[i] = 0;
						enemy13.radius[i] = 0;
						enemy13.dedTimer[i] = 0;
						enemy13.childIsAlive[i] = false;
						enemy13.dedTimer[i] = 0;
						enemy13.isDed[i] = false;
						enemy13.z[i] = i * 6.0f;
						enemy13.thta[i] = (enemy13.z[i] / 180) * 3.1415f;
						enemy13.relativePos[i].x = float(cosf(enemy13.thta[i]) - sinf(enemy13.thta[i])) * enemy13.length;
						enemy13.relativePos[i].y = float(cosf(enemy13.thta[i]) + sinf(enemy13.thta[i])) * enemy13.length;
					}
#pragma endregion

					player.pos = { 0,0 };
					player.velocity = { 0,0 };
					player.aim = false;
					player.aimTimer = 0;
					player.count = 0;
					player.dashAttack = false;
					player.flick = false;
					player.flickCT = 0;
					player.flickLength = 0;
					player.flickTimer = 0;
					for (int i = 0; i < 3; i++)
					{
						player.prepos[i] = { 0,0 };
					}
					player.triangulAttack = 0;
					player.trigerA = 0;
					player.velocityRatio = 0;


					gameTimer = 0;


					allReset = false;
				}
				if (blackEaseT > 0)
				{
					blackEaseT -= 1.0f;
					blackColor = int(easeInExpo(blackEaseT / 100) * 255);

				}
				if (blackEaseT <= 50)
				{
					Novice::SetJoystickDeadZone(0, 7000, 0);


					//ボタン入力ここから
					player.trigerA = false;
					if (Novice::IsTriggerButton(0, kPadButton10))
					{
						player.trigerA = true;
					}

					///スティック入力ここから
					Novice::GetAnalogInputLeft(0, &joystickX, &joystickY);

					//コントローラーの原点からの距離をはかる
					player.flickLength = floatLength({ (float)joystickX, (float)joystickY }, { 0,0 });

					//ジョイスティック入力を-100から100まで
					player.joystick.x = (clump(float(joystickX / 32767.0f), -1, 1) * 100);
					player.joystick.y = (clump(float(joystickY / 32767.0f), -1, 1) * 100);

					//円の当たり判定的な。
					xy = vectorLength(player.joystick, { 0,0 });
					//length = sqrtf(xy.x * xy.x + xy.y * xy.y);
					length = clump(sqrtf(xy.x * xy.x + xy.y * xy.y), 0, 100);

					//長さがデッドゾーンを超えたら方向を代入

					prechoice = choice;

					if (!transitionFlag && game == kTypeGameResult)
					{
						if (length >= dedZone)
						{
							if (choiceTimer > 0)
							{
								choiceTimer--;
							}
							if (choiceTimer == 0 && resultEasePos[3] > 50)
							{
								if (player.joystick.x > 0.3f)
								{
									choice = 1;
								}
								else if (player.joystick.x < -0.3f)
								{
									choice = 0;
								}
							}

							if (prechoice != choice)
							{
								yesnoEaseT = 0;
								choiceTimer = 5;
							}

						}

						if (choice == 0 && player.trigerA && resultEaseT[3] >= 90)
						{
							UIBackPos = 0;
							transitionFlag = true;
							if (!startNewGame)
							{
								score = 0;
								ennergy.count = 0;
								gamestartEaseT = 0;
								setScreenEaseT = 0;
								setScreenEaseTChange = 1;
								screenSize = 1.0f;
								player.pos = { 0,0 };
								for (int i = 0; i < kPreNum; i++)
								{
									playerPrePos[i] = { 0,0 };
								}
								player.preDirection = { 0,0 };
								scroll.x = (-player.pos.x / screenSize + 960);
								scroll.y = (-player.pos.y / screenSize + 540);
							}
							startNewGame = true;
							SelectAudioFlag = true;
						}
						else if (choice == 1 && player.trigerA && resultEaseT[3] >= 90)
						{
							SelectAudioFlag = true;
							titleBackFlag = true;
							transitionFlag = true;
						}
					}


					if (startNewGame)
					{
						if (gamestartEaseT >= 100)

						{
							game = 1;
							setScreenEaseTChange = -1;
							player.preDirection = { 0,0 };
							player.pos = { 0,0 };
						}
						//スクリーン拡縮処理
						screenSize = easeOutCubic(setScreenEaseT / 100) * (70 - 1) + 1;//easeOutBack
						if (setScreenEaseT < 100 || setScreenEaseT > 0)
						{
							setScreenEaseT += setScreenEaseTChange * 2;
							if (setScreenEaseT > 100)setScreenEaseT = 100;
							if (setScreenEaseT < 0)setScreenEaseT = 0;
						}
						gamestartPos = easeOutQuart(gamestartEaseT / 100) * 2192;
						if (gamestartEaseT < 100)gamestartEaseT += 2;

					}
					if (titleBackFlag)
					{
						if (blackEaseT2 < 100)
						{
							blackEaseT2 += 2.0f;
							blackColor2 = int(easeInExpo(blackEaseT2 / 100) * 255);


						}
						if (blackEaseT2 >= 100)
						{
							gaugeEaseT = 0;
							gaugeEasePos = 0;
							UIPosy = 0;
							screenSize = 1.0f;
							player.pos = { 0,0 };
							for (int i = 0; i < kPreNum; i++)
							{
								playerPrePos[i] = { 0,0 };
							}
							player.preDirection = { 0,0 };
							scroll.x = (-player.pos.x / screenSize + 960);
							scroll.y = (-player.pos.y / screenSize + 540);
							ennergy.count = 0;
							game = 0;
							allReset = true;

						}
					}
				}

				break;

			}






			///                                                            ///
			/// --------------------↑更新処理ここまで-------------------- ///
			///                                                            ///

			///                                                            ///
			/// --------------------↓描画処理ここから-------------------- ///
			///                                                            ///


		/*	Novice::ScreenPrintf(0, 200, "joystickX=%d joystickY=%d", joystickX, joystickY);
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
			Novice::ScreenPrintf(0, 540, "setScreenEaseT=%f", setScreenEaseT);
			Novice::ScreenPrintf(0, 560, "gameTimer=%d seconds=%d", gameTimer, gameTimer / 60);
			Novice::ScreenPrintf(0, 580, "score=%d", score);
			Novice::ScreenPrintf(0, 600, "cameraEaseT=%0.3f", cameraEaseT);
			Novice::ScreenPrintf(0, 620, "game=%d", game);
			Novice::ScreenPrintf(0, 640, "blackColor=%d", blackColor);*/

			/*for (int i = 0; i < variation; i++)
			{
				Novice::ScreenPrintf(700, i * 20, "patterrnIsAlive[%d]=%d", i, patterrnIsAlive[i]);
			}*/

			//Novice::ScreenPrintf(1000, 0, "enemy7.count=%d", enemy7.count);
			/*Novice::ScreenPrintf(700, 20 * variation, "randSpown=%d", randSpown);
			Novice::ScreenPrintf(700, 20 + 20 * variation, "preRandSpown=%d", preRandSpown);
			Novice::ScreenPrintf(700, 40 + 20 * variation, "prepreRandSpown=%d", prepreRandSpown);
			Novice::ScreenPrintf(700, 60 + 20 * variation, "pre3RandSpown=%d", pre3RandSpown);
			Novice::ScreenPrintf(1000, 0, "enemy2.hostIsAlive=%d enemy2.timer=%d", enemy2.hostIsAlive, enemy9.timer);
			Novice::ScreenPrintf(1000, 20, "enemy7.hostIsAlive=%d enemy7.timer=%d", enemy7.hostIsAlive, enemy9.timer);
			Novice::ScreenPrintf(1000, 40, "enemy8.hostIsAlive=%d enemy8.timer=%d", enemy8.hostIsAlive, enemy9.timer);
			Novice::ScreenPrintf(1000, 60, "enemy9.hostIsAlive=%d enemy9.timer=%d", enemy9.hostIsAlive, enemy9.timer);
			Novice::ScreenPrintf(1000, 80, "enemy13.hostIsAlive=%d", enemy13.hostIsAlive);*/
			//Novice::ScreenPrintf(1000, 100, "quickTimer=%d", quickTimer);


			//一番後ろの背景
			Novice::DrawBox(0, 0, 1920, 1080, 0, 0x111111ff, kFillModeSolid);
			//フィールドの画像表示
			Functions.DrawQuadPlus(int(scroll.x), int(scroll.y), int(fieldRadius * 2 / screenSize), int(fieldRadius * 2 / screenSize), 1.0f, 1.0f, 0, 0, 0, 4000, 4000, fieldTexture, 0x777777ff, "center");
			//三角形を作った時
			if (player.aim)Novice::DrawBox(0, 0, 1920, 1080, 0, 0x88888811, kFillModeSolid);
			//三角形で攻撃時間
			if (player.aimTimer > 0)Novice::DrawBox(0, 0, 1920, 1080, 0, 0xaa666611, kFillModeSolid);

			//デバッグ用
			//Novice::DrawEllipse(int(100 + player.joystick.x), int(100 + player.joystick.y), 10, 10, 0, GREEN, kFillModeSolid);//直接入力
			//Novice::DrawEllipse(int(100 + player.direction.x * 100), int(100 + player.direction.y * 100), 10, 10, 0, RED, kFillModeSolid);//方向
			//Novice::DrawEllipse(int(100 + player.velocity.x), int(100 + player.velocity.y), 10, 10, 0, BLUE, kFillModeSolid);//速度
			//Novice::DrawEllipse(100, 100, 100, 100, 0, WHITE, kFillModeWireFrame);//最大範囲
			//Novice::DrawEllipse(100, 100, (int)dedZone, (int)dedZone, 0, GREEN, kFillModeWireFrame);//デッドゾーン

			////フィールドの円周
			//Novice::DrawEllipse(int(scroll.x), int(scroll.y), (int)(fieldRadius / screenSize), (int)(fieldRadius / screenSize), 0, GREEN, kFillModeWireFrame);


			//スポーン地点
			//Novice::DrawBox(int(-50 / screenSize + scroll.x), int(-50 / screenSize + scroll.y), 100, 100, 0, RED, kFillModeWireFrame);
			//プレイヤーの方向表示
			//Novice::DrawLine(int(player.pos.x / screenSize + scroll.x), int(player.pos.y / screenSize + scroll.y), int(player.pos.x / screenSize + scroll.x + player.preDirection.x / screenSize * 150), int(player.pos.y / screenSize + scroll.y + player.preDirection.y / screenSize * 150), WHITE);

			for (int i = 0; i < kPreNum; i++)
			{
				Functions.DrawQuadPlus(int(playerPrePos[i].x / screenSize + scroll.x), int(playerPrePos[i].y / screenSize + scroll.y), int(player.radius.x * 2 / screenSize), int(player.radius.x * 2 / screenSize), (0.4f * (float(kPreNum - i) / kPreNum) + 0.1f), (0.4f * (float(kPreNum - i) / kPreNum) + 0.1f), ((gameTimer * (kPreNum - i + 1) + 1) % 360 / 180.0f) * 3.1415f, 0, 0, 1, 1, kWhiteTexture, prePosColor[i], "center");
			}
			//プレイヤー
			//Novice::DrawEllipse(int(player.pos.x / screenSize + scroll.x), int(player.pos.y / screenSize + scroll.y), int(player.radius.x / screenSize), int(player.radius.y / screenSize), 0, 0xffffffff, kFillModeSolid);
			//プレイヤーフリック時
			//if (player.flick)Novice::DrawEllipse(int(player.pos.x / screenSize + scroll.x), int(player.pos.y / screenSize + scroll.y), int(player.radius.x / screenSize), int(player.radius.y / screenSize), 0, 0x00ffffff, kFillModeSolid);//0x55ff5599
			//if (player.aim || player.triangulAttack)Novice::DrawEllipse(int(player.pos.x / screenSize + scroll.x), int(player.pos.y / screenSize + scroll.y), int(player.radius.x / screenSize), int(player.radius.y / screenSize), 0, 0x33ff33ff, kFillModeSolid);
			if (player.preDirection.x != 0 || player.preDirection.y != 0)
			{
				if (player.flick) {
					Functions.DrawQuadPlus(int(player.pos.x / screenSize + scroll.x), int(player.pos.y / screenSize + scroll.y), int(310 / screenSize), int(310 / screenSize), 1, 1, directionTheta, 310 * (directionMove / 2), 0, 310, 310, directionTexture, 0x00ffffff, "center");
				}
				else if (player.aim || player.triangulAttack)
				{
					Functions.DrawQuadPlus(int(player.pos.x / screenSize + scroll.x), int(player.pos.y / screenSize + scroll.y), int(310 / screenSize), int(310 / screenSize), 1, 1, directionTheta, 310 * (directionMove / 2), 0, 310, 310, directionTexture, 0x33ff33ff, "center");

				}
				else
				{
					Functions.DrawQuadPlus(int(player.pos.x / screenSize + scroll.x), int(player.pos.y / screenSize + scroll.y), int(310 / screenSize), int(310 / screenSize), 1, 1, directionTheta, 310 * (directionMove / 2), 0, 310, 310, directionTexture, WHITE, "center");

				}
			}
			directionMove++;
			if (directionMove >= 46)directionMove = 0;

			plth.x = fabsf(player.velocity.x / 2);
			plth.y = fabsf(player.velocity.y / 2);
			Playerth += plth.x + plth.y;

			//プレイヤー
			if (player.flick) {
				Functions.DrawQuadPlus(int(player.pos.x / screenSize + scroll.x), int(player.pos.y / screenSize + scroll.y), int(player.radius.x * 2 / screenSize), int(player.radius.y * 2 / screenSize), 1.0f, 1.0f, (Playerth / 180.0f) * 3.1415f, 0, 0, 64, 64, playerTexture, 0x00ffffff, "center");
			}
			else if (player.aim || player.triangulAttack)
			{
				Functions.DrawQuadPlus(int(player.pos.x / screenSize + scroll.x), int(player.pos.y / screenSize + scroll.y), int(player.radius.x * 2 / screenSize), int(player.radius.y * 2 / screenSize), 1.0f, 1.0f, (Playerth / 180.0f) * 3.1415f, 0, 0, 64, 64, playerTexture, 0x33ff33ff, "center");

			}
			else
			{
				Functions.DrawQuadPlus(int(player.pos.x / screenSize + scroll.x), int(player.pos.y / screenSize + scroll.y), int(player.radius.x * 2 / screenSize), int(player.radius.y * 2 / screenSize), 1.0f, 1.0f, (Playerth / 180.0f) * 3.1415f, 0, 0, 64, 64, playerTexture, WHITE, "center");

			}


			//仮敵描画
#pragma region enemy
			for (int j = 0; j < kenemy10Num; j++)
			{
				if (enemy10[j].parentIsAlive)
				{
					for (int i = 0; i < 4; i++)
					{
						if (enemy10[j].childIsAlive[i])
						{
							if (!enemy10[j].isDed[i])Functions.DrawQuadPlus(int(enemy10[j].relativePos[i].x / screenSize + scroll.x), int(enemy10[j].relativePos[i].y / screenSize + scroll.y), int(enemy10[j].radius * 2 / screenSize), int(enemy10[j].radius * 2 / screenSize), 1, 1, float(float(gameTimer * 2 % 360) / 180 * 3.1415f), 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[0], 0xffffffff, "center");
							if (enemy10[j].isDed[i] && enemy10[j].dedTimer[i] > 0) {
								Functions.DrawQuadPlus(int(enemy10[j].relativePos[i].x / screenSize + scroll.x), int(enemy10[j].relativePos[i].y / screenSize + scroll.y), int((enemy10[j].radius * 2 - (kDedTimer - enemy10[j].dedTimer[i]) * 3) / screenSize), int((enemy10[j].radius * 2 - (kDedTimer - enemy10[j].dedTimer[i]) * 3) / screenSize), 1, 1, float(float(gameTimer * 1 % 360) / 180 * 3.1415f), 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[0], BLUE, "center");
								Functions.DrawQuadPlus(int(enemy10[j].relativePos[i].x / screenSize + scroll.x), int(enemy10[j].relativePos[i].y / screenSize + scroll.y), int((256) / screenSize), int((256) / screenSize), 1, 1, float(float(gameTimer * 5 % 360) / 180 * 3.1415f), 256 * ((kDedTimer - enemy10[j].dedTimer[i]) / 3), 0, 256, 256, enemyDeadTexture, WHITE, "center");

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
						if (enemy11[j].childIsAlive[i])
						{
							if (!enemy11[j].isDed[i])Functions.DrawQuadPlus(int(enemy11[j].relativePos[i].x / screenSize + scroll.x), int(enemy11[j].relativePos[i].y / screenSize + scroll.y), int(enemy11[j].radius * 2 / screenSize), int(enemy11[j].radius * 2 / screenSize), 1, 1, float(float(gameTimer * 2 % 360) / 180 * 3.1415f), 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[0], 0xffffffff, "center");
							if (enemy11[j].isDed[i] && enemy11[j].dedTimer[i] > 0) {
								Functions.DrawQuadPlus(int(enemy11[j].relativePos[i].x / screenSize + scroll.x), int(enemy11[j].relativePos[i].y / screenSize + scroll.y), int((enemy11[j].radius * 2 - (kDedTimer - enemy11[j].dedTimer[i]) * 3) / screenSize), int((enemy11[j].radius * 2 - (kDedTimer - enemy11[j].dedTimer[i]) * 3) / screenSize), 1, 1, float(float(gameTimer * 1 % 360) / 180 * 3.1415f), 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[0], BLUE, "center");
								Functions.DrawQuadPlus(int(enemy11[j].relativePos[i].x / screenSize + scroll.x), int(enemy11[j].relativePos[i].y / screenSize + scroll.y), int((256) / screenSize), int((256) / screenSize), 1, 1, float(float(gameTimer * 5 % 360) / 180 * 3.1415f), 256 * ((kDedTimer - enemy11[j].dedTimer[i]) / 3), 0, 256, 256, enemyDeadTexture, WHITE, "center");
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
						if (enemy12[j].childIsAlive[i])
						{
							if (!enemy12[j].isDed[i])Functions.DrawQuadPlus(int(enemy12[j].relativePos[i].x / screenSize + scroll.x), int(enemy12[j].relativePos[i].y / screenSize + scroll.y), int(enemy12[j].radius * 2 / screenSize), int(enemy12[j].radius * 2 / screenSize), 1, 1, float(float(gameTimer * 2 % 360) / 180 * 3.1415f), 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[0], 0xffffffff, "center");
							if (enemy12[j].isDed[i] && enemy12[j].dedTimer[i] > 0) {
								Functions.DrawQuadPlus(int(enemy12[j].relativePos[i].x / screenSize + scroll.x), int(enemy12[j].relativePos[i].y / screenSize + scroll.y), int((enemy12[j].radius * 2 - (kDedTimer - enemy12[j].dedTimer[i]) * 3) / screenSize), int((enemy12[j].radius * 2 - (kDedTimer - enemy12[j].dedTimer[i]) * 3) / screenSize), 1, 1, float(float(gameTimer * 1 % 360) / 180 * 3.1415f), 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[0], BLUE, "center");
								Functions.DrawQuadPlus(int(enemy12[j].relativePos[i].x / screenSize + scroll.x), int(enemy12[j].relativePos[i].y / screenSize + scroll.y), int((256) / screenSize), int((256) / screenSize), 1, 1, float(float(gameTimer * 5 % 360) / 180 * 3.1415f), 256 * ((kDedTimer - enemy12[j].dedTimer[i]) / 3), 0, 256, 256, enemyDeadTexture, WHITE, "center");

							}
						}
					}
				}
			}



			for (int i = 0; i < 4; i++)
			{
				if (enemy5.childIsAlive[i])
				{
					if (!enemy5.isDed[i])Functions.DrawQuadPlus(int(enemy5.relativePos[i].x / screenSize + scroll.x), int(enemy5.relativePos[i].y / screenSize + scroll.y), int(enemy5.radius * 2 / screenSize), int(enemy5.radius * 2 / screenSize), 1, 1, float(float(gameTimer * 2 % 360) / 180 * 3.1415f), 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[0], 0xffffffff, "center");
					if (enemy5.isDed[i] && enemy5.dedTimer[i] > 0) {
						Functions.DrawQuadPlus(int(enemy5.relativePos[i].x / screenSize + scroll.x), int(enemy5.relativePos[i].y / screenSize + scroll.y), int((enemy5.radius * 2 - (kDedTimer - enemy5.dedTimer[i]) * 3) / screenSize), int((enemy5.radius * 2 - (kDedTimer - enemy5.dedTimer[i]) * 3) / screenSize), 1, 1, float(float(gameTimer * 1 % 360) / 180 * 3.1415f), 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[0], BLUE, "center");
						Functions.DrawQuadPlus(int(enemy5.relativePos[i].x / screenSize + scroll.x), int(enemy5.relativePos[i].y / screenSize + scroll.y), int((256) / screenSize), int((256) / screenSize), 1, 1, float(float(gameTimer * 5 % 360) / 180 * 3.1415f), 256 * ((kDedTimer - enemy5.dedTimer[i]) / 3), 0, 256, 256, enemyDeadTexture, WHITE, "center");

					}
				}
			}

			for (int i = 0; i < 4; i++)
			{
				if (enemy6.childIsAlive[i])
				{
					if (!enemy6.isDed[i])Functions.DrawQuadPlus(int(enemy6.relativePos[i].x / screenSize + scroll.x), int(enemy6.relativePos[i].y / screenSize + scroll.y), int(enemy6.radius * 2 / screenSize), int(enemy6.radius * 2 / screenSize), 1, 1, float(float(gameTimer * 2 % 360) / 180 * 3.1415f), 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[0], 0xffffffff, "center");
					if (enemy6.isDed[i] && enemy6.dedTimer[i] > 0) {
						Functions.DrawQuadPlus(int(enemy6.relativePos[i].x / screenSize + scroll.x), int(enemy6.relativePos[i].y / screenSize + scroll.y), int((enemy6.radius * 2 - (kDedTimer - enemy6.dedTimer[i]) * 3) / screenSize), int((enemy6.radius * 2 - (kDedTimer - enemy6.dedTimer[i]) * 3) / screenSize), 1, 1, float(float(gameTimer * 1 % 360) / 180 * 3.1415f), 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[0], BLUE, "center");
						Functions.DrawQuadPlus(int(enemy6.relativePos[i].x / screenSize + scroll.x), int(enemy6.relativePos[i].y / screenSize + scroll.y), int((256) / screenSize), int((256) / screenSize), 1, 1, float(float(gameTimer * 5 % 360) / 180 * 3.1415f), 256 * ((kDedTimer - enemy6.dedTimer[i]) / 3), 0, 256, 256, enemyDeadTexture, WHITE, "center");

					}
				}
			}
			if (enemy2.hostIsAlive)
			{
				for (int j = 0; j < 4; j++)
				{
					for (int i = 0; i < kEnemy2Num; i++)
					{
						if (enemy2.childIsAlive[j][i])
						{
							if (!enemy2.isDed[j][i])Functions.DrawQuadPlus(int(enemy2.relativePos[j][i].x / screenSize + scroll.x), int(enemy2.relativePos[j][i].y / screenSize + scroll.y), int(enemy2.radius * 2 / screenSize), int(enemy2.radius * 2 / screenSize), 1, 1, float(float(gameTimer * 2 % 360) / 180 * 3.1415f), 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[1], WHITE, "center");
							if (enemy2.isDed[j][i] && enemy2.dedTimer[j][i] > 0) {
								Functions.DrawQuadPlus(int(enemy2.relativePos[j][i].x / screenSize + scroll.x), int(enemy2.relativePos[j][i].y / screenSize + scroll.y), int((enemy2.radius * 2 - (kDedTimer - enemy2.dedTimer[j][i]) * 3) / screenSize), int((enemy2.radius * 2 - (kDedTimer - enemy2.dedTimer[j][i]) * 3) / screenSize), 1, 1, float(float(gameTimer * 1 % 360) / 180 * 3.1415f), 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[1], BLUE, "center");
								Functions.DrawQuadPlus(int(enemy2.relativePos[j][i].x / screenSize + scroll.x), int(enemy2.relativePos[j][i].y / screenSize + scroll.y), int((256) / screenSize), int((256) / screenSize), 1, 1, float(float(gameTimer * 5 % 360) / 180 * 3.1415f), 256 * ((kDedTimer - enemy2.dedTimer[j][i]) / 3), 0, 256, 256, enemyDeadTexture, WHITE, "center");

							}
						}
					}
				}
			}

			if (enemy8.hostIsAlive)
			{
				for (int j = 0; j < 4; j++)
				{
					for (int i = 0; i < 12; i++)
					{
						if (enemy8.childIsAlive[j][i])
						{
							if (!enemy8.isDed[j][i])Functions.DrawQuadPlus(int(enemy8.relativePos[j][i].x / screenSize + scroll.x), int(enemy8.relativePos[j][i].y / screenSize + scroll.y), int(enemy8.radius * 2 / screenSize), int(enemy8.radius * 2 / screenSize), 1, 1, float(float(gameTimer * 2 % 360) / 180 * 3.1415f), 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[2], 0xffffffff, "center");
							if (enemy8.isDed[j][i] && enemy8.dedTimer[j][i] > 0) {
								Functions.DrawQuadPlus(int(enemy8.relativePos[j][i].x / screenSize + scroll.x), int(enemy8.relativePos[j][i].y / screenSize + scroll.y), int((enemy8.radius * 2 - (kDedTimer - enemy8.dedTimer[j][i])) / screenSize), int((enemy8.radius * 2 - (kDedTimer - enemy8.dedTimer[j][i])) / screenSize), 1, 1, float(float(gameTimer * 1 % 360) / 180 * 3.1415f), 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[2], BLUE, "center");
								Functions.DrawQuadPlus(int(enemy8.relativePos[j][i].x / screenSize + scroll.x), int(enemy8.relativePos[j][i].y / screenSize + scroll.y), int((256) / screenSize), int((256) / screenSize), 1, 1, float(float(gameTimer * 5 % 360) / 180 * 3.1415f), 256 * ((kDedTimer - enemy8.dedTimer[j][i]) / 3), 0, 256, 256, enemyDeadTexture, WHITE, "center");

							}
						}
					}
				}
			}
			if (enemy7.hostIsAlive)
			{
				for (int j = 0; j < 8; j++)
				{

					for (int i = 0; i < 4; i++)
					{
						if (enemy7.childIsAlive[j][i])
						{
							if (!enemy7.isDed[j][i])Functions.DrawQuadPlus(int(enemy7.relativePos[j][i].x / screenSize + scroll.x), int(enemy7.relativePos[j][i].y / screenSize + scroll.y), int(enemy7.radius * 2 / screenSize), int(enemy7.radius * 2 / screenSize), 1, 1, float(float(gameTimer * 2 % 360) / 180 * 3.1415f), 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[3], 0xffffffff, "center");
							if (enemy7.isDed[j][i] && enemy7.dedTimer[j][i] > 0) {
								Functions.DrawQuadPlus(int(enemy7.relativePos[j][i].x / screenSize + scroll.x), int(enemy7.relativePos[j][i].y / screenSize + scroll.y), int((enemy7.radius * 2 - (kDedTimer - enemy7.dedTimer[j][i]) * 3) / screenSize), int((enemy7.radius * 2 - (kDedTimer - enemy7.dedTimer[j][i]) * 3) / screenSize), 1, 1, float(float(gameTimer * 1 % 360) / 180 * 3.1415f), 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[3], BLUE, "center");
								Functions.DrawQuadPlus(int(enemy7.relativePos[j][i].x / screenSize + scroll.x), int(enemy7.relativePos[j][i].y / screenSize + scroll.y), int((256) / screenSize), int((256) / screenSize), 1, 1, float(float(gameTimer * 5 % 360) / 180 * 3.1415f), 256 * ((kDedTimer - enemy7.dedTimer[j][i]) / 3), 0, 256, 256, enemyDeadTexture, WHITE, "center");

							}
						}
					}
				}
			}
			if (enemy9.hostIsAlive)
			{
				for (int j = 0; j < 2; j++)
				{
					for (int i = 0; i < 8; i++)
					{
						if (enemy9.childIsAlive[j][i])
						{
							if (!enemy9.isDed[j][i])Functions.DrawQuadPlus(int(enemy9.relativePos[j][i].x / screenSize + scroll.x), int(enemy9.relativePos[j][i].y / screenSize + scroll.y), int(enemy9.radius * 2 / screenSize), int(enemy9.radius * 2 / screenSize), 1, 1, float(float(gameTimer * 2 % 360) / 180 * 3.1415f), 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[4], 0xffffffff, "center");
							if (enemy9.isDed[j][i] && enemy9.dedTimer[j][i] > 0) {
								Functions.DrawQuadPlus(int(enemy9.relativePos[j][i].x / screenSize + scroll.x), int(enemy9.relativePos[j][i].y / screenSize + scroll.y), int((enemy9.radius * 2 - (kDedTimer - enemy9.dedTimer[j][i]) * 3) / screenSize), int((enemy9.radius * 2 - (kDedTimer - enemy9.dedTimer[j][i]) * 3) / screenSize), 1, 1, float(float(gameTimer * 1 % 360) / 180 * 3.1415f), 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[4], BLUE, "center");
								Functions.DrawQuadPlus(int(enemy9.relativePos[j][i].x / screenSize + scroll.x), int(enemy9.relativePos[j][i].y / screenSize + scroll.y), int((256) / screenSize), int((256) / screenSize), 1, 1, float(float(gameTimer * 5 % 360) / 180 * 3.1415f), 256 * ((kDedTimer - enemy9.dedTimer[j][i]) / 3), 0, 256, 256, enemyDeadTexture, WHITE, "center");

							}
						}
					}
				}
			}




			for (int i = 0; i < 60; i++)
			{
				if (enemy13.childIsAlive[i])
				{
					if (!enemy13.isDed[i])Functions.DrawQuadPlus(int(enemy13.relativePos[i].x / screenSize + scroll.x), int(enemy13.relativePos[i].y / screenSize + scroll.y), int(enemy13.radius[i] * 2 / screenSize), int(enemy13.radius[i] * 2 / screenSize), 1, 1, float(float(gameTimer * 2 % 360) / 180 * 3.1415f), 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[5], 0xffffffff, "center");
					if (enemy13.isDed[i] && enemy13.dedTimer[i] > 0) {
						Functions.DrawQuadPlus(int(enemy13.relativePos[i].x / screenSize + scroll.x), int(enemy13.relativePos[i].y / screenSize + scroll.y), int((enemy13.radius[i] * 2 - (kDedTimer - enemy13.dedTimer[i]) * 3) / screenSize), int((enemy13.radius[i] * 2 - (kDedTimer - enemy13.dedTimer[i]) * 3) / screenSize), 1, 1, float(float(gameTimer * 1 % 360) / 180 * 3.1415f), 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[5], BLUE, "center");
						Functions.DrawQuadPlus(int(enemy13.relativePos[i].x / screenSize + scroll.x), int(enemy13.relativePos[i].y / screenSize + scroll.y), int((256) / screenSize), int((256) / screenSize), 1, 1, float(float(gameTimer * 5 % 360) / 180 * 3.1415f), 256 * ((kDedTimer - enemy13.dedTimer[i]) / 3), 0, 256, 256, enemyDeadTexture, WHITE, "center");

					}
				}
			}
#pragma endregion

			if (player.aimTimer > 0)Novice::DrawTriangle(int(player.prepos[0].x / screenSize + scroll.x), int(player.prepos[0].y / screenSize + scroll.y), int(player.prepos[1].x / screenSize + scroll.x), int(player.prepos[1].y / screenSize + scroll.y), int(player.prepos[2].x / screenSize + scroll.x), int(player.prepos[2].y / screenSize + scroll.y), 0x55cc5577, kFillModeSolid);

			//点と点を結ぶ線
			if (player.prepos[0].x != 0 && player.prepos[1].x != 0)Novice::DrawLine(int(player.prepos[0].x / screenSize + scroll.x), int(player.prepos[0].y / screenSize + scroll.y), int(player.prepos[1].x / screenSize + scroll.x), int(player.prepos[1].y / screenSize + scroll.y), 0x00ff00ff);
			if (player.prepos[0].x != 0 && player.prepos[1].x != 0)Novice::DrawLine(int(player.prepos[0].x / screenSize + scroll.x), int(player.prepos[0].y / screenSize + scroll.y - 1), int(player.prepos[1].x / screenSize + scroll.x), int(player.prepos[1].y / screenSize + scroll.y - 1), 0x00ff00ff);
			if (player.prepos[0].x != 0 && player.prepos[1].x != 0)Novice::DrawLine(int(player.prepos[0].x / screenSize + scroll.x), int(player.prepos[0].y / screenSize + scroll.y + 1), int(player.prepos[1].x / screenSize + scroll.x), int(player.prepos[1].y / screenSize + scroll.y + 1), 0x00ff00ff);
			if (player.prepos[1].x != 0 && player.prepos[2].x != 0)Novice::DrawLine(int(player.prepos[1].x / screenSize + scroll.x), int(player.prepos[1].y / screenSize + scroll.y), int(player.prepos[2].x / screenSize + scroll.x), int(player.prepos[2].y / screenSize + scroll.y), 0x00ff00ff);
			if (player.prepos[1].x != 0 && player.prepos[2].x != 0)Novice::DrawLine(int(player.prepos[1].x / screenSize + scroll.x), int(player.prepos[1].y / screenSize + scroll.y - 1), int(player.prepos[2].x / screenSize + scroll.x), int(player.prepos[2].y / screenSize + scroll.y - 1), 0x00ff00ff);
			if (player.prepos[1].x != 0 && player.prepos[2].x != 0)Novice::DrawLine(int(player.prepos[1].x / screenSize + scroll.x), int(player.prepos[1].y / screenSize + scroll.y + 1), int(player.prepos[2].x / screenSize + scroll.x), int(player.prepos[2].y / screenSize + scroll.y + 1), 0x00ff00ff);
			if (player.prepos[2].x != 0 && player.prepos[0].x != 0)Novice::DrawLine(int(player.prepos[2].x / screenSize + scroll.x), int(player.prepos[2].y / screenSize + scroll.y), int(player.prepos[0].x / screenSize + scroll.x), int(player.prepos[0].y / screenSize + scroll.y), 0x00ff00ff);
			if (player.prepos[2].x != 0 && player.prepos[0].x != 0)Novice::DrawLine(int(player.prepos[2].x / screenSize + scroll.x), int(player.prepos[2].y / screenSize + scroll.y - 1), int(player.prepos[0].x / screenSize + scroll.x), int(player.prepos[0].y / screenSize + scroll.y - 1), 0x00ff00ff);
			if (player.prepos[2].x != 0 && player.prepos[0].x != 0)Novice::DrawLine(int(player.prepos[2].x / screenSize + scroll.x), int(player.prepos[2].y / screenSize + scroll.y + 1), int(player.prepos[0].x / screenSize + scroll.x), int(player.prepos[0].y / screenSize + scroll.y + 1), 0x00ff00ff);
			//三角形の点
			for (int i = 0; i < 3; i++)
			{
				if (player.prepos[i].x != 0)Functions.DrawQuadPlus(int(player.prepos[i].x / screenSize + scroll.x), int(player.prepos[i].y / screenSize + scroll.y), int(player.anchorRadius * 2 / screenSize), int(player.anchorRadius * 2 / screenSize), 1, 1, ((gameTimer * 3 + 1) % 360 / 180.0f) * 3.1415f, 0, 0, 1, 1, kWhiteTexture, 0x00ff00ff, "center");
				//if (player.prepos[i].x != 0)Novice::DrawEllipse(int(player.prepos[i].x / screenSize + scroll.x), int(player.prepos[i].y / screenSize + scroll.y), int(player.anchorRadius / screenSize), int(player.anchorRadius / screenSize), 0, 0x00ff00ff, kFillModeSolid);
			}


			//エクセレント文字を

			if (quickFlag)
			{
				Functions.DrawQuadPlus(784, 660, quickTimer, 48, 1, 1, 0, 0, 0, quickTimer, 48, excellentTexture[quickNum], WHITE, "leftTop");
			}
			//フィーバーゲージ仮


			//後ろ
			Novice::DrawQuad(gaugex + int(shakeGaugePos.x), gaugey - int(gaugeEasePos) + int(shakeGaugePos.y), gaugex + int(shakeGaugePos.x) + gaugeWidth, gaugey - int(gaugeEasePos) + int(shakeGaugePos.y), gaugex + int(shakeGaugePos.x), gaugey - int(gaugeEasePos) + int(shakeGaugePos.y) + gaugeHeidth, gaugex + int(shakeGaugePos.x) + gaugeWidth, gaugey - int(gaugeEasePos) + int(shakeGaugePos.y) + gaugeHeidth, 0, 0, gaugeWidth, gaugeHeidth, gaugeBackTexture, WHITE);

			//変動するレイヤー
			if (!ennergy.fever)
			{
				Novice::DrawQuad(gaugex + int(shakeGaugePos.x), gaugey - int(gaugeEasePos) + int(shakeGaugePos.y), gaugex + int(shakeGaugePos.x) + int(gaugeWidth * (ennergy.count / ennergy.max)), gaugey - int(gaugeEasePos) + int(shakeGaugePos.y), gaugex + int(shakeGaugePos.x), gaugey - int(gaugeEasePos) + int(shakeGaugePos.y) + gaugeHeidth, gaugex + int(shakeGaugePos.x) + int(gaugeWidth * (ennergy.count / ennergy.max)), gaugey - int(gaugeEasePos) + int(shakeGaugePos.y) + gaugeHeidth, 0, 0, int(gaugeWidth * (ennergy.count / ennergy.max)), gaugeHeidth, gaugeTexture, WHITE);

			}
			else
			{
				Novice::DrawQuad(gaugex + int(shakeGaugePos.x), gaugey - int(gaugeEasePos) + int(shakeGaugePos.y), gaugex + int(shakeGaugePos.x) + int(gaugeWidth * (ennergy.count / ennergy.max)), gaugey - int(gaugeEasePos) + int(shakeGaugePos.y), gaugex + int(shakeGaugePos.x), gaugey - int(gaugeEasePos) + int(shakeGaugePos.y) + gaugeHeidth, gaugex + int(shakeGaugePos.x) + int(gaugeWidth * (ennergy.count / ennergy.max)), gaugey - int(gaugeEasePos) + int(shakeGaugePos.y) + gaugeHeidth, 0, 0, int(gaugeWidth * (ennergy.count / ennergy.max)), gaugeHeidth, gaugeTexture, 0xffff00ff);

			}

			//ミニマップ
			Novice::DrawEllipse(int(50 + fieldRadius / miniMap), int(1020 - fieldRadius / miniMap), int(fieldRadius / miniMap), int(fieldRadius / miniMap), 0, GREEN, kFillModeWireFrame);


			//ミニマップランダム横一列
			for (int j = 0; j < kenemy10Num; j++)
			{
				if (enemy10[j].parentIsAlive)
				{
					for (int i = 0; i < 4; i++)
					{
						if (enemy10[j].childIsAlive[i])
						{
							if (!enemy10[j].isDed[i])Functions.DrawQuadPlus(int((50 + fieldRadius / miniMap) + enemy10[j].relativePos[i].x / miniMap), int((1020 - fieldRadius / miniMap) + enemy10[j].relativePos[i].y / miniMap), int(enemy10[j].radius * miniMapPlayerSize / miniMap), int(enemy10[j].radius * miniMapPlayerSize / miniMap), 1, 1, 0, 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[0], 0xffffffff, "center");
							if (enemy10[j].isDed[i] && enemy10[j].dedTimer[i] > 0)Functions.DrawQuadPlus(int((50 + fieldRadius / miniMap) + enemy10[j].relativePos[i].x / miniMap), int((1020 - fieldRadius / miniMap) + enemy10[j].relativePos[i].y / miniMap), int(enemy10[j].radius * miniMapPlayerSize / miniMap), int(enemy10[j].radius * miniMapPlayerSize / miniMap), 1, 1, 0, 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[0], BLUE, "center");
						}
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
							if (!enemy11[j].isDed[i])Functions.DrawQuadPlus(int((50 + fieldRadius / miniMap) + enemy11[j].relativePos[i].x / miniMap), int((1020 - fieldRadius / miniMap) + enemy11[j].relativePos[i].y / miniMap), int(enemy11[j].radius * miniMapPlayerSize / miniMap), int(enemy11[j].radius * miniMapPlayerSize / miniMap), 1, 1, 0, 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[0], 0xffffffff, "center");
							if (enemy11[j].isDed[i] && enemy11[j].dedTimer[i] > 0)Functions.DrawQuadPlus(int((50 + fieldRadius / miniMap) + enemy11[j].relativePos[i].x / miniMap), int((1020 - fieldRadius / miniMap) + enemy11[j].relativePos[i].y / miniMap), int(enemy11[j].radius * miniMapPlayerSize / miniMap), int(enemy11[j].radius * miniMapPlayerSize / miniMap), 1, 1, 0, 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[0], BLUE, "center");
						}
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
							if (!enemy12[j].isDed[i])Functions.DrawQuadPlus(int((50 + fieldRadius / miniMap) + enemy12[j].relativePos[i].x / miniMap), int((1020 - fieldRadius / miniMap) + enemy12[j].relativePos[i].y / miniMap), int(enemy12[j].radius * miniMapPlayerSize / miniMap), int(enemy12[j].radius * miniMapPlayerSize / miniMap), 1, 1, 0, 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[0], 0xffffffff, "center");
							if (enemy12[j].isDed[i] && enemy12[j].dedTimer[i] > 0)Functions.DrawQuadPlus(int((50 + fieldRadius / miniMap) + enemy12[j].relativePos[i].x / miniMap), int((1020 - fieldRadius / miniMap) + enemy12[j].relativePos[i].y / miniMap), int(enemy12[j].radius * miniMapPlayerSize / miniMap), int(enemy12[j].radius * miniMapPlayerSize / miniMap), 1, 1, 0, 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[0], BLUE, "center");
						}
					}
				}
			}

			//ミニマップ左上
			for (int i = 0; i < 4; i++)
			{
				if (enemy5.childIsAlive[i])
				{
					if (!enemy5.isDed[i])Functions.DrawQuadPlus(int((50 + fieldRadius / miniMap) + enemy5.relativePos[i].x / miniMap), int((1020 - fieldRadius / miniMap) + enemy5.relativePos[i].y / miniMap), int(enemy5.radius * miniMapPlayerSize / miniMap), int(enemy5.radius * miniMapPlayerSize / miniMap), 1, 1, 0, 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[0], 0xffffffff, "center");
					if (enemy5.isDed[i] && enemy5.dedTimer[i] > 0)Functions.DrawQuadPlus(int((50 + fieldRadius / miniMap) + enemy5.relativePos[i].x / miniMap), int((1020 - fieldRadius / miniMap) + enemy5.relativePos[i].y / miniMap), int(enemy5.radius * miniMapPlayerSize / miniMap), int(enemy5.radius * miniMapPlayerSize / miniMap), 1, 1, 0, 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[0], BLUE, "center");
				}
			}
			//ミニマップ右上
			for (int i = 0; i < 4; i++)
			{
				if (enemy6.childIsAlive[i])
				{
					if (!enemy6.isDed[i])Functions.DrawQuadPlus(int((50 + fieldRadius / miniMap) + enemy6.relativePos[i].x / miniMap), int((1020 - fieldRadius / miniMap) + enemy6.relativePos[i].y / miniMap), int(enemy6.radius * miniMapPlayerSize / miniMap), int(enemy6.radius * miniMapPlayerSize / miniMap), 1, 1, 0, 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[0], 0xffffffff, "center");
					if (enemy6.isDed[i] && enemy6.dedTimer[i] > 0)Functions.DrawQuadPlus(int((50 + fieldRadius / miniMap) + enemy6.relativePos[i].x / miniMap), int((1020 - fieldRadius / miniMap) + enemy6.relativePos[i].y / miniMap), int(enemy6.radius * miniMapPlayerSize / miniMap), int(enemy6.radius * miniMapPlayerSize / miniMap), 1, 1, 0, 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[0], BLUE, "center");
				}
			}
			//ミニマップシシカク
			if (enemy2.hostIsAlive)
			{
				for (int j = 0; j < 4; j++)
				{
					for (int i = 0; i < kEnemy2Num; i++)
					{
						if (enemy2.childIsAlive[j][i])
						{
							if (!enemy2.isDed[j][i])Functions.DrawQuadPlus(int((50 + fieldRadius / miniMap) + enemy2.relativePos[j][i].x / miniMap), int((1020 - fieldRadius / miniMap) + enemy2.relativePos[j][i].y / miniMap), int(enemy2.radius * miniMapPlayerSize / miniMap), int(enemy2.radius * miniMapPlayerSize / miniMap), 1, 1, 0, 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[1], WHITE, "center");
							if (enemy2.isDed[j][i] && enemy2.dedTimer[j][i] > 0)Functions.DrawQuadPlus(int((50 + fieldRadius / miniMap) + enemy2.relativePos[j][i].x / miniMap), int((1020 - fieldRadius / miniMap) + enemy2.relativePos[j][i].y / miniMap), int(enemy2.radius * miniMapPlayerSize / miniMap), int(enemy2.radius * miniMapPlayerSize / miniMap), 1, 1, 0, 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[1], BLUE, "center");
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
							if (!enemy8.isDed[j][i])Functions.DrawQuadPlus(int((50 + fieldRadius / miniMap) + enemy8.relativePos[j][i].x / miniMap), int((1020 - fieldRadius / miniMap) + enemy8.relativePos[j][i].y / miniMap), int(enemy8.radius * miniMapPlayerSize / miniMap), int(enemy8.radius * miniMapPlayerSize / miniMap), 1, 1, 0, 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[2], 0xffffffff, "center");
							if (enemy8.isDed[j][i] && enemy8.dedTimer[j][i] > 0)Functions.DrawQuadPlus(int((50 + fieldRadius / miniMap) + enemy8.relativePos[j][i].x / miniMap), int((1020 - fieldRadius / miniMap) + enemy8.relativePos[j][i].y / miniMap), int(enemy8.radius * miniMapPlayerSize / miniMap), int(enemy8.radius * miniMapPlayerSize / miniMap), 1, 1, 0, 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[2], BLUE, "center");

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
							if (!enemy7.isDed[j][i])Functions.DrawQuadPlus(int((50 + fieldRadius / miniMap) + enemy7.relativePos[j][i].x / miniMap), int((1020 - fieldRadius / miniMap) + enemy7.relativePos[j][i].y / miniMap), int(enemy7.radius * miniMapPlayerSize / miniMap), int(enemy7.radius * miniMapPlayerSize / miniMap), 1, 1, 0, 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[3], 0xffffffff, "center");
							if (enemy7.isDed[j][i] && enemy7.dedTimer[j][i] > 0)Functions.DrawQuadPlus(int((50 + fieldRadius / miniMap) + enemy7.relativePos[j][i].x / miniMap), int((1020 - fieldRadius / miniMap) + enemy7.relativePos[j][i].y / miniMap), int(enemy7.radius * miniMapPlayerSize / miniMap), int(enemy7.radius * miniMapPlayerSize / miniMap), 1, 1, 0, 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[3], BLUE, "center");
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
							if (!enemy9.isDed[j][i])Functions.DrawQuadPlus(int((50 + fieldRadius / miniMap) + enemy9.relativePos[j][i].x / miniMap), int((1020 - fieldRadius / miniMap) + enemy9.relativePos[j][i].y / miniMap), int(enemy9.radius * miniMapPlayerSize / miniMap), int(enemy9.radius * miniMapPlayerSize / miniMap), 1, 1, 0, 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[4], 0xffffffff, "center");
							if (enemy9.isDed[j][i] && enemy9.dedTimer[j][i] > 0)Functions.DrawQuadPlus(int((50 + fieldRadius / miniMap) + enemy9.relativePos[j][i].x / miniMap), int((1020 - fieldRadius / miniMap) + enemy9.relativePos[j][i].y / miniMap), int(enemy9.radius * miniMapPlayerSize / miniMap), int(enemy9.radius * miniMapPlayerSize / miniMap), 1, 1, 0, 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[4], BLUE, "center");

						}
					}
				}
			}

			//ミニマップ円

			for (int i = 0; i < 60; i++)
			{
				if (enemy13.childIsAlive[i])
				{
					if (!enemy13.isDed[i])Functions.DrawQuadPlus(int((50 + fieldRadius / miniMap) + enemy13.relativePos[i].x / miniMap), int((1020 - fieldRadius / miniMap) + enemy13.relativePos[i].y / miniMap), int(enemy13.radius[i] * miniMapPlayerSize / miniMap), int(enemy13.radius[i] * miniMapPlayerSize / miniMap), 1, 1, 0, 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[5], 0xffffffff, "center");
					if (enemy13.isDed[i] && enemy13.dedTimer[i] > 0)Functions.DrawQuadPlus(int((50 + fieldRadius / miniMap) + enemy13.relativePos[i].x / miniMap), int((1020 - fieldRadius / miniMap) + enemy13.relativePos[i].y / miniMap), int(enemy13.radius[i] * miniMapPlayerSize / miniMap), int(enemy13.radius[i] * miniMapPlayerSize / miniMap), 1, 1, 0, 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[5], BLUE, "center");
				}
			}

			for (int i = 0; i < 3; i++)
			{//ミニマップ三角形の点
				if (player.prepos[i].x != 0)Novice::DrawEllipse(int((50 + fieldRadius / miniMap) + player.prepos[i].x / miniMap), int(int(1020 - fieldRadius / miniMap) + player.prepos[i].y / miniMap), int(player.anchorRadius * miniMapPlayerSize / miniMap), int(player.anchorRadius * miniMapPlayerSize / miniMap), 0, 0x00ff00ff, kFillModeSolid);
			}
			if (player.prepos[0].x != 0 && player.prepos[1].x != 0)Novice::DrawLine(int((50 + fieldRadius / miniMap) + player.prepos[0].x / miniMap), int((1020 - fieldRadius / miniMap) + player.prepos[0].y / miniMap), int((50 + fieldRadius / miniMap) + player.prepos[1].x / miniMap), int((1020 - fieldRadius / miniMap) + player.prepos[1].y / miniMap), 0x00ff00ff);
			if (player.prepos[1].x != 0 && player.prepos[2].x != 0)Novice::DrawLine(int((50 + fieldRadius / miniMap) + player.prepos[1].x / miniMap), int((1020 - fieldRadius / miniMap) + player.prepos[1].y / miniMap), int((50 + fieldRadius / miniMap) + player.prepos[2].x / miniMap), int((1020 - fieldRadius / miniMap) + player.prepos[2].y / miniMap), 0x00ff00ff);
			if (player.prepos[2].x != 0 && player.prepos[0].x != 0)Novice::DrawLine(int((50 + fieldRadius / miniMap) + player.prepos[2].x / miniMap), int((1020 - fieldRadius / miniMap) + player.prepos[2].y / miniMap), int((50 + fieldRadius / miniMap) + player.prepos[0].x / miniMap), int((1020 - fieldRadius / miniMap) + player.prepos[0].y / miniMap), 0x00ff00ff);

			//ミニマップ範囲
			if (player.aimTimer > 0)Novice::DrawTriangle(int((50 + fieldRadius / miniMap) + player.prepos[0].x / miniMap), int(int(1020 - fieldRadius / miniMap) + player.prepos[0].y / miniMap), int((50 + fieldRadius / miniMap) + player.prepos[1].x / miniMap), int(int(1020 - fieldRadius / miniMap) + player.prepos[1].y / miniMap), int((50 + fieldRadius / miniMap) + player.prepos[2].x / miniMap), int(int(1020 - fieldRadius / miniMap) + player.prepos[2].y / miniMap), 0x55cc5577, kFillModeSolid);
			//ミニマッププレイヤー
			Novice::DrawEllipse(int((50 + fieldRadius / miniMap) + player.pos.x / miniMap), int(int(1020 - fieldRadius / miniMap) + player.pos.y / miniMap), int(player.radius.x * miniMapPlayerSize / miniMap), int(player.radius.y * miniMapPlayerSize / miniMap), 0, WHITE, kFillModeSolid);
			if (player.triangulAttack)Novice::DrawEllipse(int((50 + fieldRadius / miniMap) + player.pos.x / miniMap), int(int(1020 - fieldRadius / miniMap) + player.pos.y / miniMap), int(player.radius.x * miniMapPlayerSize / miniMap), int(player.radius.y * miniMapPlayerSize / miniMap), 0, 0x00ff00ff, kFillModeSolid);
			if (player.dashAttack)Novice::DrawEllipse(int((50 + fieldRadius / miniMap) + player.pos.x / miniMap), int(int(1020 - fieldRadius / miniMap) + player.pos.y / miniMap), int(player.radius.x * miniMapPlayerSize / miniMap), int(player.radius.y * miniMapPlayerSize / miniMap), 0, 0x00ffffff, kFillModeSolid);

			//攻撃有効時間を表示
			/*if (player.triangulAttack)
			{
				Novice::DrawBox(1820, 0, 100, 100, 0, RED, kFillModeSolid);
			}
			if (player.dashAttack)
			{
				Novice::DrawBox(1720, 0, 100, 100, 0, BLUE, kFillModeSolid);
			}*/

			/*if (!isEnemyDead)
			{
				Functions.DrawQuadPlus(
					static_cast<int>(enemyPos.x + scroll.x), static_cast<int>(enemyPos.y + scroll.y),
					30, 30,
					1.0f, 1.0f,
					0.0f,
					0, 0, 1, 1,
					kWhiteTexture,
					0xffff00ff,
					"center"
				);
			}*/


			switch (game)
			{
			case kTypeGameTitle:
				//Novice::DrawBox(1800, 700, 100, 100, 0, WHITE, kFillModeSolid);




				for (int i = 0; i < 3; i++)
				{

					if (enemy1.childIsAlive[i])
					{
						if (!enemy1.isDed[i])Functions.DrawQuadPlus(int(enemy1.relativePos[i].x / screenSize + scroll.x), int(enemy1.relativePos[i].y / screenSize + scroll.y), int(enemy1.radius * 2 / screenSize), int(enemy1.radius * 2 / screenSize), 1, 1, float(float(gameTimer * 2 % 360) / 180 * 3.1415f), 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[0], 0xffffffff, "center");
						if (enemy1.isDed[i] && enemy1.dedTimer[i] > 0)
						{
							Functions.DrawQuadPlus(int(enemy1.relativePos[i].x / screenSize + scroll.x), int(enemy1.relativePos[i].y / screenSize + scroll.y), int((enemy1.radius * 2 - (kDedTimer - enemy1.dedTimer[i]) * 3) / screenSize), int((enemy1.radius * 2 - (kDedTimer - enemy1.dedTimer[i]) * 3) / screenSize), 1, 1, float(float(gameTimer * 1 % 360) / 180 * 3.1415f), 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[0], BLUE, "center");
							Functions.DrawQuadPlus(int(enemy1.relativePos[i].x / screenSize + scroll.x), int(enemy1.relativePos[i].y / screenSize + scroll.y), int((256) / screenSize), int((256) / screenSize), 1, 1, float(float(gameTimer * 5 % 360) / 180 * 3.1415f), 256 * ((kDedTimer - enemy1.dedTimer[i]) / 3), 0, 256, 256, enemyDeadTexture, WHITE, "center");


						}
					}
				}


				//ミニマップ三角形

				for (int i = 0; i < 3; i++)
				{
					if (enemy1.childIsAlive[i])
					{
						if (!enemy1.isDed[i])Functions.DrawQuadPlus(int((50 + fieldRadius / miniMap) + enemy1.relativePos[i].x / miniMap), int((1020 - fieldRadius / miniMap) + enemy1.relativePos[i].y / miniMap), int(enemy1.radius * miniMapPlayerSize / miniMap), int(enemy1.radius * miniMapPlayerSize / miniMap), 1, 1, 0, 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[0], 0xffffffff, "center");
						if (enemy1.isDed[i] && enemy1.dedTimer[i] > 0)Functions.DrawQuadPlus(int((50 + fieldRadius / miniMap) + enemy1.relativePos[i].x / miniMap), int((1020 - fieldRadius / miniMap) + enemy1.relativePos[i].y / miniMap), int(enemy1.radius * miniMapPlayerSize / miniMap), int(enemy1.radius * miniMapPlayerSize / miniMap), 1, 1, 0, 0, 0, enemyTextureSize, enemyTextureSize, enemyTexture[0], BLUE, "center");
					}
				}
				if (titleCount == 0)
				{
					//Novice::ScreenPrintf(700, 540, "Walk");
					Functions.DrawQuadPlus(1500, 500, 192, 192, 1.2f * informationEase, 1.2f * informationEase, 0, 0, 0, 192, 192, informationTexture[0], WHITE, "center");
					informationEase = easeOutElastic(informationEaseT / 100);
					if (informationEaseT < 100)informationEaseT += 2;

					Functions.DrawQuadPlus(1500, 670, 690, 32, 1.0f * infoFontEase, 1.0f * infoFontEase, 0, 0, 0, 690, 32, infoFont[0], WHITE, "center");

					infoFontEase = easeOutElastic(infoFontEaseT / 100);
					if (infoFontEaseT < 100)infoFontEaseT += 2;

				}
				else if (titleCount == 1)
				{
					//Novice::ScreenPrintf(700, 540, "Dash");
					Functions.DrawQuadPlus(1500, 500, 192, 192, 1.2f * informationEase, 1.2f * informationEase, 0, 0, 0, 192, 192, informationTexture[1], WHITE, "center");
					informationEase = easeOutElastic(informationEaseT / 100);
					if (informationEaseT < 100)informationEaseT += 2;
					Functions.DrawQuadPlus(1500, 670, 690, 32, 1.0f * infoFontEase, 1.0f * infoFontEase, 0, 0, 0, 690, 32, infoFont[1], WHITE, "center");

					infoFontEase = easeOutElastic(infoFontEaseT / 100);
					if (infoFontEaseT < 100)infoFontEaseT += 2;

				}
				else if (titleCount == 2)
				{
					//Novice::ScreenPrintf(700, 540, "Triangle");
					Functions.DrawQuadPlus(1500, 500, 192, 192, 1.2f * informationEase, 1.2f * informationEase, 0, 0, 0, 192, 192, informationTexture[2], WHITE, "center");
					informationEase = easeOutElastic(informationEaseT / 100);
					if (informationEaseT < 100)informationEaseT += 2;
					Functions.DrawQuadPlus(1500, 670, 690, 32, 1.0f * infoFontEase, 1.0f * infoFontEase, 0, 0, 0, 690, 32, infoFont[2], WHITE, "center");
					Functions.DrawQuadPlus(1500, 700, 304, 32, 1.0f * infoFontEase, 1.0f * infoFontEase, 0, 0, 0, 304, 32, infoFont[4], WHITE, "center");

					infoFontEase = easeOutElastic(infoFontEaseT / 100);
					if (infoFontEaseT < 100)infoFontEaseT += 2;

				}
				else if (titleCount == 3)
				{
					//Novice::ScreenPrintf(700, 540, "kill to gamestart");
					Functions.DrawQuadPlus(1500, 670, 690, 32, 1.5f * infoFontEase, 1.5f * infoFontEase, 0, 0, 0, 690, 32, infoFont[3], WHITE, "center");

					if (!startNewGame)
					{
						infoFontEase = easeOutElastic(infoFontEaseT / 100);
						if (infoFontEaseT < 100)infoFontEaseT += 2;
					}
					else
					{
						infoFontEase = easeInQuint(infoFontEaseT / 100);
						if (infoFontEaseT > 0)infoFontEaseT -= 2;


					}


				}

				//	if (gameTimer < 5700 && gameTimer != 0)scoreObj.Draw((6060 - gameTimer) / 60, 0, 0, 0.3f, 0.3f, 0, ScoreObject::DPATTERN_ONLY_DIGIT);
				//	if (gameTimer == 0)scoreObj.Draw((6000 - gameTimer) / 60, 0, 0, 0.3f, 0.3f, 0, ScoreObject::DPATTERN_ONLY_DIGIT);

				Functions.DrawQuadPlus(int(gamestartPos + gamestartPos2 - 1232), 540, 1232, 128, 1, 1, 0, 0, 0, 1232, 128, gamestartTexture, WHITE, "center");
				break;

			case kTypeGameGame:

				scoreEaseT += scoreEaseChange * 10;
				if (scoreEaseT >= 100)scoreEaseChange *= -1;
				if (scoreEaseT < 0)scoreEaseT = 0;
				scorePos = easeOutCirc(scoreEaseT / 100) * 20;

				//Novice::DrawBox(1800, 700, 100, 100, 0, RED, kFillModeSolid);
				Functions.DrawQuadPlus(int(gamestartPos + gamestartPos2 - 1232), 540, 1232, 128, 1, 1, 0, 0, 0, 1232, 128, gamestartTexture, WHITE, "center");
				//if (quickTimer >= 150)Novice::ScreenPrintf(900, 600, "EXCELLENT");

				//タイマー
				if (gameTimer < 5700 && gameTimer != 0)scoreObj.Draw((6060 - gameTimer) / 60, 50, 50, 0.6f, 0.6f, 0, ScoreObject::DPATTERN_ONLY_DIGIT);
				if (gameTimer == 0)scoreObj.Draw((6000 - gameTimer) / 60, 50, 50 + -200 + int(UIPosy), 0.6f, 0.6f, 0, ScoreObject::DPATTERN_ONLY_DIGIT);
				//スコア
				scoreObj.Draw(score, 1550, 50 - 200 + int(UIPosy - UIBackPos - scorePos), 0.5f, 0.5f, 0, ScoreObject::DPATTERN_FILLED_BY_ZERO);
				Functions.DrawQuadPlus(1250, 45 - 200 + int(UIPosy - UIBackPos), 532, 118, 0.5f, 0.5f, 0, 0, 0, 528, 118, scoreTexture, WHITE, "leftTop");
				Functions.DrawQuadPlus(985, 1160 - int(UIPosy), 288, 48, 1.0f, 1.0f, 0, 0, 0, 288, 48, speedUpTexture, WHITE, "leftTop");

				break;

			case kTypeGameResult:
				if (yesnoEaseT < 100)yesnoEaseT += 10;
				yesnoEaseSize = easeOutCubic(yesnoEaseT / 100) * 0.2f;
				continueEaseT += continueEaseChange * 2;
				if (continueEaseT > 100 || continueEaseT < 0)continueEaseChange *= -1;
				continueEasePos = easeInExpo(continueEaseT / 100) * 50;
				for (int i = 0; i < 2; i++)
				{
					if (resultBackPos[i] >= 1080)resultBackPos[i] = -1080;
					resultBackPos[i]++;
				}
				if (blackEaseT <= 50)
				{

					for (int i = 0; i < 4; i++)
					{
						resultEasePos[i] = easeOutQuart(resultEaseT[i] / 100) * 1920;
					}

					if (resultEaseT[0] > 40 && resultEaseT[1] < 100)resultEaseT[1]++;
					if (resultEaseT[1] > 70 && resultEaseT[2] < 100)resultEaseT[2]++;
					if (resultEaseT[2] > 30 && resultEaseT[3] < 100)resultEaseT[3]++;



					if (resultEaseT[0] < 100)resultEaseT[0]++;


				}
				dashCount = int(dash.count);
				trieCount = int(triangle.count);
				//タイマー
				if (gameTimer < 5700 && gameTimer != 0)scoreObj.Draw((6060 - gameTimer) / 60, 50, 50, 0.6f, 0.6f, 0, ScoreObject::DPATTERN_ONLY_DIGIT);
				if (gameTimer == 0)scoreObj.Draw((6000 - gameTimer) / 60, 50, 50 + -200 + int(UIPosy), 0.6f, 0.6f, 0, ScoreObject::DPATTERN_ONLY_DIGIT);
				scoreObj.Draw(score, 1550, 50 - 200 + int(UIPosy), 0.5f, 0.5f, 0, ScoreObject::DPATTERN_FILLED_BY_ZERO);
				if (!startNewGame)
				{
					//Novice::DrawBox(0, 0, 1920, 1080, 0, 0x444444ff, kFillModeSolid);
					for (int i = 0; i < 2; i++)
					{
						Functions.DrawQuadPlus(0, resultBackPos[i], 1920, 1080, 1, 1, 0, 0, 0, 1920, 1080, resultBackTexture, 0x999999ff, "leftTop");

					}
					//スコア
					scoreObj.Draw(score, 1050 + int(1920 - resultEasePos[0]), 230, 0.7f, 0.7f, 0, ScoreObject::DPATTERN_FILLED_BY_ZERO);
					scoreObj.Draw(int(count), 1050 + int(1920 - resultEasePos[1]), 380, 0.7f, 0.7f, 0, ScoreObject::DPATTERN_ONLY_DIGIT);



					Functions.DrawQuadPlus(420 + int(1920 - resultEasePos[0]), 230, 532, 118, 1, 1, 0, 0, 0, 528, 118, scoreTexture, WHITE, "leftTop");
					Functions.DrawQuadPlus(420 + int(1920 - resultEasePos[1]), 380, 532, 118, 1, 1, 0, 0, 0, 528, 118, killTexture, WHITE, "leftTop");
					Functions.DrawQuadPlus(960 + int(1920 - resultEasePos[2]), 650 + int(continueEasePos), 1056, 128, 1, 1, 0, 0, 0, 1056, 128, continueTexture, WHITE, "center");

					if (choice == 0)
					{
						Functions.DrawQuadPlus(960 - 200 + int(1920 - resultEasePos[3]), 880, 320, 96, 1 + yesnoEaseSize, 1 + yesnoEaseSize, 0, 0, 0, 320, 96, yesTexture, 0xff6666ff, "center");
						Functions.DrawQuadPlus(960 + 200 + int(1920 - resultEasePos[3]), 880, 320, 96, 1, 1, 0, 0, 0, 320, 96, noTexture, WHITE, "center");

					}
					else if (choice == 1)
					{
						Functions.DrawQuadPlus(960 + 200 + int(1920 - resultEasePos[3]), 880, 320, 96, 1 + yesnoEaseSize, 1 + yesnoEaseSize, 0, 0, 0, 320, 96, noTexture, 0x6666ffff, "center");
						Functions.DrawQuadPlus(960 - 200 + int(1920 - resultEasePos[3]), 880, 320, 96, 1, 1, 0, 0, 0, 320, 96, yesTexture, WHITE, "center");

					}




				}
				//Novice::DrawBox(1800, 700, 100, 100, 0, BLUE, kFillModeSolid);
			//	Novice::ScreenPrintf(500, 500, "-H- Title -J- NewGame");

				Functions.DrawQuadPlus(int(gamestartPos + gamestartPos2 - 1232), 540, 1232, 128, 1, 1, 0, 0, 0, 1232, 128, gamestartTexture, WHITE, "center");



				break;
			}



			Functions.DrawQuadPlus(960, 540, 800, 800, bigNumSize, bigNumSize, 0, 0, 0, 800, 800, bigNumTexture[bigNumCount], bigNumColor[bigNumCount], "center");

			Functions.DrawQuadPlus(960, 540, 1920, 300, 1, -(finishEase - finishEase2), 0, 0, 0, 1, 1, kWhiteTexture, 0x00000066, "center");
			Functions.DrawQuadPlus(960, 540, 1248, 288, (finishSize - finishSize2), (finishSize - finishSize2), 0, 0, 0, 1248, 288, finishTexture, 0xff0000ff, "center");


			/// UIなど (一番手前になるように)
			// スコア (大きさ調整は [### ScoreClass] で検索)



				//暗転用
			Novice::DrawBox(0, 0, 1920, 1080, 0, blackColor + blackColor2, kFillModeSolid);


			//コントローラーの振動

			if (player.velocityRatio >= 0.8f)
			{
				vibeVolume = 7000;
				if (ennergy.fever)vibeVolume = 15000;
			}

			if (bigNumEaseT > 0 && bigNumEaseT < 100.0f)
			{


				vibeVolume = WORD(4000 * (bigNumCount + 1));



			}

			XInputSetState(0, &vibration);

			vibration.wLeftMotorSpeed = vibeVolume;
			vibration.wRightMotorSpeed = vibeVolume;


			///                                                            ///
			/// --------------------↑描画処理ここまで-------------------- ///
			///                                                            ///


			/// ---------------------------------------------------------- ///
			/// --------------------		音		  -------------------- ///
			/// ---------------------------------------------------------- ///

			// 10 , 11, 12 は enemy[0]でやる

#pragma region EnemySounds

			if (gameTimer < 6000)
			{
				if (enemy1.easeT == 2)
				{
					//	Original::SoundPlay("enemySpawn");
					SpownAudioFlag = true;
				}
				if (enemy2.easeT == 2)
				{
					SpownAudioFlag = true;
					//Original::SoundPlay("enemySpawn");
				}
				if (enemy5.easeT == 2)
				{
					SpownAudioFlag = true;
					//Original::SoundPlay("enemySpawn");
				}
				if (enemy6.easeT == 2)
				{
					SpownAudioFlag = true;
					//Original::SoundPlay("enemySpawn");
				}
				if (enemy7.easeT == 2)
				{
					SpownAudioFlag = true;
					//Original::SoundPlay("enemySpawn");
				}
				if (enemy8.easeT == 2)
				{
					SpownAudioFlag = true;
					//Original::SoundPlay("enemySpawn");
				}
				if (enemy9.easeT == 2)
				{
					SpownAudioFlag = true;
					//Original::SoundPlay("enemySpawn");
				}



			}
			for (int i = 0; i < sizeof(enemy1.dedTimer) / sizeof(int); i++)
			{

				if (enemy1.dedTimer[i] == kDedTimer - 1)
				{
					//Original::SoundPlay("enemyDead");
					deadAudioFlag = true;
				}
			}
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 16; j++)
				{
					if (enemy2.dedTimer[i][j] == kDedTimer - 1)
					{
						//Original::SoundPlay("enemyDead");
						deadAudioFlag = true;
					}
				}

			}
			for (int i = 0; i < sizeof(enemy5.dedTimer) / sizeof(int); i++)
			{
				if (enemy5.dedTimer[i] == kDedTimer - 1)
				{
					//Original::SoundPlay("enemyDead");
					deadAudioFlag = true;
				}
			}
			for (int i = 0; i < sizeof(enemy6.dedTimer) / sizeof(int); i++)
			{
				if (enemy6.dedTimer[i] == kDedTimer - 1)
				{
					//Original::SoundPlay("enemyDead");
					deadAudioFlag = true;
				}
			}
			for (int i = 0; i < sizeof(enemy7.dedTimer) / sizeof(int); i++)
			{
				if (enemy7.dedTimer[i / 4][i % 4] == kDedTimer - 1)
				{
					//Original::SoundPlay("enemyDead");
					deadAudioFlag = true;
				}
			}
			for (int i = 0; i < sizeof(enemy8.dedTimer) / sizeof(int); i++)
			{
				if (enemy8.dedTimer[i / 12][i % 12] == kDedTimer - 1)
				{
					//Original::SoundPlay("enemyDead");

					deadAudioFlag = true;
				}
			}
			for (int i = 0; i < sizeof(enemy9.dedTimer) / sizeof(int); i++)
			{
				if (enemy9.dedTimer[i / 8][i % 8] == kDedTimer - 1)
				{
					//Original::SoundPlay("enemyDead");
					deadAudioFlag = true;
				}
			}
			for (int i = 0; i < sizeof(enemy10) / sizeof(testEnemy3); i++)
			{
				if (enemy10[i].easeT == 2 && gameTimer < 6000)
				{
					SpownAudioFlag = true;
					//Original::SoundPlay("enemySpawn");
				}
				for (int j = 0; j < sizeof(enemy10[i].dedTimer) / sizeof(int); j++)
				{
					if (enemy10[i].dedTimer[j] == kDedTimer - 1)
					{
						deadAudioFlag = true;
						//Original::SoundPlay("enemyDead");
					}
				}

			}
			for (int i = 0; i < sizeof(enemy11) / sizeof(testEnemy1); i++)
			{
				if (enemy11[i].easeT == 2 && gameTimer < 6000)
				{
					SpownAudioFlag = true;
					//Original::SoundPlay("enemySpawn");
				}
				for (int j = 0; j < sizeof(enemy11[i].dedTimer) / sizeof(int); j++)
				{
					if (enemy11[i].dedTimer[j] == kDedTimer - 1)
					{
						deadAudioFlag = true;
						//Original::SoundPlay("enemyDead");
					}
				}

			}
			for (int i = 0; i < sizeof(enemy12) / sizeof(testEnemy3); i++)
			{
				if (enemy12[i].easeT == 2 && gameTimer < 6000)
				{
					SpownAudioFlag = true;
					//Original::SoundPlay("enemySpawn");
				}
				for (int j = 0; j < sizeof(enemy12[i].dedTimer) / sizeof(int); j++)
				{
					if (enemy12[i].dedTimer[j] == kDedTimer - 1)
					{
						deadAudioFlag = true;
						//Original::SoundPlay("enemyDead");
					}
				}

			}
			for (int i = 0; i < sizeof(enemy13.dedTimer) / sizeof(unsigned int); i++)
			{
				if (enemy13.easeT[30] == 2 && gameTimer < 6000)
				{
					SpownAudioFlag = true;
					//Original::SoundPlay("enemySpawn");
				}
				if (enemy13.dedTimer[i] == kDedTimer - 1)
				{
					//Original::SoundPlay("enemyDead");
					deadAudioFlag = true;
				}
			}


#pragma endregion

			//敵死亡
			if (deadAudioFlag)
			{
				Novice::PlayAudio(audioHandleDead, 0, 0.2f);
				deadAudioFlag = false;
			}
			//敵スポーン
			if (SpownAudioFlag)
			{
				/*if (enemy13.easeT[0] > 0 && enemy13.easeT[59] < 100) {
					Novice::PlayAudio(audioHandleSpown, 0, 0.05f);
				}
				else
				{

				}*/
				Novice::PlayAudio(audioHandleSpown, 0, 0.2f);
				SpownAudioFlag = false;

			}
			//ダッシュアタック
			if (dashAudioFlag)
			{
				Novice::PlayAudio(audioHandleDash, 0, 0.15f);
				dashAudioFlag = false;
			}
			//三角形の点設置
			if (pointsetAudioFlag)
			{
				Novice::PlayAudio(audioHandlePointSet, 0, 0.2f);
				pointsetAudioFlag = false;
			}
			//三角形攻撃時
			if (TriangleAudioFlag)
			{
				Novice::PlayAudio(audioHandleTriangle, 0, 0.25f);
				TriangleAudioFlag = false;

			}
			//エクセレント出た時
			if (successAudioFlag)
			{
				Novice::PlayAudio(audioHandlesuccess, 0, 0.5f);
				successAudioFlag = false;

			}
			//チュートリアルのイラストを表示する時
			if (infoFontEaseT == 2 && !startNewGame)
			{
				Novice::PlayAudio(audioHandlTaskClear, 0, 0.5f);
			}
			//ゲームスタート時
			if (gamestartEaseT == 2)
			{
				Novice::PlayAudio(audioHandlGameStart, 0, 0.5f);

			}
			//スコア(ゲージ)が加算されるとき
			if (preScore != score)
			{
				scoreEaseChange = 1;
				Novice::PlayAudio(audioHandlScoreUP, 0, 0.5f);

			}
			//カウントダウン
			if (bigNumEaseT == 2)
			{
				Novice::PlayAudio(audioHandlCountDown, 0, 0.5f);

			}
			//フィニッシュの文字
			if (finishEaseT == 2)
			{
				Novice::PlayAudio(audioHandlFinish, 0, 0.5f);

			}
			//リザルトで文字が表示される時
			if (resultEaseT[0] == 1 || resultEaseT[1] == 1 || resultEaseT[2] == 1 || resultEaseT[3] == 1)
			{
				Novice::PlayAudio(audioHandlScoreDisplay, 0, 0.5f);

			}
			//リザルトでYESNO選択する時
			if (prechoice != choice)
			{
				Novice::PlayAudio(audioHandlYESNO, 0, 0.5f);

			}
			//リザルトでYESNO確定したとき
			if (SelectAudioFlag)
			{
				Novice::PlayAudio(audioHandlSelect, 0, 0.5f);
				SelectAudioFlag = false;
			}
			//フィーバー始まった時
			if (ennergy.count == ennergy.max)
			{
				Novice::PlayAudio(audioHandlFeverStart, 0, 0.5f);
			}
			//フィーバー終わった時
			if (FeverEndAudioFlag)
			{
				Novice::PlayAudio(audioHandlFeverEnd, 0, 0.5f);
				FeverEndAudioFlag = false;
			}



			//BGM

			//タイトル
			if (game == 0)
			{
				if (soundHandle == -1)
				{
					soundHandle = Novice::PlayAudio(audioHandletitleBGM, 1, titleBGMValume);

				}
				if (titleBGMValume < setTitleBGMValume)
				{
					titleBGMValume += setTitleBGMValume / 60;
				}
			}

			//ゲームスタートしたら
			if (startNewGame)
			{
				titleBGMValume -= setTitleBGMValume / 40;
				if (titleBGMValume <= 0)
				{
					Novice::StopAudio(soundHandle);
					soundHandle = -1;
					titleBGMValume = 0;
				}
			}
			Novice::SetAudioVolume(soundHandle, titleBGMValume);


			//ゲーム中
			if (game == 1 && gameTimer > 0 && gameTimer < 2)
			{
				Novice::PlayAudio(gameBGM, 0, 0.05f);

			}

			//リザルト
			if (game == 2)
			{
				if (ResultsoundHandle == -1)
				{
					ResultsoundHandle = Novice::PlayAudio(audioHandleResultBGM, 1, ResultBGMValume);

				}
				if (ResultBGMValume < setResultBGMValume)
				{
					ResultBGMValume += setResultBGMValume / 60;
				}
			}
			if (startNewGame || titleBackFlag)
			{
				ResultBGMValume -= setResultBGMValume / 40;
				if (ResultBGMValume <= 0)
				{
					Novice::StopAudio(ResultsoundHandle);
					ResultsoundHandle = -1;
					ResultBGMValume = 0;
				}
			}
			Novice::SetAudioVolume(ResultsoundHandle, ResultBGMValume);

		}

		//Novice::ScreenPrintf(0, 0, "ResultsoundHandle=%d ResultBGMValume=%0.3f ", ResultsoundHandle, ResultBGMValume);
		//Novice::ScreenPrintf(0, 20, "startNewGame=%d titleBackFlag=%d ",startNewGame, titleBackFlag);

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

