#include "LoadScene.h"
#include "GameScene.h"
#include "SceneManager.h"
#include "../common/manager/ImageMng.h"
#include "../common/manager/ModelMng.h"
#include "../common/manager/MovieMng.h"
#include "../common/manager/SoundMng.h"

LoadScene::LoadScene()
{
	Init();
	DrawScreen();
}

LoadScene::~LoadScene()
{
}

void LoadScene::Init(void)
{
	LoadVol = 0;
	IsLoadingEnd = false;
	Load();
}

UniqueScene LoadScene::Update(UniqueScene scene)
{
	//進み具合の更新
	UpdateProgress();
	
	//ロードが終わったら
	if (IsLoadingEnd)
	{
		return std::move(std::make_unique<GameScene>());
	}
	DrawScreen();
	return scene;
}

void LoadScene::DrawScreen(void)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	DrawFormatString(0, 0, 0xffffff, "LoadScene");
	//DrawFormatString(0, 1000, 0xffffff, "vol %d", LoadVol);
	//DrawFormatString(0, 1050, 0xffffff, "num %d", GetASyncLoadNum());

	//仮描画
	//ーーーーーーーーーーーーーーーーーーーーーーー
	DrawBox(0,0, 1920, 1080,
		0x333333,
		true);

	DrawBox(lpSceneMng.GetScreenSize().x / 2 - 500,
		lpSceneMng.GetScreenSize().y / 2 - 50,
		lpSceneMng.GetScreenSize().x / 2 + 500,
		lpSceneMng.GetScreenSize().y / 2 + 50,
		0x555555,
		false);
	DrawBox(lpSceneMng.GetScreenSize().x / 2 - 500,
		lpSceneMng.GetScreenSize().y / 2 - 50,
		lpSceneMng.GetScreenSize().x / 2 - 500 + LoadVol,
		lpSceneMng.GetScreenSize().y / 2 + 50,
		0x881100,
		true);
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーー
}

void LoadScene::Release(void)
{
}

void LoadScene::Load()
{
	SetUseASyncLoadFlag(true);
	lpImageMng.GetID("resource/image/minCamera.png");
	lpImageMng.GetID("resource/image/Font/gameover.png");
	lpModelMng.GetID("./resource/model/Item/old_key.mv1",5);
	lpModelMng.GetID("resource/model/stage/map2.mv1");
	lpModelMng.GetID("resource/model/stage/Coll3.mv1");

	std::string gimmickPath = "./resource/model/gimmick/";
	//扉-------------------
	//白
	lpModelMng.GetID(gimmickPath + "Door_White1.mv1", 25);
	lpModelMng.GetID(gimmickPath + "Door_White1_Double.mv1",25);
	lpModelMng.GetID(gimmickPath + "Door_White2.mv1",25);
	lpModelMng.GetID(gimmickPath + "Door_White2_Double.mv1",25);
	lpModelMng.GetID(gimmickPath + "Door_White3.mv1",25);
	lpModelMng.GetID(gimmickPath + "Door_White3_Double.mv1",25);
	//赤
	lpModelMng.GetID(gimmickPath + "Door_Red1.mv1",25);
	lpModelMng.GetID(gimmickPath + "Door_Red2.mv1",25);
	//黒
	lpModelMng.GetID(gimmickPath + "Door_Dark1.mv1",25);
	lpModelMng.GetID(gimmickPath + "Door_Dark1_Double.mv1",25);
	lpModelMng.GetID(gimmickPath + "Door_Dark2.mv1",25);
	lpModelMng.GetID(gimmickPath + "Door_Dark2_Double.mv1",25);
	//引き出し
	lpModelMng.GetID(gimmickPath + "drawer_frame_L.mv1", 25);
	lpModelMng.GetID(gimmickPath + "drawer.mv1", 25);
	//ダイアル
	lpModelMng.GetID(gimmickPath + "Dial.mv1", 25);
	//枠
	lpModelMng.GetID(gimmickPath + "Dial_Frame.mv1", 25);

	//ARマーカー
	lpModelMng.GetID("./resource/model/marker/redMarker.mv1");
	lpModelMng.GetID("./resource/model/marker/blueMarker.mv1");
	lpModelMng.GetID("./resource/model/marker/greenMarker.mv1");
	lpModelMng.GetID("./resource/model/marker/orangeMarker.mv1");
	lpModelMng.GetID("./resource/model/marker/purpleMarker.mv1");


	SetUseASyncLoadFlag(false);

	// 3Dサウンドの読み込み
	SetCreate3DSoundFlag(true);
	lpSoundMng.GetID("./resource/sound/WalkVoice.mp3");
	SetCreate3DSoundFlag(false);
}

void LoadScene::UpdateProgress()
{
	//仮で時間経過で勝手に上がっていく
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//TRACE("非同期読み込みの数:%d\n", GetASyncLoadNum());
	if (GetASyncLoadNum() == 0)//非同期読み込みの残数がいくつか調べる
	{
		//０なら
		//TRACE("非同期読み込み終了\n");
		if (LoadVol >= 1000)
		{
			IsLoadingEnd = true;//ロード終了
		}
		else//バーをマックスにする
		{
			LoadVol += 30;
			if (LoadVol > 1000)
			{
				LoadVol = 1000;
			}
		}
	}
	else
	{
		IsLoadingEnd = false;
		//ロードの進捗具合を表現してる
		if (LoadVol < 950)
		{
			LoadVol += 1;

		}
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
}
