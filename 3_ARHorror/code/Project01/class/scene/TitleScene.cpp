#include<DxLib.h>
#include<string>

#include "TitleScene.h"
#include "LoadScene.h"
#include "OptionScene.h"
#include "SceneManager.h"

#include"../input/KeyInput.h"
#include"../input/PadInput.h"
#include "../tmx/TmxObj.h"
#include"../camera/Camera.h"

#include "../common/CheckHit.h"
#include "../common/OriginalShader.h"
#include "../common/manager/CBufferMng.h"
#include "../common/manager/ImageMng.h"
#include "../common/manager/ModelMng.h"

#include"../../_debug/_DebugConOut.h"

TitleScene::TitleScene()
{
    Init();
    DrawScreen();
}

TitleScene::~TitleScene()
{
    Release();
}

void TitleScene::Init()
{
    //tmx
    tmxObj_.LoadTmx("resource/tmx/title.tmx", false);
    //�J�����쐬
    camera_         = std::make_unique<Camera>();	//�J����
    //�V�F�[�_�[�N���X�̍쐬
    originalShader_ = std::make_unique<OriginalShader>(
        LoadVertexShader("resource/shader/vs/NormalModel_vs.vso"),
        LoadPixelShader("resource/shader/ps/TitleModel_ps.pso"));

    //�R���g���[���̏�����
    InitController();
    //��ǂݍ���
    LoadingAhead();
    //C�o�b�t�@�̏�����
    InitCBuffer();

    pos_ = {0,0,0};
    //�}�E�X�擾
    GetMousePoint(&mousePos_.x, &mousePos_.y);
}

UniqueScene TitleScene::Update(UniqueScene scene)
{
    //�R���g���[���̍X�V
    controller_->Update();
    //C�o�b�t�@�̍X�V
    UpdateCBuffer();
    //�N���b�N���ē��������R���W�����擾
    std::string hitStr = ClickHitTmxCol();

    if (hitStr == "play")
    {
        return std::make_unique<LoadScene>();
    }
    if (hitStr == "oprion")
    {
        return std::make_unique<OptionScene>(std::move(scene), controller_->GetControllerType(), GetSceneID());
    }
    if (hitStr == "exit")
    {
        SetWindowVisibleFlag(false);
        DxLib_End();
    }
    
    DrawScreen();
    return scene;
}

void TitleScene::DrawScreen(void)
{
    SetDrawScreen(screenID_);
    ClsDrawScreen();

    //�J��������(�I�u�W�F�N�g�`��O�ɏ���)
    camera_->SetBeforeDraw(pos_);
    //�^�C�g�����f���̕`��
    originalShader_->Draw(lpModelMng.GetID(modleStr_)[0]);
    
    //tmxcol�̍��W�ɉ摜��`��
    DrawTmxCollPosition();
    //���̕\�� �ŏI�I�ɂ�tmx�ɂȂ�\��
    DrawFormatString(screenSizeX_ - 300, screenSizeY_ - 50, 0xffffff, "�N���b�N�Ō���");

    //�f�o�b�O�p
    // �[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
    //MV1DrawModelDebug(lpModelMng.GetID("resource/model/stage/titlemap.mv1")[0],0xffff44, 1, 0.001, 1, 1);
    //camera_->Draw();
    //lpCBufferMng.DebugBufferDraw("�X�|�b�g���C�g", 9, 16);
    //�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
}

void TitleScene::Release(void)
{
}

void TitleScene::InitController()
{
    //�p�b�h�̐����P�ȏ�̏ꍇ�̓p�b�h����ɂ���
    if (GetJoypadNum() > 0)
    {
        controller_ = std::make_unique<PadInput>();
    }
    else
    {
        controller_ = std::make_unique<KeyInput>();
    }
}

std::string TitleScene::ClickHitTmxCol()
{
    if (controller_->CheckInputKey(KeyID::Decide))
    {
        for (auto& coll : tmxObj_.GetTitleCollList())
        {
            if (CheckCollMouse()(mousePos_, coll.second))
            {
                return coll.first;
            }
        }
    }
    return "";
}

void TitleScene::DrawTmxCollPosition()
{
    Vector2Int GraphSize;
    for (auto& coll : tmxObj_.GetTitleCollList())
    {
        Vector2 cen = { coll.second.first.x + coll.second.second.x / 2 ,coll.second.first.y + coll.second.second.y / 2 };
        if (coll.first == "title")
        {
            GetGraphSize(lpImageMng.GetID("title")[0], &GraphSize.x, &GraphSize.y);
            GraphSize.x /= 2;
            DrawGraph(cen.x - GraphSize.x, coll.second.first.y, lpImageMng.GetID("title")[0], true);
        }
        if (coll.first == "play")
        {
            //DrawBox(coll.second.first.x, coll.second.first.y, coll.second.first.x + coll.second.second.x, coll.second.first.y + coll.second.second.y, 0x552222, false);
            GetGraphSize(lpImageMng.GetID("play")[0], &GraphSize.x, &GraphSize.y);
            GraphSize.x /= 2;

            DrawGraph(cen.x - GraphSize.x, coll.second.first.y, lpImageMng.GetID("play")[0], true);
        }
        if (coll.first == "oprion")
        {
            //DrawBox(coll.second.first.x, coll.second.first.y, coll.second.first.x + coll.second.second.x, coll.second.first.y + coll.second.second.y, 0x552222, false);
            GetGraphSize(lpImageMng.GetID("option")[0], &GraphSize.x, &GraphSize.y);
            GraphSize.x /= 2;

            DrawGraph(cen.x - GraphSize.x, coll.second.first.y, lpImageMng.GetID("option")[0], true);
        }
        if (coll.first == "exit")
        {
            //DrawBox(coll.second.first.x, coll.second.first.y, coll.second.first.x + coll.second.second.x, coll.second.first.y + coll.second.second.y, 0x552222, false);
            GetGraphSize(lpImageMng.GetID("exit")[0], &GraphSize.x, &GraphSize.y);
            GraphSize.x /= 2;

            DrawGraph(cen.x - GraphSize.x, coll.second.first.y, lpImageMng.GetID("exit")[0], true);
        }
        if (coll.first == "mouse")
        {
            DrawBox(coll.second.first.x, coll.second.first.y, coll.second.first.x + coll.second.second.x, coll.second.first.y + coll.second.second.y, 0x552222, true);
        }
    }
}

void TitleScene::LoadingAhead()
{
    SetUseASyncLoadFlag(true);

    //�^�C�g���摜
    lpImageMng.GetID("resource/image/Font/box.png", "title");
    lpImageMng.GetID("resource/image/Font/play.png", "play");
    lpImageMng.GetID("resource/image/Font/option.png", "option");
    lpImageMng.GetID("resource/image/Font/exit.png", "exit");

    //�Q�[���摜
    lpImageMng.GetID("resource/image/Font/play.png", "tutorial");

    //�I�v�V�����摜
    lpImageMng.GetID("resource/image/Font/optionTag.png", "optionTag");
    lpImageMng.GetID("resource/image/Font/controlTag.png", "controlTag");
    lpImageMng.GetID("resource/image/Font/audioTag.png", "audioTag");
    lpImageMng.GetID("resource/image/Font/graphicTag.png", "graphicTag");
    lpImageMng.GetID("resource/image/Font/titleTag.png", "titleReturnTag");
    lpImageMng.GetID("resource/image/Font/returnTag.png", "returnTag");
    lpImageMng.GetID("resource/image/Font/volume.png", "volume");
    lpImageMng.GetID("resource/image/Font/brightness.png", "brightness");
    lpImageMng.GetID("resource/image/Font/key.png", "key");
    lpImageMng.GetID("resource/image/Font/smp.png", "smp");

    lpImageMng.GetID("resource/image/option/select10.png", "selection");
    lpImageMng.GetID("resource/image/option/select2.png", "selection1");
    lpImageMng.GetID("resource/image/option/select2.png", "selection2");

    lpImageMng.GetID("resource/image/Font/optionTag.png", "supportControl");
    lpImageMng.GetID("resource/image/Font/controlTag.png", "supportAudio");
    lpImageMng.GetID("resource/image/Font/audioTag.png", "supportGraphic");
    lpImageMng.GetID("resource/image/Font/graphicTag.png", "supportTitleReturn");
    lpImageMng.GetID("resource/image/Font/key.png", "supportReturn");

    lpImageMng.GetID("resource/image/option/line3.png", "line2");
    lpImageMng.GetID("resource/image/option/box.png", "select5");
    lpImageMng.GetID("resource/image/option/Bbox.png", "Bbox");
    lpImageMng.GetID("resource/image/Font/masterValume.png", "masterValume");
    lpImageMng.GetID("resource/image/Font/screenBrightness.png", "screenBrightness");

    //���U���g�摜
    lpImageMng.GetID("resource/image/Font/congratulation.png", "resultCongratulation");
    lpImageMng.GetID("resource/image/Font/gameclear.png", "gameclear");
    lpImageMng.GetID("resource/image/Font/totitle.png", "Totitle");
   
    //�Q�[���I�[�o�[�摜
    lpImageMng.GetID("resource/image/Font/gameover.png", "gameover");
    lpImageMng.GetID("resource/image/Font/restart.png", "reStartTag");
    lpImageMng.GetID("resource/image/Font/totitle.png", "toTitleTag");
    lpImageMng.GetID("resource/image/gameover/Blood5.png", "selectB");

    //���f���摜
    lpModelMng.GetID("resource/model/stage/map2.mv1");
    lpModelMng.GetID("resource/model/stage/Coll3.mv1");
    lpModelMng.GetID("./resource/model/gimmick/DoorDouble.mv1", 20);
    lpModelMng.GetID("./resource/model/gimmick/LockDoor.mv1", 10);
    lpModelMng.GetID("./resource/model/gimmick/Door_Red1.mv1", 10);
    lpModelMng.GetID("./resource/model/gimmick/Door_White1.mv1", 10);
    lpModelMng.GetID("./resource/model/gimmick/Door_White1_Double.mv1", 10);
    lpModelMng.GetID("./resource/model/gimmick/Door_White2.mv1", 5);
    lpModelMng.GetID("./resource/model/gimmick/Door_White2_Double.mv1", 5);
    lpModelMng.GetID("./resource/model/gimmick/Door_White3_Double.mv1", 10);
    lpModelMng.GetID("./resource/model/gimmick/Door_Dark1_Double.mv1", 10);
    lpModelMng.GetID("./resource/model/gimmick/Door_Dark1.mv1", 10);
    lpModelMng.GetID("./resource/model/gimmick/drawer_frame_L.mv1", 10);
    lpModelMng.GetID("./resource/model/gimmick/drawer.mv1", 10);



    modleStr_ = "resource/model/stage/titlemap.mv1";
    lpModelMng.GetID(modleStr_);
    SetUseASyncLoadFlag(false);
}

void TitleScene::InitCBuffer()
{
    //�o�b�t�@�̏�����
    lpCBufferMng.Remove();
    lpCBufferMng.Create();

    //�J�����o�b�t�@�̍쐬
    lpCBufferMng.CreatePixelCbuffer(8);

    //�o�b�t�@�֑��
    lpCBufferMng.SetPixelCbuffer(8, 0,
        { camera_->GetPos().x,
        camera_->GetPos().y,
        camera_->GetPos().z,
        0 });
    camera_->GetPos();
    //���C�g�o�b�t�@�̍쐬
    lpCBufferMng.CreatePixelCbuffer(16);

    //���C�g�̏�����
    titleLight_.SpotLights.Position = camera_->GetPos();

    titleLight_.SpotLights.Color.x = 10.0f;
    titleLight_.SpotLights.Color.y = 10.0f;
    titleLight_.SpotLights.Color.z = 10.0f;

    titleLight_.SpotLights.Direction.x = 0.0f;
    titleLight_.SpotLights.Direction.y = 0.0f;
    titleLight_.SpotLights.Direction.z = 1.0f;

    titleLight_.SpotLights.Direction.Normalize();

    titleLight_.SpotLights.Range = 8000.0f;
    titleLight_.SpotLights.Angle = Deg2RadF(15.0f);

    titleLight_.SpotLights.pad0 = 0;

    //���C�g�o�b�t�@�ɒl�̑��
    lpCBufferMng.SetPixelCbuffer(9, 0,
        { titleLight_.SpotLights.Position.x ,
        titleLight_.SpotLights.Position.y,
        titleLight_.SpotLights.Position.z,
        titleLight_.SpotLights.pad0 });
    lpCBufferMng.SetPixelCbuffer(9, 1,
        { titleLight_.SpotLights.Color.x ,
        titleLight_.SpotLights.Color.y,
        titleLight_.SpotLights.Color.z,
        titleLight_.SpotLights.Range });
    lpCBufferMng.SetPixelCbuffer(9, 2,
        { titleLight_.SpotLights.Direction.x ,
        titleLight_.SpotLights.Direction.y,
        titleLight_.SpotLights.Direction.z,
        titleLight_.SpotLights.Angle });
}

void TitleScene::UpdateCBuffer()
{
    //���ׂẴo�b�t�@�̍X�V
    lpCBufferMng.Update();

    //�O�̍��W������
    oldMousePos_ = mousePos_;

    //�}�E�X���W�̍X�V
    GetMousePoint(&mousePos_.x, &mousePos_.y);

    //�X�|�b�g���C�g�̍X�V
    titleLight_.SpotLights.Position = camera_->GetPos();

    titleLight_.SpotLights.Direction += {
        static_cast<float>(mousePos_.x - oldMousePos_.x) / 1000,
            static_cast<float>(oldMousePos_.y - mousePos_.y) / 1000,
            0.0f };
    titleLight_.SpotLights.Direction.Normalize();

    //���C�g�o�b�t�@�̑��
    lpCBufferMng.SetPixelCbuffer(9, 0,
        { titleLight_.SpotLights.Position.x ,
        titleLight_.SpotLights.Position.y,
        titleLight_.SpotLights.Position.z,
        0 });
    lpCBufferMng.SetPixelCbuffer(9, 2,
        { titleLight_.SpotLights.Direction.x ,
        titleLight_.SpotLights.Direction.y,
        titleLight_.SpotLights.Direction.z,
        titleLight_.SpotLights.Angle });
    lpCBufferMng.Update();
}
