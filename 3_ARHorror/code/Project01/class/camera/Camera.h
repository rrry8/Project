#pragma once
#include<DxLib.h>
#include<memory>
#include"../Common/Geometry.h"
#include"../common/Quaternion.h"

//�J�����̍���(��)
constexpr float CAMERA_HEIGHT = 1250.0f;

class Camera
{
public:
	Camera();
	~Camera();

	//������
	void Init(void);

	//�X�V
	void Update(bool dashFlag);
	void Update(Vector3 targetPos);//�Q�[���I�[�o�[���̉��o�p
	//�Ǐ]�J����(�����������̂ŃI�u�W�F�N�g�`��O�ɂ���K�v������)
	void SetBeforeDraw(Vector3 playerpos);

	//�`��
	void Draw(void);

	//�J�������W��Ԃ�
	Vector3 GetPos();

	//�����_��Ԃ�
	Vector3 GetTargetPos();

	//�p�x��Ԃ�
	Vector3 GetAngles();

	//��ʗh��
	float GetSwingWidth(void);

private:
	//�J�����̈ʒu
	Vector3 pos_;

	//�J�����p�x(rad)
	Vector3 angle_;

	//�p�x(�N�H�[�^�j�I��)
	Quaternion quaRot_;

	//�����_
	Vector3 targetPos_;

	Vector2Int mousePos_;

	int sinCnt_;

};
