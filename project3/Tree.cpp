#include "Tree.h"

// 初期化
bool Tree::Init()
{
    // 木の画像を読み込む
    tree_image = LoadGraph(_T("data/tree.png"));

    if (tree_image == -1)
    {
        return false;
    }

    trees.clear();

    // 乱数生成器
    std::random_device rd;
    std::mt19937 mt(rd());

    // 道路幅に対する割合をランダムに決定
    std::uniform_real_distribution<float> offsetDist(0.0f, 1.0f);

    for (int i = 0; i < 50; i++)
    {
        TreeData tree;

        tree.depth = 0.05f + (i / 2) * 0.095f;

        tree.side = (i % 2 == 0) ? -1 : 1;

        // ここで一度だけランダム設定
        tree.sideOffset = offsetDist(mt);

        // 内側と外側で速度を分ける
        if (i < 30)
        {
            tree.speedRate = 1.0f;
        }
        else
        {
            tree.speedRate = 0.45f;
        }

        trees.push_back(tree);
    }

    return true;
}

// 更新
void Tree::Update(float deltaTime, float speed)
{
    // 木が手前に移動する速さ
    constexpr float TREE_SCROLL_SPEED = 0.08f;

    for (auto& tree : trees)
    {
        // 手前に来るほど移動速度を上げる
        float depthSpeed = 0.5f + tree.depth * 15.0f;

        tree.depth +=
            deltaTime * speed * TREE_SCROLL_SPEED * depthSpeed * tree.speedRate;

        // 手前まで来たら奥に戻す
        if (tree.depth > 1.0f)
        {
            tree.depth -= 1.0f;
        }
    }
}


// 描画
void Tree::Draw(float playerX) const
{
    if (tree_image == -1)
        return;

    // 奥から手前の順番で描画する
    for (float depth = 0.0f; depth <= 1.0f; depth += 0.01f)
    {
        for (const auto& tree : trees)
        {
            // 現在の奥行きと一致する木だけ描画
            if (tree.depth < depth || tree.depth >= depth + 0.01f)
                continue;

            float t = tree.depth;

            float y = HORIZON_Y +
                (SCREEN_H - HORIZON_Y) * t;

            float roadHalfWidth =
                ROAD_TOP_HALF +
                (ROAD_BOTTOM_HALF - ROAD_TOP_HALF) * t;

            float randomDistance = roadHalfWidth * tree.sideOffset;

            float x;

            // 内側の木
            if (tree.speedRate == 1.0f)
            {
                float roadShift = -playerX * 300.0f * t;

                x = SCREEN_W / 2.0f + roadShift
                    + tree.side * (roadHalfWidth + TREE_MARGIN + randomDistance);
            }
            // 外側の木
            else
            {
                float outerMargin = TREE_MARGIN + 100.0f;

                x = SCREEN_W / 2.0f
                    + tree.side * (roadHalfWidth + outerMargin + randomDistance);
            }

            float scale = 0.06f + t * 3.0f;

            int drawY = static_cast<int>(y - 150.0f * scale);

            // フェード処理
            constexpr float FADE_RANGE = 0.10f;

            float fadeIn = t / FADE_RANGE;
            float fadeOut = (1.0f - t) / FADE_RANGE;

            if (fadeIn < 0.0f) fadeIn = 0.0f;
            if (fadeIn > 1.0f) fadeIn = 1.0f;

            if (fadeOut < 0.0f) fadeOut = 0.0f;
            if (fadeOut > 1.0f) fadeOut = 1.0f;

            float alphaRate;

            if (fadeIn < fadeOut)
                alphaRate = fadeIn;
            else
                alphaRate = fadeOut;

            int alpha = static_cast<int>(alphaRate * 255.0f);

            if (alpha <= 0)
                continue;

            SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

            DrawRotaGraph(
                static_cast<int>(x),
                drawY,
                scale,
                0.0,
                tree_image,
                TRUE
            );

            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }
    }
}


// 終了処理
void Tree::Release()
{
    if (tree_image != -1)
    {
        DeleteGraph(tree_image);
        tree_image = -1;
    }

    trees.clear();
}