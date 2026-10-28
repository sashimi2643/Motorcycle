#pragma once
#include "DxLib.h"
#include <cmath>

// ”wŒi‚ğŠÇ—‚·‚éƒNƒ‰ƒX
class Background
{
private:
    // ”wŒi‚Ì–Í—l‚ğ—¬‚·‚½‚ß‚ÌˆÚ“®—Ê
    float scrollOffset = 0.0f;

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