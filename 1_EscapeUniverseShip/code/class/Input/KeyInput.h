#pragma once
#include"Controller.h"

class KeyInput :
	public Controller
{
public:
	KeyInput();
	~KeyInput();

	virtual void Update(void) override;
	virtual ControllerType GetControllerType(void) override { return ControllerType::Key; };
private:
	std::array<char, 256U> KeyData_;
	std::map<KeyID, unsigned int> InputTable_;
};

