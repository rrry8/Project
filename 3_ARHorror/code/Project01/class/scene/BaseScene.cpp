#include<DxLib.h>
#include "BaseScene.h"

BaseScene::BaseScene()
{
	SetDrawScreen(DX_SCREEN_BACK);
	GetDrawScreenSize(&screenSizeX_, &screenSizeY_);
	screenID_ = MakeScreen(screenSizeX_, screenSizeY_, true);
	albedRT_ = MakeScreen(screenSizeX_, screenSizeY_, true);
	normalSpecRT_ = MakeScreen(screenSizeX_, screenSizeY_, true);


	/*SetDrawValidFloatTypeGraphCreateFlag(true);
	SetCreateDrawValidGraphChannelNum(1);
	SetCreateGraphChannelBitDepth(32);*/
	depthRT_ = MakeScreen(screenSizeX_, screenSizeY_, true);
	/*SetDrawValidFloatTypeGraphCreateFlag(FALSE);
	SetCreateDrawValidGraphChannelNum(0);
	SetCreateGraphChannelBitDepth(0);*/

	//浮動小数点RGBA 32ビット
	/*SetDrawValidFloatTypeGraphCreateFlag(true);
	SetCreateDrawValidGraphChannelNum(4);
	SetCreateGraphChannelBitDepth(32);
	worldPosRT_ = MakeScreen(screenSizeX_, screenSizeY_, true);
	SetDrawValidFloatTypeGraphCreateFlag(FALSE);
	SetCreateDrawValidGraphChannelNum(0);
	SetCreateGraphChannelBitDepth(0);*/
	InitVertext();
}

BaseScene::~BaseScene()
{
}

void BaseScene::InitVertext(void)
{
	//頂点初期化

	//共通
	for (auto& v : verts_)
	{
		v.dif = GetColorU8(255, 255, 255, 255);
		v.spc = GetColorU8(0, 0, 0, 0);
		v.pos.z = 0.0f;
		v.rhw = 1.0f;
		v.su = 0.0f;
		v.sv = 0.0f;
	}
	//左上
	verts_[0].pos.x = 0.0f;
	verts_[0].pos.y = 0.0f;
	//右上
	verts_[1].pos.x = static_cast<float>(screenSizeX_);
	verts_[1].pos.y = 0.0f;
	//左下
	verts_[2].pos.x = 0.0f;
	verts_[2].pos.y = static_cast<float>(screenSizeY_);
	//右下
	verts_[3].pos.x = static_cast<float>(screenSizeX_);
	verts_[3].pos.y = static_cast<float>(screenSizeY_);
	//UV座標
	verts_[0].u = 0.0f;
	verts_[0].v = 0.0f;

	verts_[1].u = 1.0f;
	verts_[1].v = 0.0f;

	verts_[2].u = 0.0f;
	verts_[2].v = 1.0f;

	verts_[3].u = 1.0f;
	verts_[3].v = 1.0f;
}

void BaseScene::Draw()
{
	DrawGraph(0, 0, screenID_, true);
}

void BaseScene::DrawPostEffect(int ps)
{
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	SetUsePixelShader(ps);
	SetUseTextureToShader(0, screenID_);
	SetUseTextureToShader(1, -1);
	DrawPrimitive2DToShader(verts_.data(), verts_.size(), DX_PRIMTYPE_TRIANGLESTRIP);
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

void BaseScene::DrawPostEffect(int ps, int texture1, int texture2)
{
	SetUsePixelShader(ps);
	SetUseTextureToShader(0, texture1);
	SetUseTextureToShader(1, texture2);
	DrawPrimitive2DToShader(verts_.data(), verts_.size(), DX_PRIMTYPE_TRIANGLESTRIP);
}

