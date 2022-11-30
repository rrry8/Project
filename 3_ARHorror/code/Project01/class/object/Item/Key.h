#pragma once
#include "Item.h"

class Key
	: public Item
{
public:
    Key(Vector3 pos,ItemID keyid, int num);
    ~Key();

    //������
    void Init(void)override;

    //�X�V
    void Update(void)override;

    //�`��
    void Draw(void)override;
    void Draw(bool cameraflag);
    //�J��
    void Release(void)override;

    ItemID GetItemID(void)override { return itemId_; };
private:

protected:
};

