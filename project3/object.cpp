
#include "Object.h"

namespace
{
    constexpr int SCREEN_W = 1280;
    constexpr int SCREEN_H = 720;
    constexpr int CENTER_X = SCREEN_W / 2;

    constexpr float ROAD_TOP_HALF = 8.0f;
    constexpr float ROAD_BOTTOM_HALF = 620.0f;

    // コーン画像の高さ
    constexpr float OBJECT_IMAGE_HEIGHT = 765.0f;
}

// 初期化処理
void Object::Init(int laneNumber)
{
    // 5レーンに対応
    lane = laneNumber * 0.325f;
    z = 1.0f;

    // 障害物画像の読み込み
    if (object_image == -1)
    {
        object_image = LoadGraph(_T("data/object.png"));
    }
}

// 更新処理
void Object::Update(float deltaTime, float speed)
{
    // 奥から手前へ移動
    z -= deltaTime * speed;
}

// 描画処理
void Object::Draw(float playerX) const
{
    if (object_image == -1)
        return;

    // 奥から手前への進行度
    float t = GetDepthRate();

    // 障害物の画面上の高さ
    float y = 360.0f
        + (SCREEN_H - 360.0f) * t * t;

    // 遠近率
    float perspective = t * t;

    // 道路の幅
    float roadHalfWidth = ROAD_TOP_HALF
        + (ROAD_BOTTOM_HALF - ROAD_TOP_HALF)
        * perspective;

    // 道路全体の左右移動
    float roadShift = -playerX * 300.0f * perspective;

    float roadCenterX = CENTER_X + roadShift;

    // レーン位置
    float x = roadCenterX
        + lane * roadHalfWidth * 0.75f;

    // 奥行きに応じて画像を拡大
    float scale = 0.015f + 0.14f * t;

    // 画像の中心位置
    // コーンの底面が道路に接するように調整
    float drawY = y - (OBJECT_IMAGE_HEIGHT * scale / 2.0f);

    // コーン画像を描画
    DrawRotaGraph(
        static_cast<int>(x),
        static_cast<int>(drawY),
        scale,
        0.0f,
        object_image,
        TRUE
    );
}

// 終了処理
void Object::Release()
{
    if (object_image != -1)
    {
        DeleteGraph(object_image);
        object_image = -1;
    }
}

// 奥行きから画面上の位置を計算
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

// 画面外判定
bool Object::IsOutOfScreen() const
{
    return z < 0.0f;
}

// プレイヤーとの距離判定
bool Object::IsNearPlayer() const
{
    return z <= 0.10f;
}

// 奥行き取得
float Object::GetZ() const
{
    return z;
}

// レーン取得
float Object::GetLane() const
{
    return lane;
}
