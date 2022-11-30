#pragma once
#include <array>
#include"Geometry.h"

class OriginalShader
{
public:

	/// <summary>
	/// インスタンス2D用
	/// </summary>
	/// <param name="pso">ピクセルシェーダー</param>
	OriginalShader(const int pso);
	/// <summary>
	/// インスタンス3D用
	/// </summary>
	/// <param name="vso">頂点シェーダー</param>
	/// <param name="pso">ピクセルシェーダー</param>
	OriginalShader(const int vso, const int pso);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~OriginalShader();



	/// <summary>
	/// シェーダーを使った描画 (モデル単体)
	/// </summary>
	/// <param name="model">モデル</param>
	void Draw(int model)const;

	/// <summary>
	/// シェーダーを使った描画
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="tex">テクスチャマップ</param>
	void Draw(int model, int tex)const;

	/// <summary>
	/// シェーダーを使った描画
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="tex">テクスチャマップ</param>
	/// <param name="norm">法線マップ</param>
	void Draw(int model, int tex, int norm)const;

	/// <summary>
	/// シェーダーを使った描画
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="tex">テクスチャマップ</param>
	/// <param name="norm">法線マップ</param>
	/// <param name="rough"></param>
	/// <param name="metallic">メタリック</param>
	/// <param name="sphmap">スフィアマップ</param>
	void Draw(int model, int tex, int norm, int rough, int metallic, int sphmap)const;

	/// <summary>
	/// ポストエフェクト描画用
	/// </summary>
	/// <param name="x">画像の左上のX</param>
	/// <param name="y">画像の左上のY</param>
	/// <param name="img">画像ハンドル</param>
	/// <param name="tex1">シェーダで使うテクスチャ１</param>
	/// <param name="tex2">シェーダで使うテクスチャ２</param>
	/// <param name="tex3">シェーダで使うテクスチャ３</param>
	void DrawOnlyPixel(float x, float y, int img, int tex1, int tex2, int tex3);

	//メモ用写真関数
	void DrawOnlyPixel(const std::array<Vector3, 4>& pos, int img);
private:

	//変数
	//使う頂点シェーダーの格納場所
	int vertexShader_;
	//使うピクセルシェーダーの格納場所
	int pixelShader_;
};