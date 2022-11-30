#pragma once
#include <array>
#include"Geometry.h"

class OriginalShader
{
public:

	/// <summary>
	/// �C���X�^���X2D�p
	/// </summary>
	/// <param name="pso">�s�N�Z���V�F�[�_�[</param>
	OriginalShader(const int pso);
	/// <summary>
	/// �C���X�^���X3D�p
	/// </summary>
	/// <param name="vso">���_�V�F�[�_�[</param>
	/// <param name="pso">�s�N�Z���V�F�[�_�[</param>
	OriginalShader(const int vso, const int pso);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~OriginalShader();



	/// <summary>
	/// �V�F�[�_�[���g�����`�� (���f���P��)
	/// </summary>
	/// <param name="model">���f��</param>
	void Draw(int model)const;

	/// <summary>
	/// �V�F�[�_�[���g�����`��
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="tex">�e�N�X�`���}�b�v</param>
	void Draw(int model, int tex)const;

	/// <summary>
	/// �V�F�[�_�[���g�����`��
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="tex">�e�N�X�`���}�b�v</param>
	/// <param name="norm">�@���}�b�v</param>
	void Draw(int model, int tex, int norm)const;

	/// <summary>
	/// �V�F�[�_�[���g�����`��
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="tex">�e�N�X�`���}�b�v</param>
	/// <param name="norm">�@���}�b�v</param>
	/// <param name="rough"></param>
	/// <param name="metallic">���^���b�N</param>
	/// <param name="sphmap">�X�t�B�A�}�b�v</param>
	void Draw(int model, int tex, int norm, int rough, int metallic, int sphmap)const;

	/// <summary>
	/// �|�X�g�G�t�F�N�g�`��p
	/// </summary>
	/// <param name="x">�摜�̍����X</param>
	/// <param name="y">�摜�̍����Y</param>
	/// <param name="img">�摜�n���h��</param>
	/// <param name="tex1">�V�F�[�_�Ŏg���e�N�X�`���P</param>
	/// <param name="tex2">�V�F�[�_�Ŏg���e�N�X�`���Q</param>
	/// <param name="tex3">�V�F�[�_�Ŏg���e�N�X�`���R</param>
	void DrawOnlyPixel(float x, float y, int img, int tex1, int tex2, int tex3);

	//�����p�ʐ^�֐�
	void DrawOnlyPixel(const std::array<Vector3, 4>& pos, int img);
private:

	//�ϐ�
	//�g�����_�V�F�[�_�[�̊i�[�ꏊ
	int vertexShader_;
	//�g���s�N�Z���V�F�[�_�[�̊i�[�ꏊ
	int pixelShader_;
};