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

    // 本数を指定
    constexpr int INNER_TREE_COUNT = 30;
    constexpr int OUTER_TREE_COUNT = 40;

    // 内側の木を生成
    for (int i = 0; i < INNER_TREE_COUNT; i++)
    {
        TreeData tree;

        tree.depth = 0.05f + (i / 2) * 0.095f;
        tree.side = (i % 2 == 0) ? -1 : 1;
        tree.sideOffset = offsetDist(mt);

        tree.isOuter = false;

        trees.push_back(tree);
    }

    // 外側の木を生成
    for (int i = 0; i < OUTER_TREE_COUNT; i++)
    {
        TreeData tree;

        tree.depth = 0.05f + (i / 2) * 0.095f;
        tree.side = (i % 2 == 0) ? -1 : 1;
        tree.sideOffset = offsetDist(mt);

        tree.isOuter = true;

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

        // 内側・外側ともに同じ速度で移動
        tree.depth += deltaTime
            * speed
            * TREE_SCROLL_SPEED
            * depthSpeed;

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

            // 道路の幅に対するランダムな距離
            float randomDistance = roadHalfWidth * tree.sideOffset;

            // 道路の移動に合わせて木も横に移動
            float roadShift = -playerX * 300.0f * t;

            // 木の大きさ
            float scale = 0.06f + t * 3.0f;

            // 内側・外側で共通の横移動量を使用
            float x = SCREEN_W / 2.0f + roadShift;

            if (tree.isOuter)
            {
                // 外側の木
                // 内側の木が存在する範囲よりも外側に配置する
                float outerDistance =
                    roadHalfWidth
                    + TREE_MARGIN
                    + roadHalfWidth
                    + 120.0f
                    + scale * 80.0f;

                x += tree.side * (outerDistance + randomDistance);
            }
            else
            {
                // 内側の木
                x += tree.side * (roadHalfWidth + TREE_MARGIN + randomDistance);
            }


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
                (int)x,
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