#include "DialLockDoor.h"
#include "../../common/AnimationController.h"
#include "../../scene/SceneManager.h"

DialLockDoor::DialLockDoor(Vector3 pos, Vector3 angle, std::string name, int num,std::string unlockNum)
{
	pos_ = pos;
	angle_ = angle;
	name_ = name;
	num_ = num;
	unlockNum_ = unlockNum;
	Init();
}

DialLockDoor::~DialLockDoor()
{
}

void DialLockDoor::Init(void)
{

	//���f���p�X
	path_ = "./resource/model/gimmick/Door_Red2.mv1";
	
	//�A�j���[�V�����ǉ�
	animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(path_)[num_]);
	animController_->Add((int)ANIM_GIMMICK::OPEN, "./resource/model/gimmick/anim/DoorOpen.mv1", 50.0f);
	animController_->Add((int)ANIM_GIMMICK::CLOSE, "./resource/model/gimmick/anim/DoorClose.mv1", 50.0f);

	//���W��ݒ�
	MV1SetPosition(lpModelMng.GetID(path_)[num_], pos_.toVECTOR());

	//�g��l��ݒ�
	scale_ = { 12.0f,8.8f,10.0f };
	MV1SetScale(lpModelMng.GetID(path_)[num_], VECTOR(scale_.toVECTOR()));

	//�x�����烉�W�A���ɕϊ�
	angle_ = { Deg2RadF(angle_.x),Deg2RadF(angle_.y),Deg2RadF(angle_.z) };
	//��]
	MV1SetRotationXYZ(lpModelMng.GetID(path_)[num_], angle_.toVECTOR());
	state_ = ANIM_GIMMICK::CLOSE;

}

void DialLockDoor::Update(void)
{
	if (interactFlg_)
	{
		isPlayFlg_ = true;
		interactFlg_ = false;
	}
	Animation();
}

void DialLockDoor::Draw(void)
{
	//�`��
	MV1DrawModel(lpModelMng.GetID(path_)[num_]);
}

void DialLockDoor::Release(void)
{
}

int DialLockDoor::GetModelNumber(void)
{
	return num_;
}

std::string DialLockDoor::GetModelString(void)
{
	return path_;
}

void DialLockDoor::Animation(void)
{
	//�A�j���[�V�����̏�Ԃ��󂯎��
	ANIM_STATE animState = animController_->GetPlayState();

	//���b�N������
	if (!isLock_)
	{
		//�Đ���
		if (isPlayFlg_)
		{
			//�܂��Ă�����
			if (state_ == ANIM_GIMMICK::CLOSE)
			{
				//�J����
				animController_->Play((int)ANIM_GIMMICK::OPEN, false);
				//�A�j���[�V�������~�܂�����
				if (animState == ANIM_STATE::STOPPING)
				{
					//�J���Ă����Ԃɐ؂�ւ���
					state_ = ANIM_GIMMICK::OPEN;
					//�Đ��t���O���~�߂�
					isPlayFlg_ = false;
				}
			}
			else if (state_ == ANIM_GIMMICK::OPEN)	//�J���Ă�����
			{
				//����
				animController_->Play((int)ANIM_GIMMICK::CLOSE, false);
				//�A�j���[�V�������~�܂�����
				if (animState == ANIM_STATE::STOPPING)
				{
					//���Ă����Ԃɐ؂�ւ���
					state_ = ANIM_GIMMICK::CLOSE;
					//�Đ��t���O���~�߂�
					isPlayFlg_ = false;
				}
			}
		}
	}
	if (animState == ANIM_STATE::STOPPING)	//��~��
	{
		//�����蔻����X�V
		MV1RefreshCollInfo(lpModelMng.GetID(path_)[num_]);
	}
	//�A�j���[�V�����X�V����
	animController_->Update(lpSceneMng.GetDeltaTime());
}
