#include "FakeWall.h"

FakeWall::FakeWall(Vector3 pos, Vector3 angle, std::string name, int num, ItemID itemID)
{
	pos_ = pos;
	angle_ = angle;
	name_ = name;
	num_ = num;
	itemID_ = itemID;
	Init();
}

FakeWall::~FakeWall()
{
}

void FakeWall::Init(void)
{
	isLock_ = true;
	path_ = "./resource/model/gimmick/Fake_Wall.mv1";

	//座標を設定
	MV1SetPosition(lpModelMng.GetID(path_)[num_], pos_.toVECTOR());

	//拡大値設定
	scale_ = { 1.0f,1.0f,1.0f };
	MV1SetScale(lpModelMng.GetID(path_)[num_], VECTOR(scale_.toVECTOR()));

	//度数空ラジアンに変換
	angle_ = { Deg2RadF(angle_.x),Deg2RadF(angle_.y),Deg2RadF(angle_.z) };

	//回転
	MV1SetRotationXYZ(lpModelMng.GetID(path_)[num_], angle_.toVECTOR());
}

void FakeWall::Update(void)
{
}

void FakeWall::Draw(void)
{
	if (isLock_)
	{
		MV1DrawModel(lpModelMng.GetID(path_)[num_]);
	}
}

void FakeWall::Release(void)
{
}

int FakeWall::GetModelNumber(void)
{
    return num_;
}

std::string FakeWall::GetModelString(void)
{
    return path_;
}
