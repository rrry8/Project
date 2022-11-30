#pragma once
#include<memory>
#include "BaseScene.h"
#include "../tmx/TmxObj.h"

class Controller;
enum class ControllerType;

class OptionScene :
    public BaseScene
{
public:
    OptionScene(UniqueScene ownScene,ControllerType type, SceneID beforeID);
    ~OptionScene();

    // ����������
    void Init(void) override;

    // �X�V�X�e�b�v
    UniqueScene Update(UniqueScene scene) override;

    // �`�揈��
    void DrawScreen(void) override;

    // �������
    void Release(void) override;

    //�V�[����ID��Ԃ�
    SceneID GetSceneID(void) override { return SceneID::Option; };
private:
    
    UniqueScene beforeScene_;    //�O�̃V�[��(��ɃQ�[���V�[��)
    UniqueScene returningScene_; //�V�[����؂�ւ��鎞�p
    SceneID beforeSceneID_;      //�O�̃V�[��ID

    std::unique_ptr<Controller> controller_;//�R���g���[�����

    //�t�@�[�X�g��tmx�̕�����Z�J���h�͏����̃|�C���^�[
    std::map<std::string, void (OptionScene::*)()> selectList_;//�����܂Ƃ߃��X�g

    //std::map<std::string, std::string>* listToDraw_;        //�`�悷�郊�X�g�̃|�C���^

    //�t�@�[�X�g��tmx�̕�����Z�J���h�͉摜�̕�����
    //�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
    std::map<std::string, std::string> displayList_;        //�`�悾�����X�g
    std::map<std::string, std::string> controlList_;        //���샊�X�g
    std::map<std::string, std::string> audioList_;          //�����X�g
    std::map<std::string, std::string> graphicList_;        //�O���t�B�b�N���X�g
    std::map<std::string, std::string> titleRetureList_;    //�^�C�g���ɍs�����X�g
    std::map<std::string, std::string> retureList_;         //�߂郊�X�g

    std::map<std::string, std::string> supportList_;         
    //�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[

    TmxObj tmxObj_;     //�Փ˔���ptmx
    
    std::string selection_; //���I�����Ă�����̂��i�[����ꏊ

    std::pair<std::string, std::string> OldhitMouse_;       //���}�E�X���������Ă�����̂��i�[����ꏊ
    std::pair<std::string,std::string> hitMouse_;       //���}�E�X���������Ă�����̂��i�[����ꏊ

    int Cnt_;       //�_�ł̃J�E���g
    int Volume_;    //����
    int brightness_;//���邳


    //�摜�Ƀt�B���^��������
    void InitGraphFilter();
    //���X�g�Ɋi�[
    void InList();
    //�ϐ��̏�����
    void InitVariable();
    //�Փ˔���
    void HitCollAndAssignProcess();

    /// <summary>
    /// �N���b�N�����Ƃ�����������
    /// </summary>
    /// <param name="coll">�l�p�`�Փ˔���</param>
    void HitCollClickAndAssignProcess(std::pair<std::string, Collision> coll);

    /// <summary>
    /// �}�E�X���������Ă��邩
    /// </summary>
    /// <param name="coll">�l�p�`�Փ˔���</param>
    std::pair<std::string, std::string> HitCollMouse(std::pair<std::string, Collision> coll);


    //�֐��|�C���^
    //�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
    void (OptionScene::* UpdateFuncPtr_)();
    
    //�X�V�֐��|�C���^�ɑ������֐�
    void UpdateControl();         //�L�[����
    void UpdateAudio();           //����
    void UpdateGraphic();         //���邳����
    void UpdateToTitle();     // �^�C�g���ɍs��
    void UpdateReturnScreen();    //�߂�

    //�`��֐��|�C���^
    void (OptionScene::* DrawFuncPtr_)();

    //�`��֐��|�C���^�ɑ������֐�
    void DrawControl();         //�L�[����
    void DrawAudio();           //����
    void DrawGraphic();         //���邳����
    void DrawToTitle();     // �^�C�g���ɍs��
    void DrawReturnScreen();    //�߂�
    //�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
    
    //�܂Ƃ߂��R���W������draw
    void DrawColl();
    /// <summary>
    /// �R���W������Y���𒆐S�ɕ`��
    /// </summary>
    /// <param name="coll">�l�p�`�Փ˔���</param>
    /// <param name="imageStr">�摜�̃n���h���p������</param>
    void DrawCollCenterY(std::pair<std::string, Collision> coll, std::string imageStr);

    void DrawCollCenter(std::pair<std::string, Collision> coll, std::string imageStr);


    /// <summary>
    /// SelectList��p�`��
    /// </summary>
    /// <param name="coll">�l�p�`�Փ˔���</param>
    void DrawSelectList(std::pair<std::string, Collision> coll);

    /// <summary>
    /// ���X�g�̕`��
    /// </summary>
    /// <param name="coll">�l�p�`�Փ˔���</param>
    /// <param name="maplist">�`��Ɏg���}�b�v���X�g</param>
    void DrawList(std::pair<std::string, Collision> coll, std::map<std::string, std::string> maplist);
};