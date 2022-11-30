#pragma once
#include "Obj.h"
//#include "gimmick/Gimmick.h"
#include "marker/ARmarker.h"

class Gimmick;
class DrawerFrame;
class Drawer;
class Door;
class LockDoor;
class InnerLockDoor;
class Item;
class Dial;
class DialFrame;
class DialLockDoor;
class FakeWall;
class OriginalShader;

struct ItemObj;
struct GimmickObj;

enum class ItemID;

class Stage :
    public Obj
{
public:
	Stage();
	~Stage();

	//初期化
	void Init(void) override;
	//破棄
	void Release(void) override;
	//更新
	void Update(void) override;
	//描画
	void Draw(void) override;
	void Draw(bool minCameraFlg);

	//ギミックリストを返す
	std::vector<std::shared_ptr<Gimmick>> GetGimmickList(void);

	/// <summary>
	/// アイテムリストを返す
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	std::vector<std::shared_ptr<Item>> GetItemList(void);


private:
	/// <summary>
	/// ギミックリストへ追加
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="gList : ギミックリスト"></param>
	/// <param name="objName : 追加したいオブジェクト名"></param>
	template<typename T>
	void AddGimmickList(std::vector<GimmickObj>& gList, std::wstring objName);

	/// <summary>
	/// ギミックリストへ追加
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="gList : ギミックリスト"></param>
	/// <param name="objName : 追加したいオブジェクト名"></param>
	/// <param name="itemID : 対応させたいアイテム名"></param>
	template<typename T>
	void AddGimmickList(std::vector<GimmickObj>& gList, std::wstring objName, ItemID itemID);
	/// <summary>
	/// ギミックリストへ追加
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="gList : ギミックリスト"></param>
	/// <param name="objName : 追加したいオブジェクト名"></param>
	/// <param name="normal : 法線"></param>
	template<typename T>
	void AddGimmickList(std::vector<GimmickObj>& gList, std::wstring objName, float normal);
	/// <summary>
	/// ギミックリストへ追加
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="gList : ギミックリスト"></param>
	/// <param name="objName : 追加したいオブジェクト名"></param>
	/// <param name="normal : 法線"></param>
	template<typename T>
	void AddGimmickList(std::vector<GimmickObj>& gList, std::wstring objName, std::string unlockNumber);

	//ギミックリスト追加処理まとめ
	void AddGimmick(void);
	//確認用リスト
	std::vector<GimmickObj> gimmickObjList_;
	//ギミックで使用するリスト
	std::vector<std::shared_ptr<Gimmick>> gimmickList_;

	//アイテムリストの追加
	template<typename T>
	void AddItemList(std::vector<ItemObj>& iList, std::wstring objName);
	std::vector<std::shared_ptr<Item>> itemList_;
	std::vector<ItemObj> photList_;
	std::vector<ItemObj> keyList_;

	//マーカー
	std::vector<std::shared_ptr<ARmarker>> markerList_;

	int stageHandle_;
	//int stageHandle_;
	//int stageCollisionHandle_;
	std::unique_ptr<OriginalShader> originalShader_;
};

