#pragma once
#include "../scene/BaseScene.h"

#define lpMouseIcon MouseIcon::GetInstance()
class MouseIcon 
{
public:
	static MouseIcon& GetInstance()
	{
		static MouseIcon s_Instance;
		return s_Instance;
	}
	
	void Init(void);
	void Release(void);
	void Update(SceneID id);
	void Draw(void);	
	virtual void SetFlg(bool flg) { flg_ = flg; }

private:
	MouseIcon();
	~MouseIcon();


	

	SceneID sceneID_;
protected:
	bool flg_ = false;
};

