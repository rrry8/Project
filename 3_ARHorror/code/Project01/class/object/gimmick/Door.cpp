#include "Door.h"
#include "../../common/AnimationController.h"
#include "../../Scene/SceneManager.h"

using namespace std;

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="���W"></param>
/// <param name="�p�x"></param>
/// <param name="�I�u�W�F�N�g��"></param>
/// <param name="������"></param>
Door::Door(Vector3 pos, Vector3 angle, std::string name, int num)
{
	pos_ = pos;
	angle_ = angle;
	name_ = name;
	num_ = num;
	Init();
}

Door::~Door()
{
}

void Door::Init(void)
{
	if (name_ == "Dark_A_Double")
	{
		path_ = "./resource/model/gimmick/Door_Dark1_Double.mv1";

		//�A�j���[�V�����ǉ�
		animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(path_)[num_]);
		animController_->Add((int)ANIM_GIMMICK::OPEN, "./resource/model/gimmick/anim/DoorDoubleOpen.mv1", 50.0f);
		animController_->Add((int)ANIM_GIMMICK::CLOSE, "./resource/model/gimmick/anim/DoorDoubleClose.mv1", 50.0f);
		scale_ = { 10.7f,8.5f,10.0f };
	}
	else
	{
		path_ = "./resource/model/gimmick/Door_Dark1.mv1";

		//�A�j���[�V�����ǉ�
		animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(path_)[num_]);
		animController_->Add((int)ANIM_GIMMICK::OPEN, "./resource/model/gimmick/anim/DoorOpen.mv1", 50.0f);
		animController_->Add((int)ANIM_GIMMICK::CLOSE, "./resource/model/gimmick/anim/DoorClose.mv1", 50.0f);
		scale_ = { 12.0f,8.8f,10.0f };
	}
	
	//���W��ݒ�
	MV1SetPosition(lpModelMng.GetID(path_)[num_],pos_.toVECTOR());
	
	//�g��l��ݒ�
	MV1SetScale(lpModelMng.GetID(path_)[num_], VECTOR(scale_.toVECTOR()));
	
	//�x�����烉�W�A���ɕϊ�
	angle_ = { Deg2RadF(angle_.x),Deg2RadF(angle_.y),Deg2RadF(angle_.z) };
	//��]
	MV1SetRotationXYZ(lpModelMng.GetID(path_)[num_], angle_.toVECTOR());
	state_ = ANIM_GIMMICK::CLOSE;
}

void Door::Update(void)
{
	if (interactFlg_)
	{
		isPlayFlg_ = true;
		interactFlg_ = false;
	}
	//�A�j���[�V��������
	Animation();
}

void Door::Draw(void)
{
	//�`��
	MV1DrawModel(lpModelMng.GetID(path_)[num_]);
	//MV1DrawModelDebug(lpModelMng.GetID("./resource/model/gimmick/Door_White.mv1")[num_],0xffffff,1,0.001,1,1);
}

void Door::Release(void)
{
	//���
	//MV1DeleteModel(lpModelMng.GetID("./resource/model/gimmick/DoorDouble.mv1")[num_]);
}

int Door::GetModelNumber(void)
{
	return num_;
}

std::string Door::GetModelString(void)
{
	return path_;
}

void Door::Animation(void)
{
	//�A�j���[�V�����̏�Ԃ��󂯎��
	ANIM_STATE animState = animController_->GetPlayState();

	//�Đ���
	if (isPlayFlg_)
	{
		//���Ă���Ƃ�
		if (state_ == ANIM_GIMMICK::CLOSE)
		{
			//�J����
			animController_->Play((int)ANIM_GIMMICK::OPEN, false);
			//�A�j���[�V��������~������
			if (animState == ANIM_STATE::STOPPING)
			{
				//�J���Ă����Ԃɐ؂�ւ���
				state_ = ANIM_GIMMICK::OPEN;
				//�Đ��t���O�����~�߂�
				isPlayFlg_ = false;
			}
		}
		else if (state_ == ANIM_GIMMICK::OPEN)	//���Ă���Ƃ�
		{
			//����
			animController_->Play((int)ANIM_GIMMICK::CLOSE, false);
			//�A�j���[�V��������~������
			if (animState == ANIM_STATE::STOPPING)
			{
				//�J���Ă����Ԃɐ؂�ւ���
				state_ = ANIM_GIMMICK::CLOSE;
				//�Đ��t���O�����~�߂�
				isPlayFlg_ = false;
			}
		}
	}
	if (animState == ANIM_STATE::STOPPING)
	{
		//�����蔻����X�V
		MV1RefreshCollInfo(lpModelMng.GetID(path_)[num_]);
	}
	//�A�j���[�V�����X�V����
	animController_->Update(lpSceneMng.GetDeltaTime());
}


