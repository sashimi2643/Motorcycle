#include "Background.h"

// ‰Šú‰»
void Background::Init()
{
    background_image = LoadGraph(_T("data/background.png"));
    kusa_image = LoadGraph(_T("data/kusa.png"));
    kusa_y = 0;
}

// XV
void Background::Update(float deltaTime, float speed)
{
    kusa_y += 10.0f * speed;
    if (kusa_y >= 360) {
        kusa_y = 0;
    }
}

// •`‰æ
void Background::Draw(float playerX) const
{
    int kusa1 = kusa_y;
    int kusa2 = kusa_y + 360;
    for (int i = 0; i < 2; i++) {
        DrawGraph(0, kusa1, kusa_image, TRUE);
        DrawGraph(0, kusa2, kusa_image, TRUE);
    }
	DrawGraph(0, -1, background_image, TRUE);
	DrawLine(0, 360, 1280, 360, GetColor(100, 100, 100));
}