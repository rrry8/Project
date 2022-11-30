#include "Door.h"
#include "../../common/AnimationController.h"
#include "../../Scene/SceneManager.h"

using namespace std;

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="座標"></param>
/// <param name="角度"></param>
/// <param name="オブジェクト名"></param>
/// <param name="複製数"></param>
Door::Door(Vector3 pos, Vector3 angle, std::string name, int num)
{
	pos_ = pos;
	angle_ = angle;
	name_ = name;
	num_ = num;
	Init();
}

Door::~Door()
{
}

void Door::Init(void)
{
	if (name_ == "Dark_A_Double")
	{
		path_ = "./resource/model/gimmick/Door_Dark1_Double.mv1";

		//アニメーション追加
		animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(path_)[num_]);
		animController_->Add((int)ANIM_GIMMICK::OPEN, "./resource/model/gimmick/anim/DoorDoubleOpen.mv1", 50.0f);
		animController_->Add((int)ANIM_GIMMICK::CLOSE, "./resource/model/gimmick/anim/DoorDoubleClose.mv1", 50.0f);
		scale_ = { 10.7f,8.5f,10.0f };
	}
	else
	{
		path_ = "./resource/model/gimmick/Door_Dark1.mv1";

		//アニメーション追加
		animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(path_)[num_]);
		animController_->Add((int)ANIM_GIMMICK::OPEN, "./resource/model/gimmick/anim/DoorOpen.mv1", 50.0f);
		animController_->Add((int)ANIM_GIMMICK::CLOSE, "./resource/model/gimmick/anim/DoorClose.mv1", 50.0f);
		scale_ = { 12.0f,8.8f,10.0f };
	}
	
	//座標を設定
	MV1SetPosition(lpModelMng.GetID(path_)[num_],pos_.toVECTOR());
	
	//拡大値を設定
	MV1SetScale(lpModelMng.GetID(path_)[num_], VECTOR(scale_.toVECTOR()));
	
	//度数からラジアンに変換
	angle_ = { Deg2RadF(angle_.x),Deg2RadF(angle_.y),Deg2RadF(angle_.z) };
	//回転
	MV1SetRotationXYZ(lpModelMng.GetID(path_)[num_], angle_.toVECTOR());
	state_ = ANIM_GIMMICK::CLOSE;
}

void Door::Update(void)
{
	if (interactFlg_)
	{
		isPlayFlg_ = true;
		interactFlg_ = false;
	}
	//アニメーション処理
	Animation();
}

void Door::Draw(void)
{
	//描画
	MV1DrawModel(lpModelMng.GetID(path_)[num_]);
	//MV1DrawModelDebug(lpModelMng.GetID("./resource/model/gimmick/Door_White.mv1")[num_],0xffffff,1,0.001,1,1);
}

void Door::Release(void)
{
	//解放
	//MV1DeleteModel(lpModelMng.GetID("./resource/model/gimmick/DoorDouble.mv1")[num_]);
}

int Door::GetModelNumber(void)
{
	return num_;
}

std::string Door::GetModelString(void)
{
	return path_;
}

void Door::Animation(void)
{
	//アニメーションの状態を受け取る
	ANIM_STATE animState = animController_->GetPlayState();

	//再生中
	if (isPlayFlg_)
	{
		//閉じているとき
		if (state_ == ANIM_GIMMICK::CLOSE)
		{
			//開ける
			animController_->Play((int)ANIM_GIMMICK::OPEN, false);
			//アニメーションが停止したら
			if (animState == ANIM_STATE::STOPPING)
			{
				//開いている状態に切り替える
				state_ = ANIM_GIMMICK::OPEN;
				//再生フラグをを止める
				isPlayFlg_ = false;
			}
		}
		else if (state_ == ANIM_GIMMICK::OPEN)	//閉じているとき
		{
			//閉じる
			animController_->Play((int)ANIM_GIMMICK::CLOSE, false);
			//アニメーションが停止したら
			if (animState == ANIM_STATE::STOPPING)
			{
				//開いている状態に切り替える
				state_ = ANIM_GIMMICK::CLOSE;
				//再生フラグをを止める
				isPlayFlg_ = false;
			}
		}
	}
	if (animState == ANIM_STATE::STOPPING)
	{
		//当たり判定情報更新
		MV1RefreshCollInfo(lpModelMng.GetID(path_)[num_]);
	}
	//アニメーション更新処理
	animController_->Update(lpSceneMng.GetDeltaTime());
}


