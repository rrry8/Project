#include<DxLib.h>
#include "InventoryScene.h"
#include "SceneManager.h"
#include "../object/Item/Item.h"

#include "../input/PadInput.h"
#include "../input/KeyInput.h"

#include "../tmx/TmxObj.h"
#include "../common/CheckHit.h"
#include "../common/Geometry.h"

#include "../common/manager/ModelMng.h"
#include "../common/manager/ImageMng.h"
#include"../common/OriginalShader.h"
#include "../common/SocketServer.h"

#include "../../_debug/_DebugConOut.h"

//�ڂ����̓x����
constexpr float BLUR_DEGREE = 8.0f;

//�����̃I�t�Z�b�g��������
constexpr float OFFSET = 50.0f;

//�ʐ^�Ƀ}�E�X�����킹���Ƃ��ɒǉ�����l
constexpr float   PHOTO_ADD_POS = 5.0f;

//�t���[���̐F(��)
constexpr int   FRAME_COLOR = 125;


InventoryScene::InventoryScene(UniqueScene beforeScene,int bgHandle,ControllerType type, const std::vector<InventoryObj>& inventoryList, const std::vector<int>& photo)
{
    beforeScene_ = std::move(beforeScene);

    if (type == ControllerType::Pad)
    {
        controller_ = std::make_unique<PadInput>();
    }
    else
    {
        controller_ = std::make_unique<KeyInput>();
    }

    //�w�i
    backGroundHandle_ = bgHandle;
    controller_->Update();

    inventoryList_ = inventoryList;
    photoVector_ = photo;

    Init();

    //������h�~
    SetDrawScreen(screenID_);
    ClsDrawScreen();
    DrawPrimitive2D(totalBlurVerts_.data(), totalBlurVerts_.size(), DX_PRIMTYPE_TRIANGLESTRIP, backGroundHandle_, false);
}

InventoryScene::~InventoryScene()
{
    Release();
}

void InventoryScene::Init(void)
{
    //�e�[�u��������
    InitTable();

    psBlurXandYHandle_ = LoadPixelShader("./resource/shader/ps/blur_ps.pso");
    mainRenderTarget_ = MakeScreen(screenSizeX_, screenSizeY_);

    for (auto& v : totalBlurVerts_)
    {
        v.rhw = 1.0f;
        v.dif = GetColorU8(255, 255, 255, 255);
        v.pos.z = 0.0f;
    }

    totalBlurVerts_[0].pos.x = 0.0f;
    totalBlurVerts_[0].pos.y = 0.0f;

    totalBlurVerts_[1].pos.x = static_cast<float>(screenSizeX_);
    totalBlurVerts_[1].pos.y = 0.0f;

    totalBlurVerts_[2].pos.x = 0.0f;
    totalBlurVerts_[2].pos.y = static_cast<float>(screenSizeY_);

    totalBlurVerts_[3].pos.x = static_cast<float>(screenSizeX_);
    totalBlurVerts_[3].pos.y = static_cast<float>(screenSizeY_);

    totalBlurVerts_[0].u = 0.0f;
    totalBlurVerts_[0].v = 0.0f;

    totalBlurVerts_[1].u = 1.0f;
    totalBlurVerts_[1].v = 0.0f;

    totalBlurVerts_[2].u = 0.0f;
    totalBlurVerts_[2].v = 1.0f;

    totalBlurVerts_[3].u = 1.0f;
    totalBlurVerts_[3].v = 1.0f;

    //�d�݌v�Z
    CalcBlurWeights();

    for (int n = 0; n < WEIGHTS_NUM; n++)
    {
        lpSceneMng.SetWriterData(n, weights_[n]);
    }

    InitBlur();

    if (lpSceneMng.GetInventoryCnt() ==1)
    {
        isDrawTips_ = true;
    }

    //�ʐ^�֌W
    psPhotoHandle_  = LoadPixelShader("./resource/shader/ps/Memo_ps.pso");
    photo_          = std::make_unique<OriginalShader>(psPhotoHandle_);
    photoAddPos_ = 0.0f;

    InitInventory();
}

void InventoryScene::InitBlur(void)
{
    //���u���[
    //�����̉𑜓x����ʃT�C�Y�̔�����
    blurX_.renderTarget = MakeScreen(screenSizeX_ / 2, screenSizeY_);
    blurX_.vsHandle = LoadVertexShader("./resource/shader/vs/blurX_vs.vso");

    for (auto& v : blurX_.verts)
    {
        v.dif = GetColorU8(255, 255, 255, 255);
        v.spc = GetColorU8(0, 0, 0, 0);
    }
    //����
    blurX_.verts[0].pos.x = 0.0f;
    blurX_.verts[0].pos.y = static_cast<float>(screenSizeY_);
    //�E��
    blurX_.verts[1].pos.x = static_cast<float>(screenSizeX_ / 2);
    blurX_.verts[1].pos.y = static_cast<float>(screenSizeY_);
    //����
    blurX_.verts[2].pos.x = 0.0f;
    blurX_.verts[2].pos.y = 0.0f;
    //�E��
    blurX_.verts[3].pos.x = static_cast<float>(screenSizeX_ / 2);
    blurX_.verts[3].pos.y = 0.0f;
    //UV���W
    blurX_.verts[0].u = 0.0f;
    blurX_.verts[0].v = 0.0f;

    blurX_.verts[1].u = 1.0f;
    blurX_.verts[1].v = 0.0f;

    blurX_.verts[2].u = 0.0f;
    blurX_.verts[2].v = 1.0f;

    blurX_.verts[3].u = 1.0f;
    blurX_.verts[3].v = 1.0f;

    //�c�u���[
    //�c�u���[�͏c���̉𑜓x����ʃT�C�Y�̔�����
    blurY_.renderTarget = MakeScreen(screenSizeX_, screenSizeY_ / 2);
    blurY_.vsHandle = LoadVertexShader("./resource/shader/vs/blurY_vs.vso");

    for (auto& v : blurY_.verts)
    {
        v.dif = GetColorU8(255, 255, 255, 255);
        v.spc = GetColorU8(0, 0, 0, 0);
    }
    //����
    blurY_.verts[0].pos.x = 0.0f;
    blurY_.verts[0].pos.y = static_cast<float>(screenSizeY_ / 2);
    //�E��
    blurY_.verts[1].pos.x = static_cast<float>(screenSizeX_);
    blurY_.verts[1].pos.y = static_cast<float>(screenSizeY_ / 2);
    //����
    blurY_.verts[2].pos.x = 0.0f;
    blurY_.verts[2].pos.y = 0.0f;
    //�E��
    blurY_.verts[3].pos.x = static_cast<float>(screenSizeX_);
    blurY_.verts[3].pos.y = 0.0f;

    //UV���W
    blurY_.verts[0].u = 0.0f;
    blurY_.verts[0].v = 0.0f;

    blurY_.verts[1].u = 1.0f;
    blurY_.verts[1].v = 0.0f;

    blurY_.verts[2].u = 0.0f;
    blurY_.verts[2].v = 1.0f;

    blurY_.verts[3].u = 1.0f;
    blurY_.verts[3].v = 1.0f;

}

UniqueScene InventoryScene::Update(UniqueScene scene)
{
    controller_->Update();
    
    if (controller_->CheckInputKey(KeyID::Inventory))
    {
        //�}�E�X�J�[�\����^�񒆂ɖ߂��ăJ������]�����Ȃ��悤�ɂ���
        //�^�񒆂ɖ߂��Ȃ������ꍇ�̓C���x���g������߂����ۂɓ�����������]���Ă��܂�
        SetMousePoint(lpSceneMng.GetScreenSize().x / 2, lpSceneMng.GetScreenSize().y / 2);
        return std::move(beforeScene_);
    }

    UpdateInventory();
    
    //�f�o�b�O�p
    if (CheckHitKey(KEY_INPUT_6))
    {
        isDrawTips_ = false;
    }

    DrawScreen();
    return scene;
}

void InventoryScene::DrawScreen(void)
{
    //���C�������_�����O�^�[�Q�b�g�ɕ`��
    SetDrawScreen(mainRenderTarget_);
    ClsDrawScreen();

    //�w�i
    DrawPrimitive2D(totalBlurVerts_.data(), totalBlurVerts_.size(), DX_PRIMTYPE_TRIANGLESTRIP, backGroundHandle_, false);

    //���u���[��������
    SetDrawScreen(blurX_.renderTarget);
    ClsDrawScreen();
    DrawBlur(blurX_.vsHandle, psBlurXandYHandle_, mainRenderTarget_, blurX_.verts); //���C�������_�����O�^�[�Q�b�g�̃n���h��

    //�c�u���[��������
    SetDrawScreen(blurY_.renderTarget);
    ClsDrawScreen();
    DrawBlur(blurY_.vsHandle, psBlurXandYHandle_, blurX_.renderTarget, blurY_.verts);  //���u���[�̃n���h��

    //�ŏI�`��
    SetDrawScreen(screenID_);
    ClsDrawScreen();

    
    //�c�u���[�̂��̂�`��
    DrawPrimitive2D(totalBlurVerts_.data(), totalBlurVerts_.size(), DX_PRIMTYPE_TRIANGLESTRIP, blurY_.renderTarget, false);
    DrawString(0, 150, "�C���x���g��", 0x00ffff);

    DrawInventory();
}

void InventoryScene::Release(void)
{
    DeleteGraph(mainRenderTarget_);
    DeleteGraph(blurX_.renderTarget);
    DeleteGraph(blurY_.renderTarget);

    DeleteShader(psBlurXandYHandle_);
    DeleteShader(blurX_.vsHandle);
    DeleteShader(blurY_.vsHandle);
    DeleteShader(psPhotoHandle_);
}

void InventoryScene::CalcBlurWeights(void)
{
    float totalWeight = 0.0f;

    for (int x = 0; x < WEIGHTS_NUM; x++)
    {
        weights_[x] = expf(-0.5f * static_cast<float>(x * x) / BLUR_DEGREE);
        totalWeight += 2.0f * weights_[x];
    }

    for (int i = 0; i < WEIGHTS_NUM; i++)
    {
        weights_[i] /= totalWeight;
    }
}

void InventoryScene::CollMousePosAndPhoto(void)
{
    //�}�E�X�̈ʒu�擾
    Vector2Int mPos;
    GetMousePoint(&mPos.x, &mPos.y);

    for (const auto& coll : tmxObj_.GetInventoryCollList())
    {
        //�}�E�X�̈ʒu�Ǝʐ^�̈ʒu���������ǂ�������
        if (CheckCollMouse()(mPos, coll.second))
        {
            std::string collName = coll.first;

            //�Փˏꏊ���ʐ^�ł͂Ȃ��ꍇ��continue
            if (collName != "photo1" &&
                collName != "photo2" &&
                collName != "photo3")
            {
                continue;
            }
            photoAddPos_ = 5.0f;
        }
    }
}

void InventoryScene::DrawBlur(int vs, int ps,int target,const std::array<VERTEX3DSHADER, 4>& verts)
{
    SetUseVertexShader(vs);
    SetUsePixelShader(ps);
    SetUseTextureToShader(0, target);
    DrawPrimitive3DToShader(verts.data(), verts.size(),DX_PRIMTYPE_TRIANGLESTRIP);
}

void InventoryScene::DrawPhoto(std::string name, Vector2 pos, Vector2 imageSize)
{
    //�ʐ^�ł͂Ȃ��ꍇ�͕Ԃ�
    if (name != "photo1"&&
        name != "photo2"&&
        name != "photo3")
    {
        return;
    }

    //���_���
    std::array<Vector3, 4> vertPos;

    vertPos[0] = Vector3(pos.x, pos.y, 0.0f);
    vertPos[1] = Vector3(pos.x + imageSize.x, pos.y, 0.0f);
    vertPos[2] = Vector3(pos.x, pos.y + imageSize.y, 0.0f);
    vertPos[3] = Vector3(pos.x + imageSize.x, pos.y + imageSize.y, 0.0f);

    //�ʐ^�̐��l�𒊏o
    int num = atoi(name.substr(name.length() - 1, 1).c_str());

    //�z��ɐ��l�����킹�ĕ`��
    photo_->DrawOnlyPixel(vertPos, photoVector_[num - 1]);
}

void InventoryScene::InitInventory(void)
{
    //�C���x���g���̃f�[�^�ǂݍ���
    tmxObj_.LoadTmx("resource/tmx/inventory.tmx", false);
    for (auto& inventory : inventoryList_)
    {
        //�l�̏������ƃT�C�Y�̒���
        inventory.pos_ = { 0.0f,0.0f,0.0f };
        MV1SetScale(lpModelMng.GetID(inventory.modelString_)[inventory.modelNum_], VScale(inventory.scale_.toVECTOR(), 0.1));
    }

    for (auto& coll : tmxObj_.GetInventoryCollList())
    {
        for (auto& itemTable : itemTable_)
        {
            if (coll.first == itemTable.first)
            {
                for (auto& inventory : inventoryList_)
                {
                    //�l�̃Z�b�g
                    if (inventory.pos_ == Vector3{ 0.0f,0.0f,0.0f })
                    {
                        //�l�ƃC���x���g���̔Ԓn�̃Z�b�g
                        inventory.pos_ = { coll.second.first.x + OFFSET,lpSceneMng.GetScreenSize().y - coll.second.first.y - OFFSET,0 };
                        inventory.inventoryNum_ = itemTable.first;
                        break;
                    }
                }
            }
        }
    }
}

void InventoryScene::UpdateInventory(void)
{
    hitItemID_ = ItemID::Non;
    Vector2Int mPos;
    GetMousePoint(&mPos.x, &mPos.y);

    for (auto& coll : tmxObj_.GetInventoryCollList())
    {
        for (auto& inventory : inventoryList_)
        {
            if (coll.first != inventory.inventoryNum_)
            {
                continue;
            }
            for (auto& itemTable : itemTable_)
            {
                if (itemTable.first != inventory.inventoryNum_)
                {
                    continue;
                }
                //�}�E�X�̓����������̐F��ς���
                //�ǂ�ID���i�[
                if (CheckCollMouse()(mPos, coll.second))
                {
                    hitItemID_ = inventory.itemID_;
                    itemTable.second = { 255,255,255 };
                }
                else
                {
                    itemTable.second = { 125,125,125 };
                }
               
                if (inventory.inventoryNum_ == lpSocketServer.GetinventorySelect())
                {
                    hitItemID_ = inventory.itemID_;
                    itemTable.second = { 255,255,255 };
                }
            }
            
        }
    }
    
    for (auto& inventory : inventoryList_)
    {
        MV1SetPosition(lpModelMng.GetID(inventory.modelString_)[inventory.modelNum_], inventory.pos_.toVECTOR());
    }
}

void InventoryScene::DrawInventory(void)
{
    int color = FRAME_COLOR;
    for (auto& coll : tmxObj_.GetInventoryCollList())
    {
        for (auto& inventory : inventoryTable_)
        {
            if (coll.first == inventory.first)
            {
                //�X�g�����O�ɉ��������ĂȂ������畁�ʂ̎l�p������
                if (inventory.second == "")
                {
                    DrawBox(coll.second.first.x,
                        coll.second.first.y,
                        coll.second.first.x + coll.second.second.x,
                        coll.second.first.y + coll.second.second.y,
                        GetColor(color, color, color), false);
                    color += 1;
                }
                else
                {
                    //�����Ă����炻���`�悷��
                    DrawExtendGraph(coll.second.first.x,
                        coll.second.first.y,
                        coll.second.first.x + coll.second.second.x,
                        coll.second.first.y + coll.second.second.y, lpImageMng.GetID(inventory.second)[0], true);
                    break;
                }
            }

        }
        //�A�C�e���̘g�̕`��
        for (auto& itemTable : itemTable_)
        {
            if (coll.first == itemTable.first)
            {
                DrawBox(coll.second.first.x,
                    coll.second.first.y,
                    coll.second.first.x + coll.second.second.x,
                    coll.second.first.y + coll.second.second.y,
                    GetColor(itemTable.second.x, itemTable.second.y, itemTable.second.z), false);
            }
        }
        //�ʐ^�`��
        DrawPhoto(coll.first, { coll.second.first }, { coll.second.second });
    }
    //���f���`��
    for (auto& inventory : inventoryList_)
    {
        MV1DrawModel(lpModelMng.GetID(inventory.modelString_)[inventory.modelNum_]);
    }
    //�����̕`��i���̓X�g�����O�ŏI�I�ɉ摜�j
    for (auto& coll : tmxObj_.GetInventoryCollList())
    {
        if (coll.first == "explanation")
        {
            for (auto& explanation : explanationTable_)
            {
                //ID�m�F
                if (hitItemID_ == explanation.first)
                {
                    DrawFormatString(coll.second.first.x, coll.second.first.y, 0xffffff, "%s", explanation.second.c_str());

                }
            }
        }
        if (coll.first == "target")
        {
            DrawFormatString(coll.second.first.x + OFFSET, coll.second.first.y + OFFSET, 0x000000, "���ւ̌��������ĒE�o���悤");
            DrawFormatString(coll.second.first.x + OFFSET, coll.second.first.y + OFFSET*2, 0x000000, "�T���Ă��镨");
            DrawGraph(coll.second.first.x + OFFSET*5, coll.second.first.y + OFFSET * 1.5,lpImageMng.GetID("resource/image/marker/marker.png")[0], true);
        }
    }
    for (auto& coll : tmxObj_.GetInventoryCollList())
    {
        for (auto& option : optionTable_)
        {
            if (coll.first == option.first)
            {
                DrawFormatString(coll.second.first.x + OFFSET, coll.second.first.y + OFFSET / 2, 0xffff55, "%s", option.second.c_str());
                if (isDrawTips_)
                {
                    if(coll.first == "invtyTips")
                    DrawBox(coll.second.first.x,
                        coll.second.first.y,
                        coll.second.first.x + coll.second.second.x,
                        coll.second.first.y + coll.second.second.y,
                        GetColor(144, 144, 144), false);
                }
            }
        }
    }
}

void InventoryScene::InitTable(void)
{
    inventoryTable_ = {
            {"belongings",	"resource/image/inventory/Inventory.png"},
            {"target",	"resource/image/inventory/memo.png"},
            {"explanation",""}
    };
    Vector3 Zero = { 125,125,125 };
    itemTable_ = {
        {"1",Zero},
        {"2",Zero},
        {"3",Zero},
        {"4",Zero},
        {"5",Zero},
        {"6",Zero},
        {"7",Zero},
        {"8",Zero},
        {"9",Zero},
        {"10",Zero},
        {"11",Zero},
        {"12",Zero},
        {"13",Zero},
        {"14",Zero},
        {"15",Zero},
        {"16",Zero}
    };

    explanationTable_ = {
        {ItemID::Key1,"���ւ̌�"},
        {ItemID::Key2,"��ꂽ��"},
        {ItemID::Key3,"�K�т���"},
        {ItemID::Key4,"��ꂽ��"},
        {ItemID::Key5,"��ꂽ��"},
        {ItemID::Phot,"�V��"}/*,
        {ItemID::Key1,""},
        {ItemID::Key1,""},
        {ItemID::Key1,""},
        {ItemID::Key1,""},
        {ItemID::Key1,""},*/
    };
    optionTable_ = {
            {"tab",	"tab�ŃQ�[���ɖ߂�"},
            {"mouse","�}�E�X�����킹�ăA�C�e���̊m�F"},
            {"invtyTips",""}
    };
}
