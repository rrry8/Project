#pragma once
#include"../../Common/Geometry.h"

class MouseIcon
{
public:
	MouseIcon();
	~MouseIcon();

	//������
	void Init(void);

	//�X�V
	void Update(void);

	//�`��
	void Draw(void);

	//�j��
	void Release(void);

private:

	//�}�E�X�A�C�R���̕`��
	void DrawMouseIcon(int shader,int icon,int norm);

	//�F�ύX����
	void ChangeColor(void);

	//��ʓ��ɕ␳
	void CorrectScreen(void);

	//�A�C�R���̈ʒu(�}�E�X�̈ʒu)
	Vector2 pos_;

	//���_���
	std::array<VERTEX2DSHADER, 4> verts_;

	//�}�E�X�A�C�R���̃T�C�Y
	Vector2 graphSize_;

	bool colorChangeFlag_;

	//�V�F�[�_�n���h��
	int shaderHandle_;

	//�萔�o�b�t�@
	int cbuff_;

	//�n�����
	float* writer_;
};

