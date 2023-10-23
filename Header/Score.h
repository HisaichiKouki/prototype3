#pragma once
#include "Struct.h"
#include "Functions.h"

class ScoreObject
{
public:
	
	enum DrawPattern
	{
		DPATTERN_FILLED_BY_ZERO,
		DPATTERN_ONLY_DIGIT
	};

	/// <summary>
	/// インストラクタ
	/// </summary>
	ScoreObject();

	/// <summary>
	/// スコア用の数字画像のテクスチャハンドルを記憶します。テクスチャハンドルを配列に0～9の順で入れてください。
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void SetScoreTextureHandle(int _textureHandle[10]);


	void SetSize(int _width, int _height);

	/// <summary>
	/// ０埋め時の最大桁数を設定します。
	/// </summary>
	/// <param name="_maxDigit"></param>
	void SetMaxDigit(int _maxDigit);

	/// <summary>
	/// 画像の色を設定します。
	/// </summary>
	/// <param name="_color"></param>
	void SetColor(unsigned int _color);

	/// <summary>
	/// スコアを事前設定に基づいて描画します。
	/// </summary>
	/// <param name="_decimalNum">数字</param>
	/// <param name="_posX">x座標</param>
	/// <param name="_posY">y座標</param>
	/// <param name="_width">画像ソースの横幅</param>
	/// <param name="_height">画像ソースの高さ</param>
	/// <param name="_margin">文字と文字の間隔</param>
	void Draw(int _decimalNum, int _posX, int _posY, float _scaleX, float _scaleY, int _margin, DrawPattern _dPattern);

private:
	ScoreData score;
	static int PowI(int _x, int _y);
	int maxDigit;
	Func func;
	int textureHandles[10];
	int handleCnt;
	
};