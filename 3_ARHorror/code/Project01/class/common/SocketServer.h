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
	//�ʐM�ɐڑ�
	void InternetConnection();
	//�ʐM���肩��̒l���o��
	bool Receive();
	//�ʐM��؂�
	void CommunicationCutoff();

	//�擾�֐�
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

	//geometry.h�������ɏ����Ɠ{��ꂽ�̂Ńy�A���g�p
	CameraValue cameraValue_;	//�ʐM�����������J�����̈ړ��l	
	//�}�E�X�̒ʐM�����Ă邩
	bool mouseflag_;
	//�L�[�ړ�����
	int keyValue_;
	//�ړ��ȊO�̃L�[����
	int supplementValue_;
	//�g�т̃A�C�e����ID
	ItemID itemId_;
	//�C���x���g���̑I��
	std::string inventorySelect_;
	
};

