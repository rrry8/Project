#include "InnerLockDoor.h"
#include "../../common/AnimationController.h"
#include "../../Scene/SceneManager.h"
InnerLockDoor::InnerLockDoor(Vector3 pos, Vector3 angle, std::string name, int num,float normal)
{
	pos_ = pos;
	angle_ = angle;
	name_ = name;
	num_ = num;
	normal_ = normal;
	Init();
}

InnerLockDoor::~InnerLockDoor()
{
}

void InnerLockDoor::Init(void)
{
	if (name_ == "White_B_Double")
	{
		path_ = "./resource/model/gimmick/Door_White2_Double.mv1";
		//�A�j���[�V�����ǉ�
		animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(path_)[num_]);
		//animController_ = std::make_unique<AnimationController>(lpModelMng.GetID("./resource/model/gimmick/Door_Dark1_Double.mv1")[num_]);
		animController_->Add((int)ANIM_GIMMICK::OPEN, "./resource/model/gimmick/anim/DoorDoubleOpen.mv1", 40.0f);
		animController_->Add((int)ANIM_GIMMICK::CLOSE, "./resource/model/gimmick/anim/DoorDoubleClose.mv1", 40.0f);
		scale_ = { 10.7f,8.5f,10.0f };
	}
	else if(name_ == "White_B")
	{
		path_ = "./resource/model/gimmick/Door_White2.mv1";
		//�A�j���[�V�����ǉ�
		animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(path_)[num_]);
		//animController_ = std::make_unique<AnimationController>(lpModelMng.GetID("./resource/model/gimmick/Door_Dark1_Double.mv1")[num_]);
		animController_->Add((int)ANIM_GIMMICK::OPEN, "./resource/model/gimmick/anim/DoorOpen.mv1", 40.0f);
		animController_->Add((int)ANIM_GIMMICK::CLOSE, "./resource/model/gimmick/anim/DoorClose.mv1", 40.0f);
		scale_ = { 12.0f,8.8f,10.0f };
	}
	else if (name_ == "White_C")
	{
		path_ = "./resource/model/gimmick/Door_White3.mv1";
		//�A�j���[�V�����ǉ�
		animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(path_)[num_]);
		//animController_ = std::make_unique<AnimationController>(lpModelMng.GetID("./resource/model/gimmick/Door_Dark1_Double.mv1")[num_]);
		animController_->Add((int)ANIM_GIMMICK::OPEN, "./resource/model/gimmick/anim/DoorOpen.mv1", 40.0f);
		animController_->Add((int)ANIM_GIMMICK::CLOSE, "./resource/model/gimmick/anim/DoorClose.mv1", 40.0f);
		scale_ = { 12.0f,8.8f,10.0f };

	}

	//���W��ݒ�
	MV1SetPosition(lpModelMng.GetID(path_)[num_], pos_.toVECTOR());

	//�g��l��ݒ�
	MV1SetScale(lpModelMng.GetID(path_)[num_], VECTOR(scale_.toVECTOR()));

	//�x�����烉�W�A���ɕϊ�
	angle_ = { Deg2RadF(angle_.x),Deg2RadF(angle_.y),Deg2RadF(angle_.z) };
	//��]
	MV1SetRotationXYZ(lpModelMng.GetID(path_)[num_], angle_.toVECTOR());
	state_ = ANIM_GIMMICK::CLOSE;

}

void InnerLockDoor::Update(void)
{
	//�@���̒l����v���Ă�����
	if (hitNormal_.z == normal_)
	{
		//���b�N������
		isLock_ = false;
	}

	//���b�N��
	if (isLock_)
	{
		//�C���^���N�g�͍s��Ȃ�
		interactFlg_ = false;
	}
	else
	{
		//�C���^���N�g���ꂽ��
		if (interactFlg_)
		{
			//�A�j���[�V�����Đ�
			isPlayFlg_ = true;
			interactFlg_ = false;
		}
	}
	//�A�j���[�V��������
	Animation();
}

void InnerLockDoor::Draw(void)
{
	//�`��
	MV1DrawModel(lpModelMng.GetID(path_)[num_]);
}

void InnerLockDoor::Release(void)
{
}

int InnerLockDoor::GetModelNumber(void)
{
    return num_;
}

std::string InnerLockDoor::GetModelString(void)
{
    return path_;
}

void InnerLockDoor::Animation(void)
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
