#include <DxLib.h>
#include "HandLight.h"
#include "../../camera/Camera.h"



HandLight::HandLight()
{
	Init();
}

HandLight::~HandLight()
{
	Release();
}

void HandLight::Init()
{

	/*spotLightH_ = CreateSpotLightHandle({ 0,0,0 }, VGet(0.0f, 0.0f, -1.0f),
		DX_PI_F / 10.0f,
		DX_PI_F / 10.0f,
		20000.0f,
		0.0f,
		0.00005f,
		0.0f);*/
	
}

void HandLight::Update()
{
	
}

void HandLight::Update(Vector3 playerPos,Vector3 cameraAngle, Vector3 targetPos)
{
	pPos_ = playerPos;
	pPos_.y += CAMERA_HEIGHT;
	targetPos_ = targetPos;
	auto angle = targetPos_ - pPos_;

	//SetLightDirectionHandle(spotLightH_, { angle.x,angle.y,angle.z });
	//SetLightPositionHandle(spotLightH_, pPos_.toVECTOR());
}

void HandLight::Draw()
{

}

void HandLight::Release()
{
	DeleteLightHandle(spotLightH_);
}
