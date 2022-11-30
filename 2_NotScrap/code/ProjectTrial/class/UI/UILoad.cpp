#include "UILoad.h"
#include "../Common/ImageMng.h"
#include "../Scene/SceneManager.h"
#include "../../_debug/_DebugConOut.h"
#include "../../_debug/_DebugDispOut.h"


UILoad::UILoad()
{
	Init();
	DrawOwnScreen(0.0);
}

UILoad::~UILoad()
{
	DeleteShader(psH);
	DeleteShaderConstantBuffer(cBuf);
}

bool UILoad::Init(void)
{
	LoadVol = 0;
	LoadingTime = 0;
	IsLoadingEnd = false;
	PSInit();
	psH = LoadPixelShader("./source/shader/UVScroll.pso");
	return true;
}

void UILoad::Update(double delta)
{
	//TRACE("�񓯊��ǂݍ��݂̐�:%d\n", GetASyncLoadNum());
	if (GetASyncLoadNum() == 0)//�񓯊��ǂݍ��݂̎c�������������ׂ�
	{
		//�c����0�Ȃ�
		//TRACE("�񓯊��ǂݍ��ݏI��\n");
		if (LoadVol >= 1000)//�o�[���ő�l�ɂȂ�����
		{
			IsLoadingEnd = true;//���[�h�I��
		}
		else//�o�[���ő�l�ȉ��̎�
		{
			//�o�[���}�b�N�X�ɂ���
			LoadVol += 30;
			if (LoadVol > 1000)
			{
				LoadVol = 1000;
			}
		}
	}
	else
	{
		IsLoadingEnd = false;
		//���[�h�̐i�����\�����Ă�
		if (LoadVol < 950)
		{
			if (LoadVol < 1000 / (GetASyncLoadNum() + 1))//�c��̐��ōő�l�������āA�o�[�̑�������ő�l�𐧌��i�P�Ŋ����Ă��Ӗ��Ȃ�����+1�j
			{
				LoadVol += 1;	
			}
			if (LoadVol < 1000 && (GetASyncLoadNum()) == 1)//���[�h���ő�ȉ��@�����@+1���ĂȂ���Ԃ̎c�����P
			{
				LoadVol += 1;	//�Ō�̈�ɂȂ����灪�̂ƍ��킹��+2�����₷
			}
		}
	}
	LoadingTime++;
	DrawOwnScreen(delta);
}

void UILoad::DrawOwnScreen(double delta)
{
	DrawGraph(STR_POINT_X, STR_POINT_Y, lpImageMng.GetID("./source/UIImage/string/LOAD/NOWLOADING.png")[0], true);
	switch (LoadingTime / 20 % 3)//�E�E�E�𑝂₵���茸�炵���肵�Ă��
	{
	case 0:
		DrawStringToHandle(STR_POINT_X + 340, STR_POINT_Y, ".", 0x000000, lpSceneMng.GetFontHandle(), 0xffffff, 0);
		break;
	case 1:
		DrawStringToHandle(STR_POINT_X + 340, STR_POINT_Y, "..", 0x000000, lpSceneMng.GetFontHandle(), 0xffffff, 0);
		break;
	case 2:
		DrawStringToHandle(STR_POINT_X + 340, STR_POINT_Y, "...", 0x000000, lpSceneMng.GetFontHandle(), 0xffffff, 0);
		break;
	default:
		break;
	}
	//--�f�o�b�O�p--
	//_dbgDrawFormatString(500, 300, 0xffffff, "load�o�ߎ���:%d�b", LoadingTime / 60);
	//_dbgDrawFormatString(500, 316, 0xffffff, "loading:%d��", (LoadVol/10));
	
	//--���[�h�o�[�̘g--
	DrawModiGraph(
		BAR_LEFT_TOP_X, BAR_LEFT_TOP_Y,//����
		BAR_RIGHT_TOP_X, BAR_RIGHT_TOP_Y,//�E��
		BAR_RIGHT_DOWN_X, BAR_RIGHT_DOWN_Y,//�E��
		BAR_LEFT_DOWN_X, BAR_LEFT_DOWN_Y,//����
		lpImageMng.GetID("./source/UIImage/Load/ProgressBar_01/BarV1_Bar.png")[0], true);
	int StrWidth, StrLen;
	StrLen = strlen("100");
	StrWidth = GetDrawStringWidthToHandle("100", StrLen, lpSceneMng.GetFontHandle());
	DrawModiGraph(
		1300, 670,//����
		1500, 670,//�E��i���[�h�J�n���͍��[�j
		1500, 720,//�E���i���[�h�J�n���͍��[�j
		1300, 720,//����
		lpImageMng.GetID("./source/UIImage/Load/Icons/Arrows03.png")[0], true);
	DrawFormatStringToHandle(1340, 670, 0xffffff, lpSceneMng.GetFontHandle(), "%d", (LoadVol / 10));
	DrawStringToHandle(1340 + StrWidth, 670, "��", 0xffffff, lpSceneMng.GetFontHandle());

	MyDrawGraph(-1, psH);
}

bool UILoad::GetLoadingEndFlg(void)
{
	return IsLoadingEnd;
}

void UILoad::SetIsNext(bool flg)
{
}


void UILoad::MyDrawGraph(int Imghandle, int pshandle) {
	
	verts[1].pos.x = static_cast<float>(BAR_LEFT_TOP_X + LoadVol);
	verts[3].pos.x = static_cast<float>(BAR_LEFT_TOP_X + LoadVol);
	
	SetShaderConstantBuffer(cBuf, DX_SHADERTYPE_PIXEL, 0);
	writer[0] += 0.01f;
	//if (writer[0] > 1.5f)
	//{
	//	writer[0] = 0.0f;
	//}
	UpdateShaderConstantBuffer(cBuf);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	SetDrawAlphaTest(DX_CMP_GREATER, 0);
	SetUseAlphaTestFlag(true);
	SetUsePixelShader(pshandle);
	SetUseTextureToShader(0, lpImageMng.GetID("./source/UIImage/Load/ProgressBar_01/BarV1_ProgressBar.png")[0]);
	SetUseTextureToShader(1, lpImageMng.GetID("./source/UIImage/Load/ProgressBar_01/Bar_normal.png")[0]);
	DrawPrimitive2DToShader(verts.data(), verts.size(), DX_PRIMTYPE_TRIANGLESTRIP);
	
}

void UILoad::PSInit(void)
{
	for (auto& v : verts) {
		v.rhw = 1.0f;//�RD�̏ꍇ���ꂪ1����Ȃ�
		v.dif = GetColorU8(255, 255, 255, 255);
		v.u = 0.0f;
		v.v = 0.0f;
		v.su = 0.0f;
		v.sv = 0.0f;
		v.pos.z = 0.0f;
		//v.spc = GetColorU8(0, 0, 0, 0);
		v.spc = GetColorU8(255, 255, 255, 255);
	}
	// Z�̎�
		//����
	verts[0].pos.x = BAR_LEFT_TOP_X;
	verts[0].pos.y = BAR_LEFT_TOP_Y;
	verts[1].pos.x = BAR_LEFT_TOP_X + LoadVol;
	verts[1].pos.y = BAR_LEFT_TOP_Y;
	verts[2].pos.x = BAR_LEFT_TOP_X;
	verts[2].pos.y = BAR_LEFT_TOP_Y + 50;
	verts[3].pos.x = BAR_LEFT_TOP_X + LoadVol;
	verts[3].pos.y = BAR_LEFT_TOP_Y + 50;
	verts[0].u = 0.0f;
	verts[0].v = 0.0f;
	verts[1].u = 1.0f;
	verts[1].v = 0.0f;
	verts[2].u = 0.0f;
	verts[2].v = 1.0f;
	verts[3].u = 1.0f;
	verts[3].v = 1.0f;
	cBuf = CreateShaderConstantBuffer(sizeof(float) * 4);
	writer = static_cast<float*>(GetBufferShaderConstantBuffer(cBuf));
	writer[0] = 0.0f;

}

void UILoad::PSUpdate(void)
{

}

