#pragma once
#include "BaseScene.h"
#include "GameScene.h"
#include"../Common/Geometry.h"

// �u������
#define lpSceneMng SceneManager::GetInstance()

//�V�[���Ǘ��p
enum SCENE_ID
{
	SCENE_TITLE,
	SCENE_SELECT,
	SCENE_GAME,
	SCENE_OPTION,
	MAX
};

class SceneManager
{
public:
	static SceneManager& GetInstance()
	{
		static SceneManager s_Instance;
		return s_Instance;
	}

	void Run(void);
	uniqueScene Update(void);
	void Draw(double delta);

	double GetDeltaTime(void);
	int GetFontHandle(void);
	void SetMultiFlg(bool flg);//�}���`���I�΂ꂽ��true
	bool GetMultiFlg(void);//�}���`���Ȃ�true
	Vector2Int GetScreenSize(void);
	void SetEndFlg(bool flg);//true���Z�b�g����ƃQ�[�����[�v���I������
	void SetSoundVol(int vol);
	int GetSoundVol();

	//���݂̃V�[�����擾
	Scene GetSceneID(void);
private:
	SceneManager();
	~SceneManager();

	int mScreenSizeX;		//��ʂ�X�T�C�Y
	int mScreenSizeY;		//��ʂ�Y�T�C�Y
	int iconH;
	SCENE_ID mSceneID;		// �e�̃C���X�^���X

	bool initFlg_;			//�������t���O
	uniqueScene scene_;		//�V�[��
	std::unique_ptr<Controller> controller_;
	double deltaTime_;
	int FontHandle_;
	bool SysInit(void);		//������
	bool gameEndFlg;//�Q�[�����[�v���I��������t���O
	int soundVol;
	Vector2Int MousePos_;
	bool multiFlg_;//�}���`���Ȃ�true

	int cnt_ = 0;
};
