#include "Background.h"

// ‰Šú‰»
void Background::Init()
{
    background_image = LoadGraph(_T("data/background.png"));
    kusa_image = LoadGraph(_T("data/kusa.png"));
}

// XV
void Background::Update(float deltaTime, float speed)
{
}

// •`‰æ
void Background::Draw(float playerX) const
{
	DrawGraph(0, -1, background_image, TRUE);
    DrawGraph(0, 360, kusa_image, TRUE);
}