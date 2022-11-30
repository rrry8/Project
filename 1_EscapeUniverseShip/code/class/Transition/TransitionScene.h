#pragma once
#include "../BaseScene.h"

//•’Ê‚Ì‘å‚«‚³‚Ì‚Ìƒ^ƒCƒ„‚Ì‰ÁZ’l
constexpr float NormalAddWheelRX = 36.0f;
constexpr float NormalAddWheelLX = -45.0f;
constexpr float NormalAddWheelY = 19.0f;

enum class Transition
{
	CrossFade,
	FadeInOut,
	MAX
};

class TransitionScene :
	public BaseScene
{
public:
	TransitionScene(UniqueScene beforeScene,UniqueScene afterScene);
	~TransitionScene();

private:
	void Init(void) override;
	UniqueScene Update(UniqueScene ownScene) override;
	virtual bool UpdateTransition(double delta) = 0;
	void Release(void) override;
protected:
	//Scene GetScene(void) override final { return Scene::Transition; };

	UniqueScene beforeScene_;
	UniqueScene afterScene_;

	double count;
	double count2;
};

