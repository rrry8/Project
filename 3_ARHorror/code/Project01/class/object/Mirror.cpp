#include "Mirror.h"

#include "../common/manager/ModelMng.h"

Mirror::Mirror()
{
	Mirror_Initialize();
}

Mirror::~Mirror()
{
}

// 鏡の初期化
void Mirror::Mirror_Initialize(void)
{
	
	// カメラの角度を初期化
	CameraAngle = 325;
	// カメラの座標をセット
	CameraEyePosition = VGet(-2605.0f, 670.0f, -2561.0f);
	
	// ライトの向きをセット
	SetLightDirection(VGet(1.0f, -1.0f, -1.0f));
	
	int i;

	// 鏡に映る映像の取得に使用するスクリーンの作成
	for (i = 0; i < MIRROR_NUM; i++)
	{
		MirrorHandle[i] = MakeScreen(MIRROR_SCREEN_W, MIRROR_SCREEN_H, FALSE);
	}

}

void Mirror::Mirror_Update(void)
{
	// カメラの向いている方向を算出
	CameraDirection.x = cos(CameraAngle * 3.14159f / 180.0f);
	CameraDirection.y = 0.0f;
	CameraDirection.z = sin(CameraAngle * 3.14159f / 180.0f);
	// カメラの注視点座標を算出
	CameraTargetPosition = VAdd(CameraEyePosition, CameraDirection);
}

// 鏡に映る映像を描画するためのカメラの設定を行う
void Mirror::Mirror_SetupCamera(int MirrorNo, VECTOR CameraEyePos, VECTOR CameraTargetPos)
{
	int i;
	float EyeLength, TargetLength;
	VECTOR MirrorNormal;
	VECTOR* MirrorWorldPosP;
	VECTOR MirrorCameraEyePos, MirrorCameraTargetPos;

	MirrorWorldPosP = MirrorWorldPos[MirrorNo];

	// 鏡の面の法線を算出
	MirrorNormal = VNorm(VCross(VSub(MirrorWorldPosP[1], MirrorWorldPosP[0]), VSub(MirrorWorldPosP[2], MirrorWorldPosP[0])));

	// 鏡の面からカメラの座標までの最短距離、鏡の面からカメラの注視点までの最短距離を算出
	EyeLength = Plane_Point_MinLength(MirrorWorldPosP[0], MirrorNormal, CameraEyePos);
	TargetLength = Plane_Point_MinLength(MirrorWorldPosP[0], MirrorNormal, CameraTargetPos);

	// 鏡に映る映像を描画する際に使用するカメラの座標とカメラの注視点を算出
	MirrorCameraEyePos = VAdd(CameraEyePos, VScale(MirrorNormal, -EyeLength * 2.0f));
	MirrorCameraTargetPos = VAdd(CameraTargetPos, VScale(MirrorNormal, -TargetLength * 2.0f));

	// 計算で得られたカメラの座標とカメラの注視点の座標をカメラの設定する
	SetCameraPositionAndTarget_UpVecY(MirrorCameraEyePos, MirrorCameraTargetPos);

	// 鏡に映る映像の中での鏡の四隅の座標を算出( 同次座標 )
	for (i = 0; i < 4; i++)
	{
		MirrorScreenPosW[MirrorNo][i] = ConvWorldPosToScreenPosPlusW(MirrorWorldPosP[i]);
	}
}

// 鏡の描画
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

	// 鏡の描画に使用するマテリアルのセットアップ
	Material.Ambient = MirrorAmbientColor[MirrorNo];
	Material.Diffuse = GetColorF(0.0f, 0.0f, 0.0f, 0.0f);
	Material.Emissive = GetColorF(0.0f, 0.0f, 0.0f, 0.0f);
	Material.Specular = GetColorF(0.0f, 0.0f, 0.0f, 0.0f);
	Material.Power = 1.0f;
	SetMaterialParam(Material);

	// 鏡の面の法線を算出
	MirrorNormal = VNorm(VCross(VSub(MirrorWorldPosP[1], MirrorWorldPosP[0]), VSub(MirrorWorldPosP[2], MirrorWorldPosP[0])));

	// 鏡に映る映像を書き込んだ画像のテクスチャのサイズを取得
	GetGraphTextureSize(MirrorHandle[MirrorNo], &TextureW, &TextureH);

	// 鏡の描画に使用する頂点のセットアップ
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

	// 鏡の描画に使用する頂点インデックスをセットアップ
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
	// 描画モードをバイリニアフィルタリングに設定
	SetDrawMode(DX_DRAWMODE_BILINEAR);

	// 描画ブレンドモードを変更
	SetDrawBlendMode(MirrorBlendParam[MirrorNo][0], MirrorBlendParam[MirrorNo][1]);

	// 鏡を描画
	DrawPolygonIndexed3D(Vert, MIRROR_POINTNUM * MIRROR_POINTNUM, Index, (MIRROR_POINTNUM - 1) * (MIRROR_POINTNUM - 1) * 2, MirrorHandle[MirrorNo], FALSE);

	// 描画ブレンドモードを元に戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	// 描画モードを元に戻す
	SetDrawMode(DX_DRAWMODE_NEAREST);
}
// 鏡のデバッグ描画
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

	// 指定の座標に鏡の映像の取得に使用したスクリーンを描画
	DrawExtendGraph(DrawX, DrawY, DrawX + MIRROR_SCREEN_W / MIRROR_DEBUG_SCALE, DrawY + MIRROR_SCREEN_H / MIRROR_DEBUG_SCALE, MirrorHandle[MirrorNo], FALSE);

	// 鏡の映像の取得に使用したスクリーンの中の鏡の部分に点を描画
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

// ３Ｄ空間の描画
void Mirror::World_RenderProcess()
{
	// ステージモデルの描画
	//MV1DrawModel(StageModel);

	// キャラクターモデルの描画
	//MV1DrawModel(CharaModel);
}

void Mirror::MirrorDraw()
{

	// 鏡に映る映像を描画
	for (int i = 0; i < MIRROR_NUM; i++)
	{
		// 描画先を鏡に映る映像の取得に使用する画像を描画するスクリーンにする
		SetDrawScreen(MirrorHandle[i]);

		// 画面を初期化する
		ClearDrawScreen();

		// 鏡に映る映像を描画する際に使用するカメラの設定を行う
		Mirror_SetupCamera(i, CameraEyePosition, CameraTargetPosition);

		// ３Ｄ空間の描画
		World_RenderProcess();
	}
}

void Mirror::MirrorDraw2()
{
	


	// 鏡の描画
	for (int i = 0; i < MIRROR_NUM; i++)
	{
		Mirror_Render(i);
	}


	// 画面左上に鏡のデバッグ表示
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
