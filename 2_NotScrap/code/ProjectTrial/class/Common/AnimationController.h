#pragma once
#include <string>
#include <map>

class SceneManager;
enum class ANIM_TYPE
{
	IDLE,
	RUN,
	JUMP,
	FALL,
	DEAD,
	WAIT,
	MAX
};

class AnimationController
{
	
public :

	struct Animation
	{
		int model;
		int attachNo;
		int animIndex;
		float speed;
		float totalTime;
		float step = 0.0f;
	};

	AnimationController(int modelId);
	~AnimationController();
	//AnimationController(SceneManager* manager, int modelId);

	// アニメーション追加
	void Add(int type, std::string path, float speed);
	void Add(int type, int handle, float speed);

	// アニメーション再生
	void Play(int type, bool isLoop = true, 
		float startStep = 0.0f, float endStep = -1.0f, bool isStop = false, bool isForce = false);

	void Update(double delta);
	void Release(void);

	// アニメーション終了後に繰り返すループステップ
	void SetEndLoop(float startStep, float endStep, float speed);

	// 再生中のアニメーション
	int GetPlayType(void);

	// 再生終了
	bool IsEnd(void);

private:


	int mModelId;
	std::map<int, Animation> mAnimations;

	int mPlayType;
	Animation mPlayAnim;

	// アニメーションをループするかしないか
	bool mIsLoop;

	// アニメーションを止めたままにする
	bool mIsStop;

	// アニメーション終了後に繰り返すループステップ
	float mStepEndLoopStart;
	float mStepEndLoopEnd;
	float mEndLoopSpeed;
	// 逆再生
	float mSwitchLoopReverse;

};

