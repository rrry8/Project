#pragma once
#include "../Scene/GameScene.h"
#include"../Common/Geometry.h"
#include "../Scene/SceneManager.h"
#include "UIBase.h"
#include "../Scene/OptionScene.h"
#include "../Scene/MenuScene.h"
#include "../Transition/CrossOver.h"

class MouseIcon;

// �u������
#define lpSceneUIMng SceneUIManager::GetInstance()

enum class UI_MAIN_ITEM//���ږ�
{
	//-----------TITLE-----------
	SINGLEPLAY, //�P�lPLAY
	MULTIPLAY,  //�Q�l�v���C
	OPTION, //�ݒ�
	QUIT,   //�Q�[������߂�
	//----------SELECT-----------
	//-----------GAME------------
	//-----------RESULT----------
	TO_SELECT,//�Z���N�g�V�[����
	//----------OPTION-----------
	KEYCONFIG,//�L�[�R���t�B�O
	VOLUME,//���ʐݒ�
	TIPS,//�Q�[�����v���C�����ł̔錍�◠�Z���݂��
	RETURN,//�O�̃V�[���ɖ߂�
	//----------Menu-------------
	M_GAME,//�Q�[���ɂ��ǂ�
	M_RESTART,//���X�^�[�g
	M_OPTION,//�ݒ���J��
	M_SELECT,//�Z���N�g�ɖ߂�
	//----------NONE-------------
	NONE,
	MAX
};

enum class UI_SUB_ITEM//�e���ڂ̒����̍��ږ�
{
	//�L�[�R���t�B�O
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_MENU,
	KEY_RESET,
	KEY_CHANGE,
	
	//���ʐݒ�
	VOL_BGM,
	VOL_SE,
	//�y�[�W�؂�ւ��ȂǂŎg��
	NEXT,
	BACK,
	//�O�̃V�[���ɖ߂�@�Ƃ��@yes or no �𕷂��Ƃ��Ɏg��
	YES,
	NO,
	//----------NONE-------------
	NONE,
	MAX
};


class SceneUIManager
{
public:
	static SceneUIManager& GetInstance()
	{
		static SceneUIManager s_Instance;
		return s_Instance;
	}
	
	uniqueScene Init(uniqueScene &&scene);
	uniqueScene Init(uniqueScene &&scene,bool resetflg);
	bool Init(UI_MAIN_ITEM item);
	void Update(double delta);
	void Draw(double delta);

	void SetHitItem_Main(UI_MAIN_ITEM num);//�J�[�\�����d�Ȃ������ڂ̔ԍ����Z�b�g
	void SetHitItem_Sub(UI_SUB_ITEM num);//�J�[�\�����d�Ȃ������ڂ̔ԍ����Z�b�g
	UI_MAIN_ITEM GetHitItem_Main(void);//�J�[�\�����d�Ȃ������ڂ̔ԍ����Q�b�g
	UI_SUB_ITEM GetHitItem_Sub(void);//�J�[�\�����d�Ȃ������ڂ̔ԍ����Q�b�g
	void SetOptionState(OptionState state);//���ڕʂ̏�Ԃ̐ݒ�
	void SetMenuState(MenuState state);//���ڕʂ̏�Ԃ̐ݒ�
	OptionState GetOptionState();//���ڕʂ̏�Ԃ̎擾
	MenuState GetMenuState();//���ڕʂ̏�Ԃ̎擾
	Vector2Int GetScreenSize(void);
	bool GetLoadingEndFlg(void);
	void SetIsNext(bool flg);
	void SetStageNum(int stageNum);
	int GetStageNum(void);

	void SetSelectMapOffset(Vector2Int offset);
	Vector2Int GetSelectMapOffset();
	void SetSelectMapMoveFlg(bool flg);
	bool GetSelectMapMoveFlg();

	//int CoinData_Save(const char* FilePath,int data,int coinNum, int stageNum); // �R�C���̃f�[�^���t�@�C���ɕۑ�����( �߂�l : �ۑ������FTRUE   �ۑ����s�FFALSE )
	//int CoinData_Load(const char* FilePath,int coinNum, int stageNum); // �R�C���̃f�[�^���t�@�C������ǂݍ���( �߂�l : �ǂݍ��ݐ����FTRUE   �ǂݍ��ݎ��s�FFALSE )
	int CoinData_Save(const char* FilePath, bool coin1,bool coin2,bool coin3, int stageNum); // �R�C���̃f�[�^���t�@�C���ɕۑ�����( �߂�l : �ۑ������FTRUE   �ۑ����s�FFALSE )
	int CoinData_Load(const char* FilePath, int coinNum, int stageNum); // �R�C���̃f�[�^���t�@�C������ǂݍ���( �߂�l : �ǂݍ��ݐ����FTRUE   �ǂݍ��ݎ��s�FFALSE )
	int Data_Save(const char* FilePath, int data, int stageNum); // �N���A���ԂƂ����t�@�C���ɕۑ�����( �߂�l : �ۑ������FTRUE   �ۑ����s�FFALSE )
	int Data_Load(const char* FilePath, int stageNum); // �N���A���ԂƂ����t�@�C������ǂݍ���( �߂�l : �ǂݍ��ݐ����FTRUE   �ǂݍ��ݎ��s�FFALSE )
	void Data_SetDef(void);
private:
	SceneUIManager();
	~SceneUIManager();

	SCENE_ID mSceneID;		// �e�̃C���X�^���X

	UI_MAIN_ITEM MainItemNo;	//	�}�E�X�J�[�\�����d�Ȃ��Ă��鍀�ڂ̎��ʔԍ�
	UI_SUB_ITEM SubItemNo;	//	�}�E�X�J�[�\�����d�Ȃ��Ă��鍀�ڂ̎��ʔԍ�
	OptionState state_;	//�ݒ��ʂ̏��
	MenuState state_m;	//���j���[��ʂ̏��
	uniqueSceneUI sceneUI_;		//�V�[��
	int stageNum_;
	std::unique_ptr<MouseIcon> mouseIcon_;
	Vector2Int selectMapOffset_;
	bool selectMapMoveFlg_;
	int Data_;
	int data_[(int)STAGE_NAME::MAX];
	int coinData_[(int)STAGE_NAME::MAX][COIN_MAX];

};

