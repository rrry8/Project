#pragma once
#include "Geometry.h"
// �f�B���N�V�������C�g�\����
struct DirectionlLight
{
	Vector3  color;     // ���C�g�̃J���[
	float    pad0;

	Vector3  direction; // ���C�g�̕���
	float   pad1;

};

//�|�C���g���C�g�\����
struct PointLight
{
	Vector3 position;   // ���W
	float   pad0;

	Vector3 color;      // ���C�g�̃J���[
	float   range;      // ���C�g�̉e����^����͈�

};

//// �|�C���g���C�g�\����
//struct PointLight
//{
//	Vector3 position;   // ���W
//	float   pad0;
//	Vector3 color;      // ���C�g�̃J���[
//	float   range;      // ���C�g�̉e����^����͈�
//
//
//	Vector3 position2;   // ���W
//	float   pad1;
//	Vector3 color2;      // ���C�g�̃J���[
//	float   range2;      // ���C�g�̉e����^����͈�
//};

// �X�|�b�g���C�g�\����
struct SpotLight
{
	Vector3 Position;     // �ʒu
	float   pad0;

	Vector3 Color;        // �J���[
	float   Range;        // �e���͈�

	Vector3 Direction;    // �ˏo����
	float   Angle;        // �ˏo�p�x
};