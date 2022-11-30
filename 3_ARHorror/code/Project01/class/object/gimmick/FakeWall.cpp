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

	//���W��ݒ�
	MV1SetPosition(lpModelMng.GetID(path_)[num_], pos_.toVECTOR());

	//�g��l�ݒ�
	scale_ = { 1.0f,1.0f,1.0f };
	MV1SetScale(lpModelMng.GetID(path_)[num_], VECTOR(scale_.toVECTOR()));

	//�x���󃉃W�A���ɕϊ�
	angle_ = { Deg2RadF(angle_.x),Deg2RadF(angle_.y),Deg2RadF(angle_.z) };

	//��]
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
