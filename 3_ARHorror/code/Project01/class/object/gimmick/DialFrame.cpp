#include "DialFrame.h"

DialFrame::DialFrame(Vector3 pos, Vector3 angle, std::string name, int num)
{
    pos_ = pos;
    angle_ = angle;
    name_ = name;
    num_ = num;
    Init();
}

DialFrame::~DialFrame()
{
}

void DialFrame::Init(void)
{
	//モデルパス
	path_ = "./resource/model/gimmick/Dial_Frame.mv1";
	//位置設定
	MV1SetPosition(lpModelMng.GetID(path_)[num_], pos_.toVECTOR());
	//拡大値設定
	scale_ = { 0.5f,0.5f,0.5f };
	MV1SetScale(lpModelMng.GetID(path_)[num_], VECTOR(scale_.toVECTOR()));
	//度数からラジアンに変換
	angle_ = { Deg2RadF(angle_.x),Deg2RadF(angle_.y),Deg2RadF(angle_.z) };
	//回転
	MV1SetRotationXYZ(lpModelMng.GetID(path_)[num_], angle_.toVECTOR());
}

void DialFrame::Update(void)
{
}

void DialFrame::Draw(void)
{
	MV1DrawModel(lpModelMng.GetID(path_)[num_]);
}

void DialFrame::Release(void)
{
}

int DialFrame::GetModelNumber(void)
{
	return num_;
}

std::string DialFrame::GetModelString(void)
{
    return path_;
}
