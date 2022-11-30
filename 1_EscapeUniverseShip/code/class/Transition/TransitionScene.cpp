#include<DxLib.h>
#include "TransitionScene.h"
#include"../SceneManager.h"
#include"../../_debug/_DebugConOut.h"


TransitionScene::TransitionScene(UniqueScene beforeScene, UniqueScene afterScene)
{
	beforeScene_ = std::move(beforeScene);
	afterScene_ = std::move(afterScene);
	TRACE("TransitionScene‚Ì¶¬\n");
	count = 0.0;
	count2 = 0.0;
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	beforeScene_->Draw();
}

TransitionScene::~TransitionScene()
{
}

void TransitionScene::Init(void)
{

}

UniqueScene TransitionScene::Update(UniqueScene ownScene)
{
	//if (++count > 100)
	//{
	//	TRACE("ŽŸ‚ÌƒV[ƒ“‚ÉˆÚs");
	//	return std::move(afterScene_);
	//}
	//return ownScene;
	
	//count += delta;
	if (UpdateTransition(0.016))
	{
		afterScene_->DrawScreen();
		return std::move(afterScene_);
	}
	DrawScreen();
	return ownScene;
}

void TransitionScene::Release(void)
{
}

