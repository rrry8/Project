#include "DrawerFrame.h"
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="座標"></param>
/// <param name="角度"></param>
/// <param name="オブジェクト名"></param>
/// <param name="複製数"></param>
DrawerFrame::DrawerFrame(Vector3 pos, Vector3 angle,std::string name,int num)
{
	pos_ = pos;
	angle_ = angle;
	name_ = name;
	num_ = num;
	Init();
}

DrawerFrame::~DrawerFrame()
{
}

void DrawerFrame::Init(void)
{
	path_ = "./resource/model/gimmick/drawer_frame_L.mv1";
	//座標を設定
	MV1SetPosition(lpModelMng.GetID(path_)[num_], pos_.toVECTOR());
	
	//拡大値設定
	scale_ = { 10.0f,10.0f,10.0f };
	MV1SetScale(lpModelMng.GetID(path_)[num_], VECTOR(scale_.toVECTOR()));
	
	//度数空ラジアンに変換
	angle_ = { Deg2RadF(angle_.x),Deg2RadF(angle_.y),Deg2RadF(angle_.z) };
	
	//回転
	MV1SetRotationXYZ(lpModelMng.GetID(path_)[num_], angle_.toVECTOR());
}

void DrawerFrame::Update(void)
{
}

void DrawerFrame::Draw(void)
{
	MV1DrawModel(lpModelMng.GetID(path_)[num_]);
}

void DrawerFrame::Release(void)
{
	//MV1DeleteModel(lpModelMng.GetID("./resource/model/gimmick/drawer_frame_L.mv1")[num_]);

}

int DrawerFrame::GetModelNumber(void)
{
	return num_;
}

std::string DrawerFrame::GetModelString(void)
{
	return path_;
}

