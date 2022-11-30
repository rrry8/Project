#include<DxLib.h>
#include "MouseIcon.h"
#include"../common/manager/ImageMng.h"
#include"../scene/SceneManager.h"

MouseIcon::MouseIcon()
{
	Init();
}

MouseIcon::~MouseIcon()
{
	Release();
}

void MouseIcon::Init(void)
{
	sceneID_ = SceneID::Title;
}

void MouseIcon::Release(void)
{
	//DeleteGraph(mouseIconHandle_);
}

void MouseIcon::Update(SceneID id)
{
	sceneID_ = id;
}

void MouseIcon::Draw(void)
{


	if (sceneID_ == SceneID::Game)
	{
		if (flg_)
		{
			DrawRotaGraph(lpSceneMng.GetScreenSize().x / 2, lpSceneMng.GetScreenSize().y / 2, 1.0f, 0.0f, lpImageMng.GetID("./resource/image/mouse/Getreticle.png")[0], true);
		}
		else
		{
			DrawRotaGraph(lpSceneMng.GetScreenSize().x / 2, lpSceneMng.GetScreenSize().y / 2, 1.0f, 0.0f, lpImageMng.GetID("./resource/image/mouse/reticle.png")[0], true);
		}
		//DrawRotaGraph(lpSceneMng.GetScreenSize().x / 2, lpSceneMng.GetScreenSize().y / 2, 1.0f, 0.0f, lpImageMng.GetID("./resource/image/mouse/black.png")[0], true);
	}
	else
	{
		int x, y;
		GetMousePoint(&x, &y);
		DrawRotaGraph(x, y, 1.0f, 0.0f, lpImageMng.GetID("./resource/image/mouse/mouse.png")[0], true);
	}
}

