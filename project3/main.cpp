#include "Main.h"

// ゲームループ
void Game::Game_Loop()
{
	// =======================================
	//  初期化処理
	// =======================================
	this->Init();

	// ループ開始
	while (ProcessMessage() == 0)
	{
		// リフレッシュレートを設定するための処理
		clock_t check_fps = clock() + CLOCKS_PER_SEC / 60;

		// マウスカーソル表示設定
		SetMouseDispFlag(TRUE);

		// 画面上の描画を初期化（画面を消去）
		ClearDrawScreen();


		// =======================================
		//  入力処理
		// =======================================
		this->Input();


		// =======================================
		//  更新処理
		// =======================================
		this->Update();


		// =======================================
		//  画面描画処理
		// =======================================
		this->Render();


		// =======================================
		//  音声再生処理
		// =======================================
		this->Sound_play();


		// リフレッシュレートが一定になるまで待つ処理
		while (clock() < check_fps) {}

		// 裏画面の描画を表に反映
		ScreenFlip();


		// ESCキーでループから抜ける
		if (CheckHitKey(KEY_INPUT_ESCAPE)) {
			break;
		}
	}
}


// 初期化処理
void Game::Init()
{
	this->play.Init();
	/*this->title.Init();
	this->player.Init();
	this->result.Init();*/
}

// 入力処理
void Game::Input()
{
	this->play.Input();
	/*this->play.SetPlayer(&player);
	this->play.SetTitle(&title);
	this->play.SetResult(&result);
	this->title.Input();
	this->player.Input();
	this->result.Input();*/
}

// 更新処理
void Game::Update()
{
	this->play.Update();  // 全体の処理
	//if (this->play.game_mode == this->play.TITLE) {  // タイトル時の処理
	//	this->title.Update();
	//}
	//if (this->play.game_mode == this->play.PLAY) {  // プレイ時の処理
	//	this->player.Update();
	//}
	//if (this->play.game_mode == this->play.RESULT) {  // リザルト時の処理
	//	this->result.Update();
	//}
}

// 画面描画処理
void Game::Render()
{
	if (this->play.game_mode == this->play.TITLE) {  // タイトル時の処理
		//this->title.Render();
	}
	if (this->play.game_mode == this->play.PLAY) {  // プレイ時の処理
		//this->player.Render();
		this->play.Render();  // 全体の処理
	}
	if (this->play.game_mode == this->play.RESULT) {  // リザルト時の処理
		//this->result.Render();
	}
}

// 音声再生処理
void Game::Sound_play()
{
	this->play.Sound_play();  // 全体の処理
	//if (this->play.game_mode == this->play.TITLE) {  // タイトル時の処理
	//	this->title.Sound_play();
	//}
	//if (this->play.game_mode == this->play.PLAY) {  // プレイ時の処理
	//	this->player.Sound_play();
	//}
	//if (this->play.game_mode == this->play.RESULT) {  // リザルト時の処理
	//	this->result.Sound_play();
	//}
}