#include<EffekseerForDXLib.h>
#include"../Scene/SceneManager.h"
#include "../Obj/Stage.h"
#include "../Common/AsoUtility.h"
#include "../Obj/Unit/Player.h"
#include "Camera.h"

Camera::Camera()
{
	Init();
}

Camera::~Camera()
{
}

void Camera::Init(void)
{
	//�\�������E�B���h�E�̌��_
	//�J�����̏����ݒ�
	mPos = { 0.0f,0.0f,-DIS_TARGET_CAMERA };
	//�J�����p�x
	mAngles = {
		//30.0f *(DX_PI_F/180),
		(float)AsoUtility::Deg2RadF(30.0f),
		0.0f,
		0.0f
	};
	stopCameraPosY_ = -1000.0f;

}

void Camera::Update()
{
	////�L�[�{�[�h����ŃJ�����p�x�𑀍�
	//if (CheckHitKey(KEY_INPUT_RIGHT))
	//{
	//	//�E��]
	//	mAngles.y += AsoUtility::Deg2RadF(1.0);
	//}
	//if (CheckHitKey(KEY_INPUT_LEFT))
	//{
	//	//����]
	//	mAngles.y -= AsoUtility::Deg2RadF(1.0);
	//}

	//////���グ��A�����낷�J�����̂���]
	//if (CheckHitKey(KEY_INPUT_UP))
	//{
	//	//���グ��
	//	mAngles.x -= AsoUtility::Deg2RadF(1.0);
	//}
	//if (CheckHitKey(KEY_INPUT_DOWN))
	//{
	//	//�����낷
	//	mAngles.x += AsoUtility::Deg2RadF(1.0);
	//}

}

void Camera::SetBeforeDraw(Vector3 pPos)
{
	float revRad = AsoUtility::Deg2RadF(180.0f);
	float dirX = sin(mAngles.y + revRad);
	float dirZ = cos(mAngles.y + revRad);
	VECTOR dir = VNorm({ dirX,0.0f,dirZ });
	VECTOR movePow = VScale(dir, DIS_TARGET_CAMERA);
	movePow.y += HEIGHT;
	switch (lpStage.GetStage())
	{
	case STAGE_NAME::STAGE_1:
	case STAGE_NAME::STAGE_2:
	case STAGE_NAME::STAGE_3:
	case STAGE_NAME::STAGE_4:
		stopCameraPosY_ = -1000.0f;
		break;
	case STAGE_NAME::STAGE_5:
		stopCameraPosY_ = -4363.0f;
		break;
	case STAGE_NAME::STAGE_6:
		stopCameraPosY_ = -1224.0f;
		break;
	case STAGE_NAME::STAGE_7:
		stopCameraPosY_ = -1895.0f;
		break;
	case STAGE_NAME::MAX:
		break;
	}
	if (pPos.y > stopCameraPosY_)
	{
		pPos.y += 300;
		mTargetPos = pPos;
		mPos = ConvertVECTORtoVector3(VAdd(mTargetPos.toVECTOR(), movePow));
	}
	SetCameraPositionAndTarget_UpVecY(
		mPos.toVECTOR(),
		mTargetPos.toVECTOR());
//	SetupCamera_Perspective(60.0f * DX_PI_F / 180.0f);
	//SetCameraNearFar(1.0f, 15000.0f);
	//_dbgDrawFormatString(100, 100,0xffffff,"mPos:%f,TargetPos:%f", mPos.x, mTargetPos.x);
	Effekseer_Sync3DSetting();
	

}

void Camera::Draw(void)
{
}

void Camera::Release(void)
{
}

Vector3 Camera::GetPos(void)
{
	return mPos;
}

void Camera::SetPos(Vector3 pos)
{
	float revRad = AsoUtility::Deg2RadF(180.0f);
	float dirX = sin(mAngles.y + revRad);
	float dirZ = cos(mAngles.y + revRad);
	VECTOR dir = VNorm({ dirX,0.0f,dirZ });
	VECTOR movePow = VScale(dir, DIS_TARGET_CAMERA);
	movePow.y += HEIGHT;

	if (pos.y > stopCameraPosY_)
	{
		pos.y += 300;
		mTargetPos = pos;
		mPos = ConvertVECTORtoVector3(VAdd(mTargetPos.toVECTOR(), movePow));
	}
	SetCameraPositionAndTarget_UpVecY(
		mPos.toVECTOR(),
		mTargetPos.toVECTOR());
}

Vector3 Camera::GetTargetPos()
{
	return mTargetPos;
}

Vector3 Camera::GetAngles()
{
	return mAngles;
}


