#pragma once
#include <sstream>


enum class ItemID;
using CameraValue = std::pair<int, int>;
#define lpSocketServer SocketServer::GetInstance()
class SocketServer
{
public:
	static SocketServer& GetInstance()
	{
		static SocketServer s_Instance;
		return s_Instance;
	}
	//通信に接続
	void InternetConnection();
	//通信相手からの値を出力
	bool Receive();
	//通信を切る
	void CommunicationCutoff();

	//取得関数
	CameraValue GetCameraValue()const;
	bool GetMouseflag()const;
	int GetKeyValue()const;
	int GetSupplementValue()const;
	ItemID GetItemID()const;
	std::string GetinventorySelect()const;
	
private:
	SocketServer();
	~SocketServer();

	void SplitStorage(int& value, std::stringstream& dataStream);

	//geometry.hがここに書くと怒られたのでペアを使用
	CameraValue cameraValue_;	//通信からもらったカメラの移動値	
	//マウスの通信が着てるか
	bool mouseflag_;
	//キー移動入力
	int keyValue_;
	//移動以外のキー入力
	int supplementValue_;
	//携帯のアイテムのID
	ItemID itemId_;
	//インベントリの選択
	std::string inventorySelect_;
	
};

