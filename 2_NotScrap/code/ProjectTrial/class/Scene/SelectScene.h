#pragma once
#include "BaseScene.h"

class Camera;

enum class Point
{
	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	MAX
};

class SelectScene :
	public BaseScene
{
public:
	SelectScene(bool multiFlg);//multiFlg : true(MULTIPLAY)/false(SINGLEPLAY)
	~SelectScene();
private:
	//������
	bool Init(void)override;
	//�X�V
	uniqueScene Update(double delta, uniqueScene ownScene)override;
	//�`��
	void DrawOwnScreen(double delta)override;
	void MovePlayer();
	void UpdateCamera();

	//�V�[��ID��Ԃ�
	Scene GetSceneID(void) override { return Scene::Select; };

	//�J�������
	std::unique_ptr<Camera> camera_;
	//�J�����̈ʒu
	Vector3 cameraPos_;

	//�v���C���[
	Vector3 pos_;		//�ʒu
	Vector3 GoalPos_;	//�ڕW�n�_
	Point point_;		//
	//�A�j���[�V����
	std::unique_ptr<AnimationController> animController_;

	//�X�e�[�W�̔ԍ�
	int	stageNum_;

	Vector2Int selectMapOffset_;

	int playerHandle_;
};

