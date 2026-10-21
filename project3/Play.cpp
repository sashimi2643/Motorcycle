#include "Play.h"

//   WINDOW_W 1280	// 画面サイズ　横
//   WINDOW_H 720	// 画面サイズ　縦

// 初期化処理
void Play::Init()
{
	// 開始画面
	game_mode = TITLE;

	road.Init();

	playerX = 0.0f;
	elapsedTime = 0.0f;
	spawnTimer = 0.0f;
	speed = 0.28f;
	score = 0;


	lastUpdateTime = GetNowCount();
}

// 入力処理
void Play::Input()
{
	GetHitKeyStateAll(Key);
}

// 更新処理
void Play::Update()
{
	Input();

	switch (game_mode) {
	case TITLE:  // タイトル画面の時の処理
		if (KeyPush(KEY_INPUT_SPACE)) {
			ResetGame();
			game_mode = PLAY;
		}
		break;

	case PLAY:
	{
		int now = GetNowCount();

		deltaTime = (now - lastUpdateTime) / 1000.0f;
		lastUpdateTime = now;

		// 処理落ち時の急激な移動を防止
		if (deltaTime < 0.0f)
		{
			deltaTime = 0.0f;
		}

		if (deltaTime > 0.05f)
		{
			deltaTime = 0.05f;
		}

		elapsedTime += deltaTime;

		// 徐々に速度を上げる
		speed = 0.28f + elapsedTime * 0.008f;

		if (speed > 0.65f)
		{
			speed = 0.65f;
		}

		UpdatePlayer();

		road.Update(deltaTime, speed);

		UpdateObject();

		CheckCollision();

		if (game_mode == PLAY)
		{
			score = static_cast<int>(elapsedTime * 10.0f);
		}
		break;
	}

	case RESULT:
		if (KeyPush(KEY_INPUT_SPACE))
		{
			ResetGame();
			game_mode = PLAY;
		}
		break;
	}

	// 今回のキー状態を次回の比較用に保存
	memcpy(OldKey, Key, 256);
}

// 画面描画処理
void Play::Render()
{
	switch (game_mode) {
	case TITLE:
		DrawBox(0, 0, 1280, 720, 0x202830, TRUE);

		DrawString(
			440, 220,
			_T("MOTORCYCLE RUN"),
			0xFFFFFF);

		DrawString(
			390, 330,
			_T("PRESS SPACE TO START"),
			0xFFFFFF);
		break;

	case PLAY:
		road.Draw(playerX);

		for (const Object& object : objects)
		{
			object.Draw(playerX);
		}

		DrawPlayer();

		DrawFormatString(
			20, 20, 0xFFFFFF,
			_T("SCORE: %d"), score);

		DrawFormatString(
			20, 50, 0xFFFFFF,
			_T("SPEED: %.2f"), speed);
	break;

	case RESULT:
		DrawBox(0, 0, 1280, 720, 0x101010, TRUE);

		DrawString(
			510, 220,
			_T("GAME OVER"),
			0xFF3333);

		DrawFormatString(
			550, 300, 0xFFFFFF,
			_T("SCORE: %d"), score);

		DrawString(
			430, 380,
			_T("PRESS SPACE TO RETRY"),
			0xFFFFFF);
		break;
	}
}

// 音声再生処理
void Play::Sound_play()
{
}

// ゲーム初期化
void Play::ResetGame()
{
	playerX = 0.0f;
	elapsedTime = 0.0f;
	spawnTimer = 0.0f;
	speed = 0.28f;
	score = 0;

	objects.clear();

	road.Init();

	lastUpdateTime = GetNowCount();
}

// プレイヤー移動
void Play::UpdatePlayer()
{
	float move = 0.0f;

	if (Key[KEY_INPUT_LEFT] ||
		Key[KEY_INPUT_A])
	{
		move -= 1.0f;
	}

	if (Key[KEY_INPUT_RIGHT] ||
		Key[KEY_INPUT_D])
	{
		move += 1.0f;
	}

	// 左右に滑らかに移動
	playerX += move * 1.15f * deltaTime;

	// 道路の範囲内に制限
	if (playerX < -0.90f)
	{
		playerX = -0.90f;
	}

	if (playerX > 0.90f)
	{
		playerX = 0.90f;
	}
}

// 障害物生成と移動
void Play::UpdateObject()
{
	spawnTimer += deltaTime;

	// 時間が経過するほど障害物の出現間隔を短くする
	float interval = 1.5f - elapsedTime * 0.003f;

	if (interval < 0.75f)
	{
		interval = 0.75f;
	}

	if (spawnTimer >= interval)
	{
		spawnTimer = 0.0f;
		SpawnObject();
	}

	for (Object& object : objects)
	{
		object.Update(deltaTime, speed);
	}

	objects.erase(
		std::remove_if(
			objects.begin(),
			objects.end(),
			[](const Object& object)
			{
				return object.IsOutOfScreen();
			}),
		objects.end());
}

// 障害物生成
void Play::SpawnObject()
{
	Object object;

	// -1、0、1 のいずれかの位置に出現
	int lane = std::rand() % 3 - 1;

	object.Init(lane);

	objects.push_back(object);
}

// 衝突判定
void Play::CheckCollision()
{
	for (const Object& object : objects)
	{
		// プレイヤー付近まで来ていなければ無視
		if (!object.IsNearPlayer())
		{
			continue;
		}

		// プレイヤーと障害物の横方向の距離
		float distance =
			std::fabs(playerX - object.GetLane());

		// 一定距離以内ならゲームオーバー
		if (distance < 0.27f)
		{
			game_mode = RESULT;
			return;
		}
	}
}

// ハンドル描画

void Play::DrawPlayer()
{
	// 画面サイズ
	const int SCREEN_W = 1280;
	const int SCREEN_H = 720;

	// プレイヤーの左右移動
	int centerX = SCREEN_W / 2
		+ static_cast<int>(playerX * 150.0f);

	// コックピットの配置範囲
	const int cockpitTop = 550;

	// バイク前方のカウル
	DrawTriangle(
		centerX - 170, SCREEN_H,
		centerX + 170, SCREEN_H,
		centerX, cockpitTop + 80,
		GetColor(25, 28, 35), TRUE
	);

	// メーター本体
	DrawBox(
		centerX - 80, cockpitTop + 35,
		centerX + 80, cockpitTop + 105,
		GetColor(15, 18, 22), TRUE
	);

	// メーター画面
	DrawBox(
		centerX - 60, cockpitTop + 45,
		centerX + 60, cockpitTop + 75,
		GetColor(35, 65, 65), TRUE
	);

	DrawFormatString(
		centerX - 45, cockpitTop + 51,
		GetColor(100, 255, 180),
		_T("SPD %02d"),
		static_cast<int>(speed * 100)
	);

	// ミラーの支柱
	DrawLine(
		centerX - 100, cockpitTop + 50,
		centerX - 145, cockpitTop + 5,
		GetColor(35, 35, 35), 6
	);

	DrawLine(
		centerX + 100, cockpitTop + 50,
		centerX + 145, cockpitTop + 5,
		GetColor(35, 35, 35), 6
	);

	// 左右のミラー
	DrawBox(
		centerX - 180, cockpitTop - 5,
		centerX - 120, cockpitTop + 25,
		GetColor(25, 30, 35), TRUE
	);

	DrawBox(
		centerX + 120, cockpitTop - 5,
		centerX + 180, cockpitTop + 25,
		GetColor(25, 30, 35), TRUE
	);

	// ミラーの反射面
	DrawBox(
		centerX - 173, cockpitTop,
		centerX - 127, cockpitTop + 20,
		GetColor(110, 145, 155), TRUE
	);

	DrawBox(
		centerX + 127, cockpitTop,
		centerX + 173, cockpitTop + 20,
		GetColor(110, 145, 155), TRUE
	);

	// ハンドル
	DrawLine(
		centerX - 230, cockpitTop + 100,
		centerX - 80, cockpitTop + 65,
		GetColor(25, 25, 25), 18
	);

	DrawLine(
		centerX + 230, cockpitTop + 100,
		centerX + 80, cockpitTop + 65,
		GetColor(25, 25, 25), 18
	);

	// グリップ
	DrawCircle(
		centerX - 230, cockpitTop + 100,
		13, GetColor(10, 10, 10), TRUE
	);

	DrawCircle(
		centerX + 230, cockpitTop + 100,
		13, GetColor(10, 10, 10), TRUE
	);
}


// キーが押された瞬間を判定
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