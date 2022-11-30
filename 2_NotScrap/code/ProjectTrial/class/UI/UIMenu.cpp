#include "UIMenu.h"
#include "SceneUIManager.h"
#include "../Obj/CheckHit.h"
#include "../Common/ImageMng.h"

UIMenu::UIMenu()
{
    Init();
    DrawOwnScreen(0.0);
}

UIMenu::~UIMenu()
{
}

bool UIMenu::Init(void)
{
	tmxObj_.LoadTmx("././tmx/menuUI.tmx");
	rapidxml::file<> moduleFileName = "tmx/CheckHitObj.tsx";
	stateVec_ = moduleFileName.vecData();
	stateDoc.parse<0>(stateVec_.data());
	stateNode_ = stateDoc.first_node("MouseCursormodule");
	return true;
}

void UIMenu::Update(double delta)
{
	auto node = stateNode_->first_node("module");
	GetMousePoint(&MousePos.x, &MousePos.y);
	if (lpCntMng.GetController()->GetCntType() == CntType::Key)
	{
		MousePos.x += 32;//�摜����Ă镪�␳
		MousePos.y += 32;
	}
	int i = (int)UI_MAIN_ITEM::M_GAME;//�͈�for���̐擪��ݒ�
	menuState_ = lpSceneUIMng.GetMenuState();//�ݒ��ʂőI�����Ă��鍀�ڂ̏�Ԃ��擾

	//�J�[�\���ƃ��C������'�̓����蔻��				�����C�����ځF�ݒ��ʍ����̍���
	for (auto& coll : tmxObj_.GetItemCollList())
	{
		if (CheckCollM()(raycast_, MousePos, node, coll))
		{
			//TRACE("���������I\n");
			lpSceneUIMng.SetHitItem_Main((UI_MAIN_ITEM)i);
			break;
		}
		else
		{
			lpSceneUIMng.SetHitItem_Main(UI_MAIN_ITEM::NONE);
		}
		i++;
	}

	DrawOwnScreen(delta);
}

void UIMenu::DrawOwnScreen(double delta)
{
	auto tilesize = tmxObj_.GetTileSize();
	auto offset = Vector2{ 0.0f, 0.0f }/*camera_->GetPos()*/;

SetDrawBlendMode(DX_BLENDMODE_ALPHA, 233);
	DrawBox(745, 276, 1159, 868, 0x111111, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
DrawRotaGraph(screenSize_.x/2, 351, 1, 0, lpImageMng.GetID("source/UIIMage/string/MENU/�Q�[���ɖ߂�.png")[0], true);
	DrawRotaGraph(screenSize_.x/2, 495, 1, 0, lpImageMng.GetID("source/UIIMage/string/MENU/���X�^�[�g.png")[0], true);
	DrawRotaGraph(screenSize_.x/2, 649, 1, 0, lpImageMng.GetID("source/UIIMage/string/MENU/�ݒ�.png")[0], true);
	DrawRotaGraph(screenSize_.x/2, 799, 1, 0, lpImageMng.GetID("source/UIIMage/string/MENU/�Z���N�g��.png")[0], true);
	//���F�̘g��`��

	DrawRotaGraph(screenSize_.x / 2, screenSize_.y / 2, 1, 0, lpImageMng.GetID("source/UIIMage/menuBox.png")[0], true);

	/*for (const auto& data : tmxObj_.GetMapData())	{
		for (int y = static_cast<int>(offset.y / tilesize.y) - 3; y < offset.y / tilesize.y + lpSceneMng.GetScreenSize().y / tilesize.y + 3; y++)
		{
			for (int x = static_cast<int>(offset.x / tilesize.x); x < offset.x / tilesize.x + lpSceneMng.GetScreenSize().x / tilesize.x + 3; x++)
			{
				if (y * tmxObj_.GetWorldArea().x + x < data.second.size())
				{
					int y_ = y;
					if (y < 0)
					{
						y_ = 0;
					}
					if (y * tmxObj_.GetWorldArea().x + x < data.second.size())
					{
						int gid = data.second[y_ * tmxObj_.GetWorldArea().x + x] - tmxObj_.GetFirstGID();
						if (gid >= 0)
						{
							DrawGraph(x * tilesize.x, y * tilesize.y,
								lpImageMng.GetID("FantasyUIPack.png")[gid], true);
						}
					}
				}
			}
		}
	}*/
}

bool UIMenu::GetLoadingEndFlg(void)
{
	return true;
}

void UIMenu::SetIsNext(bool flg)
{
}
