#pragma once
#include<DxLib.h>
#include<memory>
#include<array>

class BaseScene;

using UniqueScene = std::unique_ptr<BaseScene>;

//�V�[����ID
enum class SceneID
{
	Game,		//�Q�[��
	Inventory,	//�C���x���g��
	Result,		//���U���g
	GameOver,	//�Q�[���I�[�o�[
	Select,		//�Z���N�g
	Title,		//�^�C�g��
	Option,
	Load,
	Max
};

class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	// ����������
	virtual void Init(void) = 0;

	void InitVertext(void);

	// �X�V�X�e�b�v
	virtual UniqueScene Update(UniqueScene scene) = 0;

	// �`�揈��
	virtual void Draw();
	virtual void DrawPostEffect(int ps);
	virtual void DrawPostEffect(int ps,int texture1,int texture2);
	virtual void DrawScreen(void) = 0;

	// �������
	virtual void Release(void) = 0;

	virtual SceneID GetSceneID(void) = 0;
	
private:

protected:
	//�X�N���[�����
	int screenID_;

	//�A���x�h�����_�[�^�[�Q�b�g
	int albedRT_;
	//�@�������_�[�^�[�Q�b�g
	int normalSpecRT_;
	//���[���h���W�����_�[�^�[�Q�b�g
	int depthRT_;

	//�X�N���[���̑傫��
	int screenSizeX_;	//��
	int screenSizeY_;	//�c

	//Primitive2D�p�̒��_�f�[�^
	std::array<VERTEX2DSHADER, 4> verts_;
};