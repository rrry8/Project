#include "Player.h"
#include"Stamina.h"
#include"MinCamera.h"
#include "../../input/KeyInput.h"
#include "../../input/PadInput.h"
#include "../gimmick/Gimmick.h"
#include "../Item/Item.h"
#include "../../common/Capsule.h"
#include "../../scene/SceneManager.h"
#include"../../camera/Camera.h"
#include"../../../_debug/_DebugDispOut.h"
#include"../../../_debug/_DebugConOut.h"
#include "../../common/manager/ModelMng.h"
#include "../../common/MouseIcon.h"
#include "../../common/SocketServer.h"
#include "../../common/manager/ImageMng.h"
#include "../../common/manager/ModelMng.h"
#include"../../object/player/HandLight.h"

//移動関係
constexpr float MOVE_SPEED = 30.0f;				//移動スピード
constexpr float FALL_SPEED = 30.0f;				//落下スピード
constexpr float FALL_GRAVITY = 3.0f;			//重力
constexpr float MOVE_COEFFICIENT = 1.0f;		//移動係数
constexpr float HALF_ROOT_TWO = 0.70710678118f;	//√２分の１

//カメラからオブジェクトの当たり判定の長さ(仮)
constexpr float INTERACT_LINE_LENGTH = 1500.0f;
constexpr float FLASH_LINE_LENGTH = 30000.0f;
constexpr float WALL_INTERACT_LINE_LENGTH = 2000.0f;

//カプセル関係
constexpr float CAPSULE_RELATIVE_TOP_Y  = CAMERA_HEIGHT;//上の球の相対位置
constexpr float CAPSULE_RELATIVE_DOWN_Y = 450.0f;		//下の球の相対位置
constexpr float CAPSULE_RADIUS		    = 400.0f;		//半径

//コリジョン関係
constexpr int WALL_EXTRUSION_NUM = 24;			//壁の押し出し回数
constexpr float SLIDE_DISTANCE = 5.0f;			//押し出し距離
constexpr float COLLISION_LINE_LENGTH = 300.0f;	//足元ラインの長さ

//配置ミスでオフセットが必要
constexpr float COLLSION_STAGE_OFFSET_X = 16990.02f;
constexpr float COLLSION_STAGE_OFFSET_Y = -1323.776f;
constexpr float COLLSION_STAGE_OFFSET_Z = -20603.69f;


constexpr float PICKUP_OFFSETX = -30;
constexpr float PICKUP_OFFSETY = +20;

constexpr int MAX_DIAL_NUM = 4;

Player::Player(ControllerType type, const std::vector<std::shared_ptr<Gimmick>>& gList ,const std::vector<std::shared_ptr<Item>>& iList)
{
	collisionHandle_ = lpModelMng.GetID("resource/model/stage/Coll3.mv1")[0];
	MV1SetPosition(collisionHandle_, VGet(COLLSION_STAGE_OFFSET_X, COLLSION_STAGE_OFFSET_Y, COLLSION_STAGE_OFFSET_Z));
	//コントローラ生成
	if (type == ControllerType::Pad)
	{
		controller_ = std::make_unique<PadInput>();
	}
	else
	{
		controller_ = std::make_unique<KeyInput>();
	}
	gimmickList_ = gList;
	itemList_    = iList;
	pickUpFlag_  = false;
	Init();
}

Player::~Player()
{
	Release();
}

void Player::Init(void)
{
	Vector3 vectorZero = {0.0f,0.0f,0.0f};
	capsule_ = Capsule(VGet(0.0f, CAPSULE_RELATIVE_TOP_Y, 0.0f), VGet(0.0f, CAPSULE_RELATIVE_DOWN_Y, 0.0f), CAPSULE_RADIUS);

	stamina_ = std::make_unique<Stamina>(controller_->GetControllerType());
	minCamera_ = std::make_unique<MinCamera>(controller_->GetControllerType());
	state_ = State::Walk;
	fallGravity_ = 0.0f;

	handLight_ = std::make_unique<HandLight>();
	dashFlag_ = false;
}

void Player::Release(void)
{
	handLight_->Release();
}

void Player::Update(void)
{
	controller_->Update();
}

void Player::Update(Vector3 cameraAngle,Vector3 targetPos)
{
	quaRot_ = Quaternion::Euler(cameraAngle.toVECTOR());
	//各アップデート
	Update();
	handLight_->Update(pos_, cameraAngle,targetPos);

	//移動処理
	MovePosition(cameraAngle);

	minCamera_->Update();

	pickUpFlag_ = false;

	//ステージとの当たり判定
	CollisionStage();

	//ギミックとの当たり判定
	CollisionGimmick();

	//インタラクト処理(放置)
	AddInventory(targetPos);

	//インタラクト処理(ギミック)
	InteractGimmick(targetPos);	

	//ギミックのイベント処理
	GimmickEvent(targetPos);

	//ゴールとの判定
	HitCapsuleAndGoal();
}

void Player::Draw(void)
{
	CameraDraw();
	//MV1DrawModel(collisionHandle_);
	//capsule_->Draw();
	//DrawLine3D(pos_.toVECTOR(), VGet(pos_.x, pos_.y + 300.0f, pos_.z), 0xff0000);

	/*DrawLine3D(PlayerRot_.toVECTOR(), VGet(pos_.x, pos_.y + 300.0f, pos_.z), 0xff0000);*/

	//DrawSphere3D(pos_.toVECTOR() , 50, 100, 0xff0000, 0xff0000, true);

	if (pickUpFlag_)
	{
		DrawStringF((lpSceneMng.GetScreenSize().x / 2.0f) + PICKUP_OFFSETX, (lpSceneMng.GetScreenSize().y / 2.0f) + PICKUP_OFFSETY, "Eで拾う", 0x999999);
	}

	if (isHitGoal_)
	{
		DrawStringF((lpSceneMng.GetScreenSize().x / 2.0f) + PICKUP_OFFSETX, (lpSceneMng.GetScreenSize().y / 2.0f) + PICKUP_OFFSETY, "玄関の鍵が必要", 0x992222);
	}
	stamina_->Draw();
	for (auto& g : gimmickList_)
	{
		std::string n = g->GetName();
		if (n == "Dial")
		{
			
			DrawFormatString(0, 100 + 20 * g->GetModelNumber(), 0xffffff, "number : %d", dial_[g->GetModelNumber()]);
		}
	}

}

void Player::CameraDraw(void)
{
	minCamera_->Draw();
}

Vector3 Player::GetPosition(void)
{
	return pos_;
}

bool Player::GetIsOnCamera()
{
	return minCamera_->GetIsOnCamera();
}

bool Player::GetIsGameClear()
{
	return isGameClear_;
}

bool Player::GetDashFlag(void)
{
	return dashFlag_;
}

void Player::SetMinCameraRenderTarget(int screenID)
{
	minCamera_->SetRenderTarget(screenID);
}

bool Player::CheckHitFlash(Vector3 targetPos)
{
	//フラッシュをしていない場合はfalseを返す
	if (!minCamera_->GetFlashFlag())
	{
		return false;
	}

	Vector3 from = Vector3(pos_.x, pos_.y + CAMERA_HEIGHT, pos_.z);
	Vector3 player2TargetPosVector = (targetPos - from).Normalized();
	Vector3 to = from + player2TargetPosVector * FLASH_LINE_LENGTH;

	//ステージと衝突した場合は当たり判定をステージの当たった場所までにする
	auto stageHits = MV1CollCheck_Line(collisionHandle_, -1, from.toVECTOR(), to.toVECTOR());
	if (stageHits.HitFlag)
	{
		to = ConvertVECTORtoVector3(stageHits.HitPosition);
	}

	//敵との当たり判定
	auto enemyHits = MV1CollCheck_Line(lpModelMng.GetID("./resource/model/enemy/ghost3.mv1")[0], -1, from.toVECTOR(), to.toVECTOR());
	//当たっていない場合はfalseを返す
	if (!enemyHits.HitFlag)
	{
		return false;
	}

	return true;
}

std::vector<int>& Player::GetMinCameraPhotoVector(void)
{
	return minCamera_->GetPhotoVector();
}

std::vector<Collider>& Player::GetColliders(void)
{
	return colliders_;
}

std::vector<InventoryObj>& Player::GetInventoryList(void)
{
	return inventoryList_;
}

Capsule Player::GetCapsule(void)
{
	return capsule_;
}

void Player::MovePosition(Vector3 cameraAngle)
{
	//移動量係数
	float moveCoefficient = MOVE_COEFFICIENT;

	if (state_ == State::Fall)
	{
		//moveCoefficient *= 0.5f;
	}

	//斜め移動しているかどうかチェック
	bool diagonalFlag = CheckDiagonalMove();

	//X軸移動用
	auto movePowerX = [](Vector3 cAngle, float rot, float moveCoefficient_,bool diagonalFlag) {
		float movePower = MOVE_SPEED * moveCoefficient_;	//移動量
		float halfRoot2 = diagonalFlag ? HALF_ROOT_TWO : 1.0f;
		return sinf(cAngle.y + rot) * movePower * halfRoot2;
	};

	//Z軸移動用
	auto movePowerZ = [](Vector3 cAngle, float rot, float moveCoefficient_,bool diagonalFlag) {
		float movePower = MOVE_SPEED * moveCoefficient_;	//移動量
		float halfRoot2 = diagonalFlag ? HALF_ROOT_TWO : 1.0f;
		return cosf(cAngle.y + rot) * movePower * halfRoot2;
	};

	dashFlag_ = false;
	//移動処理
	//ダッシュ処理(スタミナ管理)
	moveCoefficient *= stamina_->Update();

	if (moveCoefficient > 1.0f)
	{
		//ダッシュしている場合はフラグを立てる
		dashFlag_ = true;
	}

	//前進
	if (controller_->CheckLongInputKey(KeyID::Up))
	{
		pos_.x += movePowerX(cameraAngle, Deg2RadF(0.0f), moveCoefficient, diagonalFlag);
		pos_.z += movePowerZ(cameraAngle, Deg2RadF(0.0f), moveCoefficient, diagonalFlag);
	}
	//後退
	if (controller_->CheckLongInputKey(KeyID::Down))
	{
		//後退時は移動量を減少させる(移動量0.6倍)
		moveCoefficient *= 0.6f;
		pos_.x += movePowerX(cameraAngle, Deg2RadF(180.0f), moveCoefficient, diagonalFlag);
		pos_.z += movePowerZ(cameraAngle, Deg2RadF(180.0f), moveCoefficient, diagonalFlag);
	}
	//右折
	if (controller_->CheckLongInputKey(KeyID::Right))
	{
		pos_.x += movePowerX(cameraAngle, Deg2RadF(90.0f), moveCoefficient, diagonalFlag);
		pos_.z += movePowerZ(cameraAngle, Deg2RadF(90.0f), moveCoefficient, diagonalFlag);
	}
	//左折
	if (controller_->CheckLongInputKey(KeyID::Left))
	{
		pos_.x += movePowerX(cameraAngle, Deg2RadF(-90.0f), moveCoefficient, diagonalFlag);
		pos_.z += movePowerZ(cameraAngle, Deg2RadF(-90.0f), moveCoefficient, diagonalFlag);
	}

	// 向きを計算
	
	PlayerRot_.x = sin(cameraAngle.y);
	PlayerRot_.y = 0.0f;
	PlayerRot_.z = cos(cameraAngle.y);

	// リスナーの位置を向きを設定
	Set3DSoundListenerPosAndFrontPos_UpVecY(pos_.toVECTOR(), VAdd(pos_.toVECTOR(), PlayerRot_));
}

bool Player::CheckDiagonalMove(void)
{
	//斜め移動している場合はtrueを返す
	if (controller_->CheckLongInputKey(KeyID::Up)||
		controller_->CheckLongInputKey(KeyID::Down))
	{
		if (controller_->CheckLongInputKey(KeyID::Right) ||
			controller_->CheckLongInputKey(KeyID::Left))
		{
			return true;
		}
	}
	return false;
}

void Player::CollisionStage(void)
{
	bool hitFlag = false;

	capsule_.topPos = VGet(pos_.x, pos_.y + CAPSULE_RELATIVE_TOP_Y, pos_.z);
	capsule_.downPos = VGet(pos_.x, pos_.y + CAPSULE_RELATIVE_DOWN_Y, pos_.z);
	auto hits = MV1CollCheck_Capsule(collisionHandle_, -1, capsule_.topPos, capsule_.downPos, capsule_.radius);

	if (hits.HitNum != 0)
	{
		for (int i = 0; i < hits.HitNum; i++)
		{
			auto hit = hits.Dim[i];
			hitFlag = true;

			//壁のみ処理を行う
			if (!(-0.0001f < hit.Normal.y) || !(hit.Normal.y < 0.0001f))
			{
				continue;
			}

			//押し出し処理
			for (int j = 0; j < WALL_EXTRUSION_NUM; j++)
			{
				int tHit = HitCheck_Capsule_Triangle(
					capsule_.topPos,
					capsule_.downPos,
					capsule_.radius,
					hit.Position[0],
					hit.Position[1],
					hit.Position[2]
				);

				if (tHit)
				{
					pos_ += ConvertVECTORtoVector3(VScale(hit.Normal, SLIDE_DISTANCE));
					capsule_.topPos = (VGet(pos_.x, pos_.y + CAPSULE_RELATIVE_TOP_Y, pos_.z));
					capsule_.downPos = (VGet(pos_.x, pos_.y + CAPSULE_RELATIVE_DOWN_Y, pos_.z));
					continue;
				}
				break;
			}
		}
	}

	//後始末
	MV1CollResultPolyDimTerminate(hits);

	//足元判定
	SlideLinePosition();
}

void Player::CollisionGimmick(void)
{
	bool hitFlag = false;
	std::string name;
	for (auto& gimmick : gimmickList_)
	{
		name = gimmick->GetName();
		if (name == "Gimmick_Wall")
		{
			bool flg = gimmick->GetIsLockFlg();
			if (!flg)
			{
				break;
			}
		}
		capsule_.topPos = VGet(pos_.x, pos_.y + CAPSULE_RELATIVE_TOP_Y, pos_.z);
		capsule_.downPos = VGet(pos_.x, pos_.y + CAPSULE_RELATIVE_DOWN_Y, pos_.z);

		auto hits = MV1CollCheck_Capsule(lpModelMng.GetID(gimmick->GetModelString())[gimmick->GetModelNumber()], -1, capsule_.topPos, capsule_.downPos, capsule_.radius);
		if (hits.HitNum != 0)
		{
			for (int i = 0; i < hits.HitNum; i++)
			{
				auto hit = hits.Dim[i];
				hitFlag = true;
				if (!(-0.0001f < hit.Normal.y) || !(hit.Normal.y < 0.0001f))
				{
					continue;
				}

				//押し出し処理
				for (int j = 0; j < WALL_EXTRUSION_NUM; j++)
				{
					int tHit = HitCheck_Capsule_Triangle(
						capsule_.topPos,
						capsule_.downPos,
						capsule_.radius, 
						hit.Position[0],
						hit.Position[1],
						hit.Position[2]
					);

					if (tHit)
					{
						pos_ += ConvertVECTORtoVector3(VScale(hit.Normal, SLIDE_DISTANCE));
						capsule_.topPos = (VGet(pos_.x, pos_.y + CAPSULE_RELATIVE_TOP_Y, pos_.z));
						capsule_.downPos = (VGet(pos_.x, pos_.y + CAPSULE_RELATIVE_DOWN_Y, pos_.z));
						continue;
					}
					break;
				}
			}
		}
		MV1CollResultPolyDimTerminate(hits);
	}
}

//ゴールとカプセル
void Player::HitCapsuleAndGoal()
{
	isHitGoal_ = false;
	auto hits = MV1CollCheck_Capsule(
		lpModelMng.GetID("resource/model/Goal.mv1")[0],
		-1,
		capsule_.topPos,
		capsule_.downPos,
		capsule_.radius);

	if (hits.HitNum > 0)
	{
		for (auto& inventory : inventoryList_)
		{
			if (inventory.itemID_ == ItemID::Key1)
			{
				isGameClear_ = true;
			}
		}
		isHitGoal_ = true;
	}
	//衝突判定用の動的構造体のメモリー解放
	MV1CollResultPolyDimTerminate(hits);
}

void Player::AddInventory(Vector3 targetPos)
{
	for (auto& item : itemList_)
	{
		if (item->GetPickUpFlg())
		{
			continue;
		}
		Vector3 from = Vector3(pos_.x, pos_.y + CAMERA_HEIGHT, pos_.z);
		Vector3 player2TargetPosVector = (targetPos - from).Normalized();
		Vector3 to = from + player2TargetPosVector * INTERACT_LINE_LENGTH;
		auto itemcoll = MV1CollCheck_Line(lpModelMng.GetID(item->GetModelString())[item->GetModelNumber()], -1, from.toVECTOR(), to.toVECTOR());
		if (itemcoll.HitFlag == 1)
		{
			pickUpFlag_ = true;
		}
		if (pickUpFlag_)
		{
			if (controller_->CheckInputKey(KeyID::Interact))
			{
				bool flag = false;
				for (auto& inventory : inventoryList_)
				{
					if (inventory.itemID_ == item->GetItemID())
					{
						flag = true;
					}
				}
				if (!flag)
				{
					item->SetPickUpFlg(true);
					VECTOR dd = MV1GetScale(lpModelMng.GetID(item->GetModelString())[item->GetModelNumber()]);
					inventoryList_.emplace_back(
						item->GetModelString(),
						item->GetModelNumber(),
						item->GetItemID(),
						Vector3{ 0,0,0 },
						Vector3{ dd.x,dd.y,dd.z },
						""
					);
					break;
				}
			}
		}
		lpMouseIcon.SetFlg(pickUpFlag_);
	}

	for (auto& item : itemList_)
	{
		if (item->GetItemID() == lpSocketServer.GetItemID())
		{
			bool flag = false;
			for (auto& inventory : inventoryList_)
			{
				if (inventory.itemID_ == item->GetItemID())
				{
					flag = true;
				}
			}
			if (!flag)
			{
				item->SetPickUpFlg(true);
				VECTOR dd = MV1GetScale(lpModelMng.GetID(item->GetModelString())[item->GetModelNumber()]);
				inventoryList_.emplace_back(item->GetModelString(),
					item->GetModelNumber(),
					item->GetItemID(),
					Vector3{ 0,0,0 },
					Vector3{ dd.x,dd.y,dd.z },
					""
				);
				break;
			}
		}
	}
}

void Player::InteractGimmick(Vector3 targetPos)
{
	bool catchFlg = false;
	char numchar='0';
	int modelNum = 0;
	for (auto& gimmick : gimmickList_)
	{
		std::string name = gimmick->GetName();
		modelNum = gimmick->GetModelNumber();
		//特定のオブジェクトの視線の当たり判定を無視
		if (name == "drawer_frame_L" || name == "Dial_Frame" || name == "Gimmick_Wall")
		{
			continue;
		}
		Vector3 from = Vector3(pos_.x, pos_.y + CAMERA_HEIGHT, pos_.z);
		Vector3 player2TargetPosVector = (targetPos - from).Normalized();
		Vector3 to = from + player2TargetPosVector * INTERACT_LINE_LENGTH;
		auto coll = MV1CollCheck_Line(lpModelMng.GetID(gimmick->GetModelString())[modelNum], -1, from.toVECTOR(), to.toVECTOR());
		if (coll.HitFlag == 1)
		{
			//視線の距離と当たっている
			catchFlg = true;
		}
		
		//視線の距離と当たっていて
		if (catchFlg)
		{
			if (name == "Dial")
			{
				//ダイヤルカウント処理
				DialCountUpdate(gimmick, modelNum, numchar);
			}
			//法線を設定
			gimmick->SetHitNormal(coll.Normal);
			//インタラクト処理
			if (controller_->CheckInputKey(KeyID::Interact))
			{
				for (auto& inventory : inventoryList_)
				{
					//インベントリ内に特定のアイテムがあったら
					if (inventory.itemID_ == gimmick->GetItemID())
					{
						//ロックを解除
						gimmick->SetIsLockFlg(false);
						break;
					}
				}
				//ギミックを開閉
				gimmick->SetInteractFlg(true);
				//ダイヤル番号が解除番号と一致していたら
				if (dialNumStr_ == gimmick->GetUnlockNumber())
				{
					if (name == "Red_B")
					{
						//ロックを解除
						gimmick->SetIsLockFlg(false);
					}
				}
				break;
			}
		}
		//マウスアイコンの変化フラグを設定
		lpMouseIcon.SetFlg(catchFlg);
	}
}

void Player::GimmickEvent(Vector3 targetPos)
{
	bool catchFlg = false;
	char numchar = '0';
	int modelNum = 0;
	for (auto& gimmick : gimmickList_)
	{
		std::string name = gimmick->GetName();
		modelNum = gimmick->GetModelNumber();
		if (name != "Gimmick_Wall")
		{
			continue;
		}
		//特定のオブジェクトの視線の当たり判定を無視
		Vector3 from = Vector3(pos_.x, pos_.y + CAMERA_HEIGHT, pos_.z);
		Vector3 player2TargetPosVector = (targetPos - from).Normalized();
		Vector3 to = from + player2TargetPosVector * WALL_INTERACT_LINE_LENGTH;
		auto coll = MV1CollCheck_Line(lpModelMng.GetID(gimmick->GetModelString())[modelNum], -1, from.toVECTOR(), to.toVECTOR());
		if (coll.HitFlag == 1)
		{
			//視線の距離と当たっている
			catchFlg = true;
		}

		//視線の距離と当たっていて
		if (catchFlg)
		{
			bool fFlg = minCamera_->GetFlashFlag();
			if (fFlg)
			{
				for (auto& inventory : inventoryList_)
				{
					//インベントリ内に特定のアイテムがあったら
					if (inventory.itemID_ == gimmick->GetItemID())
					{
						//ロックを解除
						gimmick->SetIsLockFlg(false);
						break;
					}
				}
			}
		}
	}
}

void Player::DialCountUpdate(std::shared_ptr<Gimmick>& gimmick, int modelNum, char numchar)
{
	//モデルが増えた場合
	if (modelNum >= MAX_DIAL_NUM)
	{
		//配列の要素数を超えないように余りを出す
		modelNum %= MAX_DIAL_NUM;
	}
	//カウントアップ
	dial_[modelNum] = gimmick->GetDialNumber();
	//int->char変換(ASCIIからintに変換するため0を追加)
	numchar = static_cast<char>(dial_[modelNum] + '0');
	//更新する数字を削除
	dialNumStr_.erase(dialNumStr_.begin() + modelNum);
	//削除した場所に更新した数字を挿入
	dialNumStr_.insert(dialNumStr_.begin() + modelNum
		, numchar);
}

void Player::SlideLinePosition(void)
{
	VECTOR dis{};

	auto fallHit = MV1CollCheck_Line(collisionHandle_, -1, VGet(pos_.x, pos_.y, pos_.z), VGet(pos_.x, pos_.y + COLLISION_LINE_LENGTH, pos_.z));
	if (fallHit.HitFlag)
	{
		dis = VSub(fallHit.HitPosition, pos_.toVECTOR());
		pos_.y += dis.y;
	}
	else
	{
		if (state_ != State::Fall)
		{
			state_ = State::Fall;
			fallGravity_ = 0.0f;
		}
	}

	if (state_ == State::Fall)
	{
		pos_.y -= (FALL_SPEED + fallGravity_);
		fallGravity_ += FALL_GRAVITY;
		fallHit = MV1CollCheck_Line(collisionHandle_, -1, VGet(pos_.x, pos_.y, pos_.z), VGet(pos_.x, pos_.y + COLLISION_LINE_LENGTH, pos_.z));
		if (fallHit.HitFlag)
		{
			dis = VSub(fallHit.HitPosition, pos_.toVECTOR());
			pos_.y += dis.y;
			state_ = State::Walk;
		}
	}
	capsule_.topPos = (VGet(pos_.x, pos_.y + CAPSULE_RELATIVE_TOP_Y, pos_.z));
	capsule_.downPos = (VGet(pos_.x, pos_.y + CAPSULE_RELATIVE_DOWN_Y, pos_.z));
}
