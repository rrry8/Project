#pragma once
#include "../Obj.h"
#include "../../common/Geometry.h"
class HandLight :
    public Obj
{
public:
    HandLight();
    ~HandLight();

    void Init() override;
    void Update() override;
    void Update(Vector3 playerPos,Vector3 cameraAngle,Vector3 targetPos);
    void Draw()override;
    void Release()override;

private:
    int spotLightH_;//ライトハンドル
    Vector3 pPos_;//プレイヤーの座標
    Vector3 targetPos_;//注視点
};

