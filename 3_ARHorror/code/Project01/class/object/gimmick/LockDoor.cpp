#include "LockDoor.h"
#include "../../common/AnimationController.h"
#include "../../scene/SceneManager.h"
#include "../Item/Item.h"

LockDoor::LockDoor(Vector3 pos, Vector3 angle, std::string name, int num,ItemID itemID)
{
	pos_ = pos;
	angle_ = angle;
	name_ = name;
	num_ = num;
	itemID_ = itemID;
	Init();
}

LockDoor::~LockDoor()
{
}

void LockDoor::Init(void)
{
	//オブジェクト名をチェックし使用するモデルパスを指定
	CheckName();

	//座標を設定
	MV1SetPosition(lpModelMng.GetID(path_)[num_], pos_.toVECTOR());
	
	//拡大値を設定
	MV1SetScale(lpModelMng.GetID(path_)[num_], VECTOR(scale_.toVECTOR()));
	
	//度数からラジアンに変換
	angle_ = { Deg2RadF(angle_.x),Deg2RadF(angle_.y),Deg2RadF(angle_.z) };
	
	//回転
	MV1SetRotationXYZ(lpModelMng.GetID(path_)[num_], angle_.toVECTOR());
	state_ = ANIM_GIMMICK::CLOSE;

}

void LockDoor::Update(void)
{
	//インタラクトされたら
	if (interactFlg_)
	{
		if (!isLock_)
		{
			//アニメーション再生
			isPlayFlg_ = true;
			interactFlg_ = false;
		}
	}
	//アニメーション処理
	Animation();
}

void LockDoor::Draw(void)
{
	//描画
	MV1DrawModel(lpModelMng.GetID(path_)[num_]);
}

void LockDoor::Release(void)
{
	//解放
	//MV1DeleteModel(lpModelMng.GetID("./resource/model/gimmick/LockDoor.mv1")[num_]);
}

int LockDoor::GetModelNumber(void)
{
	return num_;
}

std::string LockDoor::GetModelString(void)
{
	return path_;
}


void LockDoor::Animation(void)
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

void LockDoor::CheckName(void)
{
	std::string directory = "./resource/model/gimmick/";

	if (name_ == "White_A_Double")
	{
		path_ = directory + "Door_White1_Double.mv1";
		//アニメーション追加
		animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(path_)[num_]);
		animController_->Add((int)ANIM_GIMMICK::OPEN, directory +"anim/DoorDoubleOpen.mv1", 40.0f);
		animController_->Add((int)ANIM_GIMMICK::CLOSE, directory + "anim/DoorDoubleClose.mv1", 40.0f);
		//拡大値を設定
		scale_ = { 10.7f,8.5f,10.0f };
	}
	else if (name_ == "Red_A")
	{
		path_ = directory + "Door_Red1.mv1";
		//アニメーション追加
		animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(path_)[num_]);
		animController_->Add((int)ANIM_GIMMICK::OPEN, directory + "anim/DoorOpen.mv1", 40.0f);
		animController_->Add((int)ANIM_GIMMICK::CLOSE, directory + "anim/DoorClose.mv1", 40.0f);
		//拡大値を設定
		scale_ = { 12.0f,8.8f,10.0f };
	}
	else if (name_ == "Dark_B")
	{
		path_ = directory + "Door_Dark2.mv1";
		//アニメーション追加
		animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(path_)[num_]);
		animController_->Add((int)ANIM_GIMMICK::OPEN, directory + "anim/DoorOpen.mv1", 40.0f);
		animController_->Add((int)ANIM_GIMMICK::CLOSE, directory + "anim/DoorClose.mv1", 40.0f);
		//拡大値を設定
		scale_ = { 12.0f,8.8f,10.0f };
	}
	else if (name_ == "Dark_B_Double")
	{
		path_ = directory + "Door_Dark2_Double.mv1";
		//アニメーション追加
		animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(path_)[num_]);
		animController_->Add((int)ANIM_GIMMICK::OPEN, directory + "anim/DoorDoubleOpen.mv1", 40.0f);
		animController_->Add((int)ANIM_GIMMICK::CLOSE, directory + "anim/DoorDoubleClose.mv1", 40.0f);
		//拡大値を設定
		scale_ = { 10.7f,8.5f,10.0f };
	}
	else if (name_ == "Dark_A")
	{
		path_ = directory + "Door_Dark1.mv1";
		//アニメーション追加
		animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(path_)[num_]);
		animController_->Add((int)ANIM_GIMMICK::OPEN, directory + "anim/DoorOpen.mv1", 40.0f);
		animController_->Add((int)ANIM_GIMMICK::CLOSE, directory + "anim/DoorClose.mv1", 40.0f);
		//拡大値を設定
		scale_ = { 12.0f,8.8f,10.0f };
	}

}
