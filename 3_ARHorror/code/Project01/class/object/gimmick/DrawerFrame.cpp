#include "DrawerFrame.h"
/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="���W"></param>
/// <param name="�p�x"></param>
/// <param name="�I�u�W�F�N�g��"></param>
/// <param name="������"></param>
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
	//���W��ݒ�
	MV1SetPosition(lpModelMng.GetID(path_)[num_], pos_.toVECTOR());
	
	//�g��l�ݒ�
	scale_ = { 10.0f,10.0f,10.0f };
	MV1SetScale(lpModelMng.GetID(path_)[num_], VECTOR(scale_.toVECTOR()));
	
	//�x���󃉃W�A���ɕϊ�
	angle_ = { Deg2RadF(angle_.x),Deg2RadF(angle_.y),Deg2RadF(angle_.z) };
	
	//��]
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

