#include<DxLib.h>
#include "SelectScene.h"

SelectScene::SelectScene()
{
    Init();
    DrawScreen();
}

SelectScene::~SelectScene()
{
}

void SelectScene::Init()
{
}

UniqueScene SelectScene::Update(UniqueScene scene)
{
    DrawScreen();
    return scene;
}

void SelectScene::DrawScreen(void)
{
    SetDrawScreen(screenID_);
    ClsDrawScreen();
    DrawFormatString(0, 0, 0xffffff, "SelectScene");

}

void SelectScene::Release(void)
{
}
