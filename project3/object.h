#pragma once
#include "Base.h"
#include "DxLib.h"
#include <algorithm>

class Object
{
private:
	float z = 1.0f;
	float lane = 0.0f;

	// 奥行きから画面上の位置を計算
	float GetDepthRate() const;

public:
	//	初期化処理
	void Init(int lane);
	//	更新処理
	void Update(float deltaTime, float speed);
	//	描画処理
	void Draw(float playerX) const;

	bool IsOutOfScreen() const;
	bool IsNearPlayer() const;

	float GetZ() const;
	float GetLane() const;
};

