#include "Road.h"

namespace
{
    constexpr int SCREEN_W = 1280;
    constexpr int SCREEN_H = 720;

    // 地平線を上げて、遠近感を強める
    constexpr int HORIZON_Y = 360;

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
    const int baseCenterX = SCREEN_W / 2;

    // 道路の各高さを描画
    for (int y = HORIZON_Y; y < SCREEN_H; ++y)
    {
        // 0 = 地平線、1 = 画面手前
        float p = static_cast<float>(y - HORIZON_Y)
            / static_cast<float>(SCREEN_H - HORIZON_Y);

        // 地平線を支点に、手前ほど横へ移動
        float shift = -playerX * 300.0f * p;

        float centerX = baseCenterX + shift;

        // 道路の幅は高さに比例して直線的に広がる
        float halfWidth = ROAD_TOP_HALF
            + (ROAD_BOTTOM_HALF - ROAD_TOP_HALF) * p;

        int leftX = static_cast<int>(centerX - halfWidth);
        int rightX = static_cast<int>(centerX + halfWidth);

        // 道路本体
        DrawLine(leftX, y, rightX, y, ROAD_COLOR, 1);

        // 道路の左右端
        DrawPixel(leftX, y, EDGE_COLOR);
        DrawPixel(rightX, y, EDGE_COLOR);
    }

    // 左右の境界線を直線で描画
    float bottomShift = -playerX * 300.0f;
    int bottomCenterX =
        baseCenterX + static_cast<int>(bottomShift);

    DrawLine(
        baseCenterX - static_cast<int>(ROAD_TOP_HALF),
        HORIZON_Y,
        bottomCenterX - static_cast<int>(ROAD_BOTTOM_HALF),
        SCREEN_H,
        EDGE_COLOR, 2
    );

    DrawLine(
        baseCenterX + static_cast<int>(ROAD_TOP_HALF),
        HORIZON_Y,
        bottomCenterX + static_cast<int>(ROAD_BOTTOM_HALF),
        SCREEN_H,
        EDGE_COLOR, 2
    );

    // 中央の破線
    // 各破線の高さに合わせて横移動量を計算
    const float dashSpacing = 80.0f;
    float offset = lineOffset;

    while (offset >= dashSpacing)
    {
        offset -= dashSpacing;
    }

    for (int i = 0; i < 20; i++)
    {
        float t = (i * dashSpacing + offset) / 600.0f;

        if (t < 0.0f || t > 1.0f)
            continue;

        float p = t * t;

        // 破線の上端
        float y1 = HORIZON_Y
            + p * (SCREEN_H - HORIZON_Y);

        // 破線の下端は少し手前
        float t2 = t + 0.035f;

        if (t2 > 1.0f)
            t2 = 1.0f;

        float p2 = t2 * t2;

        float y2 = HORIZON_Y
            + p2 * (SCREEN_H - HORIZON_Y);

        // 道路の移動量を上下で計算する
        float shift1 = -playerX * 300.0f * p;
        float shift2 = -playerX * 300.0f * p2;

        float x1 = SCREEN_W / 2.0f + shift1;
        float x2 = SCREEN_W / 2.0f + shift2;

        // 奥から手前に向かって傾く破線
        DrawLine(
            (int)x1, (int)y1,
            (int)x2, (int)y2,
            LINE_COLOR,
            3 + (int)(t * 3.0f)
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