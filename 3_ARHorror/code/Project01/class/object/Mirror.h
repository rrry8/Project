#pragma once
#include <DxLib.h>
#include <math.h>
#include "../common/Geometry.h"

#define MIRROR_SCREEN_W		640		// ���ɉf��f���̎擾�Ɏg�p����X�N���[���̉��𑜓x
#define MIRROR_SCREEN_H		480		// ���ɉf��f���̎擾�Ɏg�p����X�N���[���̏c�𑜓x
#define MIRROR_POINTNUM		64		// ���̕`��Ɏg�p����ʂ̒��_������
#define MIRROR_NUM		2		// ���̐�
#define MIRROR_DEBUG_SCALE	4		// ���̃f�o�b�O�\�����Ɍ��̉����̂P�ɂ��邩�̃T�C�Y

#define CAMERA_SPEED		32.0f		// �J�����̑��x

class Mirror
{
public:
	Mirror();
	~Mirror();

	void Mirror_Initialize(void);								// ���̏�����
	void Mirror_Update(void);
	void Mirror_SetupCamera(int MirrorNo, VECTOR CameraEyePos, VECTOR CameraTargetPos);		// ���ɉf��f����`�悷�邽�߂̃J�����̐ݒ���s��
	void Mirror_Render(int MirrorNo);								// ���̕`��
	void Mirror_DebugRender(int MirrorNo, int DrawX, int DrawY);					// ���̃f�o�b�O�`��

	void World_RenderProcess();									// �R�c��Ԃ̕`��

	void MirrorDraw();
	void MirrorDraw2();

	VECTOR GetCameraEyePosition();
	VECTOR GetCameraTargetPosition();

	int MirrorHandle[MIRROR_NUM];		// ���ɉf��f���̎擾�Ɏg�p����X�N���[��

private:

	int CameraAngle;		// �J�����̐����p�x
	VECTOR CameraEyePosition;	// �J�����̍��W
	VECTOR CameraDirection;	// �J�����̌����Ă������
	VECTOR CameraTargetPosition;	// �J�����̒����_

	//int MirrorHandle[MIRROR_NUM];		// ���ɉf��f���̎擾�Ɏg�p����X�N���[��
	FLOAT4 MirrorScreenPosW[MIRROR_NUM][4];	// ���ɉf��f���̎擾�Ɏg�p����N���[���̒��̋��̎l���̍��W( �������W )

	// ���̃��[���h���W
	VECTOR MirrorWorldPos[MIRROR_NUM][4] =
	{
		{
			{  2000.0f, 2000.0f, -4498.0f },
			{ -2000.0f, 2000.0f, -4498.0f },
			{  2000.0f,    0.0f, -4498.0f },
			{ -2000.0f,    0.0f, -4498.0f },
		},

		{
			{ 4000.0f, 100.0f,  -4500.0f },
			{  -4000.0f, 100.0f,  -4500.0f },
			{ 4000.0f, 0.0f, -4500.0f },
			{  -4000.0f, 0.0f, -4500.0f },
		}
	};

	// ���� Ambient Color
	COLOR_F MirrorAmbientColor[MIRROR_NUM] =
	{
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		{ 0.0f, 0.0f, 0.0f, 0.0f },
	};

	// ���� Diffuse Color
	int MirrorDiffuseColor[MIRROR_NUM][4] =
	{
		{ 255, 255, 255, 255 },
		{   0, 255, 255, 255 },
	};

	// ���̃u�����h���[�h�ƃp�����[�^
	int MirrorBlendParam[MIRROR_NUM][2] =
	{
		{ DX_BLENDMODE_NOBLEND, 255 },
		{ DX_BLENDMODE_ALPHA,   128 },
	};
};

