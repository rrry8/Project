#include "OptionScene.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "SceneManager.h"
#include "../tmx/TmxObj.h"
#include "../input/PadInput.h"
#include "../input/KeyInput.h"

#include "../common/CheckHit.h"
#include "../common/manager/ImageMng.h"
#include "../common/manager/SoundMng.h"
#include "../common/manager/CBufferMng.h"
#include "../../_debug/_DebugDispOut.h"


constexpr int DRAW_OFFSET = 50;
OptionScene::OptionScene(UniqueScene ownScene,ControllerType type, SceneID beforeID)
{
    beforeScene_ = std::move(ownScene);
    
    if (type == ControllerType::Pad)
    {
        controller_ = std::make_unique<PadInput>();
    }
    else
    {
        controller_ = std::make_unique<KeyInput>();
    }
    beforeSceneID_ = beforeID;

    Init();
    DrawScreen();
}

OptionScene::~OptionScene()
{
    //���ʂ̐ݒ�
    lpSoundMng.SetAllVolume(Volume_/2);
}

void OptionScene::Init(void)
{
    //tmx���[�h
    tmxObj_.LoadTmx("resource/tmx/option.tmx", false);
    //�摜�Ƀt�B���^�[��������
    InitGraphFilter();
    //���X�g�Ɋi�[
    InList();
    //�ϐ��̏�����
    InitVariable();
   
}

UniqueScene OptionScene::Update(UniqueScene scene)
{
    //�R���g���[��
    controller_->Update();
    
    //�Փ˔���Ɖ������{�^���̑��
    HitCollAndAssignProcess();

    //�֐��|�C���^�̎���
    if (UpdateFuncPtr_ != nullptr)
    {
        (this->*UpdateFuncPtr_)();
    }

    //�֐��|�C���^�ŕԂ��V�[�����o�����炻���Ԃ�
    if (returningScene_ != nullptr)
    {
        return std::move(returningScene_);
    }

    //���Ԃ̍X�V
    Cnt_++;

    //���邳�̍X�V
    lpSceneMng.SetBrightnessScrren(brightness_);

    DrawScreen();
    return scene;
}

void OptionScene::DrawScreen(void)
{
    SetDrawScreen(screenID_);
    ClsDrawScreen();
    //�w�i�`��
    DrawExtendGraph(0,
        0,
        lpSceneMng.GetScreenSize().x,
        lpSceneMng.GetScreenSize().y,
        lpImageMng.GetID("resource/image/option/bg1.png")[0],
        true);
    //tmx����̃R���W�����̏ꏊ�ɕ`��
    DrawColl();
}

void OptionScene::Release(void)
{
}

void OptionScene::InitGraphFilter()
{
    GraphFilter(lpImageMng.GetID("resource/image/option/bg1.png")[0], DX_GRAPH_FILTER_GAUSS, 16, 500);
    GraphFilter(lpImageMng.GetID("resource/image/option/bg1.png")[0], DX_GRAPH_FILTER_HSB, 0, 0, 0, -150);
    GraphFilter(lpImageMng.GetID("selection")[0], DX_GRAPH_FILTER_HSB, 0, 0, 0, 205);
    GraphFilter(lpImageMng.GetID("selection1")[0], DX_GRAPH_FILTER_HSB, 0, 0, 0, 100);
    GraphFilter(lpImageMng.GetID("selection2")[0], DX_GRAPH_FILTER_HSB, 0, 0, 0, 256);
}

void OptionScene::InList()
{
    displayList_ =
    {
        {"optionTag","optionTag"},
    };
    controlList_ =
    {
        {"key","key"},
        {"keyboard","select5"},
        {"smp","smp"},
        {"smartphone","select5"}
    };

    audioList_ =
    {
        {"volume","volume"},
        {"masterValume","masterValume"},
        {"valumeControl","select5"},
        {"non","select5"}
    };

    graphicList_ =
    {
        {"brightness","screenBrightness"},
        {"screenBrightness","brightness"},
        {"brightnessAdjustment","select5"},
    };
    titleRetureList_ =
    {
        {"",""}
    };
    retureList_ =
    {
        {"",""}
    };
    selectList_ =
    {
        {"controlTag",&OptionScene::UpdateControl},
        {"audioTag",&OptionScene::UpdateAudio },
        {"graphicTag",&OptionScene::UpdateGraphic},
        {"titleReturnTag",&OptionScene::UpdateToTitle},
        {"returnTag",&OptionScene::UpdateReturnScreen}
    };

    supportList_ =
    {
        {"controlTag","supportControl"},
        {"audioTag","supportAudio"},
        {"graphicTag","supportGraphic"},
        {"titleReturnTag","supportTitleReturn"},
        {"returnTag","supportReturn"},
    };
}

void OptionScene::InitVariable()
{
    Volume_ = lpSoundMng.GetAllVolume();
    if (Volume_ == 0)
    {
        Volume_ = 125;
    }
    brightness_ = lpSceneMng.GetBrightnessScrren();
    Cnt_ = 0;
}

void OptionScene::HitCollAndAssignProcess()
{
    Vector2Int pos;
    GetMousePoint(&pos.x, &pos.y);
    for (auto& coll : tmxObj_.GetOptionCollList())
    {
        if (CheckCollMouse()(pos, coll.second))
        {
            if (controller_->CheckInputKey(KeyID::Decide))
            {
                //�N���b�N�����Ƃ��ɓ������Ă�����֐��|�C���^�ɏ���������
                HitCollClickAndAssignProcess(coll);
            }
            //�}�E�X���������Ă����瓖�����Ă���摜��string��������
            OldhitMouse_ = hitMouse_;
            hitMouse_ = HitCollMouse(coll);
            //���������̂��ς�����Ƃ��ɕ\���J�E���g�����Z�b�g����
            if (OldhitMouse_ != hitMouse_)
            {
                Cnt_ = 0;
            }
        }
    }
}

void OptionScene::HitCollClickAndAssignProcess(std::pair<std::string, Collision> coll)
{
    //�N���b�N�����Ƃ��ɓ������Ă��邩
    for (auto& select : selectList_)
    {
        if (coll.first == select.first)
        {
            //�I���������X�g���h���[���邽�߂̃��X�g�Ɋi�[���Ă���
            selection_ = select.first;
            UpdateFuncPtr_ = select.second;
        }
    }
}

//�}�E�X���������Ă��邩
std::pair<std::string, std::string> OptionScene::HitCollMouse(std::pair<std::string, Collision> coll)
{
    for (auto& select : supportList_)
    {
        if (coll.first == select.first)
        {
            return { select.first,select.second };
        }
    }
    return { "","" };
}

void OptionScene::UpdateControl()
{
    DrawFuncPtr_ = &OptionScene::DrawControl;
}

void OptionScene::UpdateAudio()
{
    Vector2Int pos;
    GetMousePoint(&pos.x, &pos.y);
    for (auto& coll : tmxObj_.GetOptionCollList())
    {
        if (CheckCollMouse()(pos, coll.second))
        {
            if (!controller_->CheckLongInputKey(KeyID::Decide))
            {
                continue;
            }
            for (auto& UpdateAudio : audioList_)
            {
                if (coll.first != UpdateAudio.first)
                {
                    continue;
                }
                Volume_ = pos.x - coll.second.first.x;
            }

        }
    }
    DrawFuncPtr_ = &OptionScene::DrawAudio;
}

void OptionScene::UpdateGraphic()
{
    Vector2Int pos;
    GetMousePoint(&pos.x, &pos.y);
    for (auto& coll : tmxObj_.GetOptionCollList())
    {
        if (CheckCollMouse()(pos, coll.second))
        {
            if (!controller_->CheckLongInputKey(KeyID::Decide))
            {
                continue;
            }
            for (auto& graphic : graphicList_)
            {
                if (coll.first != graphic.first)
                {
                    continue;
                }
                brightness_ = pos.x - coll.second.first.x;
            }

        }
    }
    DrawFuncPtr_ = &OptionScene::DrawGraphic;
}

void OptionScene::UpdateToTitle()
{
    if (SceneID::Game == beforeSceneID_)
    {
        returningScene_ = std::make_unique<TitleScene>();
    }
    DrawFuncPtr_ = &OptionScene::DrawToTitle;
}

void OptionScene::UpdateReturnScreen()
{
    returningScene_ = std::move(beforeScene_);
    DrawFuncPtr_ = &OptionScene::DrawReturnScreen;
}

void OptionScene::DrawControl()
{
    for (auto& coll : tmxObj_.GetOptionCollList())
    {
        DrawList(coll, controlList_);
    }
}

void OptionScene::DrawAudio()
{
    for (auto& coll : tmxObj_.GetOptionCollList())
    {
        for (auto& audio : audioList_)
        {
            //����string����Ȃ�������͂���
            if (coll.first == audio.first)
            {

                if (audio.first == "volume")
                {
                    DrawCollCenterY(coll, audio.second);
                }
                if (audio.first == "masterValume")
                {
                    DrawCollCenterY(coll, audio.second);
                }
                if (audio.first == "non")
                {
                    /*DrawBox(coll.second.first.x,
                        coll.second.first.y,
                        coll.second.first.x + coll.second.second.x,
                        coll.second.first.y + coll.second.second.y,
                        0xffffff, true);*/
                    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 70);
                    DrawExtendGraph(coll.second.first.x - DRAW_OFFSET,
                        coll.second.first.y,
                        coll.second.first.x + coll.second.second.x + DRAW_OFFSET,
                        coll.second.first.y + coll.second.second.y, lpImageMng.GetID(audio.second)[0],
                        true);
                    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 125);

                }
                if (audio.first == "valumeControl")
                {
                    SetDrawBlendMode(DX_BLENDMODE_ADD, 230);
                    DrawExtendGraph(coll.second.first.x,
                        coll.second.first.y - 10,
                        coll.second.first.x + coll.second.second.x,
                        coll.second.first.y + coll.second.second.y + 10, lpImageMng.GetID(audio.second)[0],
                        true);
                    DrawExtendGraph(coll.second.first.x + 15,
                        coll.second.first.y - 10,
                        coll.second.first.x + Volume_ + 20,
                        coll.second.first.y + coll.second.second.y + 10, lpImageMng.GetID("Bbox")[0],
                        true);
                    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 125);
                }
                continue;
            }
        }
    }
}

void OptionScene::DrawGraphic()
{
    for (auto& coll : tmxObj_.GetOptionCollList())
    {
        for (auto& audio : graphicList_)
        {
            //����string����Ȃ�������͂���
            if (coll.first == audio.first)
            {
                if (audio.first == "brightness")
                {
                    DrawCollCenterY(coll, audio.second);
                }
                if (audio.first == "screenBrightness")
                {
                    DrawCollCenterY(coll, audio.second);
                }
                if (audio.first == "brightnessAdjustment")
                {
                    SetDrawBlendMode(DX_BLENDMODE_ADD, 230);
                    DrawExtendGraph(coll.second.first.x,
                        coll.second.first.y - 10,
                        coll.second.first.x + coll.second.second.x,
                        coll.second.first.y + coll.second.second.y + 10, lpImageMng.GetID(audio.second)[0],
                        true);
                    DrawExtendGraph(coll.second.first.x + 15,
                        coll.second.first.y - 10,
                        coll.second.first.x + brightness_ + 20,
                        coll.second.first.y + coll.second.second.y + 10, lpImageMng.GetID("Bbox")[0],
                        true);
                    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 125);
                }
                continue;
            }
        }
    }
}

void OptionScene::DrawToTitle()
{
    for (auto& coll : tmxObj_.GetOptionCollList())
    {
        DrawList(coll, titleRetureList_);
    }
}

void OptionScene::DrawReturnScreen()
{
    for (auto& coll : tmxObj_.GetOptionCollList())
    {
        DrawList(coll, retureList_);
    }
}

void OptionScene::DrawColl()
{
    for (auto& coll : tmxObj_.GetOptionCollList())
    {
        //�\�����邾���̕`��
        DrawList(coll, displayList_);

        //selectList�����̕`��
        DrawSelectList(coll);

        //�֐��|�C���^�̎���
        if (DrawFuncPtr_ != nullptr)
        {
            (this->*DrawFuncPtr_)();
        }
        //�f�o�b�O�`��
        /*DrawBox(coll.second.first.x,
            coll.second.first.y,
            coll.second.first.x + coll.second.second.x,
            coll.second.first.y + coll.second.second.y,
            0x222222, false);*/
    }
}

void OptionScene::DrawCollCenterY(std::pair<std::string, Collision> coll, std::string imageStr)
{
    Vector2Int GraphSize;
    Vector2 cen = { coll.second.first.x + coll.second.second.x / 2 ,coll.second.first.y + coll.second.second.y / 2 };

    GetGraphSize(lpImageMng.GetID(imageStr)[0], &GraphSize.x, &GraphSize.y);
    GraphSize.y /= 2;
    DrawGraph(coll.second.first.x, cen.y - GraphSize.y, lpImageMng.GetID(imageStr)[0], true);
}

void OptionScene::DrawCollCenter(std::pair<std::string, Collision> coll, std::string imageStr)
{
    Vector2Int GraphSize;
    Vector2 cen = { coll.second.first.x + coll.second.second.x / 2 ,coll.second.first.y + coll.second.second.y / 2 };

    GetGraphSize(lpImageMng.GetID(imageStr)[0], &GraphSize.x, &GraphSize.y);
    GraphSize.x /= 2;
    GraphSize.y /= 2;
    DrawGraph(cen.x - GraphSize.x, cen.y - GraphSize.y, lpImageMng.GetID(imageStr)[0], true);
}

void OptionScene::DrawSelectList(std::pair<std::string, Collision> coll)
{
    //titleReturnTag�����Q�[���V�[���݂̂ŏo�������̂ŃQ�[���ȊO��������Ԃ�
    if (coll.first == "titleReturnTag")
    {
        if (beforeSceneID_ != SceneID::Game)
        {
            return;
        }
    }

    //���ڂɐG��Ă�����̂�`��
    if (coll.first == hitMouse_.first)
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::sin(DX_PI * 2 / 60 * Cnt_) * 300 + 100);
        DrawExtendGraph(coll.second.first.x - DRAW_OFFSET,
            coll.second.first.y,
            coll.second.first.x + coll.second.second.x - DRAW_OFFSET,
            coll.second.first.y + coll.second.second.y, lpImageMng.GetID("selection1")[0],
            true);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
    if (coll.first == "operation")
    {
        if (hitMouse_.first == "titleReturnTag")
        {
            if (beforeSceneID_ != SceneID::Game)
            {
                return;
            }
        }
        DrawCollCenter(coll, hitMouse_.second);
    }
    //���ڂ��N���b�N���Ă�����̂�`��
    if (coll.first == selection_)
    {
        DrawExtendGraph(coll.second.first.x - DRAW_OFFSET,
            coll.second.first.y,
            coll.second.first.x + coll.second.second.x - DRAW_OFFSET,
            coll.second.first.y + coll.second.second.y, lpImageMng.GetID("selection")[0],
            true);
    }

    //�I�����ڂ̕`��
    for (auto& select : selectList_)
    {
        //�������̂���Ȃ�������e��
        if (coll.first != select.first)
        {
            continue;
        }

        //�`��
        DrawCollCenterY(coll, select.first);
        continue;

    }
}

void OptionScene::DrawList(std::pair<std::string, Collision> coll, std::map<std::string, std::string>maplist)
{
    for (auto& draw : maplist)
    {
        //����string����Ȃ�������͂���
        if (coll.first != draw.first)
        {
            continue;
        }
        DrawCollCenterY(coll, draw.second);
        continue;
    }
}

