#include <DxLib.h>
#include"KeyInput.h"
#include "../../_debug/_DebugConOut.h"



KeyInput::KeyInput()
{
	KeyConfig_SetDefault();
	KeyConfig_Load();
	
	inputTbl_ = {
		{InputID::Up,    nowKey_[0]},
		{InputID::Down,  nowKey_[1]},
		{InputID::Left,  nowKey_[2]},
		{InputID::Right, nowKey_[3]},
		{InputID::Menu,  nowKey_[4]},
		{InputID::Decide,MOUSE_INPUT_LEFT},
		{InputID::Cancel,MOUSE_INPUT_RIGHT},
		{InputID::BtnS,  MOUSE_INPUT_RIGHT},
		{InputID::BtnN,  MOUSE_INPUT_LEFT},
		{InputID::Reset, nowKey_[5]},
		{InputID::Change, nowKey_[6]},
		{InputID::Lock, nowKey_[7]},
	};

	
	for (auto id : InputID())
	{
		cntData_.try_emplace(id, TrgBool{ 0,0 });
	}

	//ringBuf_ = ringBuf_->Create(20);
	/*for (auto id : inputTbl_)
	{
		cntData_.try_emplace(id.first, TrgBool{ 0,0 });
	}*/
	warningFlg = false;
	cnt = 0;
}

KeyInput::~KeyInput()
{
	//RingBuf::Destroy(ringBuf_);
}

void KeyInput::Update(void)
{
	KeyConfig_Load();

	GetHitKeyStateAll(KeyData_.data());
	//�}�E�X������Ԃ��擾
	auto mouseInput = GetMouseInput();

	for (auto id : InputID())
	{
		if (id==InputID::Decide||
			id==InputID::Cancel ||
			id==InputID::BtnN || 
			id==InputID::BtnS)//!mouseInput)
		{
			cntData_[id][static_cast<int>(Trg::Old)] = cntData_[id][static_cast<int>(Trg::Now)];
			cntData_[id][static_cast<int>(Trg::Now)] = ((mouseInput & inputTbl_[id]) > 0);
		}
		else
		{
			cntData_[id][static_cast<int>(Trg::Old)] = cntData_[id][static_cast<int>(Trg::Now)];
			cntData_[id][static_cast<int>(Trg::Now)] = KeyData_[inputTbl_[id]];
		}
	}
	
	if (warningFlg)
	{
		if (cnt > 60)
		{
			warningFlg = false;
			cnt = 0;
		}
		cnt++;
	}
	

	//WriteCmd();
}

void KeyInput::Draw(void)
{
	
}
int KeyInput::KeyChange(int oldKey)
{
	int g_OldKey = oldKey;
	// ��v����L�[�R�[�h�����e�[�u������T��
	for (KInfo = keyInfoTable_; KInfo->Name != NULL; KInfo++)
	{
		//�����̃L�[�������ꂽ�@���@���ꂪ�}�E�X�̃N���b�N�ł͂Ȃ������ɓ���
		if (CheckHitKey(KInfo->KeyInput)&& !cntData_[InputID::Decide][static_cast<int>(Trg::Now)] && !cntData_[InputID::Decide][static_cast<int>(Trg::Old)]//����{�^������������
			&& !cntData_[InputID::Cancel][static_cast<int>(Trg::Now)] && !cntData_[InputID::Cancel][static_cast<int>(Trg::Old)])
		{
			//TRACE("Key %s\n", KInfo->Name);
			//�����ꂽ�L�[��߂�l��
			newInputKey_ = KInfo->KeyInput;
			keyChangeFlg = true;
			return newInputKey_;
			break;
		}
		
	}
	
}
int KeyInput::GetNowKey(int num)
{
	return nowKey_[num];
}
void KeyInput::SetNewKey(int num,int newKey)
{
	int wk = 0;//��Ɨp�ϐ�
	
	//�L�[�̏d�������m������L�[�����ւ�
	for (int i = 0; i < INPUT_ID_MAX; i++)
	{
		//TRACE("nowKey[%d]:%d,,,newkey:%d,,,nowKey[%d]:%d\n", i, nowKey_[i], newKey, num, nowKey_[num]);
		if (newKey == nowKey_[i])
		{
			TRACE("�d���I\n");
			TRACE("nowKey[%d]:%d<-->nowKey[%d]:%d\n", i, nowKey_[i], num, nowKey_[num]);
			warningFlg = true;
			wk = nowKey_[num];
			nowKey_[num] = nowKey_[i];
			data[num]->code = nowKey_[i];
			nowKey_[i] = wk;
			
			TRACE("�L�[�̓���ւ�����\n");
			TRACE("nowKey[%d]:%d<-->nowKey[%d]:%d\n", i, nowKey_[i], num, nowKey_[num]);
			break;
		}
		else
		{
			//TRACE("�d���Ȃ��B\n");
		}
	}
	
	if (!warningFlg)
	{
		// ��v����L�[�R�[�h�����e�[�u������T��
		for (KInfo = keyInfoTable_; KInfo->Name != nullptr; KInfo++)
		{

			if (newKey == KInfo->KeyInput)
			{
				data[num]->Name = newKeyName_ = KInfo->Name;
				nowKey_[num] = newKey;
				data[num]->code = newKey;
			}
		}
	}
	
	
	
}
NewKeyData* KeyInput::GetNewKeyData(int num)
{
	return data[num];
}
bool KeyInput::GetWarningFlg()
{
	return warningFlg;
}
int KeyInput::KeyConfig_Save(/*const char* FilePath*/)
{
	FILE* fp;
	int err;
	// �Ή�����ۑ�����t�@�C�����J��
	err = fopen_s(&fp, "./source/Data/keyconfig/keyConfig.txt", "wt");
	if (err != 0)
	{
		// �J���Ȃ������牽�������ɏI��
		TRACE("�t�@�C���̏������݂Ɏ��s���܂���\n");
		return FALSE;
	}

	// �Q�[���̊e���͂ƃL�[��p�b�h�Ȃǂ̓��͂Ƃ̑Ή������t�@�C���ɏ����o��
	for (int i = 0; i < INPUT_ID_MAX; i++)
	{
		fprintf(fp, "Key%d = %d \n",
			i,
			nowKey_[i]
		);
	}

	// �t�@�C�������
	fclose(fp);


	// �����I��
	//TRACE("�t�@�C���̏������݊���\n");
	return TRUE;
}
int KeyInput::KeyConfig_Load(/*const char* FilePath*/)
{
	FILE* fp;
	int err;

	// �Ή�����ۑ������t�@�C�����J��
	err = fopen_s(&fp, "./source/Data/keyconfig/keyConfig.txt", "rt");
	if (err != 0)
	{
		// �J���Ȃ������ꍇ�͉��������ɏI��
		TRACE("�t�@�C���̓ǂݍ��݂Ɏ��s���܂���\n");
		return FALSE;
	}

	// �Q�[���̊e���͂ƃL�[��p�b�h�Ȃǂ̓��͂Ƃ̑Ή������t�@�C������ǂݍ���
	for (int i = 0; i < INPUT_ID_MAX; i++)
	{
		fscanf_s(fp, "Key%d = %d\n",
			&i,
			&nowKey_[i]
		);
	}

	// �t�@�C�������
	//TRACE("�t�@�C���̓ǂݍ��݊���\n");
	fclose(fp);

	inputTbl_ = {
		{InputID::Up,    nowKey_[0]},
		{InputID::Down,  nowKey_[1]},
		{InputID::Left,  nowKey_[2]},
		{InputID::Right, nowKey_[3]},
		{InputID::Menu,  nowKey_[4]},
		{InputID::Decide,MOUSE_INPUT_LEFT},
		{InputID::Cancel,MOUSE_INPUT_RIGHT},
		{InputID::BtnS,  MOUSE_INPUT_RIGHT},
		{InputID::BtnN,  MOUSE_INPUT_LEFT},
		{InputID::Reset, nowKey_[5]},
		{InputID::Change, nowKey_[6]},
		{InputID::Lock, nowKey_[7]},
	};


	for (auto id : InputID())
	{
		cntData_.try_emplace(id, TrgBool{ 0,0 });
	}

	for (int i = 0; i < INPUT_ID_MAX; i++)
	{
		data[i] = new NewKeyData();
		data[i]->code = nowKey_[i];
		// ��v����L�[�R�[�h�����e�[�u������T��
		for (KInfo = keyInfoTable_; KInfo->Name != NULL; KInfo++)
		{
			if (nowKey_[i] == KInfo->KeyInput)
			{
				data[i]->Name = newKeyName_ = KInfo->Name;
			}
		}
	}

	// �����I��
	return TRUE;
}
void KeyInput::KeyConfig_SetDefault(void)
{
	nowKey_[0] = KEY_INPUT_W;
	nowKey_[1] = KEY_INPUT_S;
	nowKey_[2] = KEY_INPUT_A;
	nowKey_[3] = KEY_INPUT_D;
	nowKey_[4] = KEY_INPUT_ESCAPE;
	nowKey_[5] = KEY_INPUT_R;
	nowKey_[6] = KEY_INPUT_SPACE;
	nowKey_[7] = KEY_INPUT_LSHIFT;
}
bool KeyInput::GetKeyChangeFlg()
{
	return keyChangeFlg;
}

void KeyInput::SetKeyChangeFlg(bool flg)
{
	keyChangeFlg = flg;
}
