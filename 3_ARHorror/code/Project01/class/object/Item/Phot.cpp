#include "Phot.h"
#include "../../common/manager/ModelMng.h"
Phot::Phot(Vector3 pos)
{
	modelNum_ = 0;
	pos_ = pos;
	Init();
}

Phot::~Phot()
{

}

void Phot::Init(void)
{
	scale_ = 1.0;
	//モデルをロード
	//modelH_ = MV1LoadModel("./resource/model/Item/Phot.mv1");
	//modelH_ = lpModelMng.GetID("./resource/model/marker/newspaper.mv1")[0];
	modelString_ = "./resource/model/marker/newspaper.mv1";

	lpModelMng.GetID(modelString_);
	//拡大値を設定
	MV1SetScale(lpModelMng.GetID(modelString_)[0], VECTOR(scale_, scale_, scale_));
	//拡大分座標を増やす
	//pos_ *= scale_;
	pos_.y += 800;

	//配置
	MV1SetPosition(lpModelMng.GetID(modelString_)[0], pos_.toVECTOR());
}

void Phot::Update(void)
{
}

void Phot::Draw(void)
{
	if (!pickupFlg_)
	{
		MV1DrawModel(lpModelMng.GetID(modelString_)[0]);
	}

}

void Phot::Draw(bool cameraflag)
{
	if (!pickupFlg_)
	{
		MV1DrawModel(lpModelMng.GetID(modelString_)[0]);
	}
}

void Phot::Release(void)
{
	//解放
	//MV1DeleteModel(lpModelMng.GetID(modelString_)[0]);
}
