#include <DxLib.h>
#include "Drawer.h"
#include "../../scene/SceneManager.h"
#include "../../common/AnimationController.h"
#include "../../scene/SceneManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="座標"></param>
/// <param name="角度"></param>
/// <param name="オブジェクト名"></param>
/// <param name="複製数"></param>
Drawer::Drawer(Vector3 pos, Vector3 angle, std::string name, int num)
{
	pos_ = pos;
	angle_ = angle;
	name_ = name;
	num_ = num;
	Init();
}

Drawer::~Drawer()
{
}

//初期化
void Drawer::Init(void)
{
	path_ = "./resource/model/gimmick/drawer.mv1";
	//アニメーション追加
	animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(path_)[num_]);
	animController_->Add((int)ANIM_GIMMICK::OPEN, "./resource/model/gimmick/anim/DrawerOpen.mv1", 20.0f);
	animController_->Add((int)ANIM_GIMMICK::CLOSE, "./resource/model/gimmick/anim/DrawerClose.mv1", 20.0f);
	
	//座標を設定
	MV1SetPosition(lpModelMng.GetID(path_)[num_], pos_.toVECTOR());
	
	//拡大値を設定
	scale_ = { 10.0f,10.0f,10.0f };
	MV1SetScale(lpModelMng.GetID(path_)[num_], VECTOR(scale_.toVECTOR()));
	
	//度数からラジアンに変換
	angle_ = { Deg2RadF(angle_.x),Deg2RadF(angle_.y),Deg2RadF(angle_.z) };
	
	//回転
	MV1SetRotationXYZ(lpModelMng.GetID(path_)[num_], angle_.toVECTOR());

	state_ = ANIM_GIMMICK::CLOSE;

}

//更新
void Drawer::Update(void)
{
	//インタラクトされたら
	if (interactFlg_)
	{
		//再生する
		isPlayFlg_ = true;
		interactFlg_ = false;
	}
	//アニメーション処理
	Animation();
	//当たり判定情報更新
	MV1RefreshCollInfo(lpModelMng.GetID(path_)[num_]);
}

//描画
void Drawer::Draw(void)
{
	//描画
	MV1DrawModel(lpModelMng.GetID(path_)[num_]);
}

//開放
void Drawer::Release(void)
{
	//MV1DeleteModel(lpModelMng.GetID("./resource/model/gimmick/drawer.mv1")[num_]);
}

int Drawer::GetModelNumber(void)
{
	return num_;
}

std::string Drawer::GetModelString(void)
{
	return path_;
}

void Drawer::Animation(void)
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
			if (animState ==ANIM_STATE::STOPPING)
			{
				//開いている状態に切り替える
				state_ = ANIM_GIMMICK::CLOSE;
				//再生フラグをを止める
				isPlayFlg_ = false;
			}
		}
	}

	//アニメーション更新処理
	animController_->Update(lpSceneMng.GetDeltaTime());
}
