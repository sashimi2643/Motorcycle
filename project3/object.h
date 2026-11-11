#pragma once
#include "Base.h"
#include "DxLib.h"
#include <algorithm>

class Object
{
private:
	float z = 1.0f;
	float lane = 0.0f;

	int object_image = -1;

	// 奥行きから画面上の位置を計算
	float GetDepthRate() const;

public:
	float targetLane = 0.0f; // 最終的な出現レーン
	//	初期化処理
	void Init(int lane);
	//	更新処理
	void Update(float deltaTime, float speed);
	//	描画処理
	void Draw(float playerX) const;
	//	終了処理
	void Release();

	bool IsOutOfScreen() const;
	bool IsNearPlayer() const;

	float GetZ() const;
	float GetLane() const;
};

