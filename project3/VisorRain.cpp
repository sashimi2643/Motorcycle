#include "VisorRain.h"

// 初期化
bool VisorRain::Init()
{
    drops.clear();

    std::random_device rd;
    mt.seed(rd());

    // 水滴を70個生成
    for (int i = 0; i < 70; i++)
    {
        VisorDrop drop;

        ResetDrop(drop);

        drops.push_back(drop);
    }

    return true;
}


// 水滴をランダムな位置に発生させる
void VisorRain::ResetDrop(VisorDrop& drop)
{
    std::uniform_real_distribution<float> xDist(
        0.0f, static_cast<float>(SCREEN_W));

    std::uniform_real_distribution<float> yDist(
        0.0f, static_cast<float>(SCREEN_H));

    // 水滴を大きくする
    std::uniform_real_distribution<float> radiusDist(3.0f, 7.0f);

    std::uniform_real_distribution<float> speedDist(0.7f, 1.3f);

    // 左右へ流れる際の上下方向の速度
    std::uniform_real_distribution<float> verticalDist(-35.0f, -10.0f);

    std::uniform_real_distribution<float> stretchDist(1.0f, 2.0f);

    drop.x = xDist(mt);
    drop.y = yDist(mt);

    drop.radius = radiusDist(mt);
    drop.moveSpeed = speedDist(mt);

    drop.verticalSpeed = verticalDist(mt);

    if (drop.x < SCREEN_W / 2.0f)
    {
        drop.side = -1;
    }
    else
    {
        drop.side = 1;
    }

    drop.stretch = stretchDist(mt);

    drop.alpha = 150 + static_cast<int>(drop.radius * 12.0f);

    if (drop.alpha > 230)
        drop.alpha = 230;
}


// 更新
void VisorRain::Update(float deltaTime, float speed)
{
    // 走行速度に応じた水滴の移動速度
    float flowSpeed = 200.0f + speed * 20.0f;

    for (auto& drop : drops)
    {
        // 左右方向へ移動
        drop.x +=
            drop.side * flowSpeed * drop.moveSpeed * deltaTime;

        // 上方向へ流れる
        drop.y += drop.verticalSpeed * deltaTime;

        // 画面外に出たら再発生
        if (drop.x < 0.0f ||
            drop.x > SCREEN_W ||
            drop.y < 0.0f)
        {
            ResetDrop(drop);
        }
    }
}


// 描画
void VisorRain::Draw() const
{
    for (const auto& drop : drops)
    {
        int x = static_cast<int>(drop.x);
        int y = static_cast<int>(drop.y);

        int r = static_cast<int>(drop.radius);

        // 水滴本体
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, drop.alpha);

        DrawOval(
            x,
            y,
            r,
            r + 2,
            GetColor(180, 210, 230),
            TRUE
        );

        // 水滴の光の反射
        DrawCircle(
            x - 1,
            y - 1,
            1,
            GetColor(255, 255, 255),
            TRUE
        );

        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
}


// 終了処理
void VisorRain::Release()
{
    drops.clear();
}