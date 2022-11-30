#pragma once
#include "KeyInput.h"
class Enemy;
class EnemyInput :
    public KeyInput
{
public:
	EnemyInput();
	~EnemyInput();
private:
	void Update(void)override;
	bool turnflag_;
	Enemy* enemy_;
};

