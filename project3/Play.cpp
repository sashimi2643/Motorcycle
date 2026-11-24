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

	title.Init();
	lastUpdateTime = GetNowCount();
	background.Init();
	tree.Init();
	visorRain.Init();

	lastUpdateTime = GetNowCount();

	// 白色を透過色として画像を読み込む
	SetTransColor(255, 255, 255);
	handle_image = LoadGraph(_T("data/handle.png"));
	// 危険表示画像を読み込む
	Danger::LoadImage();

	// 画面全体を一時的に描画する画面を作成
	gameScreen = MakeScreen(1280, 720, FALSE);

	// 透過色の設定を元に戻す
	SetTransColor(0, 0, 0);

	se_title_engine = LoadSoundMem(_T("data/title_engine.mp3"));
	se_play_engine = LoadSoundMem(_T("data/play_engine.mp3"));
}

// 入力処理
void Play::Input()
{
	GetHitKeyStateAll(Key);
}

// 更新処理
void Play::Update()
{
	// 毎フレームの経過時間を計算
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

	Input();

	switch (game_mode) {
	case TITLE:  // タイトル画面の時の処理
		if (title.menu) {
			// １．ゲーム開始
			if (KeyPush(KEY_INPUT_1) && !title.operate && !title.setting) {
				// モードに応じて振動の強さを設定
				if (selectedMode == GAME_MODE_NORMAL){ shakePower = 0.0f; }else{ shakePower = 1.1f; }
				// フラッシュ演出の初期化
				fadeState = FLASH_IN;
				fadeAlpha = 0.0f;
				// エンジン音を再生
				PlaySoundMem(se_title_engine, DX_PLAYTYPE_BACK);
				// ゲームモードをFADEに変更
				game_mode = FADE;
			}
			// ２．操作方法
			if (!title.operate) {
				if (KeyPush(KEY_INPUT_2) && !title.setting) {
					title.operate = true;
				}
			}
			else {
				if (KeyPush(KEY_INPUT_2)) {
					title.operate = false;
				}
			}
			// ３．ゲームの設定
			if (!title.setting) {
				if (KeyPush(KEY_INPUT_3) && !title.operate) {
					title.setting = true;
				}
			}
			else {
				if (KeyPush(KEY_INPUT_1)) {	// 1キー：ゲームモード
					selectedMode = GAME_MODE_NORMAL;
					title.setting = false;
				}
				if (KeyPush(KEY_INPUT_2)) {	// 2キー：リアルモード
					selectedMode = GAME_MODE_REAL;
					title.setting = false;
				}
				if (KeyPush(KEY_INPUT_3)) {	// 3キー：タイトルに戻る
					title.setting = false;
				}
			}
			// ４．ゲーム終了
			if (KeyPush(KEY_INPUT_4) && !title.operate && !title.setting)
			{
				PostQuitMessage(0);
			}
		}
		else {
			// タイトルからメニューへ
			if (KeyPush(KEY_INPUT_SPACE)) {
				title.menu = true;
			}
		}
		break;

	case FADE:
		UpdateFade();
		break;

	case PLAY:
	{
		elapsedTime += deltaTime;

		// 徐々に速度を上げる
		speed = 0.50f + elapsedTime * 0.005f;

		// 最大スピードの設定
		if (speed > 0.87f)
		{
			speed = 0.87f;
		}

		UpdatePlayer();

		UpdateShake();

		road.Update(deltaTime, speed);

		tree.Update(deltaTime, speed);

		UpdateObject();

		// 当たり判定
		CheckCollision();

		background.Update(deltaTime, speed);

		//visorRain.Update(deltaTime, speed);

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
			game_mode = TITLE;
		}
		break;
	}

	// 今回のキー状態を次回の比較用に保存
	memcpy(OldKey, Key, 256);
}

// 画面描画処理
void Play::Render()
{
	// 通常の描画先に設定
	SetDrawScreen(DX_SCREEN_BACK);

	// PLAY中だけ一時画面に描画する
	bool useShake = (game_mode == PLAY && gameScreen != -1);

	if (useShake)
	{
		SetDrawScreen(gameScreen);

		// 一時画面をクリア
		ClearDrawScreen();
	}

	switch (game_mode)
	{
	case TITLE:
		title.Draw(selectedMode);

		break;


	case FADE:
		RenderFade();

		break;


	case PLAY:
		// 1. 背景を描画
		background.Draw(playerX);
		// 2. 木を描画
		tree.Draw(playerX);
		// 3. 道路を描画
		road.Draw(playerX);
		// 4. 危険表示
		for (const Danger& danger : dangers)
		{
			danger.Draw(playerX);
		}
		// 5. 障害物を描画
		for (const Object& object : objects)
		{
			object.Draw(playerX);
		}
		// 6. プレイヤーを描画
		DrawHandleImage();

		// スコア
		DrawFormatString(
			20, 20, 0xFFFFFF,
			_T("SCORE: %d"), score
		);

		// スピード
		DrawFormatString(
			20, 50, 0xFFFFFF,
			_T("SPEED: %.2f"), speed
		);

		// visorRain.Draw();

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 40);

		DrawBox(
			0, 0,
			1280, 720,
			GetColor(0, 0, 0),
			TRUE
		);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		break;


	case RESULT:

		DrawBox(
			0, 0,
			1280, 720,
			0x101010,
			TRUE
		);

		DrawString(
			510, 220,
			_T("GAME OVER"),
			0xFF3333
		);

		DrawFormatString(
			550, 300, 0xFFFFFF,
			_T("SCORE: %d"), score
		);

		DrawString(
			430, 380,
			_T("PRESS SPACE TO RETRY"),
			0xFFFFFF
		);

		break;
	}

	// =================================
	// 画面全体の振動処理
	// =================================

	if (useShake)
	{
		// 描画先を通常画面に戻す
		SetDrawScreen(DX_SCREEN_BACK);

		// 画面を少し拡大して、揺れたときの隙間を防ぐ
		DrawExtendGraph(
			-6 + shakeX,
			-4 + shakeY,
			1286 + shakeX,
			724 + shakeY,
			gameScreen,
			FALSE
		);
	}
}

// 音声再生処理
void Play::Sound_play()
{
	switch (game_mode) {
	case TITLE:  // タイトル画面の時の処理
		if (title.menu) {
			// １．ゲーム開始
			if (KeyPush(KEY_INPUT_1)) {
				PlaySoundMem(se_title_engine, DX_PLAYTYPE_BACK);
			}
			// ２．操作方法
			if (!title.operate) {
				if (KeyPush(KEY_INPUT_2)) {
				}
			}
			else {
				if (KeyPush(KEY_INPUT_2)) {
				}
			}
			// ３．ゲーム終了
			if (Key[KEY_INPUT_3] == 1)
			{
			}
		}
		else {
			// タイトルからメニューへ
			if (KeyPush(KEY_INPUT_SPACE)) {
			}
		}
		break;

	case FADE:
		
		break;

	case PLAY:

		break;

	case RESULT:

		break;
	}
}

// ゲーム初期化
void Play::ResetGame()
{
	playerX = 0.0f;
	elapsedTime = 0.0f;
	spawnTimer = 0.0f;
	speed = 0.40f;
	score = 0;

	spawnTimer = 0.0f;

	dangerTimer = 0.0f;
	waitingForDanger = false;

	previousLane1 = 99;
	previousLane2 = 99;

	dangers.clear();
	pendingLanes.clear();

	title.Init();
	objects.clear();
	road.Init();
	background.Init();
	tree.Init();
	visorRain.Init();

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

void Play::UpdateObject()
{
	// 警告中でなければ、次の出現時間を計測
	if (!waitingForDanger)
	{
		spawnTimer += deltaTime;

		float interval = 1.5f - elapsedTime * 0.003f;

		if (interval < 0.75f)
		{
			interval = 0.75f;
		}

		if (spawnTimer >= interval)
		{
			spawnTimer = 0.0f;

			// 障害物の警告を開始
			SpawnObject();
		}
	}

	// 危険表示の更新
	for (Danger& danger : dangers)
	{
		danger.Update(deltaTime);
	}

	// 警告時間の計測
	if (waitingForDanger)
	{
		dangerTimer += deltaTime;

		// 1秒経過したら障害物を出現させる
		if (dangerTimer >= 1.0f)
		{
			for (int lane : pendingLanes)
			{
				Object newObject;
				newObject.Init(lane);

				objects.push_back(newObject);
			}

			// 警告を消す
			dangers.clear();
			pendingLanes.clear();

			waitingForDanger = false;
			dangerTimer = 0.0f;
		}
	}

	// 障害物の更新
	for (Object& object : objects)
	{
		object.Update(deltaTime, speed);
	}

	// 画面外の障害物を削除
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
	// 5レーンを用意
	std::vector<int> laneCandidates = { -2, -1, 0, 1, 2 };

	// 前回使用したレーンを除外
	laneCandidates.erase(
		std::remove_if(
			laneCandidates.begin(),
			laneCandidates.end(),
			[this](int lane)
			{
				return lane == previousLane1 ||
					lane == previousLane2;
			}),
		laneCandidates.end());

	// レーンをランダムに並び替える
	std::shuffle(laneCandidates.begin(), laneCandidates.end(), mt);

	// 経過時間によって障害物数を変更
	int objectCount = 1;

	if (elapsedTime >= 20.0f)
	{
		objectCount = 2;
	}

	// 今回の出現レーンを保存
	pendingLanes.clear();
	dangers.clear();

	for (int i = 0; i < objectCount; i++)
	{
		int lane = laneCandidates[i];

		pendingLanes.push_back(lane);
	}

	// リアルモードの場合
	if (selectedMode == GAME_MODE_REAL)
	{
		// 警告を表示せず、障害物をすぐに出現させる
		for (int lane : pendingLanes)
		{
			Object newObject;
			newObject.Init(lane);

			objects.push_back(newObject);
		}

		waitingForDanger = false;
		dangerTimer = 0.0f;
	}
	else
	{
		// ゲームモードの場合は警告を生成
		for (int lane : pendingLanes)
		{
			Danger newDanger;
			newDanger.Init(lane);

			dangers.push_back(newDanger);
		}

		// 警告開始
		dangerTimer = 0.0f;
		waitingForDanger = true;
	}

	// 使用したレーンを保存
	previousLane1 = pendingLanes[0];

	if (objectCount == 2)
	{
		previousLane2 = pendingLanes[1];
	}
	else
	{
		previousLane2 = 99;
	}
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
			StopSoundMem(se_play_engine);
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
	int handleY = SCREEN_H - 75;

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

// プレイ画面を小刻みに揺らす
void Play::UpdateShake()
{
	// 時間を進める
	shakeTime += deltaTime;

	// 小刻みに揺らす
	shakeX = static_cast<int>(
		sin(shakeTime * 35.0f) * shakePower
		);

	shakeY = static_cast<int>(
		cos(shakeTime * 45.0f) * shakePower
		);
}

// フェードモードの更新処理
void Play::UpdateFade()
{
	switch (fadeState)
	{
		// 白くなる
	case FLASH_IN:

		fadeAlpha += 25.0f;

		if (fadeAlpha >= 200.0f)
		{
			fadeAlpha = 200.0f;
			fadeState = FLASH_OUT;
		}

		break;

		// 白から戻る
	case FLASH_OUT:

		fadeAlpha -= 25.0f;

		if (fadeAlpha <= 0.0f)
		{
			fadeAlpha = 0.0f;
			fade_count++;
			if (fade_count >= 120) {
				fade_count = 0;
				fadeState = BLACK_OUT;
			}
		}

		break;

		// タイトル画面を暗くする
	case BLACK_OUT:

		fadeAlpha += fadeSpeed * deltaTime;

		if (fadeAlpha >= 255.0f)
		{
			fadeAlpha = 255.0f;

			// 完全に暗くなったらゲームを初期化
			ResetGame();

			// ゲーム画面の明転へ
			fadeState = BLACK_IN;
		}

		break;

		// ゲーム画面を明るくする
	case BLACK_IN:

		fadeAlpha -= fadeSpeed * deltaTime;

		if (fadeAlpha <= 0.0f)
		{
			fadeAlpha = 0.0f;

			// フェード終了
			// プレイ画面へ
			game_mode = PLAY;
			PlaySoundMem(se_play_engine, DX_PLAYTYPE_LOOP);
		}

		break;
	}
}

// フェードモードの描画処理
void Play::RenderFade()
{
	// =================================
	// 白いフラッシュ
	// =================================
	if (fadeState == FLASH_IN || fadeState == FLASH_OUT)
	{
		// タイトル画面を背景に描画
		title.Draw(selectedMode);

		// 白いフラッシュを重ねる
		SetDrawBlendMode(
			DX_BLENDMODE_ALPHA,
			(int)fadeAlpha
		);

		DrawBox(
			0, 0, 1280, 720,
			GetColor(255, 255, 255),
			TRUE
		);
	}

	// =================================
	// タイトル画面の暗転
	// =================================
	else if (fadeState == BLACK_OUT)
	{
		// タイトル画面を描画
		title.Draw(selectedMode);

		// 徐々に黒くする
		SetDrawBlendMode(
			DX_BLENDMODE_ALPHA,
			(int)fadeAlpha
		);

		DrawBox(
			0, 0, 1280, 720,
			GetColor(0, 0, 0),
			TRUE
		);
	}

	// =================================
	// プレイ画面の明転
	// =================================
	else if (fadeState == BLACK_IN)
	{
		// 1. 背景
		background.Draw(playerX);

		// 2. 木
		tree.Draw(playerX);

		// 3. 道路
		road.Draw(playerX);

		// 4. 危険表示（ゲームモードのみ）
		if (selectedMode == GAME_MODE_NORMAL)
		{
			for (const Danger& danger : dangers)
			{
				danger.Draw(playerX);
			}
		}

		// 5. 障害物
		for (const Object& object : objects)
		{
			object.Draw(playerX);
		}

		// 6. ハンドル
		DrawHandleImage();

		// 7. スコア
		DrawFormatString(
			20, 20, 0xFFFFFF,
			_T("SCORE: %d"), score
		);

		// 8. スピード
		DrawFormatString(
			20, 50, 0xFFFFFF,
			_T("SPEED: %.2f"), speed
		);

		// 通常プレイ時と同じ画面の暗さ
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 40);

		DrawBox(
			0, 0, 1280, 720,
			GetColor(0, 0, 0),
			TRUE
		);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// 黒い画面を徐々に透明にする
		SetDrawBlendMode(
			DX_BLENDMODE_ALPHA,
			(int)fadeAlpha
		);

		DrawBox(
			0, 0, 1280, 720,
			GetColor(0, 0, 0),
			TRUE
		);
	}
	// ブレンドモードを元に戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
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