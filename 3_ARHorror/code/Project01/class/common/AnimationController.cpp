#include <DxLib.h>
#include "AnimationController.h"

AnimationController::AnimationController(int modelId)
{
	model_ = modelId;
	animType_ = -1;
	mIsLoop = false;
	mIsStop = false;
	playType = PLAY_TYPE::NORMAL;
	animState_ = ANIM_STATE::STOPPING;
}

AnimationController::~AnimationController()
{
	playAnim_.attachNo = MV1DetachAnim(model_, playAnim_.attachNo);
}

void AnimationController::Add(int type, std::string path, float speed)
{
	Animation anim;

	anim.model = MV1LoadModel(path.c_str());
	anim.attachNo = type;
	anim.speed = speed;

	if (animations_.count(type) == 0)
	{
		// �ǉ�
		animations_.emplace(type, anim);
	}
	else
	{
		// ����ւ�
		animations_[type].model = anim.model;
		animations_[type].attachNo = anim.attachNo;
		animations_[type].totalTime = anim.totalTime;
	}
}

void AnimationController::Add(int type, int handle, float speed)
{
	Animation anim;
	//�n���h��
	anim.model = handle;
	//�A�j���[�V�������
	anim.attachNo = type;
	//�Đ����x
	anim.speed = speed;

	if (animations_.count(type) == 0)
	{
		// �ǉ�
		animations_.emplace(type, anim);
	}
	else
	{
		// ����ւ�
		animations_[type].model = anim.model;
		animations_[type].attachNo = anim.attachNo;
		animations_[type].totalTime = anim.totalTime;
	}
}

void AnimationController::Play(int type, bool isLoop)
{
	if (animType_ != type) {

		if (animType_ != -1)
		{
			// ���f������A�j���[�V�������O��
			playAnim_.attachNo = MV1DetachAnim(model_, playAnim_.attachNo);
		}

		// �A�j���[�V������ʂ�ύX
		animType_ = type;
		playAnim_ = animations_[type];

		// ������
		playAnim_.frame = 0.0f;

		// ���f���ɃA�j���[�V������t����
		playAnim_.attachNo = MV1AttachAnim(model_, 0, playAnim_.model);

		// �A�j���[�V���������Ԃ̎擾
		playAnim_.totalTime = MV1GetAttachAnimTotalTime(model_, playAnim_.attachNo);
		
		//�Đ����@
		playType = PLAY_TYPE::NORMAL;

		// �A�j���[�V�������[�v
		mIsLoop = isLoop;
	}
}

void AnimationController::Play(int type, int num, bool isLoop)
{
	if (animType_ != type) {

		if (animType_ != -1)
		{
			// ���f������A�j���[�V�������O��
			playAnim_.attachNo = MV1DetachAnim(model_, playAnim_.attachNo);
		}

		// �A�j���[�V������ʂ�ύX
		animType_ = type;
		playAnim_ = animations_[type];

		// ������
		playAnim_.frame = 0.0f;

		// ���f���ɃA�j���[�V������t����
		playAnim_.attachNo = MV1AttachAnim(model_, num, playAnim_.model);

		// �A�j���[�V���������Ԃ̎擾
		playAnim_.totalTime = MV1GetAttachAnimTotalTime(model_, playAnim_.attachNo);

		//�Đ����@
		playType = PLAY_TYPE::NORMAL;

		// �A�j���[�V�������[�v
		mIsLoop = isLoop;
	}
}

//�t�Đ�
void AnimationController::PlayBack(int type, bool isLoop)
{
	if (animType_ != type) {

		if (animType_ != -1)
		{
			// ���f������A�j���[�V�������O��
			playAnim_.attachNo = MV1DetachAnim(model_, playAnim_.attachNo);
		}

		// �A�j���[�V������ʂ�ύX
		animType_ = type;
		playAnim_ = animations_[type];

		// ���f���ɃA�j���[�V������t����
		playAnim_.attachNo = MV1AttachAnim(model_, type, playAnim_.model);

		// �A�j���[�V���������Ԃ̎擾
		playAnim_.totalTime = MV1GetAttachAnimTotalTime(model_, playAnim_.attachNo);
		
		// ������
		//���t�Đ��Ȃ̂Ńt���[�������ő�l��
		playAnim_.frame = playAnim_.totalTime;


		//�Đ����@
		playType = PLAY_TYPE::REVERSE;

		// �A�j���[�V�������[�v
		mIsLoop = isLoop;
	}
}

void AnimationController::Update(double delta)
{
	// �A�j���[�V�����I������
	bool isEnd = false;

	if (!mIsStop)
	{
		// �ʏ�Đ��̏ꍇ
		if (playType == PLAY_TYPE::NORMAL)
		{
			animState_ = ANIM_STATE::PLAYING;
			playAnim_.frame += (delta * playAnim_.speed);

			if (playAnim_.frame > playAnim_.totalTime)
			{
				isEnd = true;
			}
			if (isEnd)
			{
				// �A�j���[�V�������I��������
				if (mIsLoop)
				{
					// ���[�v�Đ�
					playAnim_.frame = 0.0f;
				}
				else
				{
					// ���[�v���Ȃ�
					playAnim_.frame = playAnim_.totalTime;
					animState_ = ANIM_STATE::STOPPING;
				}
			}

		}
		// �t�Đ��̏ꍇ
		else if (playType == PLAY_TYPE::REVERSE)
		{
			playAnim_.frame -= (delta * playAnim_.speed);

			if (playAnim_.frame < 0.0f)
			{
				isEnd = true;
			}
			if (isEnd)
			{
				// �A�j���[�V�������I��������
				if (mIsLoop)
				{
					// ���[�v�Đ�
					playAnim_.frame = playAnim_.totalTime;
				}
				else
				{
					// ���[�v���Ȃ�
					playAnim_.frame = 0.0f;
				}
			}
		}
	}

	// �A�j���[�V�����ݒ�
	MV1SetAttachAnimTime(model_, playAnim_.attachNo, playAnim_.frame);
}


void AnimationController::Release(void)
{
	for (auto& anim : animations_)
	{
		MV1DeleteModel(anim.second.model);
	}
}


ANIM_STATE AnimationController::GetPlayState(void)
{
	return animState_;
}


