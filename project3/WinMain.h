#pragma once
#include <ctime>         // 標準C ライブラリ ヘッダー <time.h> をインクルードし、関連する名前を std 名前空間に追加します。
#include "DxLib.h"		 // DxLib
#include "Play.h"		 // Play.h
#include "Title.h"		 // Title.h
#include "Player.h"      // Player.h
#include "Result.h"      // Result.h
#include "ProcessBase.h" // ProcessBase.h

// ゲームクラス
class Game : public ProcessBase
{
	// プレイのインスタンス
	Play play;
	// タイトルのインスタンス
	Title title;
	// プレイヤーのインスタンス
	Player player;
	// リザルトのインスタンス
	Result result;

public:

	// 背景画面
	int bg_image;


	// ゲームループ
	void Game_Loop();  // 関数プロトタイプで宣言

	//	初期化処理
	void Init()override;

	//　入力処理
	void Input()override;

	//	更新処理
	void Update()override;

	//	描画処理
	void Render()override;

	//	音声再生処理
	void Sound_play()override;

};