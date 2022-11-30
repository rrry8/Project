#pragma once
#include "../Common/Geometry.h"
#include "Controller.h"

// �L�[�{�[�h�̂P�L�[�ӂ�̏��
struct KEYINFO
{
    int         KeyInput;           // ���̓R�[�h( KEY_INPUT_LEFT �Ȃ� )
    const char* Name;               // �L�[�̖��O
};
// �L�[�R�[�h�̏��
static KEYINFO keyInfoTable_[] =
{
    KEY_INPUT_BACK,         "BACK",         // �o�b�N�X�y�[�X�L�[
    KEY_INPUT_TAB,          "TAB",          // �^�u�L�[
    KEY_INPUT_RETURN,       "ENTER",        // �G���^�[�L�[

    KEY_INPUT_LSHIFT,       "L SHIFT",      // ���V�t�g�L�[
    KEY_INPUT_RSHIFT,       "R SHIFT",      // �E�V�t�g�L�[
    KEY_INPUT_LCONTROL,     "L CONTROL",    // ���R���g���[���L�[
    KEY_INPUT_RCONTROL,     "R CONTROL",    // �E�R���g���[���L�[
    KEY_INPUT_ESCAPE,       "ESCAPE",       // �G�X�P�[�v�L�[
    KEY_INPUT_SPACE,        "SPACE",        // �X�y�[�X�L�[
    KEY_INPUT_PGUP,         "PAGE UP",      // �o�������t�o�L�[
    KEY_INPUT_PGDN,         "PAGE DOWN",    // �o�������c�������L�[
    KEY_INPUT_END,          "END",          // �G���h�L�[
    KEY_INPUT_HOME,         "HOME",         // �z�[���L�[
    KEY_INPUT_LEFT,         "��",         // ���L�[
    KEY_INPUT_UP,           "��",           // ��L�[
    KEY_INPUT_RIGHT,        "��",        // �E�L�[
    KEY_INPUT_DOWN,         "��",         // ���L�[
    KEY_INPUT_INSERT,       "INSERT",       // �C���T�[�g�L�[
    KEY_INPUT_DELETE,       "DELETE",       // �f���[�g�L�[

    KEY_INPUT_MINUS,        "-",            // �|�L�[
    KEY_INPUT_YEN,          "\\",           // ���L�[
    KEY_INPUT_PREVTRACK,    "^",            // �O�L�[
    KEY_INPUT_PERIOD,       ".",            // �D�L�[
    KEY_INPUT_SLASH,        "/",            // �^�L�[
    KEY_INPUT_LALT,         "L ALT",        // ���`�k�s�L�[
    KEY_INPUT_RALT,         "R ALT",        // �E�`�k�s�L�[
    KEY_INPUT_SCROLL,       "SCROLL LOCK",  // ScrollLock�L�[
    KEY_INPUT_SEMICOLON,    ";",            // �G�L�[
    KEY_INPUT_COLON,        ":",            // �F�L�[
    KEY_INPUT_LBRACKET,     "[",            // �m�L�[
    KEY_INPUT_RBRACKET,     "]",            // �n�L�[
    KEY_INPUT_AT,           "@",            // ���L�[
    KEY_INPUT_BACKSLASH,    "BACK SLASH",   // �_�L�[
    KEY_INPUT_COMMA,        ",",            // �C�L�[
    KEY_INPUT_KANJI,        "KANJI",        // �����L�[
    KEY_INPUT_CONVERT,      "CONVERT",      // �ϊ��L�[
    KEY_INPUT_NOCONVERT,    "NO CONVERT",   // ���ϊ��L�[
    KEY_INPUT_KANA,         "KANA",         // �J�i�L�[
    KEY_INPUT_APPS,         "APPS",         // �A�v���P�[�V�������j���[�L�[
    KEY_INPUT_CAPSLOCK,     "CAPS LOCK",    // CaspLock�L�[
    KEY_INPUT_SYSRQ,        "PRINT SCREEN", // PrintScreen�L�[
    KEY_INPUT_PAUSE,        "PAUSE BREAK",  // PauseBreak�L�[
    KEY_INPUT_LWIN,         "L WIN",        // ���v�����L�[
    KEY_INPUT_RWIN,         "R WIN",        // �E�v�����L�[

    KEY_INPUT_NUMLOCK,      "NUM LOCK",     // �e���L�[�O
    KEY_INPUT_NUMPAD0,      "NUMPAD 0",     // �e���L�[�O
    KEY_INPUT_NUMPAD1,      "NUMPAD 1",     // �e���L�[�P
    KEY_INPUT_NUMPAD2,      "NUMPAD 2",     // �e���L�[�Q
    KEY_INPUT_NUMPAD3,      "NUMPAD 3",     // �e���L�[�R
    KEY_INPUT_NUMPAD4,      "NUMPAD 4",     // �e���L�[�S
    KEY_INPUT_NUMPAD5,      "NUMPAD 5",     // �e���L�[�T
    KEY_INPUT_NUMPAD6,      "NUMPAD 6",     // �e���L�[�U
    KEY_INPUT_NUMPAD7,      "NUMPAD 7",     // �e���L�[�V
    KEY_INPUT_NUMPAD8,      "NUMPAD 8",     // �e���L�[�W
    KEY_INPUT_NUMPAD9,      "NUMPAD 9",     // �e���L�[�X
    KEY_INPUT_MULTIPLY,     "NUMPAD *",     // �e���L�[���L�[
    KEY_INPUT_ADD,          "NUMPAD +",     // �e���L�[�{�L�[
    KEY_INPUT_SUBTRACT,     "NUMPAD -",     // �e���L�[�|�L�[
    KEY_INPUT_DECIMAL,      "NUMPAD .",     // �e���L�[�D�L�[
    KEY_INPUT_DIVIDE,       "NUMPAD /",     // �e���L�[�^�L�[
    KEY_INPUT_NUMPADENTER,  "NUMPAD ENTER", // �e���L�[�̃G���^�[�L�[

    KEY_INPUT_F1,           "F1",           // �e�P�L�[
    KEY_INPUT_F2,           "F2",           // �e�Q�L�[
    KEY_INPUT_F3,           "F3",           // �e�R�L�[
    KEY_INPUT_F4,           "F4",           // �e�S�L�[
    KEY_INPUT_F5,           "F5",           // �e�T�L�[
    KEY_INPUT_F6,           "F6",           // �e�U�L�[
    KEY_INPUT_F7,           "F7",           // �e�V�L�[
    KEY_INPUT_F8,           "F8",           // �e�W�L�[
    KEY_INPUT_F9,           "F9",           // �e�X�L�[
    KEY_INPUT_F10,          "F10",          // �e�P�O�L�[
    KEY_INPUT_F11,          "F11",          // �e�P�P�L�[
    KEY_INPUT_F12,          "F12",          // �e�P�Q�L�[

    KEY_INPUT_A,            "A",            // �`�L�[
    KEY_INPUT_B,            "B",            // �a�L�[
    KEY_INPUT_C,            "C",            // �b�L�[
    KEY_INPUT_D,            "D",            // �c�L�[
    KEY_INPUT_E,            "E",            // �d�L�[
    KEY_INPUT_F,            "F",            // �e�L�[
    KEY_INPUT_G,            "G",            // �f�L�[
    KEY_INPUT_H,            "H",            // �g�L�[
    KEY_INPUT_I,            "I",            // �h�L�[
    KEY_INPUT_J,            "J",            // �i�L�[
    KEY_INPUT_K,            "K",            // �j�L�[
    KEY_INPUT_L,            "L",            // �k�L�[
    KEY_INPUT_M,            "M",            // �l�L�[
    KEY_INPUT_N,            "N",            // �m�L�[
    KEY_INPUT_O,            "O",            // �n�L�[
    KEY_INPUT_P,            "P",            // �o�L�[
    KEY_INPUT_Q,            "Q",            // �p�L�[
    KEY_INPUT_R,            "R",            // �q�L�[
    KEY_INPUT_S,            "S",            // �r�L�[
    KEY_INPUT_T,            "T",            // �s�L�[
    KEY_INPUT_U,            "U",            // �t�L�[
    KEY_INPUT_V,            "V",            // �u�L�[
    KEY_INPUT_W,            "W",            // �v�L�[
    KEY_INPUT_X,            "X",            // �w�L�[
    KEY_INPUT_Y,            "Y",            // �x�L�[
    KEY_INPUT_Z,            "Z",            // �y�L�[

    KEY_INPUT_0,            "0",            // �O�L�[
    KEY_INPUT_1,            "1",            // �P�L�[
    KEY_INPUT_2,            "2",            // �Q�L�[
    KEY_INPUT_3,            "3",            // �R�L�[
    KEY_INPUT_4,            "4",            // �S�L�[
    KEY_INPUT_5,            "5",            // �T�L�[
    KEY_INPUT_6,            "6",            // �U�L�[
    KEY_INPUT_7,            "7",            // �V�L�[
    KEY_INPUT_8,            "8",            // �W�L�[
    KEY_INPUT_9,            "9",            // �X�L�[

    -1,                     NULL,
};

class KeyInput :
    public Controller
{
public:
    KeyInput();
    virtual ~KeyInput();
    void Update(void)override;
    void Draw(void)override;
    int KeyChange(int oldKey)override;//�L�[�������ꂽ�炻�̃L�[��newKey�Ƃ��ĕԂ�
    int GetNowKey(int num)override;//�I���������̃L�[�����擾����
    void SetNewKey(int num, int newKey)override;//newKey��I���������ɃZ�b�g
    NewKeyData * GetNewKeyData(int num)override;//�V�������͂��ꂽ�L�[�̖��O���Q�b�g
    bool GetWarningFlg(void)override;//�d���Ȃǂ̌x�������K�v�Ȏ��Ƀt���O��true�ɂȂ�
    int KeyConfig_Save(/*const char* FilePath*/); // �L�[�R���t�B�O�ݒ���t�@�C���ɕۑ�����( �߂�l : �ۑ������FTRUE   �ۑ����s�FFALSE )
    int KeyConfig_Load(/*const char* FilePath*/); // �L�[�R���t�B�O�ݒ���t�@�C������ǂݍ���( �߂�l : �ǂݍ��ݐ����FTRUE   �ǂݍ��ݎ��s�FFALSE )
    void KeyConfig_SetDefault(void);       // �L�[�R���t�B�O�ݒ���f�t�H���g�ɂ���
    bool GetKeyChangeFlg(void);//�L�[�ύX�����t���O���擾
    void SetKeyChangeFlg(bool flg);
    CntType GetCntType(void)override {
        return CntType::Key;
    };
private:
    std::array<char, 256U>KeyData_;
    int MouseX;//�}�E�X�̍��W
    int MouseY;
    std::map<InputID, unsigned int> inputTbl_;//�g�p����L�[�̏��
    int nowKey_[INPUT_ID_MAX];//���݂̃L�[���
    int newInputKey_;//�V�������͂��ꂽ�L�[
    const char* newKeyName_;//�V�������͂��ꂽ�L�[
    NewKeyData* data[INPUT_ID_MAX];//�L�[�̏��(code,name)
    KEYINFO* KInfo;//�S�L�[�̏��(code,name)
    int cnt;//warning�̕\���̌o�ߎ���
    
};
