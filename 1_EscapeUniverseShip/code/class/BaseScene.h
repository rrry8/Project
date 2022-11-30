#pragma once
#include<memory>
#include"../Geometry.h"
class BaseScene;
class Camera;

using UniqueScene = std::unique_ptr<BaseScene>;

class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	// ����������
	virtual void Init(void) = 0;

	// �X�V�X�e�b�v
	virtual UniqueScene Update(UniqueScene scene) = 0;

	// �`�揈��
	virtual void Draw();
	virtual void DrawplusOffset(Vector2 offset);
	virtual void DrawScreen(void) = 0;

	// �������
	virtual void Release(void) = 0;

	virtual void StopSound();
	virtual void ChangeSoundVolume(int volume);

private:

protected:
	int screenID_;
	int screenSizeX_;
	int screenSizeY_;

};

