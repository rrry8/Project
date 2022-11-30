#include <DxLib.h>
#include "OptionScene.h"
#include "../Input/KeyInput.h"
#include "../Transition/CrossOver.h"
#include "../../_debug/_DebugConOut.h"
#include "TitleScene.h"
#include "../Common/ImageMng.h"
#include "../Common/SoundMng.h"
#include "../UI/SceneUIManager.h"
#include "LoadScene.h"
OptionScene::OptionScene(uniqueScene& oldscene)
{
	mOldScene = std::move(oldscene);//�ЂƂO�̃V�[���̏���ێ�
	Init();
	DrawOwnScreen(0.0);
}

OptionScene::~OptionScene()
{
}

bool OptionScene::Init(void)
{
	//controller_ = std::make_shared<KeyInput>();
	keyChangeMode_ = false;
	vol = lpSceneMng.GetSoundVol();
	lpSceneUIMng.SetOptionState(OptionState::KeyConfig);//�L�[�R���t�B�O��ʂɈڍs
	return true;
}

uniqueScene OptionScene::Update(double delta, uniqueScene ownScene)
{
	cntData_ = lpCntMng.GetController()->GetCntData();
	//if (CheckHitKey(KEY_INPUT_SPACE))
	//auto a = mouseInput & usedController[InputID::ClickLeft][static_cast<int>(Trg::Now)];
	if (KeyTrgDown(InputID::Decide))
	{
		lpSoundMng.PlayingSound("./source/Sound/UI�I����.mp3");
		switch (lpSceneUIMng.GetHitItem_Main())//���J�[�\��������Ă��鍀�ڂ��擾
		{
		case UI_MAIN_ITEM::KEYCONFIG:

			TRACE("keyconfig\n");
			state_ = OptionState::KeyConfig;
			lpSceneUIMng.SetOptionState(state_);//�L�[�R���t�B�O��ʂɈڍs
			break;
		case UI_MAIN_ITEM::VOLUME:

			TRACE("volume\n");
			state_ = OptionState::Volume;
			lpSceneUIMng.SetOptionState(state_);//VOLUME��ʂɈڍs
			break;
		case UI_MAIN_ITEM::RETURN:
			state_ = OptionState::Quit;
			return lpSceneUIMng.Init(std::move(mOldScene));//�ݒ��ʂ����
			TRACE("return\n");
			break;
		case UI_MAIN_ITEM::TIPS:
			state_ = OptionState::Tips;
			lpSceneUIMng.SetOptionState(state_);//Tips��ʂɈڍs
			TRACE("tips\n");
		default:
			break;
		}
	}

	
	switch (state_)//�I������Ă��郁�C������(���̍���)���Ƃɏ�����ς���
	{
	case OptionState::KeyConfig:
	{
		if (KeyTrgDown(InputID::Reset))
		{
			lpSoundMng.PlayingSound("./source/Sound/UI�I����.mp3");
			lpCntMng.GetController()->KeyConfig_SetDefault();//�f�t�H���g�̃L�[�ɐݒ�
			lpCntMng.GetController()->KeyConfig_Save();//�ۑ�
			keyResetFlg = true;
		}
		switch (lpSceneUIMng.GetHitItem_Sub())//���J�[�\��������Ă��鍀�ڂ��擾
		{
		case UI_SUB_ITEM::KEY_CHANGE:
			if (KeyTrgDown(InputID::Decide))
			{
				lpSoundMng.PlayingSound("./source/Sound/UI�ړ���.mp3");
				keyChangeMode_ = true;//�L�[�ύX�J�n
			}
			if (keyChangeMode_)
			{
				KeyChange((int)UI_SUB_ITEM::KEY_CHANGE);
			}
			break;
		//case UI_SUB_ITEM::KEY_DOWN:
		//	if (KeyTrgDown(InputID::Decide))
		//	{
		//		lpSoundMng.PlayingSound("./source/Sound/UI�ړ���.mp3");
		//		keyChangeMode_ = true;//�L�[�ύX�J�n
		//	}
		//	if (keyChangeMode_)
		//	{
		//		KeyChange((int)UI_SUB_ITEM::KEY_DOWN);
		//	}
		//	break;
		//case UI_SUB_ITEM::KEY_LEFT:
		//	if (KeyTrgDown(InputID::Decide))
		//	{
		//		lpSoundMng.PlayingSound("./source/Sound/UI�ړ���.mp3");
		//		keyChangeMode_ = true;//�L�[�ύX�J�n
		//	}
		//	if (keyChangeMode_)
		//	{
		//		KeyChange((int)UI_SUB_ITEM::KEY_LEFT);
		//	}
		//	break;
		//case UI_SUB_ITEM::KEY_RIGHT:
		//	if (KeyTrgDown(InputID::Decide))
		//	{
		//		lpSoundMng.PlayingSound("./source/Sound/UI�ړ���.mp3");
		//		keyChangeMode_ = true;//�L�[�ύX�J�n
		//	}
		//	if (keyChangeMode_)
		//	{
		//		KeyChange((int)UI_SUB_ITEM::KEY_RIGHT);
		//	}
		//	break;
		//case UI_SUB_ITEM::KEY_MENU:
		//	if (KeyTrgDown(InputID::Decide))
		//	{
		//		lpSoundMng.PlayingSound("./source/Sound/UI�ړ���.mp3");
		//		keyChangeMode_ = true;//�L�[�ύX�J�n
		//	}
		//	if (keyChangeMode_)
		//	{
		//		KeyChange((int)UI_SUB_ITEM::KEY_MENU);
		//	}
		//	break;
		//case UI_SUB_ITEM::KEY_RESET:
		//	if (KeyTrgDown(InputID::Decide))
		//	{
		//		lpSoundMng.PlayingSound("./source/Sound/UI�ړ���.mp3");
		//		keyChangeMode_ = true;//�L�[�ύX�J�n
		//	}
		//	if (keyChangeMode_)
		//	{
		//		KeyChange((int)UI_SUB_ITEM::KEY_RESET);
		//	}
		default:
			break;
		}
	}
	case OptionState::Volume:
	{
		
		GetMousePoint(&MousePos.x, &MousePos.y);
		
		switch (lpSceneUIMng.GetHitItem_Sub())//���J�[�\��������Ă��鍀�ڂ��擾
		{
		case UI_SUB_ITEM::VOL_BGM:
			if (cntData_[InputID::Decide][static_cast<int>(Trg::Now)])
			{
				vol = (MousePos.x+32) / 2 - 480;
				if (0 < vol && vol < 255)
				{
					lpSoundMng.ChangeVolume(vol-30,"./source/Sound/walk_trim.mp3");
					lpSoundMng.ChangeVolume(vol, "./source/Sound/coinget.mp3");
					lpSoundMng.ChangeVolume(vol,"./source/Sound/��02.mp3");
					lpSoundMng.ChangeVolume(vol, "./source/Sound/���{�b�g����������3.mp3");
					lpSoundMng.ChangeVolume(vol, "./source/Sound/EnemyDeath.mp3");
					lpSoundMng.ChangeVolume(vol, "./source/Sound/UI�ړ���.mp3");
					lpSoundMng.ChangeVolume(vol, "./source/Sound/UI�I����.mp3");
					lpSoundMng.ChangeVolume(vol-30, "./source/Sound/machikouba.mp3");
					lpSoundMng.ChangeVolume(vol-10, "./source/Sound/streetofrage.mp3");
					lpSoundMng.ChangeVolume(vol-10, "./source/Sound/stageclear.mp3");
					lpSoundMng.ChangeVolume(vol-10, "./source/Sound/selectscene.mp3");
				}
				if (0 > vol)
				{
					vol = 0;
				}
				if (255 < vol)
				{
					vol = 255;
				}
				lpSceneMng.SetSoundVol(vol);
			}
			break;
		case UI_SUB_ITEM::VOL_SE:
			break;
		default:
			break;
		}
	}
	case OptionState::Tips:
	{
		switch (lpSceneUIMng.GetHitItem_Sub())//���J�[�\��������Ă��鍀�ڂ��擾
		{
		case UI_SUB_ITEM::BACK:
			break;
		case UI_SUB_ITEM::NEXT:
			break;
		default:
			break;
		}
	}
	case OptionState::Quit:
	{
		break;
	}
	default:
		break;
	}
	
	if (KeyTrgDown(InputID::Menu))
	{
		lpSoundMng.PlayingSound("./source/Sound/UI�I����.mp3");
		return lpSceneUIMng.Init(std::move(mOldScene));
	}
	//if (keyResetFlg)
	//{
	//	if (cnt > 60)//��b��������\��������
	//	{
	//		keyResetFlg = false;
	//		cnt = 0;
	//	}
	//	cnt++;
	//}


	DrawOwnScreen(delta);

	return ownScene;
}

void OptionScene::DrawOwnScreen(double delta)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	mOldScene->Draw(delta);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	DrawBox(329, 168, 1591, 951, 0x111111, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	/*if (lpCntMng.GetController()->GetWarningFlg())
	{
		DrawStringToHandle(470, 222, "�x���F�d�����Ă���L�[�����ւ��܂����B", 0xff0000, lpSceneMng.GetFontHandle());
	}*/
	switch (state_)
	{
	case OptionState::KeyConfig:
	{
		if (keyChangeMode_)//�L�[�ύX���[�h
		{
			DrawGraph(screenSize_.x / 2 - 608 / 2, 200, lpImageMng.GetID("./source/UIImage/string/OPTION/keyCon2.png")[0], true);
		}
		else
		{
			DrawGraph(screenSize_.x / 2 - 730 / 2, 222, lpImageMng.GetID("./source/UIImage/string/OPTION/keyCon1.png")[0], true);
		}
		DrawFormatStringToHandle(KEYNAME_LEFT+200, 322, 0xffffff, lpSceneMng.GetFontHandle(), "%s", lpCntMng.GetController()->GetNewKeyData((int)UI_SUB_ITEM::KEY_CHANGE)->Name);
		//DrawFormatStringToHandle(KEYNAME_LEFT, 430, 0xffffff, lpSceneMng.GetFontHandle(), "%s", lpCntMng.GetController()->GetNewKeyData((int)UI_SUB_ITEM::KEY_DOWN)->Name);
		//DrawFormatStringToHandle(KEYNAME_LEFT, 544, 0xffffff, lpSceneMng.GetFontHandle(), "%s", lpCntMng.GetController()->GetNewKeyData((int)UI_SUB_ITEM::KEY_LEFT)->Name);
		//DrawFormatStringToHandle(KEYNAME_LEFT, 656, 0xffffff, lpSceneMng.GetFontHandle(), "%s", lpCntMng.GetController()->GetNewKeyData((int)UI_SUB_ITEM::KEY_RIGHT)->Name);
	/*	DrawFormatStringToHandle(KEYNAME_LEFT, 769, 0xffffff, lpSceneMng.GetFontHandle(), "%s", lpCntMng.GetController()->GetNewKeyData((int)UI_SUB_ITEM::KEY_MENU)->Name);
		DrawFormatStringToHandle(KEYNAME_LEFT, 882, 0xffffff, lpSceneMng.GetFontHandle(), "%s", lpCntMng.GetController()->GetNewKeyData((int)UI_SUB_ITEM::KEY_RESET)->Name);*/
		switch (lpSceneUIMng.GetHitItem_Sub())//���J�[�\��������Ă��鍀�ڂ��擾
		{
		case UI_SUB_ITEM::KEY_CHANGE:
			//TRACE("%s\n", controller_->GetNewKeyData((int)UI_SUB_ITEM::KEY_UP)->Name);
			break;
		//case UI_SUB_ITEM::KEY_DOWN:
		//	//TRACE("%s\n", controller_->GetNewKeyData((int)UI_SUB_ITEM::KEY_DOWN)->Name);
		//	break;
		//case UI_SUB_ITEM::KEY_LEFT:
		//	//TRACE("%s\n", controller_->GetNewKeyData((int)UI_SUB_ITEM::KEY_LEFT)->Name);
		//	break;
		//case UI_SUB_ITEM::KEY_RIGHT:
		//	//TRACE("%s\n", controller_->GetNewKeyData((int)UI_SUB_ITEM::KEY_RIGHT)->Name);
		//	break;
		default:
			break;
		}
		break;
	}
	case OptionState::Volume:
	{
		DrawGraph(screenSize_.x / 2 - 771 / 2, 222, lpImageMng.GetID("./source/UIImage/string/OPTION/sound.png")[0], true);
		DrawLine(SUB_ITEM_LEFT_VOL, 512, 1470, 512, 0x555555, 3);
		DrawBox(SUB_ITEM_LEFT_VOL, 508, SUB_ITEM_LEFT_VOL +vol*2, 515, 0xffffff, true);
		DrawCircle(SUB_ITEM_LEFT_VOL +vol*2, 512, 10, 0xffffff, true);
		DrawFormatStringToHandle(800, 450, 0xffffff, lpSceneMng.GetFontHandle(), "Volume:%d", vol);
		switch (lpSceneUIMng.GetHitItem_Sub())//���J�[�\��������Ă��鍀�ڂ��擾
		{
		case UI_SUB_ITEM::VOL_BGM:
			
			break;
		case UI_SUB_ITEM::VOL_SE:
			break;
		default:
			break;
		}
		break;
	}
	case OptionState::Tips:
	{
		DrawGraph(screenSize_.x / 2 - 496 / 2, 222, lpImageMng.GetID("./source/UIImage/string/OPTION/tips.png")[0], true);
		switch (lpSceneUIMng.GetHitItem_Sub())//���J�[�\��������Ă��鍀�ڂ��擾
		{
		case UI_SUB_ITEM::BACK:
			break;
		case UI_SUB_ITEM::NEXT:
			break;
		default:
			break;
		}
		break;
	}
	case OptionState::Quit:
	{
		//DrawStringToHandle(912, 544, "�I�v�V���������", 0xffffff, lpSceneMng.GetFontHandle());
		//DrawStringToHandle(912, 752, "�Q�[������߂�", 0xffffff, lpSceneMng.GetFontHandle());
		//switch (lpSceneUIMng.GetHitItem_Sub())//���J�[�\��������Ă��鍀�ڂ��擾
		//{
		//case UI_SUB_ITEM::YES:
		//	
		//	break;
		//case UI_SUB_ITEM::NO:
		//	
		//	break;
		//default:
		//	break;
		//}
		break;
	}
	default:
		break;
	}
	lpCntMng.GetController()->Draw();
}

void OptionScene::KeyChange(int subItem)
{
	int mouseInput = GetMouseInput();
	if (CheckHitKeyAll() != 0)
	{

		TRACE("keyup\n");
		TRACE("�L�[�ύX�J�n\n");

		lpCntMng.GetController()->SetNewKey(
			(int)subItem,//�ύX�������L�[�̏ꏊ
			lpCntMng.GetController()->KeyChange(//���ݐݒ肳��Ă���L�[���擾���A�V�����L�[��ݒ肷��BESC�������ꂽ�炻�̂܂܂ɂ���B
				lpCntMng.GetController()->GetNowKey((int)subItem))//�ύX�������L�[������
		);
		TRACE("%s\n", lpCntMng.GetController()->GetNewKeyData(subItem)->Name);
		if (lpCntMng.GetKeyChangeFlg())
		{
			keyChangeMode_ = false;
			TRACE("�L�[�ύX����\n");
			lpCntMng.GetController()->KeyConfig_Save();//�L�[�ύX���s��ꂽ��txt�ɃZ�[�u����B
			lpCntMng.SetKeyChangeFlg(false);
		}
	}
}


OptionState OptionScene::GetOptionState()
{
	return state_;
}
