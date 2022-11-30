#include <DxLib.h>
#include "Drawer.h"
#include "../../scene/SceneManager.h"
#include "../../common/AnimationController.h"
#include "../../scene/SceneManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="���W"></param>
/// <param name="�p�x"></param>
/// <param name="�I�u�W�F�N�g��"></param>
/// <param name="������"></param>
Drawer::Drawer(Vector3 pos, Vector3 angle, std::string name, int num)
{
	pos_ = pos;
	angle_ = angle;
	name_ = name;
	num_ = num;
	Init();
}

Drawer::~Drawer()
{
}

//������
void Drawer::Init(void)
{
	path_ = "./resource/model/gimmick/drawer.mv1";
	//�A�j���[�V�����ǉ�
	animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(path_)[num_]);
	animController_->Add((int)ANIM_GIMMICK::OPEN, "./resource/model/gimmick/anim/DrawerOpen.mv1", 20.0f);
	animController_->Add((int)ANIM_GIMMICK::CLOSE, "./resource/model/gimmick/anim/DrawerClose.mv1", 20.0f);
	
	//���W��ݒ�
	MV1SetPosition(lpModelMng.GetID(path_)[num_], pos_.toVECTOR());
	
	//�g��l��ݒ�
	scale_ = { 10.0f,10.0f,10.0f };
	MV1SetScale(lpModelMng.GetID(path_)[num_], VECTOR(scale_.toVECTOR()));
	
	//�x�����烉�W�A���ɕϊ�
	angle_ = { Deg2RadF(angle_.x),Deg2RadF(angle_.y),Deg2RadF(angle_.z) };
	
	//��]
	MV1SetRotationXYZ(lpModelMng.GetID(path_)[num_], angle_.toVECTOR());

	state_ = ANIM_GIMMICK::CLOSE;

}

//�X�V
void Drawer::Update(void)
{
	//�C���^���N�g���ꂽ��
	if (interactFlg_)
	{
		//�Đ�����
		isPlayFlg_ = true;
		interactFlg_ = false;
	}
	//�A�j���[�V��������
	Animation();
	//�����蔻����X�V
	MV1RefreshCollInfo(lpModelMng.GetID(path_)[num_]);
}

//�`��
void Drawer::Draw(void)
{
	//�`��
	MV1DrawModel(lpModelMng.GetID(path_)[num_]);
}

//�J��
void Drawer::Release(void)
{
	//MV1DeleteModel(lpModelMng.GetID("./resource/model/gimmick/drawer.mv1")[num_]);
}

int Drawer::GetModelNumber(void)
{
	return num_;
}

std::string Drawer::GetModelString(void)
{
	return path_;
}

void Drawer::Animation(void)
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
			if (animState ==ANIM_STATE::STOPPING)
			{
				//�J���Ă����Ԃɐ؂�ւ���
				state_ = ANIM_GIMMICK::CLOSE;
				//�Đ��t���O�����~�߂�
				isPlayFlg_ = false;
			}
		}
	}

	//�A�j���[�V�����X�V����
	animController_->Update(lpSceneMng.GetDeltaTime());
}
