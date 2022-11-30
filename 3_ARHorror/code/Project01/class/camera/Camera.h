#pragma once
#include<DxLib.h>
#include<memory>
#include"../Common/Geometry.h"
#include"../common/Quaternion.h"

//カメラの高さ(仮)
constexpr float CAMERA_HEIGHT = 1250.0f;

class Camera
{
public:
	Camera();
	~Camera();

	//初期化
	void Init(void);

	//更新
	void Update(bool dashFlag);
	void Update(Vector3 targetPos);//ゲームオーバー時の演出用
	//追従カメラ(初期化されるのでオブジェクト描画前にする必要がある)
	void SetBeforeDraw(Vector3 playerpos);

	//描画
	void Draw(void);

	//カメラ座標を返す
	Vector3 GetPos();

	//注視点を返す
	Vector3 GetTargetPos();

	//角度を返す
	Vector3 GetAngles();

	//画面揺れ
	float GetSwingWidth(void);

private:
	//カメラの位置
	Vector3 pos_;

	//カメラ角度(rad)
	Vector3 angle_;

	//角度(クォータニオン)
	Quaternion quaRot_;

	//注視点
	Vector3 targetPos_;

	Vector2Int mousePos_;

	int sinCnt_;

};
