#include "Object.h"

namespace
{
    constexpr int SCREEN_W = 960;
    constexpr int SCREEN_H = 640;
    constexpr int HORIZON_Y = 140;
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
    float t = GetDepthRate();

    float y = HORIZON_Y
        + (SCREEN_H - HORIZON_Y) * t * t;

    float roadHalfWidth = 35.0f + 430.0f * t;

    float x = CENTER_X
        + (lane - playerX) * roadHalfWidth * 0.75f;

    // 遠いほど小さく、近いほど大きくする
    float w = 8.0f + 42.0f * t;
    float h = 12.0f + 70.0f * t;

    int left = static_cast<int>(x - w / 2);
    int right = static_cast<int>(x + w / 2);
    int top = static_cast<int>(y - h);
    int bottom = static_cast<int>(y);

    // 工事用ブロックをイメージした図形
    DrawBox(
        left, top, right, bottom,
        OBSTACLE_COLOR, TRUE);

    // 警告ライン
    if (t > 0.25f)
    {
        int stripeY = top + static_cast<int>(h * 0.45f);

        DrawLine(
            left, stripeY,
            right, stripeY,
            DETAIL_COLOR,
            std::max(1, static_cast<int>(t * 5)));
    }

    // 枠線
    DrawBox(
        left, top, right, bottom,
        0x202020, FALSE);
}

float Object::GetDepthRate() const
{
    return 1.0f - std::clamp(z, 0.0f, 1.0f);
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