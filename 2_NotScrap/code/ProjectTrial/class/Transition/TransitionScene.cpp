#include <DxLib.h>
#include "TransitionScene.h"
#include "../../_debug/_DebugConOut.h"
#include "../../_debug/_DebugDispOut.h"

TransitionScene::TransitionScene(uniqueScene beforScene, uniqueScene afterScene)
{
	// �����I�ɃL���X�g����K�v������
	beforScene_ = std::move(beforScene);
	afterScene_ = std::move(afterScene);
	//TRACE("TransitionScene�̐���\n");
	count = 0;
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	beforScene_->Draw(0);
}

TransitionScene::TransitionScene(uniqueScene afterScene)
{
	// �����I�ɃL���X�g����K�v������
	afterScene_ = std::move(afterScene);
	//TRACE("TransitionScene�̐���\n");
	count = 0;
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	afterScene_->Draw(0);
}

TransitionScene::~TransitionScene()
{
}

bool TransitionScene::Init(void)
{
	return false;
}

uniqueScene TransitionScene::Update(double delta, uniqueScene ownScene)
{
	if (UpdateTransition(delta))
	{
		//TRACE("���̃V�[���Ɉڍs\n");
		return std::move(afterScene_);
	}
	DrawOwnScreen(delta);
	return ownScene;
}




