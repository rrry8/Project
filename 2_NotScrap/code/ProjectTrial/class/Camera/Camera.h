#pragma once
#include<DxLib.h>
#include<memory>
#include"../Common/Geometry.h"
class Camera
{
public:
	//�J�����̍���
	static constexpr float HEIGHT = 70;

	//�����_����J�����܂ł�XZ����
	static constexpr float DIS_TARGET_CAMERA = 1500.0f;


	Camera();
	~Camera();

	//������
	void Init(void);

	//�X�V
	void Update(void);

	//�Ǐ]�J����
	void SetBeforeDraw(Vector3 pPos);

	//�`��
	void Draw(void);

	//�J��
	void Release(void);

	//�J�������W��Ԃ�
	Vector3 GetPos();
	//�J�������Z�b�g
	void SetPos(Vector3 pos);
	//�����_��Ԃ�
	Vector3 GetTargetPos();

	//�p�x��Ԃ�
	Vector3 GetAngles();


private:
	//�J�����̈ʒu
	Vector3 mPos;

	//�J�����p�x(rad)
	Vector3 mAngles;

	//�����_
	Vector3 mTargetPos;
	//�J�������Ǐ]����߂郉�C���iY�j
	float stopCameraPosY_;
};

