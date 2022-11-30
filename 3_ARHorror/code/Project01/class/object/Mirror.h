#pragma once
#include <DxLib.h>
#include <math.h>
#include "../common/Geometry.h"

#define MIRROR_SCREEN_W		640		// 鏡に映る映像の取得に使用するスクリーンの横解像度
#define MIRROR_SCREEN_H		480		// 鏡に映る映像の取得に使用するスクリーンの縦解像度
#define MIRROR_POINTNUM		64		// 鏡の描画に使用する面の頂点分割数
#define MIRROR_NUM		2		// 鏡の数
#define MIRROR_DEBUG_SCALE	4		// 鏡のデバッグ表示時に元の何分の１にするかのサイズ

#define CAMERA_SPEED		32.0f		// カメラの速度

class Mirror
{
public:
	Mirror();
	~Mirror();

	void Mirror_Initialize(void);								// 鏡の初期化
	void Mirror_Update(void);
	void Mirror_SetupCamera(int MirrorNo, VECTOR CameraEyePos, VECTOR CameraTargetPos);		// 鏡に映る映像を描画するためのカメラの設定を行う
	void Mirror_Render(int MirrorNo);								// 鏡の描画
	void Mirror_DebugRender(int MirrorNo, int DrawX, int DrawY);					// 鏡のデバッグ描画

	void World_RenderProcess();									// ３Ｄ空間の描画

	void MirrorDraw();
	void MirrorDraw2();

	VECTOR GetCameraEyePosition();
	VECTOR GetCameraTargetPosition();

	int MirrorHandle[MIRROR_NUM];		// 鏡に映る映像の取得に使用するスクリーン

private:

	int CameraAngle;		// カメラの水平角度
	VECTOR CameraEyePosition;	// カメラの座標
	VECTOR CameraDirection;	// カメラの向いている方向
	VECTOR CameraTargetPosition;	// カメラの注視点

	//int MirrorHandle[MIRROR_NUM];		// 鏡に映る映像の取得に使用するスクリーン
	FLOAT4 MirrorScreenPosW[MIRROR_NUM][4];	// 鏡に映る映像の取得に使用するクリーンの中の鏡の四隅の座標( 同次座標 )

	// 鏡のワールド座標
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

	// 鏡の Ambient Color
	COLOR_F MirrorAmbientColor[MIRROR_NUM] =
	{
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		{ 0.0f, 0.0f, 0.0f, 0.0f },
	};

	// 鏡の Diffuse Color
	int MirrorDiffuseColor[MIRROR_NUM][4] =
	{
		{ 255, 255, 255, 255 },
		{   0, 255, 255, 255 },
	};

	// 鏡のブレンドモードとパラメータ
	int MirrorBlendParam[MIRROR_NUM][2] =
	{
		{ DX_BLENDMODE_NOBLEND, 255 },
		{ DX_BLENDMODE_ALPHA,   128 },
	};
};

