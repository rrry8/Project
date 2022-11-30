#include "Camera.h"
#include"../Obj/Player.h"
#include"../SceneManager.h"

Camera::Camera(float worldx, float worldy)
{
	Init();
	WorldSizeX_ = worldx;
	WorldSizeY_ = worldy;
}

Camera::~Camera()
{
}

void Camera::Init(void)
{
	//表示されるウィンドウの原点
	ViewPos_ = { 0,0 };
}

void Camera::Update(Vector2 playerpos)
{
	ViewPos_.x = playerpos.x - lpSceneMng.GetViewSize().x / 5.0f;
	if (ViewPos_.x < 0.0f)
	{
		ViewPos_.x = 0.0f;
	}
	if (ViewPos_.x > WorldSizeX_ - lpSceneMng.GetViewSize().x)
	{
		ViewPos_.x = WorldSizeX_ - lpSceneMng.GetViewSize().x;
	}
}

const Vector2& Camera::GetPos(void)
{
	return ViewPos_;
}
