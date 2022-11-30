#include<DxLib.h>
#include"../Scene/SceneManager.h"
#include "Camera.h"

//�����_����J�����܂ł̋���
constexpr float TARGET2CAMERA_DISTANCE = 500.0f;

//X����]���̐����p�x
constexpr float UP_RADIAN_LIMIT_X  = -70.0f * (DX_PI_F / 180.0f);	//�����
constexpr float DOWN_RADIAN_LIMIT_X = 60.0f * (DX_PI_F / 180.0f);	//������ 60

//�J�����̕`�拗��
constexpr float CAMERA_RANGE_NEAR = 50.0f;		//��O
constexpr float CAMERA_RANGE_FAR  = 25000.0f;	//��

//��ʗh��
constexpr float SWING_WIDTH = 0.125f * (DX_PI_F / 180.0f);
constexpr float SWING_OFFSET = 3.5f;

Camera::Camera()
{
	Init();
}

Camera::~Camera()
{
}

void Camera::Init(void)
{
	//�ʒu
	pos_ = { 0.0f,CAMERA_HEIGHT,-TARGET2CAMERA_DISTANCE };

	//�p�x
	angle_ = {
		0.0f,
		0.0f,
		0.0f
	};
	quaRot_ = Quaternion();

	sinCnt_ = 0;

	// �N���b�v�����̐ݒ�
	SetCameraNearFar(CAMERA_RANGE_NEAR, CAMERA_RANGE_FAR);

	//�}�E�X�̈ʒu��^�񒆂ɌŒ�
	SetMousePoint(lpSceneMng.GetScreenSize().x / 2, lpSceneMng.GetScreenSize().y / 2);
}

void Camera::Update(bool dashFlag)
{
	Vector2Int middleMousePos = { lpSceneMng.GetScreenSize().x / 2, lpSceneMng.GetScreenSize().y / 2 };
	//�}�E�X�̈ʒu���擾
	GetMousePoint(&mousePos_.x, &mousePos_.y);

	//�}�E�X�̈ړ��������v�Z
	Vector2 mouseMoveDistance = { static_cast<float>(mousePos_.x - middleMousePos.x),static_cast<float>(mousePos_.y - middleMousePos.y) };

	//�}�E�X�ʒu�̍����ŉ�]������
	angle_.y += Deg2RadF(mouseMoveDistance.x / 30.0f);	//���E��]
	angle_.x += Deg2RadF(mouseMoveDistance.y / 30.0f);	//�㉺��]

	//������̊p�x����
	if (angle_.x < UP_RADIAN_LIMIT_X)
	{
		angle_.x = UP_RADIAN_LIMIT_X;
	}

	//�������̊p�x����
	if (angle_.x > DOWN_RADIAN_LIMIT_X)
	{
		angle_.x = DOWN_RADIAN_LIMIT_X;
	}

	if (dashFlag)
	{
		angle_.x += GetSwingWidth();
	}
	else
	{
		//angle_.x += ShakeCamera();

	}

	//�}�E�X�̈ʒu��^�񒆂ɌŒ�
	SetMousePoint(middleMousePos.x, middleMousePos.y);
}

void Camera::Update(Vector3 targetPos)
{

}

void Camera::SetBeforeDraw(Vector3 playerpos)
{
	// �N���b�v�����̐ݒ�
	SetCameraNearFar(CAMERA_RANGE_NEAR, CAMERA_RANGE_FAR);

	//�J�����̈ړ�
	//�v���C���[�̈ʒu���J�������W�ɐݒ�
	pos_ = playerpos;
	//pos_.z -= 2200.0f;
	pos_.y = playerpos.y + CAMERA_HEIGHT;

	//�����_�̈ړ�
	//�N�H�[�^�j�I���ɕϊ�
	quaRot_ = Quaternion::Euler(angle_.toVECTOR());
	//���W����](�J��������̑��΍��W)
	Vector3 localPos = ConvertVECTORtoVector3(quaRot_.PosAxis(VGet(0.0f, 0.0f, TARGET2CAMERA_DISTANCE)));
	targetPos_ = pos_ + localPos;

	//�J�����̈ʒu�ƒ����_�̐ݒ�𔽉f
	SetCameraPositionAndTarget_UpVecY(
		pos_.toVECTOR(),
		targetPos_.toVECTOR());
}

void Camera::Draw(void)
{
	//�f�o�b�O�\��
	auto xdeg = Rad2DegF(angle_.x);
	auto ydeg = Rad2DegF(angle_.y);
	auto zdeg = Rad2DegF(angle_.z);
	//DrawSphere3D(VGet(pos_.x,pos_.y - CAMERA_HEIGHT,pos_.z), 50.0f, 100, 0xff00ff, 0xff00ff, true);
	DrawFormatString(600, 0, 0xffffff,  "�J�������W : (%.1f, %.1f, %.1f)", pos_.x,pos_.y,pos_.z);
	//DrawFormatString(0, 32, 0xffffff, "�����_���W : (%.1f, %.1f, %.1f)", targetPos_.x, targetPos_.y, targetPos_.z);
	DrawFormatString(0, 64, 0xffffff, "�J�����p�x : (%.1f, %.1f, %.1f)", xdeg, ydeg,zdeg);
}

Vector3 Camera::GetPos(void)
{
	return pos_;
}

Vector3 Camera::GetTargetPos()
{
	return targetPos_;
}

Vector3 Camera::GetAngles()
{
	return angle_;
}

float Camera::GetSwingWidth(void)
{
	auto addSwing = sin(sinCnt_ / SWING_OFFSET) * SWING_WIDTH;
	sinCnt_++;
	return addSwing;
}
