#include "GameOverScene.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "LoadScene.h"
#include"../camera/Camera.h"
#include "../input/PadInput.h"
#include "../input/KeyInput.h"
#include "../tmx/TmxObj.h"
#include "../common/manager/ImageMng.h"
#include "../common/CheckHit.h"
#include "SceneManager.h"
#include "../common/OriginalShader.h"
#include "../common/manager/ModelMng.h"

constexpr float LOGO_SIZE_X = 483.5f;
constexpr float LOGO_SIZE_Y = 244.5f;

constexpr int DRAW_OFFSET = 50;
GameOverScene::GameOverScene(UniqueScene ownScene, ControllerType type, SceneID beforeID, Vector3 playerPos, Vector3 enemyPos)
{
    beforeScene_ = std::move(ownScene);
    beforeSceneID_ = beforeID;

    if (type == ControllerType::Pad)
    {
        controller_ = std::make_unique<PadInput>();
    }
    else
    {
        controller_ = std::make_unique<KeyInput>();
    }

    playerPos_ = playerPos;
    enemyPos_ = enemyPos;

    Init();
    DrawScreen();
}

GameOverScene::~GameOverScene()
{
	Release();
}

void GameOverScene::Init(void)
{
    //���[�h
    tmxObj_.LoadTmx("resource/tmx/gameover.tmx", false);
    lpImageMng.GetID("resource/image/Font/gameover.png","gameover");
    lpImageMng.GetID("resource/image/Font/restart.png","reStartTag");
    lpImageMng.GetID("resource/image/Font/totitle.png", "toTitleTag");
    lpImageMng.GetID("resource/image/gameover/Blood5.png", "selectB");
    
    InList();
    
    //�J����
    camera_ = std::make_unique<Camera>();	
    camera_->Init();

    //srand(static_cast<unsigned int>(time(nullptr)));
    brightness_ = lpSceneMng.GetBrightnessScrren();//�P�x
    Cnt_ = 0;//���S���o�̕\���J�E���g

    //�V�F�[�_�֘A
    int VertexShader = LoadVertexShader("resource/shader/vs/NormalModel_vs.vso");
    int PixelShader = LoadPixelShader("resource/shader/ps/GameModel_ps.pso");
    //int PixelShader = LoadPixelShader("resource/shader/ps/renderGBuffer.pso");
    originalShader_ = std::make_unique<OriginalShader>(VertexShader, PixelShader);
}

UniqueScene GameOverScene::Update(UniqueScene scene)
{
    

    if (!directionEndflg_)
    {
        //���S���o���I����ĂȂ�������
        //�J�����̍X�V
        camera_->Update(enemyPos_);
        
    }
    else
    {
        //���S���o���I����Ă�����

        controller_->Update();//���͏��̍X�V
        //�Փ˔���
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
        //TRACE("volume %d\n", Volume_/2);
        lpSceneMng.SetBrightnessScrren(brightness_);
    }
    DrawScreen();
    ++Cnt_;
    if (Cnt_ > 180)
    {
        directionEndflg_ = true;
    }
	return scene;
}

void GameOverScene::DrawScreen(void)
{
    SetDrawScreen(screenID_);
    ClsDrawScreen();

    camera_->SetBeforeDraw(playerPos_);//�v���C���[�̎��S�����ʒu�ɃJ������`��

    if (!directionEndflg_)//���S���o���I����Ă��邩�ǂ���
    {
        originalShader_->Draw(lpModelMng.GetID("resource/model/stage/map2.mv1")[0]);
    }
    else
    {
        DrawExtendGraph(0,
            0,
            lpSceneMng.GetScreenSize().x,
            lpSceneMng.GetScreenSize().y,
            lpImageMng.GetID("resource/image/gameOver/horrorBg.jpg")[0],
            true);
        //tmx����̃R���W�����̏ꏊ�ɕ`��
        DrawColl();
    }
}

void GameOverScene::Release(void)
{
}

void GameOverScene::InList()
{
    //UI�̏������X�g�Ɋi�[
    displayList_ =
    {
        {"logo","gameover"},
    };
    
    toTitleList_ =
    {
        {"",""}
    };
    reStartList_ =
    {
        {"",""}
    };
    selectList_ =
    {
        {"reStartTag",&GameOverScene::UpdateReStart},
        {"toTitleTag",&GameOverScene::UpdateToTitle }
    };

    supportList_ =
    {
        {"toTitleTag","supportToTitle"},
        {"reStartTag","supportReStart"},
    };
}

void GameOverScene::HitCollAndAssignProcess()
{
    Vector2Int pos;
    GetMousePoint(&pos.x, &pos.y);
    for (auto& coll : tmxObj_.GetGameoverCollList())
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

void GameOverScene::HitCollClickAndAssignProcess(std::pair<std::string, Collision> coll)
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

std::pair<std::string, std::string> GameOverScene::HitCollMouse(std::pair<std::string, Collision> coll)
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



void GameOverScene::UpdateToTitle()//�^�C�g����
{
    returningScene_ = std::make_unique<TitleScene>();
    DrawFuncPtr_ = &GameOverScene::DrawToTitle;
}

void GameOverScene::UpdateReStart()//���X�^�[�g
{
    returningScene_ = std::make_unique<GameScene>();
    DrawFuncPtr_ = &GameOverScene::DrawReStart;
}

void GameOverScene::DrawToTitle()//�^�C�g���ց@��\��
{
    for (auto& coll : tmxObj_.GetGameoverCollList())
    {
        DrawList(coll, toTitleList_);
    }
}

void GameOverScene::DrawReStart()//���X�^�[�g�@��\��
{
    for (auto& coll : tmxObj_.GetGameoverCollList())
    {
        DrawList(coll, reStartList_);
    }
}

void GameOverScene::DrawColl()//UI�̃R���W�����������Ƃ�UI��z�u
{
    for (auto& coll : tmxObj_.GetGameoverCollList())
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

void GameOverScene::DrawCollCenterY(std::pair<std::string, Collision> coll, std::string imageStr)//�`��Ώۂ�Y���W�̌��_�𒆐S�Ɉړ������ĕ`��
{
    Vector2Int GraphSize;
    Vector2 cen = { coll.second.first.x + coll.second.second.x / 2 ,coll.second.first.y + coll.second.second.y / 2 };

    GetGraphSize(lpImageMng.GetID(imageStr)[0], &GraphSize.x, &GraphSize.y);
    GraphSize.y /= 2;
    DrawGraph(coll.second.first.x, cen.y - GraphSize.y, lpImageMng.GetID(imageStr)[0], true);
}

void GameOverScene::DrawCollCenter(std::pair<std::string, Collision> coll, std::string imageStr)//�`��Ώۂ����_�𒆐S�ɂ��炵�ĕ`��
{
    Vector2Int GraphSize;
    Vector2 cen = { coll.second.first.x + coll.second.second.x / 2 ,coll.second.first.y + coll.second.second.y / 2 };

    GetGraphSize(lpImageMng.GetID(imageStr)[0], &GraphSize.x, &GraphSize.y);
    GraphSize.x /= 2;
    GraphSize.y /= 2;
    DrawGraph(cen.x - GraphSize.x, cen.y - GraphSize.y, lpImageMng.GetID(imageStr)[0], true);
}

void GameOverScene::DrawSelectList(std::pair<std::string, Collision> coll)
{
    //���ڂɐG��Ă�����̂�`��
    if (coll.first == hitMouse_.first)
    {
        //SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::sin(DX_PI * 2 / 60 * Cnt_) * 300 + 300);
        if (Cnt_ < 255)
        {
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, Cnt_*Cnt_);
        }
        DrawExtendGraph(coll.second.first.x - DRAW_OFFSET,
            coll.second.first.y,
            coll.second.first.x + coll.second.second.x - DRAW_OFFSET,
            coll.second.first.y + coll.second.second.y, lpImageMng.GetID("selectB")[0],
            true);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
    if (coll.first == "operation")
    {
        DrawCollCenter(coll, hitMouse_.second);
        _dbgDrawFormatString(0, 0, 0xffffff, "%s", hitMouse_.second.c_str());
    }
    ////���ڂ��N���b�N���Ă�����̂�`��
    //if (coll.first == selection_)
    //{
    //    DrawExtendGraph(coll.second.first.x - DRAW_OFFSET,
    //        coll.second.first.y,
    //        coll.second.first.x + coll.second.second.x - DRAW_OFFSET,
    //        coll.second.first.y + coll.second.second.y, lpImageMng.GetID("selectB")[0],
    //        true);
    //}

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

void GameOverScene::DrawList(std::pair<std::string, Collision> coll, std::map<std::string, std::string> maplist)
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
