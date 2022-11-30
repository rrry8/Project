#pragma once
#include "../Obj.h"
#include "../../common/Geometry.h"
#include "../../common/Transform.h"

class OriginalShader;

enum class MarkerID{//AR�}�[�J�[�̎��ʗpID
    test,
    red,
    blue,
    green,
    yellow,
    cyan,
    max
};
class ARmarker :public Obj
{
public:
	ARmarker();
	ARmarker(Vector3 pos,Vector3 angle,MarkerID id);
	~ARmarker();

    //������
    void Init(void)override;
    void Init(Vector3 pos, Vector3 angle, MarkerID id);

    //�X�V
    void Update(void)override;

    //�`��
    void Draw(void)override;

    //�J��
    void Release(void)override;

private:
    Transform transform_;//���f���̏��
    //bool pickupFlg_ = false;//�E�������ǂ���
    std::unique_ptr<OriginalShader> originalShader_;
};

