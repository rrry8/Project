#include "Key.h"
#include "../../common/manager/ModelMng.h"

Key::Key(Vector3 pos, ItemID keyid,int num)
{
	pos_ = pos;
	itemId_ = keyid;
	modelNum_ = num;
	Init();
}

Key::~Key()
{
}

void Key::Init(void)
{
	scale_ = 20.0;
	//モデルをロード
	modelString_ = "./resource/model/Item/old_key.mv1";
	//modelH_ = MV1LoadModel("./resource/model/Item/old_key.mv1");
	//lpModelMng.GetID("./resource/model/Item/old_key.mv1");
	//拡大値を設定
	MV1SetScale(lpModelMng.GetID(modelString_)[modelNum_], VECTOR(scale_, scale_, scale_));
	//拡大分座標を増やす
	//pos_ *= scale_;
	//配置
	MV1SetPosition(lpModelMng.GetID(modelString_)[modelNum_], pos_.toVECTOR());
}

void Key::Update(void)
{
}

void Key::Draw(void)
{
	if (!pickupFlg_) 
	{
		MV1DrawModel(lpModelMng.GetID(modelString_)[modelNum_]);
	}
}

void Key::Draw(bool cameraflag)
{
	/*if (!cameraflag)
	{
		if (itemId_ == ItemID::Key1)
		{
			return;
		}
	}*/
	if (!pickupFlg_ && cameraflag)
	{
		MV1DrawModel(lpModelMng.GetID(modelString_)[modelNum_]);
	}
}

void Key::Release(void)
{
	//解放
	//MV1DeleteModel(lpModelMng.GetID(modelString_)[0]);
}
