#pragma once
#include "Item.h"

class Phot
    : public Item
{
public:
    Phot(Vector3 pos);
    ~Phot();

    //‰Šú‰»
    void Init(void)override;

    //XV
    void Update(void)override;

    //•`‰æ
    void Draw(void)override;
    void Draw(bool cameraflag);
    //ŠJ•ú
    void Release(void)override;

    ItemID GetItemID(void)override { return ItemID::Phot; };

private:

protected:

};

