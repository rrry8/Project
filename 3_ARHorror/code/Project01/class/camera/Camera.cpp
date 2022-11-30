#include<DxLib.h>
#include"../Scene/SceneManager.h"
#include "Camera.h"

//注視点からカメラまでの距離
constexpr float TARGET2CAMERA_DISTANCE = 500.0f;

//X軸回転時の制限角度
constexpr float UP_RADIAN_LIMIT_X  = -70.0f * (DX_PI_F / 180.0f);	//上方向
constexpr float DOWN_RADIAN_LIMIT_X = 60.0f * (DX_PI_F / 180.0f);	//下方向 60

//カメラの描画距離
constexpr float CAMERA_RANGE_NEAR = 50.0f;		//手前
constexpr float CAMERA_RANGE_FAR  = 25000.0f;	//奥

//画面揺れ
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
	//位置
	pos_ = { 0.0f,CAMERA_HEIGHT,-TARGET2CAMERA_DISTANCE };

	//角度
	angle_ = {
		0.0f,
		0.0f,
		0.0f
	};
	quaRot_ = Quaternion();

	sinCnt_ = 0;

	// クリップ距離の設定
	SetCameraNearFar(CAMERA_RANGE_NEAR, CAMERA_RANGE_FAR);

	//マウスの位置を真ん中に固定
	SetMousePoint(lpSceneMng.GetScreenSize().x / 2, lpSceneMng.GetScreenSize().y / 2);
}

void Camera::Update(bool dashFlag)
{
	Vector2Int middleMousePos = { lpSceneMng.GetScreenSize().x / 2, lpSceneMng.GetScreenSize().y / 2 };
	//マウスの位置を取得
	GetMousePoint(&mousePos_.x, &mousePos_.y);

	//マウスの移動距離を計算
	Vector2 mouseMoveDistance = { static_cast<float>(mousePos_.x - middleMousePos.x),static_cast<float>(mousePos_.y - middleMousePos.y) };

	//マウス位置の差分で回転させる
	angle_.y += Deg2RadF(mouseMoveDistance.x / 30.0f);	//左右回転
	angle_.x += Deg2RadF(mouseMoveDistance.y / 30.0f);	//上下回転

	//上方向の角度制限
	if (angle_.x < UP_RADIAN_LIMIT_X)
	{
		angle_.x = UP_RADIAN_LIMIT_X;
	}

	//下方向の角度制限
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

	//マウスの位置を真ん中に固定
	SetMousePoint(middleMousePos.x, middleMousePos.y);
}

void Camera::Update(Vector3 targetPos)
{

}

void Camera::SetBeforeDraw(Vector3 playerpos)
{
	// クリップ距離の設定
	SetCameraNearFar(CAMERA_RANGE_NEAR, CAMERA_RANGE_FAR);

	//カメラの移動
	//プレイヤーの位置をカメラ座標に設定
	pos_ = playerpos;
	//pos_.z -= 2200.0f;
	pos_.y = playerpos.y + CAMERA_HEIGHT;

	//注視点の移動
	//クォータニオンに変換
	quaRot_ = Quaternion::Euler(angle_.toVECTOR());
	//座標を回転(カメラからの相対座標)
	Vector3 localPos = ConvertVECTORtoVector3(quaRot_.PosAxis(VGet(0.0f, 0.0f, TARGET2CAMERA_DISTANCE)));
	targetPos_ = pos_ + localPos;

	//カメラの位置と注視点の設定を反映
	SetCameraPositionAndTarget_UpVecY(
		pos_.toVECTOR(),
		targetPos_.toVECTOR());
}

void Camera::Draw(void)
{
	//デバッグ表示
	auto xdeg = Rad2DegF(angle_.x);
	auto ydeg = Rad2DegF(angle_.y);
	auto zdeg = Rad2DegF(angle_.z);
	//DrawSphere3D(VGet(pos_.x,pos_.y - CAMERA_HEIGHT,pos_.z), 50.0f, 100, 0xff00ff, 0xff00ff, true);
	DrawFormatString(600, 0, 0xffffff,  "カメラ座標 : (%.1f, %.1f, %.1f)", pos_.x,pos_.y,pos_.z);
	//DrawFormatString(0, 32, 0xffffff, "注視点座標 : (%.1f, %.1f, %.1f)", targetPos_.x, targetPos_.y, targetPos_.z);
	DrawFormatString(0, 64, 0xffffff, "カメラ角度 : (%.1f, %.1f, %.1f)", xdeg, ydeg,zdeg);
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
