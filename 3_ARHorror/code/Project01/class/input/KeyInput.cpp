#include<DxLib.h>
#include <bitset>
#include "KeyInput.h"
#include "../common/SocketServer.h"
#include "../../_debug/_DebugConOut.h"

constexpr int KEY_INPUT_SINGLE_MAX = 32;
KeyInput::KeyInput()
{
	inputTable_ = {
		{KeyID::Up,		  KEY_INPUT_W},
		{KeyID::Down,	  KEY_INPUT_S},
		{KeyID::Left,	  KEY_INPUT_A},
		{KeyID::Right,	  KEY_INPUT_D},

		{KeyID::Decide,	  MOUSE_INPUT_LEFT},
		{KeyID::Cancel,	  MOUSE_INPUT_RIGHT},
		{KeyID::Dash,	  KEY_INPUT_LSHIFT},
		{KeyID::Inventory,KEY_INPUT_TAB},
		{KeyID::Pause,	  KEY_INPUT_ESCAPE},
		{KeyID::Interact, KEY_INPUT_E},
		{KeyID::Shoot,	  KEY_INPUT_SPACE},
		{KeyID::Attack,	  KEY_INPUT_P}
	};

	for (auto data : inputTable_)
	{
		controllerData_.try_emplace(data.first, AgeBool{ 0,0 });
	}
}

KeyInput::~KeyInput()
{
}

void KeyInput::Update(void)
{
	GetHitKeyStateAll(keyData_.data());
	
	//マウス押下状態を取得
	auto mouseInput = GetMouseInput();

	//lpSocketClient.Receive();

	for (auto id : KeyID())
	{
		//マウスの場合は別の処理
		if (id == KeyID::Decide ||
			id == KeyID::Cancel)
		{
			controllerData_[id][static_cast<int>(ControllerAge::Old)] = controllerData_[id][static_cast<int>(ControllerAge::Now)];
			controllerData_[id][static_cast<int>(ControllerAge::Now)] = ((mouseInput & inputTable_[id]) > 0);
			
			//TelephoneMoveButton(id, true,lpSocketServer.GetMouseflag());
		}
		else
		{
			controllerData_[id][static_cast<int>(ControllerAge::Old)] = controllerData_[id][static_cast<int>(ControllerAge::Now)];
			controllerData_[id][static_cast<int>(ControllerAge::Now)] = keyData_[inputTable_[id]];

			SimultaneousTelephoneButton(id,false, lpSocketServer.GetMouseflag());
		}
		MoveOnlyTelephoneButton(id);
		
	}
	if (lpSocketServer.GetMouseflag())
	{
		controllerData_[KeyID::Decide][static_cast<int>(ControllerAge::Now)] = lpSocketServer.GetMouseflag();
	}
}

void KeyInput::SimultaneousTelephoneButton(KeyID id , bool PcMouseflag,bool mouseflag)
{
	//if (PcMouseflag)
	//{
	//	if (!mouseflag)
	//	{
	//		return;
	//	}
	//	if (lpSocketServer.GetSupplementValue() == 0)
	//	{
	//		return;
	//	}
	//	if (inputTable_[id] == lpSocketServer.GetSupplementValue())
	//	{
	//		//TRACE("id %d\n", lpSocketServer.GetKeyValue());
	//		controllerData_[id][static_cast<int>(ControllerAge::Now)] = true;
	//	}
	//}
	//else
	{
		//if (lpSocketServer.GetKeyValue() + lpSocketServer.GetSupplementValue() == 0)
		//{
		//	return;
		//}
		//if (inputTable_[id] == lpSocketServer.GetKeyValue())
		//{
		//	//TRACE("id %d\n", lpSocketServer.GetKeyValue());
		//	controllerData_[id][static_cast<int>(ControllerAge::Now)] = true;

		//}
		if (inputTable_[id] == lpSocketServer.GetSupplementValue())
		{
			//TRACE("id %d\n", lpSocketServer.GetKeyValue());
			controllerData_[id][static_cast<int>(ControllerAge::Now)] = true;
		}
	}
}

void KeyInput::MoveOnlyTelephoneButton(KeyID id)
{
	if (lpSocketServer.GetKeyValue() == 0)
	{
		return;
	}
	if (id == KeyID::Up ||
		id == KeyID::Down ||
		id == KeyID::Left ||
		id == KeyID::Right)
	{
		for (auto id2 : KeyID())
		{
			if (id2 == KeyID::Up ||
				id2 == KeyID::Down ||
				id2 == KeyID::Left ||
				id2 == KeyID::Right)
			{
				if (KEY_INPUT_SINGLE_MAX < lpSocketServer.GetKeyValue())
				{
					if (inputTable_[id] == inputTable_[id2])
					{
						continue;
					}
					if ((inputTable_[id] + inputTable_[id2]) == lpSocketServer.GetKeyValue())
					{
						/*TRACE("入力2個:	%d %d\n", inputTable_[id], inputTable_[id2]);
						TRACE("入力2個:	%d\n", inputTable_[id] + inputTable_[id2]);*/
						controllerData_[id][static_cast<int>(ControllerAge::Now)] = true;
						controllerData_[id2][static_cast<int>(ControllerAge::Now)] = true;
					}
				}
			}

		}
	}
}
