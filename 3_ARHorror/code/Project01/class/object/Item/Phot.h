#pragma once
#include "Item.h"

class Phot
    : public Item
{
public:
    Phot(Vector3 pos);
    ~Phot();

    //������
    void Init(void)override;

    //�X�V
    void Update(void)override;

    //�`��
    void Draw(void)override;
    void Draw(bool cameraflag);
    //�J��
    void Release(void)override;

    ItemID GetItemID(void)override { return ItemID::Phot; };

private:

protected:

};

