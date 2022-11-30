#pragma once
#include "../Common/Geometry.h"
#include "Controller.h"
// �L�[�{�[�h�̂P�L�[�ӂ�̏��
struct PADINFO
{
    int         PadInput;           // ���̓R�[�h( KEY_INPUT_LEFT �Ȃ� )
    const char* Name;               // �L�[�̖��O
};
// �L�[�R�[�h�̏��
static PADINFO padInfoTable_[] =
{
    PAD_INPUT_UP,           "UP",        //UP
    PAD_INPUT_DOWN,         "DOWN",     //DOWN
    PAD_INPUT_LEFT,         "LEFT",     //LEFT
    PAD_INPUT_RIGHT,        "RIGHT",    //RIGHT
    PAD_INPUT_1,            "A",        // A
    PAD_INPUT_2,            "B",        // B
    PAD_INPUT_3,            "X",        // X 
    PAD_INPUT_4,            "Y",        // Y
    PAD_INPUT_5,            "L1",       // L1
    PAD_INPUT_6,            "R1",       // R1
    PAD_INPUT_7,            "OptionL",  // �^�񒆂̍��̃{�^��
    PAD_INPUT_8,            "OptionR",  // �^�񒆂̉E�̃{�^��
    PAD_INPUT_9,            "9",        // 
    PAD_INPUT_10,           "10",       // 
    PAD_INPUT_11,           "11",       // 
    PAD_INPUT_12,           "12",       // 
    PAD_INPUT_13,           "13",       //
    PAD_INPUT_14,           "14",       // 
    PAD_INPUT_15,           "15",       // 
    PAD_INPUT_16,           "16",       //
    PAD_INPUT_17,           "17",       // 
    PAD_INPUT_18,           "18",       // 
    PAD_INPUT_19,           "19",       // 
    PAD_INPUT_20,           "20",       // 
    PAD_INPUT_21,           "21",       // 
    PAD_INPUT_22,           "22",       // 
    PAD_INPUT_23,           "23",       // 
    PAD_INPUT_24,           "24",       // 
    PAD_INPUT_25,           "25",       // 
    PAD_INPUT_26,           "26",       // 
    PAD_INPUT_27,           "27",       // 
    PAD_INPUT_28,           "28",       // 

    -1,                     NULL,
};
class PadInput :
    public Controller
{
public:
    PadInput();
    virtual ~PadInput();

   void Update(void)override;
   void Draw(void)override;
   int KeyChange(int oldKey)override;
   int GetNowKey(int num)override;
   void SetNewKey(int num, int newKey)override;
   NewKeyData* GetNewKeyData(int num)override;
   bool GetWarningFlg(void)override;
   CntType GetCntType(void)override {
       return CntType::Pad;
   };
   int KeyConfig_Save(/*const char* FilePath*/) override;   // �L�[�R���t�B�O�ݒ���t�@�C���ɕۑ�����( �߂�l : �ۑ������FTRUE   �ۑ����s�FFALSE )
   int KeyConfig_Load(/*const char* FilePath*/) override;   // �L�[�R���t�B�O�ݒ���t�@�C������ǂݍ���( �߂�l : �ǂݍ��ݐ����FTRUE   �ǂݍ��ݎ��s�FFALSE )
   void KeyConfig_SetDefault(void) override;             // �L�[�R���t�B�O�ݒ���f�t�H���g�ɂ���
   bool GetKeyChangeFlg(void);//�L�[�ύX�t���O��Ԃ�
   void SetKeyChangeFlg(bool flg);
   DINPUT_JOYSTATE GetRstick();
private:
    std::array<char, 256U>KeyData_;
    std::map<InputID, unsigned int> inputTbl_;
    DINPUT_JOYSTATE rStick_Input;//�E�X�e�B�b�N�̓��͏��
    int nowKey_[PAD_INPUTID_MAX];//���݂̃L�[���
    int newInputKey_;//�V�������͂��ꂽ�L�[
    int inputData_;//PAD�̓��͏��
    const char* newKeyName_;//�V�������͂��ꂽ�L�[
    NewKeyData* data[PAD_INPUTID_MAX];//�L�[�̏��(code,name)
    PADINFO* PInfo;//�S�L�[�̏��(code,name)
    int cnt;//warning�̕\���̌o�ߎ���
protected:
    Vector2 pos_;
    signed int speed_;
};

