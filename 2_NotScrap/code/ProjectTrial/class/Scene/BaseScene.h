#pragma once
#include <memory>
#include "../Common/Geometry.h"
#include"../Input/Controller.h"
#include "../Common/SoundMng.h"
#include "../Input/ControllerMng.h"

class BaseScene;

using uniqueScene = std::unique_ptr<BaseScene>;
using UniqueController = std::unique_ptr<Controller>;

class AnimationController;

enum class Scene
{
	Transition,
	Title,
	Select,
	Game,
	Result,
	Option,
	Load,
	Menu,
	Max
};


class BaseScene
{
public:
	//�R���X�g���N�^
	BaseScene();
	virtual ~BaseScene();

	//������
	virtual bool Init(void) = 0;
	void InitShader(void);

	//�X�V
	virtual uniqueScene Update(double delta, uniqueScene ownScene) = 0;

	//�`��
	virtual void Draw(double delta);
	virtual void DrawOwnScreen(double delta) = 0;
	virtual void DrawPostEffect(int shaderhandle,int target, int normalhandle);

	bool KeyTrgDown(InputID id);
	bool KeyTrgUp(InputID id);

	//�V�[��ID��Ԃ�
	virtual Scene GetSceneID(void) = 0;
private:
protected:
	int screenID_;
	Vector2Int screenSize_;
	Vector2 cursorPos_;
	std::shared_ptr<AnimationController> animController_[3];//player,enemy,coin���^�C�g���ŕ\�����邽�߂̔z��i��
	CntData cntData_;

	//�V�F�[�_�֌W
	std::array<VERTEX2DSHADER, 4> verts_;	//���_���

};
