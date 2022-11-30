#pragma once
#include<array>
#include"../Common/Geometry.h"
#include "UIBase.h"
#define STR_POINT_X (screenSize_.x - 440)//NowLoading��X���W
#define STR_POINT_Y (screenSize_.y - 64)//NowLoading��Y���W
//----loadingBar�̒��_�̍��W---------
#define BAR_LEFT_TOP_X (500)//����
#define BAR_LEFT_TOP_Y (600)
#define BAR_LEFT_DOWN_X (500)//����
#define BAR_LEFT_DOWN_Y (650)
#define BAR_RIGHT_TOP_X (1500)//�E��
#define BAR_RIGHT_TOP_Y (600)
#define BAR_RIGHT_DOWN_X (1500)//�E��
#define BAR_RIGHT_DOWN_Y (650)
//-----------------------------------

class UILoad ://���[�h��ʗpUI�̃N���X
    public UIBase
{
public:
    UILoad();
    ~UILoad();

private:
    bool Init(void)override;
    void Update(double delta)override;
    void DrawOwnScreen(double delta)override;
    Scene GetSceneID(void) override { return Scene::Load; }
    bool GetLoadingEndFlg(void)override;
    void SetIsNext(bool flg)override;
    void MyDrawGraph(int Imghandle, int pshandle);
    void PSInit(void);
    void PSUpdate(void);
    int LoadVol;//�o�[�̕�
    int LoadingTime;//�o�ߎ���
    bool IsLoadingEnd;//���[�h���I��������true
    std::array<VERTEX2DSHADER, 4> verts;
    int cBuf;
    float* writer;
    int psH;//�s�N�Z���V�F�[�_�p�n���h��
};


