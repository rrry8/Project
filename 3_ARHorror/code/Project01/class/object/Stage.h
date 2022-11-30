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

	//������
	void Init(void) override;
	//�j��
	void Release(void) override;
	//�X�V
	void Update(void) override;
	//�`��
	void Draw(void) override;
	void Draw(bool minCameraFlg);

	//�M�~�b�N���X�g��Ԃ�
	std::vector<std::shared_ptr<Gimmick>> GetGimmickList(void);

	/// <summary>
	/// �A�C�e�����X�g��Ԃ�
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	std::vector<std::shared_ptr<Item>> GetItemList(void);


private:
	/// <summary>
	/// �M�~�b�N���X�g�֒ǉ�
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="gList : �M�~�b�N���X�g"></param>
	/// <param name="objName : �ǉ��������I�u�W�F�N�g��"></param>
	template<typename T>
	void AddGimmickList(std::vector<GimmickObj>& gList, std::wstring objName);

	/// <summary>
	/// �M�~�b�N���X�g�֒ǉ�
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="gList : �M�~�b�N���X�g"></param>
	/// <param name="objName : �ǉ��������I�u�W�F�N�g��"></param>
	/// <param name="itemID : �Ή����������A�C�e����"></param>
	template<typename T>
	void AddGimmickList(std::vector<GimmickObj>& gList, std::wstring objName, ItemID itemID);
	/// <summary>
	/// �M�~�b�N���X�g�֒ǉ�
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="gList : �M�~�b�N���X�g"></param>
	/// <param name="objName : �ǉ��������I�u�W�F�N�g��"></param>
	/// <param name="normal : �@��"></param>
	template<typename T>
	void AddGimmickList(std::vector<GimmickObj>& gList, std::wstring objName, float normal);
	/// <summary>
	/// �M�~�b�N���X�g�֒ǉ�
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="gList : �M�~�b�N���X�g"></param>
	/// <param name="objName : �ǉ��������I�u�W�F�N�g��"></param>
	/// <param name="normal : �@��"></param>
	template<typename T>
	void AddGimmickList(std::vector<GimmickObj>& gList, std::wstring objName, std::string unlockNumber);

	//�M�~�b�N���X�g�ǉ������܂Ƃ�
	void AddGimmick(void);
	//�m�F�p���X�g
	std::vector<GimmickObj> gimmickObjList_;
	//�M�~�b�N�Ŏg�p���郊�X�g
	std::vector<std::shared_ptr<Gimmick>> gimmickList_;

	//�A�C�e�����X�g�̒ǉ�
	template<typename T>
	void AddItemList(std::vector<ItemObj>& iList, std::wstring objName);
	std::vector<std::shared_ptr<Item>> itemList_;
	std::vector<ItemObj> photList_;
	std::vector<ItemObj> keyList_;

	//�}�[�J�[
	std::vector<std::shared_ptr<ARmarker>> markerList_;

	int stageHandle_;
	//int stageHandle_;
	//int stageCollisionHandle_;
	std::unique_ptr<OriginalShader> originalShader_;
};

