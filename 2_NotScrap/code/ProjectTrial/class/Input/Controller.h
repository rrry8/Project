#pragma once
#include<array>
#include<map>
#include"InputID.h"
#include <vector>
using VecInt = std::vector<int>;
constexpr auto INPUT_ID_MAX = 8;
constexpr auto PAD_INPUTID_MAX = 14;
//���͑��u�̎��
enum class CntType
{
	Key,
	Pad,
	Max
};

enum class Trg
{
	Now,
	Old,
	Max
};

using TrgBool = std::array<bool, static_cast<size_t>(Trg::Max)>;
using CntData = std::map<InputID, TrgBool>;

struct NewKeyData
{
	int code;
	const char* Name;
};

class Controller
{
public:
	struct RingBuf
	{
		//InputID id;
		int value = 0;
		RingBuf* befor = nullptr;
		RingBuf* next = nullptr;
		static RingBuf* Create(int no);
		static void Destroy(RingBuf* beginBuf);

	};
	Controller() {};
	virtual ~Controller() {};
	
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	virtual CntType GetCntType(void) = 0;
	virtual int KeyChange(int oldKey) = 0;		//�L�[�̕ύX
	virtual int GetNowKey(int num) = 0;		//���݂̃L�[���擾
	virtual void SetNewKey(int num, int newKey) = 0;	//�V�����L�[���Z�b�g
	virtual NewKeyData* GetNewKeyData(int num) = 0;
	virtual bool GetWarningFlg() = 0;
	virtual int KeyConfig_Save(/*const char* FilePath*/) = 0;   // �L�[�R���t�B�O�ݒ���t�@�C���ɕۑ�����( �߂�l : �ۑ������FTRUE   �ۑ����s�FFALSE )
	virtual int KeyConfig_Load(/*const char* FilePath*/) = 0;   // �L�[�R���t�B�O�ݒ���t�@�C������ǂݍ���( �߂�l : �ǂݍ��ݐ����FTRUE   �ǂݍ��ݎ��s�FFALSE )
	virtual void KeyConfig_SetDefault(void) = 0;             // �L�[�R���t�B�O�ݒ��
	virtual bool GetKeyChangeFlg(void) = 0;//�L�[�ύX���I���������ǂ���
	virtual void SetKeyChangeFlg(bool flg) = 0;
	const CntData& GetCntData(void) 
	{
		return cntData_;
	};
private:
	//ChronoSysClock startTime_;
protected:
	void WriteCmd(void);
	CntData cntData_;
	RingBuf* ringBuf_ = nullptr;
	RingBuf* startBuf_ = nullptr;
	bool warningFlg = false;
	bool keyChangeFlg = false;
};

