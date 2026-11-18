#pragma once
#include "DxLib.h"

class Danger
{
private:
    float lane = 0.0f;
    float timer = 0.0f;

    static int dangerImage;

public:
    // ‰æ‘œ‚Ì“Ç‚İ‚İ
    static void LoadImage();

    // ‰Šú‰»
    void Init(int laneNumber);

    // XV
    void Update(float deltaTime);

    // •`‰æ
    void Draw(float playerX) const;

    // ‰æ‘œ‚Ì‰ğ•ú
    static void ReleaseImage();
};