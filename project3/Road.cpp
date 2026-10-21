#include "Road.h"

namespace
{
    constexpr int SCREEN_W = 1280;
    constexpr int SCREEN_H = 720;

    // 地平線を上げて、遠近感を強める
    constexpr int HORIZON_Y = 120;

    // 道路の幅
    constexpr float ROAD_BOTTOM_HALF =620.0f;
    constexpr float ROAD_TOP_HALF = 8.0f;

    const unsigned int SKY_COLOR =
        GetColor(100, 180, 235);

    const unsigned int GROUND_COLOR =
        GetColor(65, 135, 65);

    const unsigned int ROAD_COLOR =
        GetColor(65, 65, 70);

    const unsigned int EDGE_COLOR =
        GetColor(230, 230, 220);

    const unsigned int LINE_COLOR =
        GetColor(245, 245, 225);
}

//	初期化処理
void Road::Init()
{
    lineOffset = 0.0f;
}

//	更新処理
void Road::Update(float deltaTime, float speed)
{
    // 道路中央の白線を下方向へ流す
    lineOffset += deltaTime * speed * 500.0f;

    if (lineOffset >= 80.0f)
    {
        lineOffset -= 80.0f;
    }
}

//	描画処理
void Road::Draw(float playerX) const
{
    // 空
    DrawBox(
        0, 0, SCREEN_W, HORIZON_Y,
        SKY_COLOR, TRUE
    );

    // 地面
    DrawBox(
        0, HORIZON_Y, SCREEN_W, SCREEN_H,
        GROUND_COLOR, TRUE
    );

    const int centerX = SCREEN_W / 2;

    // 道路を横線ごとに描画し、奥から手前へ広がる形にする
    for (int y = HORIZON_Y; y < SCREEN_H; ++y)
    {
        float t = static_cast<float>(y - HORIZON_Y) /
            static_cast<float>(SCREEN_H - HORIZON_Y);

        float halfWidth = GetRoadHalfWidth(t);

        int leftX = centerX - static_cast<int>(halfWidth);
        int rightX = centerX + static_cast<int>(halfWidth);

        DrawLine(
            leftX, y, rightX, y,
            ROAD_COLOR, 1
        );

        // 道路の左右の白線
        DrawPixel(leftX, y, EDGE_COLOR);
        DrawPixel(rightX, y, EDGE_COLOR);
    }

    // 道路の左右の境界線を直線で描画
    DrawLine(
        centerX - static_cast<int>(ROAD_TOP_HALF),
        HORIZON_Y,
        centerX - static_cast<int>(ROAD_BOTTOM_HALF),
        SCREEN_H,
        EDGE_COLOR,
        2
    );

    DrawLine(
        centerX + static_cast<int>(ROAD_TOP_HALF),
        HORIZON_Y,
        centerX + static_cast<int>(ROAD_BOTTOM_HALF),
        SCREEN_H,
        EDGE_COLOR,
        2
    );

    // 道路中央の破線
    // 奥では短く細く、手前では長く太くする
    for (int i = 0; i < 12; ++i)
    {
        float t = (i + 1) / 12.0f;
        float yRate = t * t;

        int y = HORIZON_Y +
            static_cast<int>(yRate * (SCREEN_H - HORIZON_Y));

        int dashLength = 5 + static_cast<int>(t * 35.0f);
        int dashWidth = 1 + static_cast<int>(t * 5.0f);

        // 破線が走行速度に合わせて流れる
        int drawY = y + static_cast<int>(lineOffset * t);

        if (drawY > SCREEN_H)
        {
            drawY -= (SCREEN_H - HORIZON_Y);
        }

        float roadHalfWidth = GetRoadHalfWidth(t);
        int dashHalfWidth = static_cast<int>(
            dashWidth * (roadHalfWidth / ROAD_BOTTOM_HALF)
            );

        if (dashHalfWidth < 1)
        {
            dashHalfWidth = 1;
        }

        DrawBox(
            centerX - dashHalfWidth,
            drawY,
            centerX + dashHalfWidth,
            drawY + dashLength,
            LINE_COLOR,
            TRUE
        );
    }
}

float Road::GetRoadHalfWidth(float t) const
{
    // t = 0 が地平線、t = 1 が画面下端
    // 2乗することで、奥から手前への広がりを強調
    return ROAD_TOP_HALF +
        (ROAD_BOTTOM_HALF - ROAD_TOP_HALF) * t * t;
}