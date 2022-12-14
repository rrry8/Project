#pragma once
#include "Item.h"

class Key
	: public Item
{
public:
    Key(Vector3 pos,ItemID keyid, int num);
    ~Key();

    //初期化
    void Init(void)override;

    //更新
    void Update(void)override;

    //描画
    void Draw(void)override;
    void Draw(bool cameraflag);
    //開放
    void Release(void)override;

    ItemID GetItemID(void)override { return itemId_; };
private:

protected:
};

