#pragma once

#include "DxLib.h"
#include <vector>
#include <random>

// …“H1‚Â•ª‚Ìî•ñ
struct VisorDrop
{
    float x;
    float y;

    float radius;       // …“H‚Ì‘å‚«‚³
    float moveSpeed;    // ˆÚ“®‘¬“x
    float verticalSpeed;// ã‰º•ûŒü‚Ì‚í‚¸‚©‚ÈˆÚ“®

    int side;           // -1F¶A1F‰E
    int alpha;          // “§–¾“x

    float stretch; // …“H‚ÌL‚Ñ‹ï‡
};

class VisorRain
{
private:

    std::vector<VisorDrop> drops;

    std::mt19937 mt;

    // …“H‚ğ’†‰›‚É–ß‚·
    void ResetDrop(VisorDrop& drop);

public:

    static constexpr int SCREEN_W = 1280;
    static constexpr int SCREEN_H = 720;

    bool Init();

    void Update(float deltaTime, float speed);

    void Draw() const;

    void Release();
};