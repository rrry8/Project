#include "SocketServer.h"
#include <stdio.h>
#include <string>
#include <sstream>
//#define _WINSOCK2API_
#include <WinSock2.h>// WinSocketを使用するためのInclude

#include <WS2tcpip.h>// WinSocketを使用するためのInclude
#include <DxLib.h>
#include "../object/Item/Item.h"
#include"../../_debug/_DebugConOut.h"

// WinSocketを使用するためのlibファイル
#pragma comment(lib, "ws2_32.lib")

SOCKET sock;	//ヘッダーに書いたらWinSock2で怒られたのでここに書いている


SocketServer::SocketServer()
{
}

SocketServer::~SocketServer()
{	
}

void SocketServer::InternetConnection()
{
	//自分のIPを取得
	IPDATA IPAddress[16];
	GetMyIPAddress(IPAddress);
	TRACE("IPアドレス　　%d.%d.%d.%d\n", IPAddress->d1, IPAddress->d2, IPAddress->d3, IPAddress->d4);
	TRACE("キー:W       数字:017\n");
	TRACE("キー:A       数字:030\n");
	TRACE("キー:S       数字:031\n");
	TRACE("キー:D       数字:032\n");
	TRACE("キー:LSHIFT  数字:042\n");
	TRACE("キー:TAB     数字:015\n");
	TRACE("キー:ESC     数字:001\n");
	TRACE("キー:Space   数字:057\n");
	TRACE("キー:E       数字:018\n");
	TRACE("左クリック   数字:101\n");
	TRACE("右クリック   数字:102\n");
	TRACE("２個同時押しは数字を足した数字\n");
	//WSADATA wsaData;
	//SOCKET sockListen = socket(AF_INET, SOCK_DGRAM, 0);;

	//// Winsockの初期化
	//WSAStartup(MAKEWORD(2, 0), &wsaData);

	//struct sockaddr_in addr;
	//// AF_INETを渡すことによりIPv4で通信する事を指定
	//addr.sin_family = AF_INET;
	//// ポート番号を指定好きなポートを指定してください。
	//addr.sin_port = htons(12345);
	//// INADDR_ANYを指定する事によりどのIPからでも通信を受け取る状態にする。
	//addr.sin_addr.S_un.S_addr = INADDR_ANY;

	//// ソケットを作成
	//sockListen = socket(addr.sin_family, SOCK_DGRAM, 0);

	//// ソケットの作成に失敗していないかどうか
	//if (sockListen == INVALID_SOCKET)
	//{
	//	TRACE("socket failed\n");
	//	return;
	//}

	//// ソケットにアドレスを割り当てる
	//bind(sockListen, (struct sockaddr*)&addr, sizeof(addr));

	////// ソケットを接続待ち状態にする
	////if (listen(sockListen, 5) == SOCKET_ERROR)
	////{
	////	TRACE("listen:エラー");
	////	return;
	////}


	////struct sockaddr_in client;
	////int clientlen = sizeof(client);
	//////memset(buf, 0, sizeof(buf));
	////// クライアントからの接続を待つ
	////sock = accept(sockListen, (struct sockaddr*)&client, &clientlen);
	//// 接続ソケットを非同期モードにする
	//u_long val = 1;
	//ioctlsocket(sock, FIONBIO, &val);
	////TRACE("接続完了\n");


	WSADATA wsaData;

	//SOCKET sock;
	struct sockaddr_in sockAddr;


	WSAStartup(MAKEWORD(2, 0), &wsaData);

	sock = socket(AF_INET, SOCK_DGRAM, 0);

	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(12345);
	sockAddr.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(sock, (struct sockaddr*)&sockAddr, sizeof(sockAddr));

	// ここで、ノンブロッキングに設定しています。
	// val = 0でブロッキングモードに設定できます。
	// ソケットの初期設定はブロッキングモードです。
	u_long val = 1;
	ioctlsocket(sock, FIONBIO, &val);

	

	//  for (int i = 0; i < 100; i++) {
  //      /*std::string rdata = udp0.recieve();
  //      printf("recv:%s\n", rdata.c_str());*/
  //      memset(buf, 0, 100);
		////socklen_t client_len = sizeof(addr);
		////recvfrom(sock, buf, 100, 0, (struct sockaddr*)&addr, &client_len);
		//recv(sock, buf, sizeof(buf), 0);
  //     // printf("recv : %s \n", buf);
		//TRACE("recv : %s \n", buf);
  //      /*
  //      if (recv(sock, &buf, 1, 0) >= 0)
  //        printf("recv : %c \n", buf);*/
  //  }
}

bool SocketServer::Receive()
{

	char buf[256];

	int n;
	/*sock = socket(AF_INET, SOCK_DGRAM, 0);*/
	memset(buf, 0, sizeof(buf));
	//while (1) {
		n = recv(sock, buf, sizeof(buf), 0);
		if (n < 1) 
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK) 
			{
				// まだ来ない。
				//printf("MADA KONAI\n");
				cameraValue_.first = 0;
				cameraValue_.second = 0 ;
				mouseflag_ = false;
				keyValue_ = 0;
				supplementValue_ = false;
				itemId_ = ItemID::Non;
				//itemflag_  = false;
				
			}
		}
		else 
		{
				int keyValue = 0;
			std::stringstream dataStream;
	
			//TRACE(" buf %s\n ", buf);
			dataStream << buf;

			TRACE(" x : ");
			SplitStorage(cameraValue_.first, dataStream);
			TRACE(" y : " );
			SplitStorage(cameraValue_.second, dataStream);
			//値の取り出し
			TRACE(" falg : ");
			int mouseflagint;
			SplitStorage(mouseflagint, dataStream);
			TRACE(" key : ");
			SplitStorage(keyValue,    dataStream);
			TRACE("ダッシュ");
			int dashValue;
			SplitStorage(dashValue,  dataStream);
			TRACE("アイテムID");
			int itemId;
			SplitStorage(itemId,	   dataStream);
			TRACE("インベントリ選択");
			int inventorySelect;
			SplitStorage(inventorySelect,  dataStream);
			TRACE("\n\n");

			keyValue_ = keyValue;
			mouseflag_ = mouseflagint;
			supplementValue_ = dashValue;
			itemId_ = static_cast<ItemID>(itemId);
			inventorySelect_ = std::to_string(inventorySelect);

			//break;
		}
		//Sleep(200); //1000ms待つ（win32 API)
	//}
	//char buf[256];	//
	//int castToBuf = 0;
	//
	//

	//mouseflag_ = false;
	//	//クライアントからデータを受け取る
	//if (recv(sock, buf, sizeof(buf), 0) == SOCKET_ERROR) 
	//{
	//	//TRACE("nondata\n");
	//	return false;
	//}

	//std::stringstream dataStream;
	///*int N = buf[7] - '0';
	//if (N < 0)
	//{
	//	return false;
	//}*/
	//TRACE(" buf %s\n ", buf);
	//dataStream << buf;
	////TRACE("data %s\n", dataStream);
	////値の取り出し
	//
	////TRACE(" x : ");
	//SplitStorage(cameraValue_.first, dataStream);
	////TRACE(" y : " );
	//SplitStorage(cameraValue_.second, dataStream);
	////TRACE(" falg : ");
	//int mouse;
	//SplitStorage(mouse, dataStream);
	////TRACE(" key : ");
	//SplitStorage(castToBuf, dataStream);
	//
	//TRACE(" \n ");
	//////受け取った数字をcharからintに変換
	////std::string S = buf;
	////int a = 10;
	////for (int i = 0; i < 2; i++) {
	////	int N = S[i] - '0';
	////	if (N < 0)
	////	{
	////		return false;
	////	}
	////	//TRACE("%d\n", N);
	////	castToBuf += N * a;
	////	a /= 10;

	////}
	////TRACE("data %d", castToBuf);
	///*if (castToBuf > 100)
	//{
	//	castToBuf -= 100;
	//	mouseflag_ = true;
	//}*/
	//////int表示
	//mouseflag_ = mouse;
	//castToBuf_ = castToBuf;

	////intの値を返す
	return true;
	
}

void SocketServer::CommunicationCutoff()
{
	// ソケット通信を終了
	closesocket(sock);
	// Winsockを終了
	WSACleanup();
}

int SocketServer::GetKeyValue()const
{
	return keyValue_;
}

int SocketServer::GetSupplementValue()const
{
	return supplementValue_;
}

ItemID SocketServer::GetItemID()const
{
	return itemId_;
}

std::string SocketServer::GetinventorySelect() const
{
	return inventorySelect_;
}

//bool SocketServer::GetItemflag()const
//{
//	return itemflag_;
//}

bool SocketServer::GetMouseflag()const
{
	return mouseflag_;
}

CameraValue SocketServer::GetCameraValue()const
{
	return cameraValue_;
}

void SocketServer::SplitStorage(int& value, std::stringstream& dataStream)
{
	std::string numstr;
	std::getline(dataStream, numstr, ',');
	value = atoi(numstr.c_str());
	TRACE(" data %d ", value);
}

