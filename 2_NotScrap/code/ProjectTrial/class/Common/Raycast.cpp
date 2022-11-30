#include "Raycast.h"
#include<cmath>

bool Raycast::CheckCollisionRay(Ray ray, Collision coll, Vector2 offset)
{
	Line points[4] =
	{
		{{coll.first.x - offset.x,coll.first.y - offset.y,0.0f},{coll.first.x + coll.second.x - offset.x,coll.first.y - offset.y,0.0f}},					//���
		{{coll.first.x - offset.x,coll.first.y + coll.second.y - offset.y,0.0f} ,{coll.first.x + coll.second.x - offset.x,coll.first.y + coll.second.y - offset.y,0.0f }},	//���
		{{coll.first.x + coll.second.x - offset.x,coll.first.y - offset.y,0.0f},{coll.first.x + coll.second.x - offset.x,coll.first.y + coll.second.y - offset.y,0.0f}},	//�E��
		{{coll.first.x - offset.x,coll.first.y - offset.y,0.0f},{coll.first.x - offset.x,coll.first.y + coll.second.y - offset.y,0.0f}}					//����
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

bool Raycast::CheckCircle(Line line, Vector3 pos, float r, Vector3 offset)
{
	//�����̎n�_����I�_
	Vector3 LStoLE = line.end - line.p;
	//�����̎n�_����~�̒��S
	Vector3 LStoC = pos - line.p;
	//�����̏I�_����~�̒��S
	Vector3 LEtoC = pos - line.end;

	//�P�ʃx�N�g����
	auto N_LStoLE = LStoLE.Normalized();
	//�O�ς����߂�
	float C = Cross(LStoC, N_LStoLE).z;

	if (fabs(C) <= r)
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

bool Raycast::CheckRay(Ray ray, Line line)
{	
	 //ray��L�΂����� ��������
	auto checkcross = [](Vector3& vac1, Vector3& vac2) {
		return((vac1.x * vac2.y) - (vac2.x * vac1.y));
	};

	Vector3& vecRay = ray.vec;
	/*  Vector2 pv = Vector2{ line.p.x - ray.p.x,line.p.y - ray.p.y };
	  Vector2 ev = Vector2{ line.end.x - ray.p.x,line.end.y - ray.p.y };*/
	Vector3&& vecLine = line.end - line.p;
	/* Vector2 plv = Vector2{ ray.p.x - line.p.x, ray.p.y -line.p.y };
	 Vector2 plv = Vector2{ ray.p.x - line.p.x, ray.p.y - line.p.y };*/

	auto cross_RayLine = checkcross(vecRay, vecLine);
	if (cross_RayLine == 0.0f)
	{
		return false;
	}
	Vector3 v = line.p - ray.p;
	auto cros_ray = checkcross(v, vecRay);
	auto cros_line = checkcross(v, vecLine);
	/*  auto cros_ray = checkcross(pv, vecRay);
	  auto cros_line = checkcross(ev, vecLine);*/

	auto ans1 = cros_ray / cross_RayLine;
	auto ans2 = cros_line / cross_RayLine;

	if (ans1 > 0.0f && ans1 <= 1.0f &&
		ans2 > 0.0f && ans2 <= 1.0f)
	{
		return true;
	}
	return false;
}
