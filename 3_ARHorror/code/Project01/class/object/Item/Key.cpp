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
	//���f�������[�h
	modelString_ = "./resource/model/Item/old_key.mv1";
	//modelH_ = MV1LoadModel("./resource/model/Item/old_key.mv1");
	//lpModelMng.GetID("./resource/model/Item/old_key.mv1");
	//�g��l��ݒ�
	MV1SetScale(lpModelMng.GetID(modelString_)[modelNum_], VECTOR(scale_, scale_, scale_));
	//�g�啪���W�𑝂₷
	//pos_ *= scale_;
	//�z�u
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
	//���
	//MV1DeleteModel(lpModelMng.GetID(modelString_)[0]);
}
