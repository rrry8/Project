#pragma once
#include<memory>
#include"../Geometry.h"
class BaseScene;
class Camera;

using UniqueScene = std::unique_ptr<BaseScene>;

class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	// 初期化処理
	virtual void Init(void) = 0;

	// 更新ステップ
	virtual UniqueScene Update(UniqueScene scene) = 0;

	// 描画処理
	virtual void Draw();
	virtual void DrawplusOffset(Vector2 offset);
	virtual void DrawScreen(void) = 0;

	// 解放処理
	virtual void Release(void) = 0;

	virtual void StopSound();
	virtual void ChangeSoundVolume(int volume);

private:

protected:
	int screenID_;
	int screenSizeX_;
	int screenSizeY_;

};

