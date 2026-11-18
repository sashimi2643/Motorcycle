#pragma once
#include "DxLib.h"
#include <cmath>

class Title
{
private:
    int title_image;

    int FontHandle = CreateFontToHandle(NULL, 50, 3);
public:
    bool menu;
    bool operate;
	bool setting;
    // ‰Šú‰»
    void Init();

    // •`‰æ
    void Draw(int selectedMode);
};

