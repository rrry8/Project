#pragma once
#include<memory>
#include "BaseScene.h"
#include "../tmx/TmxObj.h"
class Controller;
class Camera;
class OriginalShader;
enum class ControllerType;

class GameOverScene :
    public BaseScene
{
public:
    GameOverScene(UniqueScene ownScene, ControllerType type, SceneID beforeID,Vector3 playerPos,Vector3 enemyPos);
    ~GameOverScene();

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
    std::map<std::string, void (GameOverScene::*)()> selectList_;//�����܂Ƃ߃��X�g

    //std::map<std::string, std::string>* listToDraw_;        //�`�悷�郊�X�g�̃|�C���^

    //�t�@�[�X�g��tmx�̕�����Z�J���h�͉摜�̕�����
    std::map<std::string, std::string> displayList_;        //�`�悾�����X�g
    std::map<std::string, std::string> toTitleList_;    //�^�C�g���ɍs�����X�g
    std::map<std::string, std::string> reStartList_;    //���X�^�[�g���X�g

    std::map<std::string, std::string> supportList_;
    TmxObj tmxObj_;     //�Փ˔���ptmx

    std::string selection_; //���I�����Ă�����̂��i�[����ꏊ

    std::pair<std::string, std::string> OldhitMouse_;       //���}�E�X���������Ă�����̂��i�[����ꏊ
    std::pair<std::string, std::string> hitMouse_;       //���}�E�X���������Ă�����̂��i�[����ꏊ

    int Cnt_;       //�_�ł̃J�E���g
    int Volume_;    //����
    int brightness_;//���邳

    //�[�[�[�[�Q�[���I�[�o�[���̉��o�֘A�[�[�[�[
    std::unique_ptr<Camera> camera_;
    bool directionEndflg_;//���o�I���t���O
    Vector3 playerPos_;
    Vector3 enemyPos_;

    std::unique_ptr<OriginalShader> originalShader_;//�X�e�[�W�`��p
    //�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
    //���X�g�Ɋi�[
    void InList();

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


    //�X�V�֐��|�C���^
    void (GameOverScene::* UpdateFuncPtr_)();

    //�X�V�֐��|�C���^�ɑ������֐�
    void UpdateToTitle();     // �^�C�g���ɍs��
    void UpdateReStart();    //���X�^�[�g

    //�`��֐��|�C���^
    void (GameOverScene::* DrawFuncPtr_)();

    //�`��֐��|�C���^�ɑ������֐�
    void DrawToTitle();         // �^�C�g���ɍs��
    void DrawReStart();         //�߂�

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

