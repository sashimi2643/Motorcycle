#pragma once
#include <string>
#include "DxLib.h"
//#include "Player.h"
//#include "Enemy.h"
#include "Title.h"
//#include "Result.h"
//#include "Stage.h"
#include "Base.h"
#include "Road.h"
#include "Object.h"
#include "Background.h"
#include "Tree.h"
#include "VisorRain.h"
#include "Danger.h"

#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <tchar.h>
#include <random>

// クラス
class Play : public ProcessBase
{
	// タイトル
	Title title;
	//// リザルトのインスタンス
	//Result* result = nullptr;
	
	// 道路・障害物・背景
	Road road;
	std::vector<Object> objects;
	Background background;
	Tree tree;
	VisorRain visorRain;

	std::mt19937 mt{ std::random_device{}() };

	// ゲームのモード
	enum {
		TITLE,
		FADE,
		PLAY,
		RESULT,
	};
	int game_mode;


	// プレイヤーの位置
	float playerX = 0.0f;

	// ゲーム進行
	float elapsedTime = 0.0f;
	float spawnTimer = 0.0f;
	float speed = 0.40f;
	float deltaTime = 0.0f;

	int score = 0;
	int lastUpdateTime = 0;

	// ハンドルに関する変数 -----------------
	// ハンドル画像
	int handle_image = -1;
	// ハンドルの回転角度（ラジアン）
	float handleAngle = 0.0f;
	// 左右移動の速度
	float playerMoveVelocity = 0.0f;
	// 左右移動の目標位置
	float targetPlayerX = 0.0f;
	// --------------------------------------

	// キー入力
	char Key[256]{};
	char OldKey[256]{};

	bool KeyPush(int key);

	// ゲーム開始前の「３・２・１」カウント
	int frame;
	int count;
	bool game;

	// 危険表示
	std::vector<Danger> dangers;

	// 警告後に出現する障害物のレーン
	std::vector<int> pendingLanes;

	// 警告時間
	float dangerTimer = 0.0f;

	// 警告中かどうか
	bool waitingForDanger = false;

	// 直前に障害物を配置したレーン
	int previousLane1 = 99;
	int previousLane2 = 99;

	// フェード演出の状態
	enum FADE_STATE
	{
		FLASH_IN,       // 白くなる
		FLASH_OUT,      // 白から戻る
		BLACK_OUT,      // 暗くなる
		BLACK_IN        // 明るくなる
	};
	FADE_STATE fadeState = FLASH_IN;
	// フェード用アルファ値
	float fadeAlpha = 0.0f;
	// フェード速度
	float fadeSpeed = 550.0f;
	// フィードからプレイへの遅延変数
	int fade_count = 0;
	// 処理が長いから関数でまとめる
	void UpdateFade();
	void RenderFade();

	// ゲームモード
	enum GAME_MODE
	{
		GAME_MODE_NORMAL,  // ゲームモード
		GAME_MODE_REAL     // リアルモード
	};

	GAME_MODE selectedMode = GAME_MODE_NORMAL;

	// 画面振動用
	float shakeTime = 0.0f;

	// 画面の揺れ幅
	float shakePower = 1.1f;

	// 画面振動用
	int gameScreen = -1;

	// 画面の移動量
	int shakeX = 0;
	int shakeY = 0;

	// SEの変数
	int se_title_engine;
	int se_play_engine;

	// 補助処理
	void ResetGame();
	void UpdatePlayer();
	void UpdateObject();
	void CheckCollision();
	void SpawnObject();
	void DrawPlayer();
	void DrawHandleImage();
	void UpdateShake();

public:

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
