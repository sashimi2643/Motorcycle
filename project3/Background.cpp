#include "Background.h"

// 画面サイズ
namespace
{
    constexpr int SCREEN_W = 1280;
    constexpr int SCREEN_H = 720;

    // 地平線の高さ
    constexpr int HORIZON_Y = 360;

    // 道路の幅（Road.cpp / Object.cpp と合わせる）
    constexpr float ROAD_TOP_HALF = 8.0f;
    constexpr float ROAD_BOTTOM_HALF = 620.0f;

    // 道路の左右への移動量
    constexpr float ROAD_SHIFT_AMOUNT = 300.0f;

    // 色は実行時に取得する
    const unsigned int SKY_COLOR =
        GetColor(70, 165, 235);

    const unsigned int CLOUD_COLOR =
        GetColor(215, 235, 245);

    const unsigned int FAR_MOUNTAIN_COLOR =
        GetColor(80, 155, 165);

    const unsigned int NEAR_MOUNTAIN_COLOR =
        GetColor(55, 125, 115);

    const unsigned int GRASS_COLOR =
        GetColor(65, 145, 65);

    const unsigned int GRASS_DARK_COLOR =
        GetColor(45, 115, 50);

    const unsigned int TREE_DARK_COLOR =
        GetColor(15, 75, 55);

    const unsigned int TREE_MIDDLE_COLOR =
        GetColor(25, 105, 65);

    const unsigned int TREE_LIGHT_COLOR =
        GetColor(45, 135, 65);

    const unsigned int TRUNK_COLOR =
        GetColor(95, 65, 40);
}

// 初期化
void Background::Init()
{
    scrollOffset = 0.0f;
    background_image = LoadGraph(_T("data/background.png"));
    kusa_image = LoadGraph(_T("data/kusa.png"));
}

// 更新
void Background::Update(float deltaTime, float speed)
{
    // 走行速度に応じて背景の模様を流す
    scrollOffset += deltaTime * speed * 120.0f;

    // 数値が大きくなりすぎないようにする
    if (scrollOffset >= 10000.0f)
    {
        scrollOffset -= 10000.0f;
    }
}

// 描画
void Background::Draw(float playerX) const
{
    const int centerX = SCREEN_W / 2;

    // ----------------------------------------
    // 1. 空
    // ----------------------------------------
    //DrawBox(
    //    0, 0,
    //    SCREEN_W, HORIZON_Y + 1,
    //    SKY_COLOR, TRUE
    //);

    //// ----------------------------------------
    //// 2. 雲
    //// ----------------------------------------
    //// 画像素材を使わず、複数の楕円で雲を表現
    //DrawOval(180, 55, 230, 68, CLOUD_COLOR, TRUE);
    //DrawOval(215, 43, 260, 67, CLOUD_COLOR, TRUE);
    //DrawOval(245, 55, 285, 68, CLOUD_COLOR, TRUE);

    //DrawOval(930, 65, 980, 77, CLOUD_COLOR, TRUE);
    //DrawOval(960, 50, 1010, 75, CLOUD_COLOR, TRUE);
    //DrawOval(990, 64, 1035, 77, CLOUD_COLOR, TRUE);

    //// ----------------------------------------
    //// 3. 遠くの山
    //// ----------------------------------------
    //DrawTriangle(
    //    0, HORIZON_Y,
    //    180, 255,
    //    390, HORIZON_Y,
    //    FAR_MOUNTAIN_COLOR, TRUE
    //);

    //DrawTriangle(
    //    250, HORIZON_Y,
    //    480, 270,
    //    700, HORIZON_Y,
    //    FAR_MOUNTAIN_COLOR, TRUE
    //);

    //DrawTriangle(
    //    570, HORIZON_Y,
    //    830, 248,
    //    1080, HORIZON_Y,
    //    FAR_MOUNTAIN_COLOR, TRUE
    //);

    //DrawTriangle(
    //    900, HORIZON_Y,
    //    1100, 268,
    //    SCREEN_W, HORIZON_Y,
    //    FAR_MOUNTAIN_COLOR, TRUE
    //);

    //// 手前側の山
    //DrawTriangle(
    //    0, HORIZON_Y,
    //    260, 290,
    //    510, HORIZON_Y,
    //    NEAR_MOUNTAIN_COLOR, TRUE
    //);

    //DrawTriangle(
    //    650, HORIZON_Y,
    //    900, 285,
    //    SCREEN_W, HORIZON_Y,
    //    NEAR_MOUNTAIN_COLOR, TRUE
    //);

	DrawGraph(0, -1, background_image, TRUE);
    DrawGraph(0, 360, kusa_image, TRUE);

    // ----------------------------------------
    // 4. 草地
    // ----------------------------------------
   /* DrawBox(
        0, HORIZON_Y,
        SCREEN_W, SCREEN_H,
        GRASS_COLOR, TRUE
    );*/

    // ----------------------------------------
    // 5. 森の木
    // ----------------------------------------
    // 左右に木を並べ、遠くは小さく、
    // 手前は大きくなるように描画する
    for (int side = -1; side <= 1; side += 2)
    {
        for (int i = 0; i < 15; i++)
        {
            // 0に近いほど遠く、1に近いほど手前
            float depth = (i + 1) / 15.0f;

            // 木の高さと道路幅を遠近法で変化させる
            float perspective = depth * depth;

            float y = HORIZON_Y
                + perspective * (SCREEN_H - HORIZON_Y);

            float roadHalfWidth = ROAD_TOP_HALF
                + (ROAD_BOTTOM_HALF - ROAD_TOP_HALF)
                * perspective;

            // 道路の左右への移動に合わせて木も少し動かす
            float roadShift =
                -playerX * ROAD_SHIFT_AMOUNT * perspective;

            // 道路の外側に木を配置する
            float treeX = centerX + roadShift
                + side * (roadHalfWidth + 25.0f
                    + depth * 100.0f);

            // 木の高さ
            int treeHeight = 15 + (int)(depth * 125.0f);

            // 木の横幅
            int treeWidth = 5 + (int)(depth * 30.0f);

            // 走行に合わせた草地の模様の移動
            float moving = std::fmod(
                scrollOffset * (0.15f + depth * 0.5f),
                24.0f
            );

            // 手前ほど少し大きく上下に動かして走行感を出す
            int treeY = (int)y + (int)moving;

            int x = (int)treeX;
            int topY = treeY - treeHeight;

            // 木の幹
            int trunkWidth = treeWidth / 4 + 1;

            DrawBox(
                x - trunkWidth / 2,
                treeY - treeHeight / 3,
                x + trunkWidth / 2,
                treeY,
                TRUNK_COLOR,
                TRUE
            );

            // 木の葉：三角形を重ねて針葉樹を表現
            DrawTriangle(
                x, topY,
                x - treeWidth, treeY,
                x + treeWidth, treeY,
                TREE_DARK_COLOR, TRUE
            );

            DrawTriangle(
                x, topY + treeHeight / 4,
                x - treeWidth * 3 / 4,
                treeY - treeHeight / 4,
                x + treeWidth * 3 / 4,
                treeY - treeHeight / 4,
                TREE_MIDDLE_COLOR, TRUE
            );

            DrawTriangle(
                x, topY + treeHeight / 2,
                x - treeWidth / 2,
                treeY - treeHeight / 2,
                x + treeWidth / 2,
                treeY - treeHeight / 2,
                TREE_LIGHT_COLOR, TRUE
            );
        }
    }

    // ----------------------------------------
    // 6. 草地に流れる小さな模様
    // ----------------------------------------
    // 木とは別に草の模様を描き、速度感を加える
    for (int i = 0; i < 30; i++)
    {
        float depth = (i + 1) / 30.0f;
        float perspective = depth * depth;

        float y = HORIZON_Y
            + perspective * (SCREEN_H - HORIZON_Y);

        float roadHalfWidth = ROAD_TOP_HALF
            + (ROAD_BOTTOM_HALF - ROAD_TOP_HALF) * perspective;

        float roadShift =
            -playerX * ROAD_SHIFT_AMOUNT * perspective;

        float move = std::fmod(
            scrollOffset * (0.2f + depth),
            35.0f
        );

        for (int side = -1; side <= 1; side += 2)
        {
            float x = centerX + roadShift
                + side * (roadHalfWidth + 35.0f
                    + depth * 180.0f);

            int grassX = (int)x;
            int grassY = (int)y + (int)move;

            int length = 2 + (int)(depth * 12.0f);

            DrawLine(
                grassX, grassY,
                grassX + side * length,
                grassY + length / 2,
                GRASS_DARK_COLOR,
                1
            );
        }
    }
}