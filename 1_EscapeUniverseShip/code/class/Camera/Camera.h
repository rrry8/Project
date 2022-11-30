#pragma once
#include"../../Geometry.h"


class Camera
{
public:
	Camera(float worldx,float worldy);
	~Camera();

	void Init(void);
	void Update(Vector2 playerpos);
	const Vector2& GetPos(void);

private:
	Vector2 ViewPos_;
	float WorldSizeX_;
	float WorldSizeY_;
};

