#pragma once
#include <string>
#include "DxLib.h"
//#include "Player.h"
//#include "Enemy.h"
//#include "Title.h"
//#include "Result.h"
//#include "Stage.h"
#include "Base.h"

// クラス
class Play : public ProcessBase
{
	// プレイヤーのインスタンス
	//Player* player = nullptr;
	//// 雲のインスタンス
	//Enemy* enemy = nullptr;
	//// タイトルのインスタンス
	//Title* title = nullptr;
	//// リザルトのインスタンス
	//Result* result = nullptr;
	//// ステージのインスタンス
	//Stage* stage = nullptr;
public:

	// ゲームのモード
	enum {
		TITLE,
		PLAY,
		RESULT,
	};
	int game_mode;

	char Key[256];
	char OldKey[256];
	bool KeyPush(int key);

	// ゲーム開始前の「３・２・１」カウント
	int frame;
	int count;
	bool game;

	// SEの変数
	int se_mov_break;  // 破壊中の音
	int se_break;      // 破壊した音


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

	//void SetEnemy(Enemy* arg_cloud);
	//void SetPlayer(Player* arg_palyer);
	//void SetTitle(Title* arg_title);
	//void SetResult(Result* arg_result);
	//void SetStage(Stage* arg_stage);
};
