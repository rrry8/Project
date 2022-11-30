#pragma once
#include <memory>
#include "../Common/Geometry.h"
#include"../Input/Controller.h"
#include"../Scene/BaseScene.h"



class UIBase;
using uniqueSceneUI = std::unique_ptr<UIBase>;


class UIBase
{
public:
	UIBase();
	~UIBase();

	//������
	virtual bool Init(void) = 0;

	//�X�V
	virtual void Update(double delta) = 0;

	//�`��
	virtual void Draw(double delta);
	virtual void DrawOwnScreen(double delta) = 0;
	bool KeyTrgDown(InputID id);
	bool KeyTrgUp(InputID id);
	//�V�[��ID��Ԃ�
	virtual Scene GetSceneID(void) = 0;
	//���[�h���I���������ǂ���
	virtual bool GetLoadingEndFlg(void) = 0;
	virtual void SetIsNext(bool flg)=0;
private:
protected:
	int screenUIID_;
	Vector2Int screenSize_;
	Vector2 cursorPos_;
	std::shared_ptr<Controller> controller_;
	std::shared_ptr<AnimationController> animController_;//�^�C�g�����S���A�j���[�V�������鎞�Ɏg��
	Vector2Int MousePos;//�}�E�X�J�[�\���̍��W
	int IsHitItem;//���Ԗڂɓ����������𔻕ʂ��邽�߂̕ϐ�
	Vector2Int offset_;//UI�����炵���肷��Ƃ��ׂ̈̃I�t�Z�b�g�p
	CntData cntData_;
};

