#include<DxLib.h>
#include "Stage.h"
#include "gimmick/Gimmick.h"
#include "gimmick/Door.h"
#include "gimmick/LockDoor.h"
#include "gimmick/DrawerFrame.h"
#include "gimmick/Drawer.h"
#include "gimmick/InnerLockDoor.h"
#include "gimmick/Dial.h"
#include "gimmick/DialFrame.h"
#include "gimmick/DialLockDoor.h"
#include "gimmick/FakeWall.h"
#include "item/Item.h"
#include "item/Phot.h"
#include "item/Key.h"
#include "../common/manager/ModelMng.h"
#include "../common/manager/ImageMng.h"
#include "../common/OriginalShader.h"

constexpr float NORMAL = 1.0f;

Stage::Stage()
{
	lpModelMng.GetID("resource/model/stage/map2.mv1");
	//stageHandle_ = MV1LoadModel("resource/model/stage/map.mv1");
	Init();
}

Stage::~Stage()
{
	Release();
}

void Stage::Init(void)
{
	//�o�C�i���f�[�^��ǂݍ��݁A�m�F�p���X�g�֒ǉ�
	Gimmick::LoadBinData(gimmickObjList_, "./resource/data/gimmick/DrawerData.dat");
	Gimmick::LoadBinData(gimmickObjList_, "./resource/data/gimmick/DoorData.dat");
	Gimmick::LoadBinData(gimmickObjList_, "./resource/data/gimmick/DialData.dat");
	Gimmick::LoadBinData(gimmickObjList_, "./resource/data/gimmick/WallData.dat");

	//�g�p����f�[�^�����M�~�b�N���X�g�֒ǉ�
	AddGimmick();
	//MV1SetPosition(stageHandle_, VGet(0, 0, 1200));
	MV1SetPosition(lpModelMng.GetID("resource/model/stage/map2.mv1")[0], VGet(0, 0, 0));
	//MV1SetScale(stage_, VGet(0.6f, 0.6f, 0.6f));

	//�o�C�i���f�[�^�ǂݍ���
	//Item::LoadBinData(photList_, "./resource/model/Item/Phot.mv1");
	//photList_.emplace_back(NULL, L"Phot1", Vector3(0,0,0), Vector3(0, 0, 0));

	//�A�C�e�����X�g�ɒǉ�
	itemList_.emplace_back(std::make_unique<Phot>(Vector3{ 13000 ,1300 ,-7200 }));
	itemList_.emplace_back(std::make_unique<Key>(Vector3{ -13717 ,2326 ,-11393 },ItemID::Key1,0));
	itemList_.emplace_back(std::make_unique<Key>(Vector3{ -11374,2326,-13257 }, ItemID::Key2,1));
	itemList_.emplace_back(std::make_unique<Key>(Vector3{ 20397 ,2326 ,-19395 }, ItemID::Key3,2));
	itemList_.emplace_back(std::make_unique<Key>(Vector3{ -13464 ,6096 ,-11360 }, ItemID::Key4,3));
	itemList_.emplace_back(std::make_unique<Key>(Vector3{ 4102 ,6096 ,-26232 }, ItemID::Key5,4));
	//�M�~�b�N���X�g�֒ǉ�
	//AddItemList<Phot>(photList_, L"Phot1");
	//�}�[�J�[���X�g�֒ǉ�
	markerList_.emplace_back(std::make_unique<ARmarker>(Vector3{ -13717.2, 2326, -11593 }, Vector3{ 0, 180, 0 },MarkerID::test));
	markerList_.emplace_back(std::make_unique<ARmarker>(Vector3{ -11574.1, 2326, -13257 }, Vector3{ 0, 270, 0 },MarkerID::red));
	markerList_.emplace_back(std::make_unique<ARmarker>(Vector3{ 20597.4, 2326, -19395.1 }, Vector3{ 0, 90, 0 },MarkerID::blue));
	markerList_.emplace_back(std::make_unique<ARmarker>(Vector3{ -13464.4, 6096, -11460.8 }, Vector3{ 0, 180, 0 },MarkerID::green));
	markerList_.emplace_back(std::make_unique<ARmarker>(Vector3{ 4102.3, 6096, -26432 }, Vector3{ 0, 180, 0 },MarkerID::yellow));
	//markerList_.emplace_back(std::make_unique<ARmarker>(Vector3{ 14000, 1000, -16000 }, Vector3{ 0, 270, 0 },MarkerID::cyan));
	int VertexShader = LoadVertexShader("resource/shader/vs/NormalModel_vs.vso");
	int PixelShader = LoadPixelShader ("resource/shader/ps/GameModel_ps.pso");
	//int PixelShader = LoadPixelShader("resource/shader/ps/renderGBuffer.pso");

	originalShader_ = std::make_unique<OriginalShader>(VertexShader, PixelShader);
}

void Stage::Release(void)
{
	//MV1DeleteModel(stageHandle_);

	//�M�~�b�N���
	for (auto& gimmick : gimmickList_)
	{
		gimmick->Release();
	}

	//�A�C�e�����
	for (auto& item : itemList_)
	{
		item->Release();
	}

	//�}�[�J�[���
	for (auto& marker : markerList_)
	{
		marker->Release();
	}

	gimmickList_.clear();
	gimmickList_.shrink_to_fit();
}

void Stage::Update(void)
{
	//�M�~�b�N�X�V
	for (auto& gimmick : gimmickList_)
	{
		gimmick->Update();
	}

	//�A�C�e���X�V
	for (auto& item : itemList_)
	{
		item->Update();
	}
	//�}�[�J�[�̍X�V
	for (auto& marker : markerList_)
	{
		marker->Update();
	}
}

void Stage::Draw(void)
{
//	MV1DrawModel(lpModelMng.GetID("resource/model/stage/map2.mv1")[0]);
//	//MV1DrawModel(lpModelMng.GetID("resource/model/stage/Coll3.mv1")[0]);
//	//MV1DrawModel(lpModelMng.GetID("resource/model/stage/Stage1.mv1")[0]);
//	//�M�~�b�N�`��
//	for (auto& gimmick : gimmickList_)
//	{
//		gimmick->Draw();
//	}
//
////�A�C�e���`��
//	for (auto& item : itemList_)
//	{
//		item->Draw();
//	}
//	//�}�[�J�[�̕`��
//	for (auto& marker : markerList_)
//	{
//		marker->Draw();
//	}
}
void Stage::Draw(bool minCameraFlg)
{
	MV1DrawModel(lpModelMng.GetID("resource/model/stage/map2.mv1")[0]);

	//originalShader_->Draw(lpModelMng.GetID("resource/model/stage/map2.mv1")[0]);

	//MV1DrawModel(lpModelMng.GetID("resource/model/stage/Coll3.mv1")[0]);
	//MV1DrawModel(lpModelMng.GetID("resource/model/stage/Stage1.mv1")[0]);
	//�M�~�b�N�`��

	for (auto& gimmick : gimmickList_)
	{
		//shader->Draw(lpModelMng.GetID(gimmick->GetModelString())[gimmick->GetModelNumber()]);
		gimmick->Draw();
	}

	//�A�C�e���`��
	
	for (auto& item : itemList_)
	{
		item->Draw(minCameraFlg);
	}
	
	//�}�[�J�[�̕`��
	for (auto& marker : markerList_)
	{
		marker->Draw();
	}
}

std::vector<std::shared_ptr<Gimmick>> Stage::GetGimmickList(void)
{
	return gimmickList_;
}

std::vector<std::shared_ptr<Item>> Stage::GetItemList(void)
{
	return itemList_;
}


void Stage::AddGimmick(void)
{
	//�g�p����f�[�^�����M�~�b�N���X�g�֒ǉ�
	//AddGimmickList<Door>(gimmickObjList_, L"DoorDouble");
	//AddGimmickList<Door>(gimmickObjList_, L"Door_Red1");
	//��
	AddGimmickList<Door>(gimmickObjList_, L"Dark_A_Double");
	//�_�C�A��
	AddGimmickList<Dial>(gimmickObjList_, L"Dial");
	AddGimmickList<DialFrame>(gimmickObjList_, L"Dial_Frame");
	//�����o��
	AddGimmickList<DrawerFrame>(gimmickObjList_, L"drawer_frame_L");
	AddGimmickList<Drawer>(gimmickObjList_, L"drawer");
	//��
	AddGimmickList<InnerLockDoor>(gimmickObjList_, L"White_B_Double",-NORMAL);
	AddGimmickList<InnerLockDoor>(gimmickObjList_, L"White_B",NORMAL);
	AddGimmickList<InnerLockDoor>(gimmickObjList_, L"White_C",-NORMAL);

	AddGimmickList<LockDoor>(gimmickObjList_, L"Dark_A",ItemID::Key4);
	AddGimmickList<LockDoor>(gimmickObjList_, L"Red_A",ItemID::Key5);
	AddGimmickList<LockDoor>(gimmickObjList_, L"White_A_Double",ItemID::Key1);
	AddGimmickList<LockDoor>(gimmickObjList_, L"Dark_B",ItemID::Key2);
	AddGimmickList<LockDoor>(gimmickObjList_, L"Dark_B_Double",ItemID::Key2);

	AddGimmickList<DialLockDoor>(gimmickObjList_, L"Red_B","1111");
	//��
	AddGimmickList<FakeWall>(gimmickObjList_, L"Gimmick_Wall",ItemID::Phot);

}

//�M�~�b�N���X�g�֒ǉ�
template<typename T>
void Stage::AddGimmickList(std::vector<GimmickObj>& gList, std::wstring objName)
{
	std::wstring wstr;
	int i = 0;
	for (auto& gimmick : gList)
	{
		wstr = Gimmick::ConvertToWstring(gimmick.objName_);
		//�o�C�i���f�[�^�̃I�u�W�F�N�g���Ǝw�肵�����O����v���Ă���Ȃ�
		if (wstr == objName)
		{
			//�M�~�b�N���X�g�ɒǉ�
			gimmickList_.emplace_back(std::make_unique<T>(gimmick.pos_,gimmick.rotate_,gimmick.objName_,i));
			i++;
		}
		else
		{
			//������玟��
			continue;
		}
	}
}

template<typename T>
void Stage::AddGimmickList(std::vector<GimmickObj>& gList, std::wstring objName,ItemID itemID)
{
	std::wstring wstr;
	int i = 0;
	for (auto& gimmick : gList)
	{
		wstr = Gimmick::ConvertToWstring(gimmick.objName_);
		//�o�C�i���f�[�^�̃I�u�W�F�N�g���Ǝw�肵�����O����v���Ă���Ȃ�
		if (wstr == objName)
		{
			//�M�~�b�N���X�g�ɒǉ�
			gimmickList_.emplace_back(std::make_unique<T>(gimmick.pos_, gimmick.rotate_, gimmick.objName_, i,itemID));
			i++;
		}
		else
		{
			//������玟��
			continue;
		}
	}
}
template<typename T>
void Stage::AddGimmickList(std::vector<GimmickObj>& gList, std::wstring objName, float normal)
{
	std::wstring wstr;
	int i = 0;
	for (auto& gimmick : gList)
	{
		wstr = Gimmick::ConvertToWstring(gimmick.objName_);
		//�o�C�i���f�[�^�̃I�u�W�F�N�g���Ǝw�肵�����O����v���Ă���Ȃ�
		if (wstr == objName)
		{
			//�M�~�b�N���X�g�ɒǉ�
			gimmickList_.emplace_back(std::make_unique<T>(gimmick.pos_, gimmick.rotate_, gimmick.objName_, i, normal));
			i++;
		}
		else
		{
			//������玟��
			continue;
		}
	}
}
template<typename T>
void Stage::AddGimmickList(std::vector<GimmickObj>& gList, std::wstring objName, std::string unlockNumber)
{
	std::wstring wstr;
	int i = 0;
	for (auto& gimmick : gList)
	{
		wstr = Gimmick::ConvertToWstring(gimmick.objName_);
		//�o�C�i���f�[�^�̃I�u�W�F�N�g���Ǝw�肵�����O����v���Ă���Ȃ�
		if (wstr == objName)
		{
			//�M�~�b�N���X�g�ɒǉ�
			gimmickList_.emplace_back(std::make_unique<T>(gimmick.pos_, gimmick.rotate_, gimmick.objName_, i,unlockNumber));
			i++;
		}
		else
		{
			//������玟��
			continue;
		}
	}
}
//�A�C�e�����X�g�֒ǉ�
template<typename T>
void Stage::AddItemList(std::vector<ItemObj>& iList, std::wstring objName)
{
	std::wstring wstr;
	for (auto& item : iList)
	{
		wstr = Item::ConvertToWstring(item.objName_);
		//�o�C�i���f�[�^�̃I�u�W�F�N�g���Ǝw�肵�����O����v���Ă���Ȃ�
		if (wstr == objName)
		{
			//�A�C�e�����X�g�ɒǉ�
			itemList_.emplace_back(std::make_unique<T>(item.pos_));
		}
		else
		{
			//������玟��
			continue;
		}
	}
}

