#pragma once
#include "Struct.h"
#include "Functions.h"

class ScoreObject
{
public:
	
	ScoreObject();
	/// <summary>
	/// スコア用の数字画像のテクスチャハンドルを記憶します。テクスチャハンドルを配列に0～9の順で入れてください。
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void SetScoreTextureHandle(int _textureHandle[10]);

	/// <summary>
	/// スコア画像のサイズを設定します。
	/// </summary>
	/// <param name="width">画像の幅</param>
	/// <param name="height">画像の高さ</param>
	void SetSize(int _width, int _height);

	void SetColor(unsigned int _color);

	/// <summary>
	/// スコアを事前設定に基づいて描画します。
	/// </summary>
	/// <param name="_decimalNum">数字</param>
	/// <param name="_posX">x座標</param>
	/// <param name="_posY">y座標</param>
	/// <param name="_margin">文字と文字の間隔</param>
	void Draw(int _decimalNum, int _posX, int _posY, int _margin);

private:
	ScoreData score;
	static int PowI(int _x, int _y);
	Func func;
	int textureHandles[10];
	int handleCnt;
};