#include <DxLib.h>
#include<EffekseerForDXLib.h>
#include<chrono>
#include<memory>
#include"class/Scene/SceneManager.h"
#include "_debug/_DebugConOut.h"
#include "_debug/_DebugDispOut.h"

//#define SCREEN_SIZE_X 1024 ‚Å‚«‚é‚¾‚¯Žg‚í‚È‚¢


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	lpSceneMng.Run();

	Effkseer_End();
	DxLib_End();

	return 0;
}