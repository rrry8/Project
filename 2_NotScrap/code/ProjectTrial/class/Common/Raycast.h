#pragma once
#include"Collision.h"
#include "Geometry.h"

class Raycast
{
public:
	struct Ray
	{
		Vector3 p;		//�n�_
		Vector3 vec;	//����
	};

	//�������m�̏Փ˔���B�ԑ̂Ƒ���Ŏg�p
	bool CheckCollisionRay(Ray ray,Collision coll,Vector2 offset);

	//�~�Ƃ̏㉺�Փ˔���B�^�C���Ƒ���Ŏg�p
	bool CheckCollisionCircleUpDown(Vector2 ray, Collision coll, Vector2 offset,float r,float& Correct);

	//�����Ɖ~�̓����蔻��
	bool CheckCircle(Line line, Vector3 pos, float r, Vector3 offset);

private:
	//�����Ɛ����̓����蔻��
	bool CheckRay(Ray ray, Line line);
};

