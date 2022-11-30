#include<DxLib.h>
#include "PadInput.h"

PadInput::PadInput()
{
	InputTable_ = {
	{KeyID::Up,PAD_INPUT_UP},
	{KeyID::Down,PAD_INPUT_DOWN},
	{KeyID::Left,PAD_INPUT_LEFT},
	{KeyID::Right,PAD_INPUT_RIGHT},
	{KeyID::Decide,PAD_INPUT_1},
	{KeyID::Cancel,PAD_INPUT_2},
	{KeyID::Pause,PAD_INPUT_3},
	{KeyID::Button,PAD_INPUT_4}
	};

	for (auto data : InputTable_)
	{
		CntData_.try_emplace(data.first, AgeBool{ 0,0 });
	}
}

PadInput::~PadInput()
{
}

void PadInput::Update(void)
{
	auto PadData = GetJoypadInputState(DX_INPUT_PAD1);
	for (auto id : KeyID())
	{
		CntData_[id][static_cast<int>(ControllerAge::Old)] = CntData_[id][static_cast<int>(ControllerAge::Now)];
		CntData_[id][static_cast<int>(ControllerAge::Now)] = (PadData & InputTable_[id]) > 0;
	}
}
