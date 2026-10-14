#include "Play.h"

//   WINDOW_W 1280	// 画面サイズ　横
//   WINDOW_H 720	// 画面サイズ　縦

// 初期化処理
void Play::Init()
{
	// 開始画面
	game_mode = TITLE;
}

// 入力処理
void Play::Input()
{
}

// 更新処理
void Play::Update()
{
	GetHitKeyStateAll(Key);
	const float speed = 5.0f;
	switch (game_mode) {
	case TITLE:  // タイトル画面の時の処理
		if (Key[KEY_INPUT_SPACE] == 1 && OldKey[KEY_INPUT_SPACE] == 0) {
			game_mode = PLAY;
		}
		break;

	case PLAY:
		break;

	case RESULT:

		break;
	}
	memcpy(OldKey, Key, 256);
}

// 画面描画処理
void Play::Render()
{
	switch (game_mode) {
	case TITLE:

		break;

	case PLAY:

	break;

	case RESULT:

		break;
	}
}

// 音声再生処理
void Play::Sound_play()
{
}


bool Play::KeyPush(int key)
{
	return Key[key] && !OldKey[key];
}

//void Play::SetEnemy(Enemy* arg_cloud)
//{
//	this->enemy = arg_cloud;
//}
//void Play::SetPlayer(Player* arg_player)
//{
//	this->player = arg_player;
//}
//void Play::SetTitle(Title* arg_title)
//{
//	this->title = arg_title;
//}
//void Play::SetResult(Result* arg_title)
//{
//	this->result = arg_title;
//}
//
//void Play::SetStage(Stage* arg_stage)
//{
//	this->stage = arg_stage;
//}