#include "SocketServer.h"
#include <stdio.h>
#include <string>
#include <sstream>
//#define _WINSOCK2API_
#include <WinSock2.h>// WinSocket���g�p���邽�߂�Include

#include <WS2tcpip.h>// WinSocket���g�p���邽�߂�Include
#include <DxLib.h>
#include "../object/Item/Item.h"
#include"../../_debug/_DebugConOut.h"

// WinSocket���g�p���邽�߂�lib�t�@�C��
#pragma comment(lib, "ws2_32.lib")

SOCKET sock;	//�w�b�_�[�ɏ�������WinSock2�œ{��ꂽ�̂ł����ɏ����Ă���


SocketServer::SocketServer()
{
}

SocketServer::~SocketServer()
{	
}

void SocketServer::InternetConnection()
{
	//������IP���擾
	IPDATA IPAddress[16];
	GetMyIPAddress(IPAddress);
	TRACE("IP�A�h���X�@�@%d.%d.%d.%d\n", IPAddress->d1, IPAddress->d2, IPAddress->d3, IPAddress->d4);
	TRACE("�L�[:W       ����:017\n");
	TRACE("�L�[:A       ����:030\n");
	TRACE("�L�[:S       ����:031\n");
	TRACE("�L�[:D       ����:032\n");
	TRACE("�L�[:LSHIFT  ����:042\n");
	TRACE("�L�[:TAB     ����:015\n");
	TRACE("�L�[:ESC     ����:001\n");
	TRACE("�L�[:Space   ����:057\n");
	TRACE("�L�[:E       ����:018\n");
	TRACE("���N���b�N   ����:101\n");
	TRACE("�E�N���b�N   ����:102\n");
	TRACE("�Q���������͐����𑫂�������\n");
	//WSADATA wsaData;
	//SOCKET sockListen = socket(AF_INET, SOCK_DGRAM, 0);;

	//// Winsock�̏�����
	//WSAStartup(MAKEWORD(2, 0), &wsaData);

	//struct sockaddr_in addr;
	//// AF_INET��n�����Ƃɂ��IPv4�ŒʐM���鎖���w��
	//addr.sin_family = AF_INET;
	//// �|�[�g�ԍ����w��D���ȃ|�[�g���w�肵�Ă��������B
	//addr.sin_port = htons(12345);
	//// INADDR_ANY���w�肷�鎖�ɂ��ǂ�IP����ł��ʐM���󂯎���Ԃɂ���B
	//addr.sin_addr.S_un.S_addr = INADDR_ANY;

	//// �\�P�b�g���쐬
	//sockListen = socket(addr.sin_family, SOCK_DGRAM, 0);

	//// �\�P�b�g�̍쐬�Ɏ��s���Ă��Ȃ����ǂ���
	//if (sockListen == INVALID_SOCKET)
	//{
	//	TRACE("socket failed\n");
	//	return;
	//}

	//// �\�P�b�g�ɃA�h���X�����蓖�Ă�
	//bind(sockListen, (struct sockaddr*)&addr, sizeof(addr));

	////// �\�P�b�g��ڑ��҂���Ԃɂ���
	////if (listen(sockListen, 5) == SOCKET_ERROR)
	////{
	////	TRACE("listen:�G���[");
	////	return;
	////}


	////struct sockaddr_in client;
	////int clientlen = sizeof(client);
	//////memset(buf, 0, sizeof(buf));
	////// �N���C�A���g����̐ڑ���҂�
	////sock = accept(sockListen, (struct sockaddr*)&client, &clientlen);
	//// �ڑ��\�P�b�g��񓯊����[�h�ɂ���
	//u_long val = 1;
	//ioctlsocket(sock, FIONBIO, &val);
	////TRACE("�ڑ�����\n");


	WSADATA wsaData;

	//SOCKET sock;
	struct sockaddr_in sockAddr;


	WSAStartup(MAKEWORD(2, 0), &wsaData);

	sock = socket(AF_INET, SOCK_DGRAM, 0);

	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(12345);
	sockAddr.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(sock, (struct sockaddr*)&sockAddr, sizeof(sockAddr));

	// �����ŁA�m���u���b�L���O�ɐݒ肵�Ă��܂��B
	// val = 0�Ńu���b�L���O���[�h�ɐݒ�ł��܂��B
	// �\�P�b�g�̏����ݒ�̓u���b�L���O���[�h�ł��B
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
				// �܂����Ȃ��B
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
			//�l�̎��o��
			TRACE(" falg : ");
			int mouseflagint;
			SplitStorage(mouseflagint, dataStream);
			TRACE(" key : ");
			SplitStorage(keyValue,    dataStream);
			TRACE("�_�b�V��");
			int dashValue;
			SplitStorage(dashValue,  dataStream);
			TRACE("�A�C�e��ID");
			int itemId;
			SplitStorage(itemId,	   dataStream);
			TRACE("�C���x���g���I��");
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
		//Sleep(200); //1000ms�҂iwin32 API)
	//}
	//char buf[256];	//
	//int castToBuf = 0;
	//
	//

	//mouseflag_ = false;
	//	//�N���C�A���g����f�[�^���󂯎��
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
	////�l�̎��o��
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
	//////�󂯎����������char����int�ɕϊ�
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
	//////int�\��
	//mouseflag_ = mouse;
	//castToBuf_ = castToBuf;

	////int�̒l��Ԃ�
	return true;
	
}

void SocketServer::CommunicationCutoff()
{
	// �\�P�b�g�ʐM���I��
	closesocket(sock);
	// Winsock���I��
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

