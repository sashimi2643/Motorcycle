#pragma once
#include "DxLib.h"
#include <cmath>

// ”wŒi‚ğŠÇ—‚·‚éƒNƒ‰ƒX
class Background
{
private:
	// ”wŒi‰æ‘œ‚Ìƒnƒ“ƒhƒ‹
    int background_image = -1;
    int kusa_image = -1;
public:
    // ‰Šú‰»
    void Init();

    // XV
    void Update(float deltaTime, float speed);

    // •`‰æ
    void Draw(float playerX) const;
};