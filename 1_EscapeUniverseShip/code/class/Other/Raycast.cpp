#include "Raycast.h"
#include<cmath>

bool Raycast::CheckCollisionRay(Ray ray, Collision coll, Vector2 offset)
{
	Line points[4] =
	{
		{coll.first - offset,{coll.first.x + coll.second.x - offset.x,coll.first.y - offset.y}},					//���
		{{coll.first.x - offset.x,coll.first.y + coll.second.y - offset.y} ,{coll.first + coll.second - offset }},	//���
		{{coll.first.x + coll.second.x - offset.x,coll.first.y - offset.y},{coll.first + coll.second - offset}},	//�E��
		{coll.first - offset,{coll.first.x - offset.x,coll.first.y + coll.second.y - offset.y}}					//����
	};

	for (auto point : points)
	{
		if (CheckRay(ray, point))
		{
			return true;
		}
	}
	return false;
}

bool Raycast::CheckCollisionRayL(Ray ray, Collision2 coll, Vector2 offset)
{
	if (CheckRay(ray, { {coll.first.first - offset},{coll.first.first.x - offset.x,coll.first.first.y + coll.first.second.y - offset.y} }))
	{
		return true;
	}
	return false;
}

bool Raycast::CheckCollisionCircleUpDown(Vector2 ray, Collision coll, Vector2 offset,float r, float& Correct)
{
	Line points[2] =
	{
		{coll.first - offset,{coll.first.x + coll.second.x - offset.x,coll.first.y - offset.y}},					//���
		{{coll.first.x - offset.x,coll.first.y + coll.second.y - offset.y} ,{coll.first + coll.second - offset}}	//���
	};

	for (auto point : points)
	{
		if (CheckCircle(ray, point,r,Correct))
		{
			return true;
		}
	}
	return false;
}

bool Raycast::CheckCollisionCircleDownUp(Vector2 ray, Collision coll, Vector2 offset, float r, float& Correct)
{
	Line points[2] =
	{
		{{coll.first.x - offset.x,coll.first.y + coll.second.y - offset.y} ,{coll.first + coll.second - offset}},	//���
		{coll.first - offset,{coll.first.x + coll.second.x - offset.x,coll.first.y - offset.y}}					//���
	};

	for (auto point : points)
	{
		if (CheckCircle(ray, point, r, Correct))
		{
			return true;
		}
	}
	return false;
}

bool Raycast::CheckCollisionCircleUpDown(Vector2 ray, Collision2 coll, Vector2 offset, float r, float& Correct)
{
	Line points[2] =
	{
		{coll.first.first - offset,{coll.first.first.x + coll.first.second.x - offset.x,coll.first.first.y - offset.y}},					//���
		{{coll.first.first.x - offset.x,coll.first.first.y + coll.first.second.y - offset.y} ,{coll.first.first + coll.first.second - offset}}	//���
	};

	for (auto point : points)
	{
		if (CheckCircle(ray, point, r, Correct))
		{
			return true;
		}
	}
	return false;
}

bool Raycast::CheckCollisionCircleAll(Ray ray, Vector2 coll, Vector2 offset, float r)
{
	if (CheckSaw(ray, coll-offset,r))
	{
		return true;
	}
	return false;
}

bool Raycast::CheckRay(Ray ray, Line line)
{	
	//Ray�̃x�N�g���R��
	Vector2 RStoRE = ray.vec;
	Vector2 RStoLS = line.p - ray.p;
	Vector2 RStoLE = line.end - ray.p;
	//Line�̃x�N�g���R��
	Vector2 LStoLE = line.vec();
	Vector2 LStoRS = ray.p - line.p;
	Vector2 LStoRE = (ray.p + ray.vec) - line.p;

	//�O�ς����߂Ċ|���Z������B�O�ȏ�Ȃ瓖�����Ă��Ȃ��B�~�Q��
	float C1 = (RStoRE.x * RStoLS.y) - (RStoLS.x * RStoRE.y);
	float C2 = (RStoRE.x * RStoLE.y) - (RStoLE.x * RStoLE.y);
	if (C1 * C2 >= 0.0f)
	{
		return false;
	}
	C1 = (LStoLE.x * LStoRS.y) - (LStoRS.x * LStoLE.y);
	C2 = (LStoLE.x * LStoRE.y) - (LStoRE.x * LStoLE.y);
	if (C1 * C2 >= 0.0f)
	{
		return false;
	}
	return true;
}

bool Raycast::CheckCircle(Vector2 pos, Line line, float r, float& Correct)
{
	//�����̎n�_����I�_
	Vector2 LStoLE = line.end - line.p;
	//�����̎n�_����~�̒��S
	Vector2 LStoC = pos - line.p;
	//�����̏I�_����~�̒��S
	Vector2 LEtoC = pos - line.end;

	//�P�ʃx�N�g����
	auto N_LStoLE = LStoLE.Normalized();
	//�O�ς����߂�
	auto C = Cross(LStoC, N_LStoLE);

	if (fabs(C) <= r)
	{
		auto dot1 = Dot(LStoLE, LStoC);
		auto dot2 = Dot(LStoLE, LEtoC);
		if (dot1 * dot2 <= 0.0f)
		{
			Correct = r - fabs(C);
			return true;
		}
		if (LStoC.Magnitude() < r ||
			LEtoC.Magnitude() < r)
		{
			Correct = r - fabs(C);
			return true;
		}
	}
	return false;
}

bool Raycast::CheckSaw(Ray ray, Vector2 coll, float r)
{
	//�����̎n�_����I�_
	Vector2 LStoLE = (ray.p + ray.vec) - ray.p;
	//�����̎n�_����~�̒��S
	Vector2 LStoC = coll - ray.p;
	//�����̏I�_����~�̒��S
	Vector2 LEtoC = coll - (ray.p + ray.vec);

	//�P�ʃx�N�g����
	auto N_LStoLE = LStoLE.Normalized();
	//�O�ς����߂�
	auto C = Cross(LStoC, N_LStoLE);

	if (fabs(C) < r)
	{
		auto dot1 = Dot(LStoLE, LStoC);
		auto dot2 = Dot(LStoLE, LEtoC);
		if (dot1 * dot2 <= 0.0f)
		{
			return true;
		}
		if (LStoC.Magnitude() < r ||
			LEtoC.Magnitude() < r)
		{
			return true;
		}
	}
	return false;
}
