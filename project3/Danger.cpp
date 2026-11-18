#include "Danger.h"

namespace
{
    constexpr int SCREEN_W = 1280;
    constexpr int SCREEN_H = 720;
    constexpr int CENTER_X = SCREEN_W / 2;

    constexpr float ROAD_TOP_HALF = 8.0f;
    constexpr float ROAD_BOTTOM_HALF = 620.0f;

    // 警告画像を表示する奥行き
    constexpr float DANGER_DEPTH = 0.55f;
}

// 画像ハンドル
int Danger::dangerImage = -1;

// 画像の読み込み
void Danger::LoadImage()
{
    if (dangerImage == -1)
    {
        dangerImage = LoadGraph(_T("data/danger.png"));
    }
}

// 初期化
void Danger::Init(int laneNumber)
{
    lane = laneNumber * 0.325f;
    timer = 0.0f;
}

// 更新
void Danger::Update(float deltaTime)
{
    timer += deltaTime;
}

// 描画
void Danger::Draw(float playerX) const
{
    if (dangerImage == -1)
        return;

    float t = DANGER_DEPTH;
    float perspective = t * t;

    // 道路の幅
    float roadHalfWidth = ROAD_TOP_HALF
        + (ROAD_BOTTOM_HALF - ROAD_TOP_HALF) * perspective;

    // 道路の中心位置
    float roadShift = -playerX * 300.0f * perspective;
    float roadCenterX = CENTER_X + roadShift;

    // 警告画像の表示位置
    float x = roadCenterX + lane * roadHalfWidth * 0.75f;

    float y = 160.0f
        + (SCREEN_H - 160.0f) * perspective;

    // 画像サイズを取得
    int imageW, imageH;
    GetGraphSize(dangerImage, &imageW, &imageH);

    // 画像の長辺が約60pxになるように調整
    float imageSize = static_cast<float>(
        imageW > imageH ? imageW : imageH
        );

    float scale = 60.0f / imageSize;

    // 点滅演出
    int alpha = 255;

    if (static_cast<int>(timer * 5.0f) % 2 == 0)
    {
        alpha = 100;
    }

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

    DrawRotaGraph(
        static_cast<int>(x),
        static_cast<int>(y),
        scale,
        0.0f,
        dangerImage,
        TRUE
    );

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

// 画像の解放
void Danger::ReleaseImage()
{
    if (dangerImage != -1)
    {
        DeleteGraph(dangerImage);
        dangerImage = -1;
    }
}