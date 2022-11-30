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
	//今はずっと右移動させている
	//ここに敵の移動アルゴリズムを書く予定
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
