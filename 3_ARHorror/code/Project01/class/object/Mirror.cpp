#include "Mirror.h"

#include "../common/manager/ModelMng.h"

Mirror::Mirror()
{
	Mirror_Initialize();
}

Mirror::~Mirror()
{
}

// ���̏�����
void Mirror::Mirror_Initialize(void)
{
	
	// �J�����̊p�x��������
	CameraAngle = 325;
	// �J�����̍��W���Z�b�g
	CameraEyePosition = VGet(-2605.0f, 670.0f, -2561.0f);
	
	// ���C�g�̌������Z�b�g
	SetLightDirection(VGet(1.0f, -1.0f, -1.0f));
	
	int i;

	// ���ɉf��f���̎擾�Ɏg�p����X�N���[���̍쐬
	for (i = 0; i < MIRROR_NUM; i++)
	{
		MirrorHandle[i] = MakeScreen(MIRROR_SCREEN_W, MIRROR_SCREEN_H, FALSE);
	}

}

void Mirror::Mirror_Update(void)
{
	// �J�����̌����Ă���������Z�o
	CameraDirection.x = cos(CameraAngle * 3.14159f / 180.0f);
	CameraDirection.y = 0.0f;
	CameraDirection.z = sin(CameraAngle * 3.14159f / 180.0f);
	// �J�����̒����_���W���Z�o
	CameraTargetPosition = VAdd(CameraEyePosition, CameraDirection);
}

// ���ɉf��f����`�悷�邽�߂̃J�����̐ݒ���s��
void Mirror::Mirror_SetupCamera(int MirrorNo, VECTOR CameraEyePos, VECTOR CameraTargetPos)
{
	int i;
	float EyeLength, TargetLength;
	VECTOR MirrorNormal;
	VECTOR* MirrorWorldPosP;
	VECTOR MirrorCameraEyePos, MirrorCameraTargetPos;

	MirrorWorldPosP = MirrorWorldPos[MirrorNo];

	// ���̖ʂ̖@�����Z�o
	MirrorNormal = VNorm(VCross(VSub(MirrorWorldPosP[1], MirrorWorldPosP[0]), VSub(MirrorWorldPosP[2], MirrorWorldPosP[0])));

	// ���̖ʂ���J�����̍��W�܂ł̍ŒZ�����A���̖ʂ���J�����̒����_�܂ł̍ŒZ�������Z�o
	EyeLength = Plane_Point_MinLength(MirrorWorldPosP[0], MirrorNormal, CameraEyePos);
	TargetLength = Plane_Point_MinLength(MirrorWorldPosP[0], MirrorNormal, CameraTargetPos);

	// ���ɉf��f����`�悷��ۂɎg�p����J�����̍��W�ƃJ�����̒����_���Z�o
	MirrorCameraEyePos = VAdd(CameraEyePos, VScale(MirrorNormal, -EyeLength * 2.0f));
	MirrorCameraTargetPos = VAdd(CameraTargetPos, VScale(MirrorNormal, -TargetLength * 2.0f));

	// �v�Z�œ���ꂽ�J�����̍��W�ƃJ�����̒����_�̍��W���J�����̐ݒ肷��
	SetCameraPositionAndTarget_UpVecY(MirrorCameraEyePos, MirrorCameraTargetPos);

	// ���ɉf��f���̒��ł̋��̎l���̍��W���Z�o( �������W )
	for (i = 0; i < 4; i++)
	{
		MirrorScreenPosW[MirrorNo][i] = ConvWorldPosToScreenPosPlusW(MirrorWorldPosP[i]);
	}
}

// ���̕`��
void Mirror::Mirror_Render(int MirrorNo)
{
	int i;
	int j;
	int k;
	VERTEX3D Vert[MIRROR_POINTNUM * MIRROR_POINTNUM];
	unsigned short Index[(MIRROR_POINTNUM - 1) * (MIRROR_POINTNUM - 1) * 6];
	MATERIALPARAM Material;
	VECTOR HUnitPos;
	VECTOR VUnitPos[2];
	VECTOR HPos;
	VECTOR VPos[2];
	FLOAT4 HUnitUV;
	FLOAT4 VUnitUV[2];
	FLOAT4 HUV;
	FLOAT4 VUV[2];
	VECTOR MirrorNormal;
	COLOR_U8 DiffuseColor;
	COLOR_U8 SpecularColor;
	int TextureW, TextureH;
	VECTOR* MirrorWorldPosP;

	MirrorWorldPosP = MirrorWorldPos[MirrorNo];

	// ���̕`��Ɏg�p����}�e���A���̃Z�b�g�A�b�v
	Material.Ambient = MirrorAmbientColor[MirrorNo];
	Material.Diffuse = GetColorF(0.0f, 0.0f, 0.0f, 0.0f);
	Material.Emissive = GetColorF(0.0f, 0.0f, 0.0f, 0.0f);
	Material.Specular = GetColorF(0.0f, 0.0f, 0.0f, 0.0f);
	Material.Power = 1.0f;
	SetMaterialParam(Material);

	// ���̖ʂ̖@�����Z�o
	MirrorNormal = VNorm(VCross(VSub(MirrorWorldPosP[1], MirrorWorldPosP[0]), VSub(MirrorWorldPosP[2], MirrorWorldPosP[0])));

	// ���ɉf��f�����������񂾉摜�̃e�N�X�`���̃T�C�Y���擾
	GetGraphTextureSize(MirrorHandle[MirrorNo], &TextureW, &TextureH);

	// ���̕`��Ɏg�p���钸�_�̃Z�b�g�A�b�v
	VUnitPos[0] = VScale(VSub(MirrorWorldPosP[2], MirrorWorldPosP[0]), 1.0f / (MIRROR_POINTNUM - 1));
	VUnitPos[1] = VScale(VSub(MirrorWorldPosP[3], MirrorWorldPosP[1]), 1.0f / (MIRROR_POINTNUM - 1));
	VUnitUV[0] = F4Scale(F4Sub(MirrorScreenPosW[MirrorNo][2], MirrorScreenPosW[MirrorNo][0]), 1.0f / (MIRROR_POINTNUM - 1));
	VUnitUV[1] = F4Scale(F4Sub(MirrorScreenPosW[MirrorNo][3], MirrorScreenPosW[MirrorNo][1]), 1.0f / (MIRROR_POINTNUM - 1));
	DiffuseColor = GetColorU8(MirrorDiffuseColor[MirrorNo][0], MirrorDiffuseColor[MirrorNo][1], MirrorDiffuseColor[MirrorNo][2], MirrorDiffuseColor[MirrorNo][3]);
	SpecularColor = GetColorU8(0, 0, 0, 0);
	VPos[0] = MirrorWorldPosP[0];
	VPos[1] = MirrorWorldPosP[1];
	VUV[0] = MirrorScreenPosW[MirrorNo][0];
	VUV[1] = MirrorScreenPosW[MirrorNo][1];
	k = 0;
	for (i = 0; i < MIRROR_POINTNUM; i++)
	{
		HUnitPos = VScale(VSub(VPos[1], VPos[0]), 1.0f / (MIRROR_POINTNUM - 1));
		HPos = VPos[0];
		HUnitUV = F4Scale(F4Sub(VUV[1], VUV[0]), 1.0f / (MIRROR_POINTNUM - 1));
		HUV = VUV[0];
		for (j = 0; j < MIRROR_POINTNUM; j++)
		{
			Vert[k].pos = HPos;
			Vert[k].norm = MirrorNormal;
			Vert[k].dif = DiffuseColor;
			Vert[k].spc = SpecularColor;
			Vert[k].u = HUV.x / (HUV.w * TextureW);
			Vert[k].v = HUV.y / (HUV.w * TextureH);
			Vert[k].su = 0.0f;
			Vert[k].sv = 0.0f;
			HUV = F4Add(HUV, HUnitUV);
			HPos = VAdd(HPos, HUnitPos);
			k++;
		}
		VUV[0] = F4Add(VUV[0], VUnitUV[0]);
		VUV[1] = F4Add(VUV[1], VUnitUV[1]);
		VPos[0] = VAdd(VPos[0], VUnitPos[0]);
		VPos[1] = VAdd(VPos[1], VUnitPos[1]);
	}

	// ���̕`��Ɏg�p���钸�_�C���f�b�N�X���Z�b�g�A�b�v
	k = 0;
	for (i = 0; i < MIRROR_POINTNUM - 1; i++)
	{
		for (j = 0; j < MIRROR_POINTNUM - 1; j++)
		{
			Index[k + 0] = (i + 0) * MIRROR_POINTNUM + j + 0;
			Index[k + 1] = (i + 0) * MIRROR_POINTNUM + j + 1;
			Index[k + 2] = (i + 1) * MIRROR_POINTNUM + j + 0;
			Index[k + 3] = (i + 1) * MIRROR_POINTNUM + j + 1;
			Index[k + 4] = Index[k + 2];
			Index[k + 5] = Index[k + 1];
			k += 6;
		}
	}
	// �`�惂�[�h���o�C���j�A�t�B���^�����O�ɐݒ�
	SetDrawMode(DX_DRAWMODE_BILINEAR);

	// �`��u�����h���[�h��ύX
	SetDrawBlendMode(MirrorBlendParam[MirrorNo][0], MirrorBlendParam[MirrorNo][1]);

	// ����`��
	DrawPolygonIndexed3D(Vert, MIRROR_POINTNUM * MIRROR_POINTNUM, Index, (MIRROR_POINTNUM - 1) * (MIRROR_POINTNUM - 1) * 2, MirrorHandle[MirrorNo], FALSE);

	// �`��u�����h���[�h�����ɖ߂�
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	// �`�惂�[�h�����ɖ߂�
	SetDrawMode(DX_DRAWMODE_NEAREST);
}
// ���̃f�o�b�O�`��
void Mirror::Mirror_DebugRender(int MirrorNo, int DrawX, int DrawY)
{
	int i;
	int j;
	FLOAT4 HUnitUV;
	FLOAT4 VUnitUV[2];
	FLOAT4 HUV;
	FLOAT4 VUV[2];
	int x;
	int y;

	// �w��̍��W�ɋ��̉f���̎擾�Ɏg�p�����X�N���[����`��
	DrawExtendGraph(DrawX, DrawY, DrawX + MIRROR_SCREEN_W / MIRROR_DEBUG_SCALE, DrawY + MIRROR_SCREEN_H / MIRROR_DEBUG_SCALE, MirrorHandle[MirrorNo], FALSE);

	// ���̉f���̎擾�Ɏg�p�����X�N���[���̒��̋��̕����ɓ_��`��
	VUnitUV[0] = F4Scale(F4Sub(MirrorScreenPosW[MirrorNo][2], MirrorScreenPosW[MirrorNo][0]), 1.0f / (MIRROR_POINTNUM - 1));
	VUnitUV[1] = F4Scale(F4Sub(MirrorScreenPosW[MirrorNo][3], MirrorScreenPosW[MirrorNo][1]), 1.0f / (MIRROR_POINTNUM - 1));
	VUV[0] = MirrorScreenPosW[MirrorNo][0];
	VUV[1] = MirrorScreenPosW[MirrorNo][1];
	for (i = 0; i < MIRROR_POINTNUM; i++)
	{
		HUnitUV = F4Scale(F4Sub(VUV[1], VUV[0]), 1.0f / (MIRROR_POINTNUM - 1));
		HUV = VUV[0];
		for (j = 0; j < MIRROR_POINTNUM; j++)
		{
			x = (int)(HUV.x / HUV.w / MIRROR_DEBUG_SCALE);
			y = (int)(HUV.y / HUV.w / MIRROR_DEBUG_SCALE);

			if (x > 0 && x < MIRROR_SCREEN_W / MIRROR_DEBUG_SCALE &&
				y > 0 && y < MIRROR_SCREEN_H / MIRROR_DEBUG_SCALE)
			{
				DrawPixel(DrawX + x, DrawY + y, GetColor(0, 255, 0));
			}

			HUV = F4Add(HUV, HUnitUV);
		}
		VUV[0] = F4Add(VUV[0], VUnitUV[0]);
		VUV[1] = F4Add(VUV[1], VUnitUV[1]);
	}
}

// �R�c��Ԃ̕`��
void Mirror::World_RenderProcess()
{
	// �X�e�[�W���f���̕`��
	//MV1DrawModel(StageModel);

	// �L�����N�^�[���f���̕`��
	//MV1DrawModel(CharaModel);
}

void Mirror::MirrorDraw()
{

	// ���ɉf��f����`��
	for (int i = 0; i < MIRROR_NUM; i++)
	{
		// �`�������ɉf��f���̎擾�Ɏg�p����摜��`�悷��X�N���[���ɂ���
		SetDrawScreen(MirrorHandle[i]);

		// ��ʂ�����������
		ClearDrawScreen();

		// ���ɉf��f����`�悷��ۂɎg�p����J�����̐ݒ���s��
		Mirror_SetupCamera(i, CameraEyePosition, CameraTargetPosition);

		// �R�c��Ԃ̕`��
		World_RenderProcess();
	}
}

void Mirror::MirrorDraw2()
{
	


	// ���̕`��
	for (int i = 0; i < MIRROR_NUM; i++)
	{
		Mirror_Render(i);
	}


	// ��ʍ���ɋ��̃f�o�b�O�\��
	for (int j = 0; j < MIRROR_NUM; j++)
	{
		Mirror_DebugRender(j, 20 + 180 * j, 0);
	}
}

VECTOR Mirror::GetCameraEyePosition()
{
	return CameraEyePosition;
}

VECTOR Mirror::GetCameraTargetPosition()
{
	return CameraTargetPosition;
}
