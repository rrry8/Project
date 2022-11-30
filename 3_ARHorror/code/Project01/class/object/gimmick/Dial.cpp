#include "Dial.h"
#include "../../common/AnimationController.h"

constexpr float STOP_ANGLE = 36.0f;
constexpr float MAX_ANGLE = 360.0f;
constexpr float ROT_SPEED = 2.0f;

Dial::Dial(Vector3 pos, Vector3 angle, std::string name, int num)
{
	pos_ = pos;
	angle_ = angle;
	name_ = name;
	num_ = num;
	Init();
}

Dial::~Dial()
{
}

void Dial::Init(void)
{
	//��~�p�x
	stopAngle_ = 0.0f;

	//���f���p�X
	path_ = "./resource/model/gimmick/Dial.mv1";

	//�ʒu�ݒ�
	MV1SetPosition(lpModelMng.GetID(path_)[num_],pos_.toVECTOR());

	//�g��l�ݒ�
	scale_ = {0.5f,0.5f,0.5f };
	MV1SetScale(lpModelMng.GetID(path_)[num_], VECTOR(scale_.toVECTOR()));

	//�x�����烉�W�A���ɕϊ�
	angle_ = { Deg2RadF(angle_.x),Deg2RadF(angle_.y),Deg2RadF(angle_.z) };

	//��]
	MV1SetRotationXYZ(lpModelMng.GetID(path_)[num_], angle_.toVECTOR());
}

void Dial::Update(void)
{
	//�C���^���N�g���ꂽ��
	if (interactFlg_)
	{
		//�A�j���[�V�����Đ�
		isPlayFlg_ = true;
		interactFlg_ = false;
	}

	//��~��
	if (!isPlayFlg_)
	{
		//��~�ʒu�����߂�
		stopAngle_ = angle_.x + Deg2RadF(STOP_ANGLE);

		//angle_��360�x�ɂȂ�����
		if (angle_.x >= Deg2RadF(MAX_ANGLE))
		{
			//angle_�̒l�����������Ȃ��悤0�ɖ߂�
			angle_.x = 0.0f;
		}
		//���݂̃_�C�����ԍ�(���݂̊p�x/��~�ʒu)
		dialNumber_ = static_cast<int>(angle_.x / Deg2RadF(STOP_ANGLE));
	}
	else	//�i�s��
	{
		//angle_����~�p�x��菬�����Ȃ�
		if(angle_.x <= stopAngle_)
		{
			//��]
			angle_.x += Deg2RadF(ROT_SPEED);
		}
		else	//��~�p�x�𒴂�����
		{
			//��~����
			isPlayFlg_ = false;
		}
 
	}
	//��]�X�V
	MV1SetRotationXYZ(lpModelMng.GetID(path_)[num_], angle_.toVECTOR());

}

//�`��
void Dial::Draw(void)
{
	MV1DrawModel(lpModelMng.GetID(path_)[num_]);
}

//�J��
void Dial::Release(void)
{
}
//���f���ԍ���Ԃ�
int Dial::GetModelNumber(void)
{
	return num_;
}
//���f�����������Ԃ�
std::string Dial::GetModelString(void)
{
	return path_;
}
