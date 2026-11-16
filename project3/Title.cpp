#include "Title.h"

// 初期化
void Title::Init()
{
    title_image = LoadGraph(_T("data/title.png"));
    menu = false;
}

// 更新
void Title::Update()
{
    
}

// 描画
void Title::Draw()
{
    DrawGraph(0, 0, title_image, FALSE);
    if (operate) {

    }
    else {
        if (menu) {
            DrawStringToHandle(420, 500, _T("１.レーススタート！"), GetColor(255, 255, 255), FontHandle);
			DrawStringToHandle(420, 550, _T("２.操作方法"), GetColor(255, 255, 255), FontHandle);
            DrawStringToHandle(420, 600, _T("３.ゲーム終了"), GetColor(255, 255, 255), FontHandle);
        }
        else {
            DrawStringToHandle(510, 600, _T("PUSH SPACE"), GetColor(255, 255, 255), FontHandle);
        }
    }
}