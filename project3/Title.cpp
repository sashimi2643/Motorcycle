#include "Title.h"

// 初期化
void Title::Init()
{
    title_image = LoadGraph(_T("data/title.png"));
    menu = false;
	operate = false;
    setting = false;
}

// 描画
void Title::Draw(int selectedMode)
{
    DrawGraph(0, 0, title_image, FALSE);
    if (!setting) {
        if (!operate) {
            if (menu) {
                DrawStringToHandle(420, 500, _T("１.レーススタート！"), GetColor(255, 255, 255), FontHandle);
                DrawStringToHandle(420, 550, _T("２.操作方法"), GetColor(255, 255, 255), FontHandle);
                DrawStringToHandle(420, 600, _T("３.ゲーム設定"), GetColor(255, 255, 255), FontHandle);
                DrawStringToHandle(420, 650, _T("４.ゲーム終了"), GetColor(255, 255, 255), FontHandle);
            }else {
                DrawStringToHandle(510, 600, _T("PUSH SPACE"), GetColor(255, 255, 255), FontHandle);
            }
        }else {
            DrawStringToHandle(70, 450, _T("ー操作方法ー"), GetColor(255, 255, 255), FontHandle);
        }
    }else {
        if (selectedMode == 0) { DrawStringToHandle(70, 450, _T("ーゲーム設定ー(現在:イージーモード)"), GetColor(255, 255, 255), FontHandle); }
        else { DrawStringToHandle(70, 450, _T("ーゲーム設定ー(現在:リアルモード)"), GetColor(255, 255, 255), FontHandle); }
        DrawStringToHandle(70, 500, _T("１.ゲームモード:気軽に遊べるイージーモード"), GetColor(255, 255, 255), FontHandle);
        DrawStringToHandle(70, 550, _T("２.リアルモード:油断はできない！？ハードモード"), GetColor(255, 255, 255), FontHandle);
        DrawStringToHandle(70, 600, _T("３.タイトルに戻る"), GetColor(255, 255, 255), FontHandle);
    }
}