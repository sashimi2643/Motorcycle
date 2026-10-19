#pragma once
#include "Base.h"
#include "DxLib.h"
#include <cmath>

class Road
{
private:
	float scrollOffset = 0.0f;

	// “¹˜H‚Ì‰“‹ßŠ´‚ğŒvZ‚·‚é
	float GetRoadHalfWidth(float t) const;

public:

	//	‰Šú‰»ˆ—
	void Init();

	//	XVˆ—
	void Update(float deltaTime, float speed);

	//	•`‰æˆ—
	void Draw(float playerX) const;
};

