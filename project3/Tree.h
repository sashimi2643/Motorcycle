#pragma once

#include "DxLib.h"
#include <vector>
#include <random>

// 木1本分の情報
struct TreeData
{
    float depth; // 奥行き（0.0f = 遠く、1.0f = 手前）
    int side;    // -1 = 左側、1 = 右側
    float sideOffset;  // 道路幅に対するランダムな割合
    
    float speedRate; // 木ごとの移動速度倍率
};

class Tree
{
private:
    int tree_image = -1;

    std::vector<TreeData> trees;

    static constexpr int SCREEN_W = 1280;
    static constexpr int SCREEN_H = 720;

    static constexpr float HORIZON_Y = 360.0f;

    // 道路の幅（Roadクラスの描画設定に合わせる）
    static constexpr float ROAD_TOP_HALF = 8.0f;
    static constexpr float ROAD_BOTTOM_HALF = 620.0f;

    // 木と道路の間隔
    static constexpr float TREE_MARGIN = 35.0f;

public:
    bool Init();
    void Update(float deltaTime, float speed);
    void Draw(float playerX) const;
    void Release();
};
