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
	//バイナリデータを読み込み、確認用リストへ追加
	Gimmick::LoadBinData(gimmickObjList_, "./resource/data/gimmick/DrawerData.dat");
	Gimmick::LoadBinData(gimmickObjList_, "./resource/data/gimmick/DoorData.dat");
	Gimmick::LoadBinData(gimmickObjList_, "./resource/data/gimmick/DialData.dat");
	Gimmick::LoadBinData(gimmickObjList_, "./resource/data/gimmick/WallData.dat");

	//使用するデータだけギミックリストへ追加
	AddGimmick();
	//MV1SetPosition(stageHandle_, VGet(0, 0, 1200));
	MV1SetPosition(lpModelMng.GetID("resource/model/stage/map2.mv1")[0], VGet(0, 0, 0));
	//MV1SetScale(stage_, VGet(0.6f, 0.6f, 0.6f));

	//バイナリデータ読み込み
	//Item::LoadBinData(photList_, "./resource/model/Item/Phot.mv1");
	//photList_.emplace_back(NULL, L"Phot1", Vector3(0,0,0), Vector3(0, 0, 0));

	//アイテムリストに追加
	itemList_.emplace_back(std::make_unique<Phot>(Vector3{ 13000 ,1300 ,-7200 }));
	itemList_.emplace_back(std::make_unique<Key>(Vector3{ -13717 ,2326 ,-11393 },ItemID::Key1,0));
	itemList_.emplace_back(std::make_unique<Key>(Vector3{ -11374,2326,-13257 }, ItemID::Key2,1));
	itemList_.emplace_back(std::make_unique<Key>(Vector3{ 20397 ,2326 ,-19395 }, ItemID::Key3,2));
	itemList_.emplace_back(std::make_unique<Key>(Vector3{ -13464 ,6096 ,-11360 }, ItemID::Key4,3));
	itemList_.emplace_back(std::make_unique<Key>(Vector3{ 4102 ,6096 ,-26232 }, ItemID::Key5,4));
	//ギミックリストへ追加
	//AddItemList<Phot>(photList_, L"Phot1");
	//マーカーリストへ追加
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

	//ギミック解放
	for (auto& gimmick : gimmickList_)
	{
		gimmick->Release();
	}

	//アイテム解放
	for (auto& item : itemList_)
	{
		item->Release();
	}

	//マーカー解放
	for (auto& marker : markerList_)
	{
		marker->Release();
	}

	gimmickList_.clear();
	gimmickList_.shrink_to_fit();
}

void Stage::Update(void)
{
	//ギミック更新
	for (auto& gimmick : gimmickList_)
	{
		gimmick->Update();
	}

	//アイテム更新
	for (auto& item : itemList_)
	{
		item->Update();
	}
	//マーカーの更新
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
//	//ギミック描画
//	for (auto& gimmick : gimmickList_)
//	{
//		gimmick->Draw();
//	}
//
////アイテム描画
//	for (auto& item : itemList_)
//	{
//		item->Draw();
//	}
//	//マーカーの描画
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
	//ギミック描画

	for (auto& gimmick : gimmickList_)
	{
		//shader->Draw(lpModelMng.GetID(gimmick->GetModelString())[gimmick->GetModelNumber()]);
		gimmick->Draw();
	}

	//アイテム描画
	
	for (auto& item : itemList_)
	{
		item->Draw(minCameraFlg);
	}
	
	//マーカーの描画
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
	//使用するデータだけギミックリストへ追加
	//AddGimmickList<Door>(gimmickObjList_, L"DoorDouble");
	//AddGimmickList<Door>(gimmickObjList_, L"Door_Red1");
	//扉
	AddGimmickList<Door>(gimmickObjList_, L"Dark_A_Double");
	//ダイアル
	AddGimmickList<Dial>(gimmickObjList_, L"Dial");
	AddGimmickList<DialFrame>(gimmickObjList_, L"Dial_Frame");
	//引き出し
	AddGimmickList<DrawerFrame>(gimmickObjList_, L"drawer_frame_L");
	AddGimmickList<Drawer>(gimmickObjList_, L"drawer");
	//扉
	AddGimmickList<InnerLockDoor>(gimmickObjList_, L"White_B_Double",-NORMAL);
	AddGimmickList<InnerLockDoor>(gimmickObjList_, L"White_B",NORMAL);
	AddGimmickList<InnerLockDoor>(gimmickObjList_, L"White_C",-NORMAL);

	AddGimmickList<LockDoor>(gimmickObjList_, L"Dark_A",ItemID::Key4);
	AddGimmickList<LockDoor>(gimmickObjList_, L"Red_A",ItemID::Key5);
	AddGimmickList<LockDoor>(gimmickObjList_, L"White_A_Double",ItemID::Key1);
	AddGimmickList<LockDoor>(gimmickObjList_, L"Dark_B",ItemID::Key2);
	AddGimmickList<LockDoor>(gimmickObjList_, L"Dark_B_Double",ItemID::Key2);

	AddGimmickList<DialLockDoor>(gimmickObjList_, L"Red_B","1111");
	//壁
	AddGimmickList<FakeWall>(gimmickObjList_, L"Gimmick_Wall",ItemID::Phot);

}

//ギミックリストへ追加
template<typename T>
void Stage::AddGimmickList(std::vector<GimmickObj>& gList, std::wstring objName)
{
	std::wstring wstr;
	int i = 0;
	for (auto& gimmick : gList)
	{
		wstr = Gimmick::ConvertToWstring(gimmick.objName_);
		//バイナリデータのオブジェクト名と指定した名前が一致しているなら
		if (wstr == objName)
		{
			//ギミックリストに追加
			gimmickList_.emplace_back(std::make_unique<T>(gimmick.pos_,gimmick.rotate_,gimmick.objName_,i));
			i++;
		}
		else
		{
			//違ったら次へ
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
		//バイナリデータのオブジェクト名と指定した名前が一致しているなら
		if (wstr == objName)
		{
			//ギミックリストに追加
			gimmickList_.emplace_back(std::make_unique<T>(gimmick.pos_, gimmick.rotate_, gimmick.objName_, i,itemID));
			i++;
		}
		else
		{
			//違ったら次へ
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
		//バイナリデータのオブジェクト名と指定した名前が一致しているなら
		if (wstr == objName)
		{
			//ギミックリストに追加
			gimmickList_.emplace_back(std::make_unique<T>(gimmick.pos_, gimmick.rotate_, gimmick.objName_, i, normal));
			i++;
		}
		else
		{
			//違ったら次へ
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
		//バイナリデータのオブジェクト名と指定した名前が一致しているなら
		if (wstr == objName)
		{
			//ギミックリストに追加
			gimmickList_.emplace_back(std::make_unique<T>(gimmick.pos_, gimmick.rotate_, gimmick.objName_, i,unlockNumber));
			i++;
		}
		else
		{
			//違ったら次へ
			continue;
		}
	}
}
//アイテムリストへ追加
template<typename T>
void Stage::AddItemList(std::vector<ItemObj>& iList, std::wstring objName)
{
	std::wstring wstr;
	for (auto& item : iList)
	{
		wstr = Item::ConvertToWstring(item.objName_);
		//バイナリデータのオブジェクト名と指定した名前が一致しているなら
		if (wstr == objName)
		{
			//アイテムリストに追加
			itemList_.emplace_back(std::make_unique<T>(item.pos_));
		}
		else
		{
			//違ったら次へ
			continue;
		}
	}
}

