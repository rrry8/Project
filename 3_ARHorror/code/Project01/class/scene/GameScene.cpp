#include<DxLib.h>
#include "GameScene.h"
#include "ResultScene.h"
#include"InventoryScene.h"
#include"GameOverScene.h"
#include"OptionScene.h"
#include"../object/player/Player.h"
#include"../object/enemy/Enemy.h"
#include"../object/Stage.h"
#include"../camera/Camera.h"
#include "../object/Goal.h"
#include "../object/Mirror.h"
#include"../input/KeyInput.h"
#include"../input/PadInput.h"
#include"../../_debug/_DebugConOut.h"

#include "../common/manager/CBufferMng.h"
#include "../common/manager/ImageMng.h"
#include "../common/manager/SoundMng.h"
#include "../common/OriginalShader.h"
#include "SceneManager.h"

constexpr int UI_MOVE_VALUE = 10;
constexpr int MAX_STOP_TIME = 100;

GameScene::GameScene()
{
	Init();

}

GameScene::~GameScene()
{
	Release();
}

void GameScene::Init(void)
{
	tmxObj_.LoadTmx("resource/tmx/game.tmx", false);
	
	
	//パッドの数が１つ以上の場合はパッド操作にする
	if (GetJoypadNum() > 0)
	{
		controller_ = std::make_unique<PadInput>();
	}
	else
	{
		controller_ = std::make_unique<KeyInput>();
	}

	
	//グラフィックサイズを確保
	inventoryBackGroundHandle_ = MakeGraph (lpSceneMng.GetScreenSize().x, lpSceneMng.GetScreenSize().y);

	//スクリーンを作成
	screenCameraID_			= MakeScreen(screenSizeX_, screenSizeY_);
	GaussScreenCameraID_	= MakeScreen(screenSizeX_, screenSizeY_);

	//インスタンス生成
	stage_			= std::make_unique<Stage> ();		//ステージ
	player_			= std::make_unique<Player>(controller_->GetControllerType(), stage_->GetGimmickList(), stage_->GetItemList());	//プレイヤー
	camera_			= std::make_unique<Camera>();	//カメラ
	enemy_			= std::make_unique<Enemy> ();	//敵
	goal_			= std::make_unique<Goal>  ("resource/model/Goal.mv1", Vector3{ 1300, 1350, 2500 });	//敵
	originalShader_ = std::make_unique<OriginalShader>(LoadPixelShader("resource/shader/ps/DfferedLighting_ps.pso"));

	InitCBuffer();
	InitUI();

	lpSoundMng.SetDefaultVolume();

	onoff_ = false;
}

UniqueScene GameScene::Update(UniqueScene scene)
{
	//コントローラ
	controller_->Update();

	//Cバッファの更新
	UpdateCBuffer();

	//UIの更新
	UIUpdate();

	//ステージ
	stage_ ->Update();
	//カメラ
	camera_->Update(player_->GetDashFlag());
	//プレイヤー
	player_->Update(camera_->GetAngles(), camera_->GetTargetPos());
	//敵
	enemy_ ->Update(player_->GetCapsule(), player_->GetPosition(), player_->CheckHitFlash(camera_->GetTargetPos()));

	//
	if (controller_->CheckInputKey(KeyID::Pause))
	{
		return std::make_unique<OptionScene>(std::move(scene), controller_->GetControllerType(), GetSceneID());
	}
	//インベントリ
	if (controller_->CheckInputKey(KeyID::Inventory))
	{
		//表示画面をインベントリで背景にする
		lpSceneMng.AddInventoryCnt();
		GetDrawScreenGraph(0, 0, lpSceneMng.GetScreenSize().x, lpSceneMng.GetScreenSize().y, inventoryBackGroundHandle_);
		return std::make_unique<InventoryScene>(std::move(scene), inventoryBackGroundHandle_, controller_->GetControllerType(), player_->GetInventoryList(), player_->GetMinCameraPhotoVector());
	}
	//ゲームオーバー時
	if (enemy_->GetHitPlayerFlag())
	{
		return std::make_unique<GameOverScene>(std::move(scene), controller_->GetControllerType(), GetSceneID(), player_->GetPosition(), enemy_->GetPosition());
	}
	//ゲームクリア時
	if (player_->GetIsGameClear())
	{
		return std::make_unique<ResultScene>(controller_->GetControllerType());
	}

	
	//デバッグ用
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	if (CheckHitKey(KEY_INPUT_R))
	{
		return std::make_unique<ResultScene>(controller_->GetControllerType());
	}
	if (CheckHitKey(KEY_INPUT_3))
	{
		return std::make_unique<GameOverScene>(std::move(scene), controller_->GetControllerType(), GetSceneID(), player_->GetPosition(), enemy_->GetPosition());
	}
	if (CheckHitKey(KEY_INPUT_I))
	{
		onoff_ = !onoff_;
	}
	// パラメータ操作処理
	{
		DebugKeyMove<float>(KEY_INPUT_Y, KEY_INPUT_H, 5, DepthOfField_Focus_);
		DebugKeyMove<float>(KEY_INPUT_U, KEY_INPUT_J, 5, DepthOfField_FocusSize_);
		DebugKeyMove<float>(KEY_INPUT_I, KEY_INPUT_K, 5, DepthOfField_InterpSize_);
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー

	//描画
	DrawScreen();

	return scene;
}

void GameScene::DrawScreen(void)
{
	////マルチレンダーターゲット
	//SetRenderTargetToShader(0, albedRT_);
	//SetRenderTargetToShader(1, normalSpecRT_);
	//SetRenderTargetToShader(2, depthRT_);
	//ClsDrawScreen();
	////カメラ処理(オブジェクト描画前に処理)
	//camera_->SetBeforeDraw(player_->GetPosition());
	////ステージ
	//stage_->Draw(player_->GetIsOnCamera());

	//// マルチレンダーターゲットのリセット
	//// 描画先の設定をリセット
	//SetRenderTargetToShader(0, -1);
	//SetRenderTargetToShader(1, -1);
	//SetRenderTargetToShader(2, -1);
	// 先に鏡の中の描画
	//DrawMirrorWorld();

	SetDrawScreen(screenCameraID_);
	ClsDrawScreen();

	//カメラ処理(オブジェクト描画前に処理)
	camera_->SetBeforeDraw(player_->GetPosition());
	//ステージ
	stage_->Draw(player_->GetIsOnCamera());
	//敵
	enemy_->Draw();
	//プレイヤー
	player_->SetMinCameraRenderTarget(screenCameraID_);

	SetDrawScreen(screenID_);
	ClsDrawScreen();

	DrawGraph(0, 0, screenCameraID_, true);
	player_->Draw();

	UIDraw();

	//バックドロー
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	
	//カメラ描画
	//camera_->Draw();
	
	//float ff = 0.01;
	//MV1DrawModelDebug(lpModelMng.GetID("resource/model/stage/Coll3.mv1")[0], 0xffff44, 1, ff, 1, 0);
	//lpCBufferMng.Draw();
	//goal_->Draw();
	
	//DrawGraph(0, 0, depthRT_, false);
	//originalShader_->DrawOnlyPixel(0, 0, screenCameraID_, albedRT_, normalSpecRT_, depthRT_);

	/*GetDrawScreenSize(&screenSizeX_, &screenSizeY_);
	BltDrawValidGraph(screenID_, 0, 0, screenSizeX_, screenSizeY_, 500, 400, screenCameraID_);
	DrawGraph(110, 110, screenCameraID_, true);*/

	DrawString(0, 400, "Iキー:操作説明・IPアドレスの表示", 0xffffff);

	if (onoff_)
	{
		IPDATA IPAddress[16];
		GetMyIPAddress(IPAddress);
		DrawFormatString(0, 450, 0xffff00, "IPアドレス　　%d.%d.%d.%d", IPAddress->d1, IPAddress->d2, IPAddress->d3, IPAddress->d4);
		DrawExtendGraph(0, 0, 1100, 370, lpImageMng.GetID("./resource/image/KeyTyuto.png")[0], true);
	}

	// パラメータを描画
	//DrawFormatString(0, 600, GetColor(255, 255, 255), "補間距離:%.1f ぼかし無し中心:%.3f ぼかし無し範囲:%.3f",
	//	DepthOfField_InterpSize_, DepthOfField_Focus_, DepthOfField_FocusSize_);

	// キー操作説明を描画
	//DrawString(0, 600 - 16, "YHキー:ぼかし無し中心変更　UJキー:ぼかし無し範囲変更　IKキー:補間距離変更", GetColor(255, 255, 255));
	//DrawString(0, 600 - 32, "方向キー:カメラの操作", GetColor(255, 255, 255));

	// ≪ F P S に注意！！ ≫
	//float fps = GetFPS();	
	//DrawFormatString(360, 32, 0xffffff, "FPS:%.1f", fps);//fpsクラスでwaitしてるが反映されてる感じがしない
	
	//lpCBufferMng.DebugBufferDraw("dライト  ",1300,9, 16 * DIRECTION_LIGHT_NUM);
	//lpCBufferMng.DebugBufferDraw("pライト  ", 1300, 10, 16 * POINT_LIGHT_NUM);
	//lpCBufferMng.DebugBufferDraw("sライト  ", 1700, 11, 16 );
	// 
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	
}

void GameScene::Release(void)
{
	DeleteGraph(inventoryBackGroundHandle_);
}

void GameScene::InitCBuffer(void)
{
	//バッファの作成
	lpCBufferMng.Remove();
	lpCBufferMng.Create();
	//カメラ
	lpCBufferMng.CreatePixelCbuffer(16);
	//ディフューズライト
	lpCBufferMng.CreatePixelCbuffer(16 * DIRECTION_LIGHT_NUM);
	//ポイントライト
	lpCBufferMng.CreatePixelCbuffer(16 * POINT_LIGHT_NUM);
	//スポットライト
	lpCBufferMng.CreatePixelCbuffer(16);
	//アンビエント＆すぺきゅらパワー
	lpCBufferMng.CreatePixelCbuffer(16);

	//被写界深度焦点
	lpCBufferMng.CreatePixelCbuffer(3 + 1 + 3 + 1);//focus + pad+area+pad
	
	// 被写界深度処理用のパラメータを初期化
	DepthOfField_InterpSize_ = 3000.0f;
	DepthOfField_Focus_ = -1000.0f;
	DepthOfField_FocusSize_ = 500.0f;

	//ライトバッファの更新
	InitLights();
}

void GameScene::InitLights(void)
{
	int i = 0;
	for (int j = 0; j < DIRECTION_LIGHT_NUM; j++)
	{
		lpCBufferMng.SetPixelCbuffer(9, i,
			{ 
			light_.directionLights[j].color.x,
			light_.directionLights[j].color.y,
			light_.directionLights[j].color.z,

			light_.directionLights[j].pad0 
			});
		i++;
		lpCBufferMng.SetPixelCbuffer(9, i,
			{ 
			light_.directionLights[j].direction.x ,
			light_.directionLights[j].direction.y,
			light_.directionLights[j].direction.z,

			light_.directionLights[j].pad1 
			});
		i++;
	}
	
	light_.pointLights[0].position = { -4600 ,1300 ,-10500 };
	light_.pointLights[0].color = { 10,8,8 };
	light_.pointLights[0].range = 4000;

	light_.pointLights[1].position = { 8200, 5000, -8300 };
	light_.pointLights[1].color = { 8,10,10 };
	light_.pointLights[1].range = 4000;

	light_.pointLights[2].position = { -5000, 1300, -1200 };
	light_.pointLights[2].color = { 4,2,7 };
	light_.pointLights[2].range = 4000;


	/*light_.pointLights[3].position = { -7400, 1300, -19700 };
	light_.pointLights[3].color = { 8,10,10 };
	light_.pointLights[3].range = 2000;*/
	i = 0;
	for (int j = 0; j < POINT_LIGHT_NUM; j++)
	{	
		/*light_.pointLights[j].position.x = 5;
		light_.pointLights[j].color.x = 6;
		light_.pointLights[j].pad1 = 7;
		light_.pointLights[j].pad5 = 8;*/
		lpCBufferMng.SetPixelCbuffer(10, i,
			{
			light_.pointLights[j].position.x,
			light_.pointLights[j].position.y,
			light_.pointLights[j].position.z,

			light_.pointLights[j].pad0 
			});
		i++;
		lpCBufferMng.SetPixelCbuffer(10, i,
			{
			light_.pointLights[j].color.x ,
			light_.pointLights[j].color.y,
			light_.pointLights[j].color.z,

			light_.pointLights[j].range 
			});
		i++;
	}

	light_.spotLights.Position = { -3000,1250,-9000 };

	light_.spotLights.Color.x = 10.0f;
	light_.spotLights.Color.y = 10.0f;
	light_.spotLights.Color.z = 10.0f;

	light_.spotLights.Direction.x = 1.0f;
	light_.spotLights.Direction.y = -0.2f;
	light_.spotLights.Direction.z = 0.0f;

	light_.spotLights.Direction.Normalize();

	light_.spotLights.Range = 10000.0f;
	light_.spotLights.Angle = Deg2RadF(15.0f);

	i = 0;
	lpCBufferMng.SetPixelCbuffer(11, i,
		{
		light_.spotLights.Position.x ,
		light_.spotLights.Position.y,
		light_.spotLights.Position.z,

		light_.spotLights.pad0 
		});
	i++;
	lpCBufferMng.SetPixelCbuffer(11, i,
		{
		light_.spotLights.Color.x ,
		light_.spotLights.Color.y,
		light_.spotLights.Color.z,

		light_.spotLights.Range 
		});
	i++;
	lpCBufferMng.SetPixelCbuffer(11, i,
		{
		light_.spotLights.Direction.x ,
		light_.spotLights.Direction.y,
		light_.spotLights.Direction.z,

		light_.spotLights.Angle 
		});
	i++;


	light_.ambientLight = { 0.1,0.1,0.1 };
	lpCBufferMng.SetPixelCbuffer(12, 0,
		{
		light_.ambientLight.x,
		light_.ambientLight.y,
		light_.ambientLight.z,
		light_.specPow });
}

void GameScene::InitUI(void)
{
	UIVariableList_ =
	{
		{"tutorial",MoveUI({0,0},{0,0},StatusUI::Stop,0)},
	};
	for (auto& coll : tmxObj_.GetTipsCollList())
	{
		for (auto& ui : UIVariableList_)
		{
			if (coll.first == ui.first)
			{
				Vector2 cen = { coll.second.first.x + coll.second.second.x / 2 ,coll.second.first.y + coll.second.second.y / 2 };
				//目的地					//現在の位置			移動中か      //ブレンドする値
				UIVariableList_[ui.first] = MoveUI(Vector2(cen.x, cen.y), Vector2(cen.x + 500, cen.y), StatusUI::Stop, 0);
			}
		}
	}
}

void GameScene::UpdateCBuffer(void)
{
	lpCBufferMng.Update();

	Vector3 eyePos = camera_->GetPos();

	lpCBufferMng.SetPixelCbuffer(8,0,
		{ 
		eyePos.x,
		eyePos.y,
		eyePos.z,
		0.0f 
		});
	light_.spotLights.Position = camera_->GetPos();

	lpCBufferMng.SetPixelCbuffer(11, 0,
		{
		light_.spotLights.Position.x ,
		light_.spotLights.Position.y,
		light_.spotLights.Position.z,

		light_.spotLights.pad0 
		});

	light_.spotLights.Direction = camera_->GetTargetPos() - camera_->GetPos();


	light_.spotLights.Direction.Normalize();
	lpCBufferMng.SetPixelCbuffer(11, 2,
		{
		light_.spotLights.Direction.x ,
		light_.spotLights.Direction.y,
		light_.spotLights.Direction.z,

		light_.spotLights.Angle 
		});


	// 補間範囲とフォーカスが合っている範囲を含めた総距離を算出
	DepthOfField_TotalSize_ = DepthOfField_InterpSize_ * 2.0f + DepthOfField_FocusSize_;

	// 被写界深度の開始位置を計算
	Dof_.Area.x = DepthOfField_Focus_ - DepthOfField_FocusSize_ / 2.0f - DepthOfField_InterpSize_;

	// 被写界深度の終了位置を計算
	Dof_.Area.y = DepthOfField_Focus_ + DepthOfField_FocusSize_ / 2.0f + DepthOfField_InterpSize_;

	// 被写界深度の範囲の逆数を計算
	Dof_.Area.z = 1.0f / (Dof_.Area.y - Dof_.Area.x);

	// 被写界深度のぼかし無しの開始位置を計算
	Dof_.Focus.x = DepthOfField_InterpSize_ / DepthOfField_TotalSize_;

	// 被写界深度のぼかし無しの終了位置を計算
	Dof_.Focus.y = (DepthOfField_InterpSize_ + DepthOfField_FocusSize_) / DepthOfField_TotalSize_;
}

void GameScene::UIUpdate(void)
{
	for (auto& ui : UIVariableList_)
	{
		if (ui.first == "tutorial")
		{
			if(CheckHitKey(KEY_INPUT_T))
			{
				
				//デバッグ用
				for (auto& coll : tmxObj_.GetTipsCollList())
				{

					if (coll.first == ui.first)
					{
						Vector2 cen = { coll.second.first.x + coll.second.second.x / 2 ,coll.second.first.y + coll.second.second.y / 2 };
						//目的地					//現在の位置			移動中か      //ブレンドする値
						UIVariableList_[ui.first] = MoveUI(Vector2(cen.x, cen.y), Vector2(cen.x + 500, cen.y), StatusUI::Stop, 0);
					}

				}
				ui.second.IsMove = StatusUI::MoveIn;
			}
		}
	}

	for (auto& ui : UIVariableList_)
	{
		if (ui.second.StopCnt < 0)
		{
			continue;
		}
		if (ui.second.IsMove == StatusUI::MoveIn)
		{
			if (ui.second.Destination.x < ui.second.Pos.x)
			{
				ui.second.Pos.x -= UI_MOVE_VALUE;
			}
			else
			{
				ui.second.StopCnt++;
				if (ui.second.StopCnt > MAX_STOP_TIME)
				{
					ui.second.IsMove = StatusUI::MoveOut;
				}
			}
		}
		if (ui.second.IsMove == StatusUI::MoveOut)
		{
			ui.second.Pos.x += UI_MOVE_VALUE;
		}
	}
}

void GameScene::UIDraw(void)
{
	Vector2Int GraphSize;
	for (auto& coll : tmxObj_.GetTipsCollList())
	{
		for (auto& ui : UIVariableList_)
		{
			if (coll.first == ui.first)
			{
				GetGraphSize(lpImageMng.GetID(ui.first)[0], &GraphSize.x, &GraphSize.y);
				GraphSize.x /= 2;
				GraphSize.y /= 2;
				//SetDrawBlendMode(DX_BLENDMODE_ALPHA, ui.second.StopCnt);
				DrawGraph(ui.second.Pos.x - GraphSize.x, ui.second.Pos.y - GraphSize.y, lpImageMng.GetID(ui.first)[0], true);
				//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

				DrawBox(coll.second.first.x,
			coll.second.first.y,
			coll.second.first.x + coll.second.second.x,
			coll.second.first.y + coll.second.second.y,
			0x222222, false);
			}
		}
	}
}

void GameScene::DrawCollCenter(Vector2 CenterPos, std::string imageStr)
{
	Vector2Int GraphSize;

	GetGraphSize(lpImageMng.GetID(imageStr)[0], &GraphSize.x, &GraphSize.y);
	GraphSize.x /= 2;
	GraphSize.y /= 2;
	DrawGraph(CenterPos.x - GraphSize.x, CenterPos.y - GraphSize.y, lpImageMng.GetID(imageStr)[0], true);
}
template<typename T>
void GameScene::DebugKeyMove(unsigned int keyUp, unsigned int keyDown, int value, T& body)
{
	if (CheckHitKey(keyUp))
	{
		body += value;
	}
	if (CheckHitKey(keyDown))
	{
		body -= value;
	}
}

//void GameScene::DrawMirrorWorld()
//{
//	for (int i = 0; i < MIRROR_NUM; i++)
//	{
//		// 描画先を鏡に映る映像の取得に使用する画像を描画するスクリーンにする
//		SetDrawScreen(mirror_->MirrorHandle[i]);
//
//		// 画面を初期化する
//		ClearDrawScreen();
//
//		// 鏡に映る映像を描画する際に使用するカメラの設定を行う
//		mirror_->Mirror_SetupCamera(i, mirror_->GetCameraEyePosition(), mirror_->GetCameraTargetPosition());
//
//		//カメラ処理(オブジェクト描画前に処理)
//		camera_->SetBeforeDraw(player_->GetPosition());
//
//
//		//ステージ
//		stage_->Draw(player_->GetIsOnCamera());
//
//		//カメラ描画
//		//camera_->Draw();
//
//		//敵
//		enemy_->Draw();
//	}
//}