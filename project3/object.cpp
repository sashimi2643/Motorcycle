#include "Object.h"

namespace
{
    constexpr int SCREEN_W = 1280;
    constexpr int SCREEN_H = 720;
    constexpr int HORIZON_Y = 160;
    constexpr int CENTER_X = SCREEN_W / 2;

    constexpr unsigned int OBSTACLE_COLOR =
        0xE64A19;

    constexpr unsigned int DETAIL_COLOR =
        0xFFC107;
}

//	初期化処理
void Object::Init(int laneNumber)
{
    // 左・中央・右の3位置
    lane = laneNumber * 0.65f;
    z = 1.0f;
}


//	更新処理
void Object::Update(float deltaTime, float speed)
{
    // 奥から手前へ移動
    z -= deltaTime * speed;
}

//	描画処理
void Object::Draw(float playerX) const
{
    constexpr int SCREEN_H = 720;
    constexpr int HORIZON_Y = 360;
    constexpr int CENTER_X = 640;

    constexpr float ROAD_TOP_HALF = 8.0f;
    constexpr float ROAD_BOTTOM_HALF = 620.0f;

    // 奥から手前への進行度
    float t = GetDepthRate();

    // 障害物の画面上の高さ
    float y = HORIZON_Y
        + (SCREEN_H - HORIZON_Y) * t * t;

    // 画面上の高さに対応する遠近率
    float perspective = t * t;

    // 道路の幅：Road.cppと同じ計算
    float roadHalfWidth = ROAD_TOP_HALF
        + (ROAD_BOTTOM_HALF - ROAD_TOP_HALF)
        * perspective;

    // 地平線では移動ゼロ、手前ほど移動量を増やす
    float roadShift = -playerX * 300.0f * perspective;

    float roadCenterX = CENTER_X + roadShift;

    // レーン位置を道路の幅に合わせる
    // 0.75fで道路の内側に余裕を持たせる
    float x = roadCenterX
        + lane * roadHalfWidth * 0.75f;

    // 遠近感に合わせて障害物を拡大
    float w = 8.0f + 42.0f * t;
    float h = 12.0f + 70.0f * t;

    int left = static_cast<int>(x - w / 2.0f);
    int right = static_cast<int>(x + w / 2.0f);
    int top = static_cast<int>(y - h);
    int bottom = static_cast<int>(y);

    // 工事用ブロック
    DrawBox(
        left, top, right, bottom,
        OBSTACLE_COLOR, TRUE
    );

    // 警告ライン
    if (t > 0.25f)
    {
        int stripeY =
            top + static_cast<int>(h * 0.45f);

        int lineWidth = static_cast<int>(t * 5.0f);
        if (lineWidth < 1) lineWidth = 1;

        DrawLine(
            left, stripeY, right, stripeY,
            DETAIL_COLOR, lineWidth
        );
    }

    // 枠線
    DrawBox(
        left, top, right, bottom,
        0x202020, FALSE
    );
}

float Object::GetDepthRate() const
{
    float depth = z;

    if (depth < 0.0f)
    {
        depth = 0.0f;
    }

    if (depth > 1.0f)
    {
        depth = 1.0f;
    }

    return 1.0f - depth;
}

bool Object::IsOutOfScreen() const
{
    return z < 0.0f;
}

bool Object::IsNearPlayer() const
{
    // 衝突判定を行う距離
    return z <= 0.10f;
}

float Object::GetZ() const
{
    return z;
}

float Object::GetLane() const
{
    return lane;
}