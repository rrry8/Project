#include<DxLib.h>
#include "FadeInOut.h"



FadeInOut::FadeInOut(double LimitTime, UniqueScene beforeScene, UniqueScene afterScene):
	TransitionScene(std::move(beforeScene), std::move(afterScene))
{
	LimitTime_ = LimitTime;
	DrawScreen();
}

FadeInOut::~FadeInOut()
{
}

bool FadeInOut::UpdateTransition(double delta)
{
	count += delta;
	if (count >= LimitTime_)
	{
		return true;
	}
	//DrawOwnScreen(delta);
	return false;
}

void FadeInOut::DrawScreen()
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();

	auto alpha = static_cast<int>(255.0 * count / LimitTime_) * 2;
	if (count < LimitTime_ / 2)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - alpha);
		beforeScene_->Draw();
	}
	else
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha - 255);
		afterScene_->Draw();
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

