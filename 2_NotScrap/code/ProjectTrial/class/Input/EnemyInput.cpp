#include <DxLib.h>
#include "EnemyInput.h"
//#include "EnemyMoveCtl.h"
EnemyInput::EnemyInput()
{
	turnflag_ = false;
}

EnemyInput::~EnemyInput()
{
}

void EnemyInput::Update(void)
{
	//¡‚Í‚¸‚Á‚Æ‰EˆÚ“®‚³‚¹‚Ä‚¢‚é
	//‚±‚±‚É“G‚ÌˆÚ“®ƒAƒ‹ƒSƒŠƒYƒ€‚ð‘‚­—\’è
	if (enemy_->ReturnChangeTurnFlag())
	{
		turnflag_ = !turnflag_;
	}
	if (turnflag_)
	{
		cntData_[InputID::Right][static_cast<int>(Trg::Now)] = 1;

	}
	/*else
	{
		cntData_[InputID::Left][static_cast<int>(Trg::Now)] = 1;

	}*/
}
