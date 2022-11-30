#include "LockDoor.h"
#include "../../common/AnimationController.h"
#include "../../scene/SceneManager.h"
#include "../Item/Item.h"

LockDoor::LockDoor(Vector3 pos, Vector3 angle, std::string name, int num,ItemID itemID)
{
	pos_ = pos;
	angle_ = angle;
	name_ = name;
	num_ = num;
	itemID_ = itemID;
	Init();
}

LockDoor::~LockDoor()
{
}

void LockDoor::Init(void)
{
	//�I�u�W�F�N�g�����`�F�b�N���g�p���郂�f���p�X���w��
	CheckName();

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

void LockDoor::Update(void)
{
	//�C���^���N�g���ꂽ��
	if (interactFlg_)
	{
		if (!isLock_)
		{
			//�A�j���[�V�����Đ�
			isPlayFlg_ = true;
			interactFlg_ = false;
		}
	}
	//�A�j���[�V��������
	Animation();
}

void LockDoor::Draw(void)
{
	//�`��
	MV1DrawModel(lpModelMng.GetID(path_)[num_]);
}

void LockDoor::Release(void)
{
	//���
	//MV1DeleteModel(lpModelMng.GetID("./resource/model/gimmick/LockDoor.mv1")[num_]);
}

int LockDoor::GetModelNumber(void)
{
	return num_;
}

std::string LockDoor::GetModelString(void)
{
	return path_;
}


void LockDoor::Animation(void)
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

void LockDoor::CheckName(void)
{
	std::string directory = "./resource/model/gimmick/";

	if (name_ == "White_A_Double")
	{
		path_ = directory + "Door_White1_Double.mv1";
		//�A�j���[�V�����ǉ�
		animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(path_)[num_]);
		animController_->Add((int)ANIM_GIMMICK::OPEN, directory +"anim/DoorDoubleOpen.mv1", 40.0f);
		animController_->Add((int)ANIM_GIMMICK::CLOSE, directory + "anim/DoorDoubleClose.mv1", 40.0f);
		//�g��l��ݒ�
		scale_ = { 10.7f,8.5f,10.0f };
	}
	else if (name_ == "Red_A")
	{
		path_ = directory + "Door_Red1.mv1";
		//�A�j���[�V�����ǉ�
		animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(path_)[num_]);
		animController_->Add((int)ANIM_GIMMICK::OPEN, directory + "anim/DoorOpen.mv1", 40.0f);
		animController_->Add((int)ANIM_GIMMICK::CLOSE, directory + "anim/DoorClose.mv1", 40.0f);
		//�g��l��ݒ�
		scale_ = { 12.0f,8.8f,10.0f };
	}
	else if (name_ == "Dark_B")
	{
		path_ = directory + "Door_Dark2.mv1";
		//�A�j���[�V�����ǉ�
		animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(path_)[num_]);
		animController_->Add((int)ANIM_GIMMICK::OPEN, directory + "anim/DoorOpen.mv1", 40.0f);
		animController_->Add((int)ANIM_GIMMICK::CLOSE, directory + "anim/DoorClose.mv1", 40.0f);
		//�g��l��ݒ�
		scale_ = { 12.0f,8.8f,10.0f };
	}
	else if (name_ == "Dark_B_Double")
	{
		path_ = directory + "Door_Dark2_Double.mv1";
		//�A�j���[�V�����ǉ�
		animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(path_)[num_]);
		animController_->Add((int)ANIM_GIMMICK::OPEN, directory + "anim/DoorDoubleOpen.mv1", 40.0f);
		animController_->Add((int)ANIM_GIMMICK::CLOSE, directory + "anim/DoorDoubleClose.mv1", 40.0f);
		//�g��l��ݒ�
		scale_ = { 10.7f,8.5f,10.0f };
	}
	else if (name_ == "Dark_A")
	{
		path_ = directory + "Door_Dark1.mv1";
		//�A�j���[�V�����ǉ�
		animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(path_)[num_]);
		animController_->Add((int)ANIM_GIMMICK::OPEN, directory + "anim/DoorOpen.mv1", 40.0f);
		animController_->Add((int)ANIM_GIMMICK::CLOSE, directory + "anim/DoorClose.mv1", 40.0f);
		//�g��l��ݒ�
		scale_ = { 12.0f,8.8f,10.0f };
	}

}
