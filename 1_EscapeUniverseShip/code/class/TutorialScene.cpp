#include<DxLib.h>
#include<string>
#include "TutorialScene.h"
#include"SelectScene.h"
#include"ClearScene.h"
#include"SceneManager.h"
#include"Obj/Player.h"
#include"Camera/Camera.h"
#include"Obj/Drum.h"
#include"Other/ImageMng.h"
#include"Other/SoundMng.h"
#include "Input/KeyInput.h"
#include "Input/PadInput.h"

TutorialScene::TutorialScene()
{
    Init();
    DrawScreen();
}

TutorialScene::~TutorialScene()
{
}

void TutorialScene::Init(void)
{
	if (GetJoypadNum() == 0)
	{
		//キーボードの場合
		controller_ = std::make_unique<KeyInput>();
	}
	else
	{
		//パッドの場合
		controller_ = std::make_unique<PadInput>();
	}

    tmxObj_.LoadTmx("./tmx/tutorial.tmx");
    player_ = std::make_unique<Player>(GetJoypadNum());
	player_->SetGoalPos(3520.0f);
	camera_ = std::make_unique<Camera>(3520.0f, 3200.0f);
	drum_ = std::make_unique<Drum>();
	drum_->Init(tmxObj_);

	tileImage_ = LoadGraph("./Image/Tile.png");
	sawImage_ = DerivationGraph(336, 144, 80, 80, tileImage_);

	textNum_ = 0;
	gravFlag_ = false;
}

UniqueScene TutorialScene::Update(UniqueScene scene)
{
	Vector2 pos = player_->GetPos();
	if (gravFlag_)
	{
		if (player_->TutorialMove(tmxObj_,camera_->GetPos(),textNum_))
		{
			PlaySoundMem(lpSoundMng.GetID("./sound/decide.mp3")[0], DX_PLAYTYPE_BACK);
			gravFlag_ = false;
		}
	}

	if (!gravFlag_)
	{
		//カメラ更新
		camera_->Update(pos);

		//ドラム缶の更新
		drum_->Update();

		//プレイヤーの更新
		player_->TutorialUpdate(tmxObj_, camera_->GetPos(), nullptr);

		//ドラム缶の生存フラグ更新
		drum_->UpdateAlive(tmxObj_);

		//のこぎり回転の更新
		angle_ -= 0.05;
	}

	DrawScreen();
	//616,1224,1708,2484
	if (pos.x == 616.0f)
	{
		gravFlag_ = true;
		textNum_ = 1;
	}
	if (pos.x == 1224.0f)
	{
		gravFlag_ = true;
		textNum_ = 2;
	}
	if (pos.x == 1708.0f)
	{
		gravFlag_ = true;
		textNum_ = 3;
	}
	if (pos.x == 2484.0f)
	{
		gravFlag_ = true;
		textNum_ = 4;
	}
	if (pos.x == 3200.0f)
	{
		gravFlag_ = true;
		textNum_ = 5;
	}
	if (pos.x == 3600.0f)
	{
		if (GetJoypadNum() == 0)
		{
			gravFlag_ = true;
			textNum_ = 7;

		}
		else
		{
			gravFlag_ = true;
			textNum_ = 6;

		}
	}

	if (pos.x > 3800.0f)
	{
		//StopSoundMem(lpSoundMng.GetID("./sound/selectscene.mp3")[0]);
		return std::make_unique<SelectScene>();
	}
    return scene;
}

void TutorialScene::DrawScreen(void)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();

	auto tilesize = tmxObj_.GetTileSize();
	auto offset = camera_->GetPos();
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

	std::string textName = "././Image/font/tutorial.png";
	std::string strTextNum = std::to_string(textNum_);
	textName.insert(textName.length() - 4, strTextNum);

	if (gravFlag_)
	{
		DrawGraph(306, 50, lpImageMng.GetID(textName)[0], true);
	}

	SetDrawZ(0.45f);
	player_->Draw(camera_->GetPos());
}

void TutorialScene::DrawStage(void)
{
}

void TutorialScene::Release(void)
{
}