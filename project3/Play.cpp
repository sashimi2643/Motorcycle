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
	speed = 0.40f;
	score = 0;


	lastUpdateTime = GetNowCount();

	background.Init();

	// 白色を透過色として画像を読み込む
	SetTransColor(255, 255, 255);
	handle_image = LoadGraph(_T("data/handle.png"));

	// 透過色の設定を元に戻す
	SetTransColor(0, 0, 0);
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
		speed = 0.40f + elapsedTime * 0.008f;

		// 最大スピードの設定
		if (speed > 0.77f)
		{
			speed = 0.77f;
		}

		UpdatePlayer();

		road.Update(deltaTime, speed);

		UpdateObject();

		CheckCollision();

		background.Update(deltaTime, speed);

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
		// 1. 背景を描画
		background.Draw(playerX);
		// 2. 道路を描画
		road.Draw(playerX);
		// 3. 障害物を描画
		for (const Object& object : objects)
		{
			object.Draw(playerX);
		}
		// 4. プレイヤーを描画
		DrawHandleImage();

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
	speed = 0.40f;
	score = 0;

	objects.clear();

	road.Init();

	lastUpdateTime = GetNowCount();

	handleAngle = 0.0f;
	playerMoveVelocity = 0.0f;
	targetPlayerX = 0.0f;
}

// プレイヤー更新処理

void Play::UpdatePlayer()
{
	// ------------------------------------
	// 1. 左右キーから入力方向を取得
	// ------------------------------------
	float input = 0.0f;

	if (Key[KEY_INPUT_LEFT] == 1 ||
		Key[KEY_INPUT_A] == 1)
	{
		input = -1.0f;
	}
	else if (Key[KEY_INPUT_RIGHT] == 1 ||
		Key[KEY_INPUT_D] == 1)
	{
		input = 1.0f;
	}

	// 左右の移動限界
	constexpr float MAX_PLAYER_X = 0.90f;

	// 目標位置の移動速度
	constexpr float TARGET_MOVE_SPEED = 0.75f;

	// ------------------------------------
	// 2. 目標位置を更新
	// ------------------------------------
	targetPlayerX += input * TARGET_MOVE_SPEED * deltaTime;

	// 道路の範囲内に制限
	if (targetPlayerX < -MAX_PLAYER_X)
		targetPlayerX = -MAX_PLAYER_X;

	if (targetPlayerX > MAX_PLAYER_X)
		targetPlayerX = MAX_PLAYER_X;

	// ------------------------------------
	// 3. 車体を目標位置へ滑らかに移動
	// ------------------------------------
	constexpr float MOVE_RESPONSE = 2.5f;

	float difference = targetPlayerX - playerX;

	playerMoveVelocity = difference * MOVE_RESPONSE;

	playerX += playerMoveVelocity * deltaTime;

	// 目標位置を通り越さないように補正
	if ((difference > 0.0f && playerX > targetPlayerX) ||
		(difference < 0.0f && playerX < targetPlayerX))
	{
		playerX = targetPlayerX;
		playerMoveVelocity = 0.0f;
	}

	// ------------------------------------
	// 4. ハンドル角度を更新
	// ------------------------------------

	// 通常は入力方向にハンドルを切る
	float targetHandleAngle = input * 0.25f;

	// 目標位置が左右の限界に達しているか判定
	const bool atLeftLimit =
		targetPlayerX <= -MAX_PLAYER_X && input < 0.0f;

	const bool atRightLimit =
		targetPlayerX >= MAX_PLAYER_X && input > 0.0f;

	// 限界方向に入力している場合は中央へ戻す
	if (atLeftLimit || atRightLimit)
	{
		targetHandleAngle = 0.0f;
	}

	// ハンドルを滑らかに回転させる
	constexpr float HANDLE_RESPONSE = 0.08f;

	handleAngle +=
		(targetHandleAngle - handleAngle) * HANDLE_RESPONSE;
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
void Play::DrawHandleImage()
{
	if (handle_image == -1)
	{
		return;
	}

	constexpr int SCREEN_W = 1280;
	constexpr int SCREEN_H = 720;

	// 画面下部の中央を基準にする
	int handleX = SCREEN_W / 2;
	int handleY = SCREEN_H - 125;

	// プレイヤーの左右位置に応じて少し横に移動
	handleX += static_cast<int>(playerX * 45.0f);

	// 左右の移動に合わせてハンドルを傾ける
	// playerX がプラスなら右、マイナスなら左に傾く
	//handleAngle = playerX * 0.25f;

	// 画像を縮小して回転描画
	DrawRotaGraph(
		handleX,
		handleY,
		0.55,           // 画像の拡大率
		handleAngle,    // 回転角度（ラジアン）
		handle_image,
		TRUE            // 透過描画
	);
}

// ハンドル描画(使ってない)
void Play::DrawPlayer()
{
	// 画面サイズ
	const int SCREEN_W = 1280;
	const int SCREEN_H = 720;

	// Playerは常に画面中央に固定
	int centerX = SCREEN_W / 2;

	const int cockpitTop = 550;

	// バイク本体
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

	// 速度表示
	DrawFormatString(
		centerX - 45, cockpitTop + 51,
		GetColor(100, 255, 180),
		_T("SPD %02d"),
		static_cast<int>(speed * 100)
	);

	// ハンドルの支柱
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

	// ミラー
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