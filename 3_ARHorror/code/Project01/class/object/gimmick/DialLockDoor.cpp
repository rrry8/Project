#include "DialLockDoor.h"
#include "../../common/AnimationController.h"
#include "../../scene/SceneManager.h"

DialLockDoor::DialLockDoor(Vector3 pos, Vector3 angle, std::string name, int num,std::string unlockNum)
{
	pos_ = pos;
	angle_ = angle;
	name_ = name;
	num_ = num;
	unlockNum_ = unlockNum;
	Init();
}

DialLockDoor::~DialLockDoor()
{
}

void DialLockDoor::Init(void)
{

	//モデルパス
	path_ = "./resource/model/gimmick/Door_Red2.mv1";
	
	//アニメーション追加
	animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(path_)[num_]);
	animController_->Add((int)ANIM_GIMMICK::OPEN, "./resource/model/gimmick/anim/DoorOpen.mv1", 50.0f);
	animController_->Add((int)ANIM_GIMMICK::CLOSE, "./resource/model/gimmick/anim/DoorClose.mv1", 50.0f);

	//座標を設定
	MV1SetPosition(lpModelMng.GetID(path_)[num_], pos_.toVECTOR());

	//拡大値を設定
	scale_ = { 12.0f,8.8f,10.0f };
	MV1SetScale(lpModelMng.GetID(path_)[num_], VECTOR(scale_.toVECTOR()));

	//度数からラジアンに変換
	angle_ = { Deg2RadF(angle_.x),Deg2RadF(angle_.y),Deg2RadF(angle_.z) };
	//回転
	MV1SetRotationXYZ(lpModelMng.GetID(path_)[num_], angle_.toVECTOR());
	state_ = ANIM_GIMMICK::CLOSE;

}

void DialLockDoor::Update(void)
{
	if (interactFlg_)
	{
		isPlayFlg_ = true;
		interactFlg_ = false;
	}
	Animation();
}

void DialLockDoor::Draw(void)
{
	//描画
	MV1DrawModel(lpModelMng.GetID(path_)[num_]);
}

void DialLockDoor::Release(void)
{
}

int DialLockDoor::GetModelNumber(void)
{
	return num_;
}

std::string DialLockDoor::GetModelString(void)
{
	return path_;
}

void DialLockDoor::Animation(void)
{
	//アニメーションの状態を受け取る
	ANIM_STATE animState = animController_->GetPlayState();

	//ロックが解除
	if (!isLock_)
	{
		//再生中
		if (isPlayFlg_)
		{
			//閉まっていたら
			if (state_ == ANIM_GIMMICK::CLOSE)
			{
				//開ける
				animController_->Play((int)ANIM_GIMMICK::OPEN, false);
				//アニメーションが止まったら
				if (animState == ANIM_STATE::STOPPING)
				{
					//開いている状態に切り替える
					state_ = ANIM_GIMMICK::OPEN;
					//再生フラグを止める
					isPlayFlg_ = false;
				}
			}
			else if (state_ == ANIM_GIMMICK::OPEN)	//開いていたら
			{
				//閉じる
				animController_->Play((int)ANIM_GIMMICK::CLOSE, false);
				//アニメーションが止まったら
				if (animState == ANIM_STATE::STOPPING)
				{
					//閉じている状態に切り替える
					state_ = ANIM_GIMMICK::CLOSE;
					//再生フラグを止める
					isPlayFlg_ = false;
				}
			}
		}
	}
	if (animState == ANIM_STATE::STOPPING)	//停止中
	{
		//当たり判定情報更新
		MV1RefreshCollInfo(lpModelMng.GetID(path_)[num_]);
	}
	//アニメーション更新処理
	animController_->Update(lpSceneMng.GetDeltaTime());
}
