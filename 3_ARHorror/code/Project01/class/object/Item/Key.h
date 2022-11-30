#pragma once
#include "Item.h"

class Key
	: public Item
{
public:
    Key(Vector3 pos,ItemID keyid, int num);
    ~Key();

    //‰Šú‰»
    void Init(void)override;

    //XV
    void Update(void)override;

    //•`‰æ
    void Draw(void)override;
    void Draw(bool cameraflag);
    //ŠJ•ú
    void Release(void)override;

    ItemID GetItemID(void)override { return itemId_; };
private:

protected:
};

