#include<DxLib.h>
#include "GameScene.h"
#include"Obj/Player.h"
#include"Obj/Drum.h"
#include"Obj/CheckPoint.h"
#include"Camera/Camera.h"
#include"Other/ImageMng.h"
#include"SceneManager.h"
#include"ResultScene.h"
#include"ClearScene.h"
#include"PauseScene.h"
#include"Transition/FadeInOut.h"

GameScene::GameScene()
{
	angle_ = 0.0;
	sawImage_ = 0;
	tileImage_ = 0;
}

GameScene::GameScene(std::string map)
{
	tmxObj_.LoadTmx(map);
	Vector2 worldarea = tmxObj_.GetWorldArea();
	Vector2 tilesize = tmxObj_.GetTileSize();
	Vector2 worldsize = { worldarea.x * tilesize.x,worldarea.y * tilesize.y };


	camera_ = std::make_unique<Camera>(worldsize.x, worldsize.y);
	player_ = std::make_unique<Player>(GetJoypadNum());
	player_->SetGoalPos(worldsize.x);
	tileImage_ = LoadGraph("./Image/Tile.png");
	sawImage_ = DerivationGraph(336, 144, 80, 80, tileImage_);
	Init();
	DrawStage();
}

GameScene::~GameScene()
{
	Release();
}

void GameScene::Init(void)
{
	drum_ = std::make_unique<Drum>();
	drum_->Init(tmxObj_);

	check_ = std::make_shared<CheckPoint>();
	check_->Init(tmxObj_);

	angle_ = 0.0;
}

UniqueScene GameScene::Update(UniqueScene scene)
{
	//カメラ更新
	camera_->Update(player_->GetPos());

	//ドラム缶の更新
	drum_->Update();
	check_->Update();

	SCENE nowScene = player_->Update(tmxObj_, camera_->GetPos(), check_);

	if (nowScene == SCENE::OVER)
	{
		GetDrawScreenGraph(0, 0, screenSizeX_, screenSizeY_, gameBG_);
		//ドラム缶のデータをリセット
		drum_->Reset(tmxObj_);

		//カメラ情報をリセット
		camera_->Update(player_->GetPos());
		DrawScreen();
		return std::make_unique<ResultScene>(std::move(scene),gameBG_);
	}
	else if (nowScene == SCENE::CLEAR)
	{
		StopSound();
		return std::make_unique<ClearScene>();
	}
	else if (nowScene == SCENE::PAUSE)
	{
		ChangeSoundVolume(80);
		GetDrawScreenGraph(0, 0, screenSizeX_, screenSizeY_, gameBG_);
		return std::make_unique<PauseScene>(std::move(scene), gameBG_);
	}

	//ドラム缶の生存フラグ更新
	drum_->UpdateAlive(tmxObj_);
	//のこぎり回転の更新
	angle_ -= 0.05;
	DrawScreen();
	//auto a =GetFPS();
	//DrawFormatStringF(0, 0, 0xffffff, "%f", a);
	return scene;
}

void GameScene::DrawScreen(void)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();

	auto tilesize = tmxObj_.GetTileSize();
	auto offset = camera_->GetPos();
	SetUseZBufferFlag(TRUE);
	SetWriteZBufferFlag(TRUE);
	//マップを描画
	for (const auto& data : tmxObj_.GetMapData())
	{
		for (int y = static_cast<int>(offset.y / tilesize.y) - 3; y < offset.y / tilesize.y + lpSceneMng.GetViewSize().y / tilesize.y + 3; y++)
		{
			for (int x = static_cast<int>(offset.x / tilesize.x); x < offset.x / tilesize.x + lpSceneMng.GetViewSize().x / tilesize.x + 3; x++)
			{
				if (y * tmxObj_.GetWorldArea().x + x < data.second.size())
				{
					int y_ = y;
					if (y < 0)
					{
						y_ = 0;
					}
					int gid = data.second[y_ * tmxObj_.GetWorldArea().x + x] - tmxObj_.GetFirstGID();
					if (gid >= 0)
					{
						//のこぎりの描画
						SetDrawZ(0.35f);
						if (DrawSaw(gid, x, y, tilesize, offset))
						{
							continue;
						}

						//ドラム缶は除外
						if (drum_->CheckDrum(gid))
						{
							continue;
						}

						//中間ポイント
						if (check_->GetFlag() && check_->CheckCheck(gid))
						{
							check_->ChangeID(gid);
							SetDrawZ(0.35f);
							DrawGraph(static_cast<int>(x * tilesize.x - offset.x), static_cast<int>(y * tilesize.y - offset.y),
								lpImageMng.GetmapID("Tile.png")[gid], true);
							continue;
						}
						SetDrawZ(0.7f);
						DrawGraph(static_cast<int>(x * tilesize.x - offset.x), static_cast<int>(y * tilesize.y - offset.y),
							lpImageMng.GetmapID("Tile.png")[gid], true);
					}
				}
			}
		}
	}

	SetDrawZ(0.3f);
	drum_->Draw(offset);

	SetDrawZ(0.45f);
	player_->Draw(camera_->GetPos());

}

void GameScene::DrawStage(void)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();

	auto tilesize = tmxObj_.GetTileSize();
	auto offset = camera_->GetPos();
	SetUseZBufferFlag(TRUE);
	SetWriteZBufferFlag(TRUE);
	//マップを描画
	for (const auto& data : tmxObj_.GetMapData())
	{
		for (int y = static_cast<int>(offset.y / tilesize.y); y < offset.y / tilesize.y + lpSceneMng.GetViewSize().y / tilesize.y; y++)
		{
			for (int x = static_cast<int>(offset.x / tilesize.x); x < offset.x / tilesize.x + lpSceneMng.GetViewSize().x / tilesize.x + 3; x++)
			{
				if (y * tmxObj_.GetWorldArea().x + x < data.second.size())
				{
					int gid = data.second[y * tmxObj_.GetWorldArea().x + x] - tmxObj_.GetFirstGID();
					if (gid >= 0)
					{
						//のこぎりの描画
						SetDrawZ(0.65f);
						if (DrawSaw(gid, x, y, tilesize, offset))
						{
							continue;
						}

						//ドラム缶は除外
						if (drum_->CheckDrum(gid))
						{
							continue;
						}

						//中間ポイント
						if (check_->GetFlag() && check_->CheckCheck(gid))
						{
							check_->ChangeID(gid);
							SetDrawZ(0.5f);
							DrawGraph(static_cast<int>(x * tilesize.x - offset.x), static_cast<int>(y * tilesize.y - offset.y),
								lpImageMng.GetmapID("Tile.png")[gid], true);
							continue;
						}
						SetDrawZ(0.7f);
						DrawGraph(static_cast<int>(x * tilesize.x - offset.x), static_cast<int>(y * tilesize.y - offset.y),
							lpImageMng.GetmapID("Tile.png")[gid], true);
					}
				}
			}
		}
	}

	SetDrawZ(0.5f);
	drum_->Draw(offset);
}

bool GameScene::DrawSaw(int gid, int x, int y, Vector2 tilesize, Vector2 offset)
{
	if (gid == 282 || gid == 283 || gid == 284 || gid == 285 || gid == 286 ||
		gid == 311 || gid == 312 || gid == 313 || gid == 314 || gid == 315 ||
		gid == 340 || gid == 341 || gid == 342 || gid == 343 || gid == 344 ||
		gid == 369 || gid == 370 || gid == 371 || gid == 372 || gid == 373 ||
		gid == 398 || gid == 399 || gid == 400 || gid == 401 || gid == 402)
	{
		if (gid == 402)
		{
			DrawRotaGraph(static_cast<int>(x * tilesize.x - 24.0f - offset.x), static_cast<int>(y * tilesize.y - 24.0f - offset.y),
				1.0, angle_, sawImage_, true);
		}

		return true;
	}
	return false;
}

void GameScene::Release(void)
{
    player_->Release();
	DeleteGraph(tileImage_);
	DeleteGraph(sawImage_);
	DeleteGraph(gameBG_);
	drum_->Release();
	if (check_ != nullptr)
	{
		check_->Release();
	}

}

