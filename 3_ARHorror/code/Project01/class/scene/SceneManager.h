#pragma once
#include<chrono>
#include<map>
#include "BaseScene.h"
#include"../common/Geometry.h"

#define lpSceneMng SceneManager::GetInstance()

class MouseIcon;

class SceneManager
{
public:

	static SceneManager& GetInstance()
	{
		static SceneManager s_Instance;
		return s_Instance;
	}

	//������
	void Init(void);
	void Init3D(void);
	//�X�V
	void Run(void);

	//��ʃT�C�Y�擾
	Vector2Int GetScreenSize(void);

	//�f���^�^�C���擾
	double GetDeltaTime(void);

	//�萔�o�b�t�@�Ƀf�[�^�����鏈��
	void SetWriterData(int num, float setData);
	void SetBrightnessScrren(int brightnessScrren);
	int GetBrightnessScrren();
	void AddInventoryCnt();
	int GetInventoryCnt();
private:
	//�V�X�e������������
	bool SysInit(void);
	SceneManager();
	~SceneManager();

	void UpdateMouse();
	//�V�[�����
	UniqueScene scene_;
	int sceneGraphFilter_;
	//�o�ߎ���
	std::chrono::system_clock::time_point tickCount_;

	//�f���^�^�C��
	double deltaTime_ = 0.0f;
	//�萔�o�b�t�@
	int constantBuffer_;
	float* writer_;

	int brightnessScrren_;
	//std::unique_ptr<MouseIcon> mouseIcon_;
	int inventoryCnt_;
};
