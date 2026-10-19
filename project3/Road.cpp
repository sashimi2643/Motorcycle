#include "Road.h"

namespace
{
    constexpr int SCREEN_W = 960;
    constexpr int SCREEN_H = 640;
    constexpr int HORIZON_Y = 140;

    constexpr unsigned int SKY_COLOR =
        0x87CEEB;

    constexpr unsigned int GRASS_COLOR =
        0x267A35;

    constexpr unsigned int ROAD_COLOR =
        0x454545;

    constexpr unsigned int WHITE_COLOR =
        0xFFFFFF;
}

//	初期化処理
void Road::Init()
{
    //scrollOffset = 0.0f;
}

//	更新処理
void Road::Update(float deltaTime, float speed)
{
    // 時間経過で道路の白線を流す
    scrollOffset += deltaTime * speed;

    // 周期的に繰り返す
    if (scrollOffset >= 1.0f)
    {
        scrollOffset -= 1.0f;
    }
}

//	描画処理
void Road::Draw(float playerX) const
{
    // 空と地面
    DrawBox(
        0, 0, SCREEN_W, HORIZON_Y,
        SKY_COLOR, TRUE);

    DrawBox(
        0, HORIZON_Y, SCREEN_W, SCREEN_H,
        GRASS_COLOR, TRUE);

    const int centerX = SCREEN_W / 2;

    // 道路を細かい四角形に分割して描画
    constexpr int SEGMENTS = 60;

    for (int i = 0; i < SEGMENTS; ++i)
    {
        float t0 = static_cast<float>(i) / SEGMENTS;
        float t1 = static_cast<float>(i + 1) / SEGMENTS;

        float y0 = HORIZON_Y
            + (SCREEN_H - HORIZON_Y) * t0 * t0;

        float y1 = HORIZON_Y
            + (SCREEN_H - HORIZON_Y) * t1 * t1;

        float w0 = GetRoadHalfWidth(t0);
        float w1 = GetRoadHalfWidth(t1);

        DrawQuadrangle(
            static_cast<int>(centerX - w0),
            static_cast<int>(y0),
            static_cast<int>(centerX + w0),
            static_cast<int>(y0),
            static_cast<int>(centerX + w1),
            static_cast<int>(y1),
            static_cast<int>(centerX - w1),
            static_cast<int>(y1),
            ROAD_COLOR, TRUE);
    }

    // 道路の左右の境界線
    DrawLine(
        centerX - static_cast<int>(GetRoadHalfWidth(0.0f)),
        HORIZON_Y,
        centerX - static_cast<int>(GetRoadHalfWidth(1.0f)),
        SCREEN_H,
        WHITE_COLOR, 2);

    DrawLine(
        centerX + static_cast<int>(GetRoadHalfWidth(0.0f)),
        HORIZON_Y,
        centerX + static_cast<int>(GetRoadHalfWidth(1.0f)),
        SCREEN_H,
        WHITE_COLOR, 2);

    // 中央付近の車線区切り線をスクロールさせる
    constexpr int DASH_COUNT = 14;

    for (int lane = -1; lane <= 1; lane += 2)
    {
        for (int i = 0; i < DASH_COUNT; ++i)
        {
            float t =
                std::fmod(
                    static_cast<float>(i) / DASH_COUNT
                    + scrollOffset,
                    1.0f);

            float tNext = t + 0.035f;

            if (tNext > 1.0f)
            {
                tNext = 1.0f;
            }

            float y0 = HORIZON_Y
                + (SCREEN_H - HORIZON_Y) * t * t;

            float y1 = HORIZON_Y
                + (SCREEN_H - HORIZON_Y)
                * tNext * tNext;

            float x0 = centerX
                + lane * GetRoadHalfWidth(t) / 3.0f;

            float x1 = centerX
                + lane * GetRoadHalfWidth(tNext) / 3.0f;

            DrawLine(
                static_cast<int>(x0),
                static_cast<int>(y0),
                static_cast<int>(x1),
                static_cast<int>(y1),
                WHITE_COLOR,
                static_cast<int>(1 + 3 * t));
        }
    }

    // プレイヤーが右に動くと、道路は左へずれる
    auto centerAt = [this, playerX](float t)
        {
            return SCREEN_W / 2.0f
                - playerX * GetRoadHalfWidth(t) * 0.75f;
        };
}

float Road::GetRoadHalfWidth(float t) const
{
    // 遠くでは細く、手前では広くする
    return 35.0f + 430.0f * t;
}