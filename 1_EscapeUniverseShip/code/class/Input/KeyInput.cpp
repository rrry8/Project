#include<DxLib.h>
#include "KeyInput.h"

KeyInput::KeyInput()
{
	InputTable_ = {
		{KeyID::Up,KEY_INPUT_UP},
		{KeyID::Down,KEY_INPUT_DOWN},
		{KeyID::Left,KEY_INPUT_LEFT},
		{KeyID::Right,KEY_INPUT_RIGHT},
		{KeyID::Decide,KEY_INPUT_Z},
		{KeyID::Cancel,KEY_INPUT_X},
		{KeyID::Pause,KEY_INPUT_Q},
		{KeyID::Button,KEY_INPUT_SPACE}
	};

	for (auto data : InputTable_)
	{
		CntData_.try_emplace(data.first, AgeBool{ 0,0 });
	}
}

KeyInput::~KeyInput()
{
}

void KeyInput::Update(void)
{
	GetHitKeyStateAll(KeyData_.data());
	for (auto id : KeyID())
	{
		CntData_[id][static_cast<int>(ControllerAge::Old)] = CntData_[id][static_cast<int>(ControllerAge::Now)];
		CntData_[id][static_cast<int>(ControllerAge::Now)] = KeyData_[InputTable_[id]];
	}
}
