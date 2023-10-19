#pragma once
#include "Novice.h"

enum DebugMode {
	kStandardMode, // 通常モード
	kDebugMode     // デバッグモード
};

class Func {
public:

	/// @brief デバッグモードを設定する
	/// @param _mode デバッグモード
	void SetDebugMode(DebugMode _mode);
	/// @brief デバッグモードを切り替える
	void ToggleDebugMode();
	/// @brief 受け取ったx値とy値で回転させる
	/// @param _x x座標
	/// @param _y y座標
	/// @param _theta 角度
	void rotate(int& _x, int& _y, float _theta);

	/// @brief スプライトを描画する
	/// @param _x 画面上のx座標
	/// @param _y 画面上のy座標
	/// @param _horizontal 矩形の横幅
	/// @param _vertical 矩形の縦幅
	/// @param _scaleX 横幅の倍率
	/// @param _scaleY 高さの倍率
	/// @param _theta 回転角度
	/// @param _srcW 画像ファイルの横幅
	/// @param _srcH 画像ファイルの高さ
	/// @param _textureHandle 描画するテクスチャハンドル
	/// @param _hexColor 16進数でのカラーコード
	void DrawQuadPlus(int _x, int _y, int _horizontal, int _vertical, float _scaleX, float _scaleY, float _theta, int _srcX, int _srcY, int _srcW, int _srcH, int _textureHandle, unsigned int _hexColor);


private:
	/// @brief デバッグ状態かどうか
	bool isDebugMode = false;
	/// @brief ToggleDebugModeの呼び出し回数
	int callCnt = 0;
};
