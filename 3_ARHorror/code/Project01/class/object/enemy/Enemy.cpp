#include<malloc.h>
#include "Enemy.h"
#include"Dijkstra.h"
#include "../../common/AnimationController.h"
#include "../../Scene/SceneManager.h"
#include "../../common/manager/SoundMng.h"
#include "../../common/manager/ModelMng.h"
#include"../../common/Capsule.h"

//�G�̑傫��
constexpr float SIZE_SCALE = 5.0f;

//�v���C���[�T���p�萔
constexpr float SEARCH_ANGLE	= 20.0f;	//���G�p�x
constexpr float SEARCH_RANGE	= 8000.0f;	//���G�͈�
constexpr float SEARCH_OFFSET_Y = 300.0f;	//����̃I�t�Z�b�g����

//�v���C���[�ǐ՗p�萔
constexpr double HUNT_LIMIT_TIME = 3.0;	//�͈͊O�ɂ���Ƃ��̒ǐՂ��鎞��

//��]����
double ROTATION_TIME = 1.0f;

//�v���C���[�̃I�t�Z�b�g
constexpr float PLAYER_OFFSET_Y = 76.0f;		//����
constexpr float PLAYER_CAPSULE_OFFSET_RADIUS = 500.0f;	//�J�v�Z���̃I�t�Z�b�g���a

//�X�^���֌W
constexpr double STAN_MAX_LIMIT_TIME  = 7.6;	//�ő厞��
constexpr double STAN_MIN_LIMIT_TIME  = 0.5;	//�ŏ�����
constexpr double STAN_RESUSTANCE_TIME = 1.52;	//�X�^�����邽�тɈ�����鎞��
constexpr double STAN_HEAL_IME		  = 0.5;	//���₷����

Enemy::Enemy()
{
	Init();
}

Enemy::~Enemy()
{
}

void Enemy::Init(void)
{
	Vector3 vectorZero = { 0.0f,0.0f,0.0f };
	//�g�����X�t�H�[��
	transform_.Init("./resource/model/enemy/ghost3.mv1", { 25402,0,-1185 }, {SIZE_SCALE, SIZE_SCALE, SIZE_SCALE}, vectorZero, vectorZero);

	animController_ = std::make_unique<AnimationController>(transform_.modelHandle_);
	animController_->Add((int)EnemyAnimation::Idle, "./resource/model/enemy/anim/Idle.mv1", 20.0f);
	animController_->Add((int)EnemyAnimation::Run, "./resource/model/enemy/anim/Run.mv1", 60.0f);
	animController_->Add((int)EnemyAnimation::FemaleDynamicPose, "./resource/model/enemy/anim/Female Dynamic Pose.mv1", 20.0f);
	animController_->Add((int)EnemyAnimation::Damage, "./resource/model/enemy/anim/Damage.mv1", 20.0f);
	animController_->Add((int)EnemyAnimation::Walk, "./resource/model/enemy/anim/Walk.mv1", 45.0f);
	animController_->Play((int)EnemyAnimation::Walk, true);

	lpSoundMng.Play3DSound("./resource/sound/WalkVoice.mp3", transform_.pos_, 10000.0f);	
	MV1SetupCollInfo(transform_.modelHandle_, -1, 1, 1, 1);
	
	transform_.quaRotLocal_ = Quaternion(VGet(0.0f, Deg2RadF(180.0f), 0.0f));
	transform_.quaRot_ = Quaternion(VGet(0.0f, Deg2RadF(180.0f), 0.0f));
	transform_.Update();

	huntFlag_ = false;
	stepHuntTime_ = 0.0;

	targetRot_ = Quaternion();
	stepRotTime_ = 0.0f;

	dijkstra_ = std::make_unique<Dijkstra>();
	playerCapsule_ = Capsule();
	hitPlayerFlag_ = false;
	stanFlag_ = false;
	nowStanTime_ = STAN_MAX_LIMIT_TIME;
	stepTime_ = 0.0;
}

void Enemy::Release(void)
{
	MV1DeleteModel(transform_.modelHandle_);
}

void Enemy::Update(void)
{
	animController_->Update(lpSceneMng.GetDeltaTime());
	//���̍��W�ݒ�
	Set3DPositionSoundMem(transform_.pos_.toVECTOR(), lpSoundMng.GetID("./resource/sound/WalkVoice.mp3")[0]);

	stepTime_ += lpSceneMng.GetDeltaTime();
}

void Enemy::Update(Capsule capsule, const Vector3 playerPos,bool hitFlash)
{
	playerCapsule_ = capsule;
	Update();

	//�X�^�����͈ړ���ǐՓ������Ȃ�
	if (stanFlag_)
	{
		RecoverFlash();
		return;
	}
	else
	{
		DamageFlash(hitFlash);
	}

	//�ǐՒ����ǂ���
	if (!huntFlag_)
	{
		//�v���C���[��T��
		SearchPlayer(playerPos);
	}
	else
	{
		//�v���C���[��ǐ�
		targetPos_ = playerPos;
		HuntPlayer();
	}

	//�X�^�����Ԃ𒷂�����
	RecoverStanTime();

	//�ړ�����(�_�C�N�X�g���@)
	DijkstraProcess(playerPos);

	transform_.Update();

	hitPlayerFlag_ = CheckHitPlayer();

}

void Enemy::Draw(void)
{
	transform_.Draw();

	//VECTOR ePos = transform_.pos_.toVECTOR();
	//ePos.y += SEARCH_OFFSET_Y;

	//auto angle = transform_.rot_;

	//float searchRad = Deg2RadF(SEARCH_ANGLE);

	//float sinX1 = sinf(angle.y);
	//float sinZ1 = cosf(angle.y);

	//float sinX2 = sinf(angle.y - searchRad);
	//float sinZ2 = cosf(angle.y - searchRad);

	//float sinX3 = sinf(angle.y + searchRad);
	//float sinZ3 = cosf(angle.y + searchRad);

	//VECTOR pos1 = transform_.pos_.toVECTOR();
	//VECTOR pos2 = transform_.pos_.toVECTOR();
	//VECTOR pos3 = transform_.pos_.toVECTOR();

	//pos1.x += sinX1 * SEARCH_RANGE;
	//pos1.y += SEARCH_OFFSET_Y;
	//pos1.z += sinZ1 * SEARCH_RANGE;

	//pos2.x += sinX2 * SEARCH_RANGE;
	//pos2.y += SEARCH_OFFSET_Y;
	//pos2.z += sinZ2 * SEARCH_RANGE;

	//pos3.x += sinX3 * SEARCH_RANGE;
	//pos3.y += SEARCH_OFFSET_Y;
	//pos3.z += sinZ3 * SEARCH_RANGE;

	//MV1_COLL_RESULT_POLY lineHit{};

	////�ڂ̑O�̐�
	//lineHit = MV1CollCheck_Line(lpModelMng.GetID("resource/model/stage/Coll3.mv1")[0], -1, ePos, pos1);
	//if (lineHit.HitFlag)
	//{
	//	pos1 = lineHit.HitPosition;
	//}

	////���̐�
	//lineHit = MV1CollCheck_Line(lpModelMng.GetID("resource/model/stage/Coll3.mv1")[0], -1, ePos, pos2);
	//if (lineHit.HitFlag)
	//{
	//	pos2 = lineHit.HitPosition;
	//}

	////�ڂ̑O�̐�
	//lineHit = MV1CollCheck_Line(lpModelMng.GetID("resource/model/stage/Coll3.mv1")[0], -1, ePos, pos3);
	//if (lineHit.HitFlag)
	//{
	//	pos3 = lineHit.HitPosition;
	//}

	//DrawLine3D(ePos, pos1, 0xffff00);
	//DrawLine3D(ePos, pos2, 0xffff00);
	//DrawLine3D(ePos, pos3, 0xffff00);

	//DrawTriangle3D(ePos, pos2, pos1, 0x888800,true);	//��
	//DrawTriangle3D(ePos, pos1, pos3, 0x888800, true);	//�E

	////DrawFormatString(0, 300, 0xffffff, "�G�̍��W : (%.1f, %.1f, %.1f)", transform_.pos_.x, transform_.pos_.y, transform_.pos_.z);

	//dijkstra_->Draw();
}

Vector3 Enemy::GetPosition(void)
{
	return transform_.pos_;
}

bool Enemy::GetHitPlayerFlag(void)
{
	return hitPlayerFlag_;
}

void Enemy::SearchPlayer(const Vector3 playerPos)
{
	if (Search(playerPos))
	{
		huntFlag_ = true;
		dijkstra_->AddHuntNode({ playerPos.x,playerPos.y - PLAYER_OFFSET_Y,playerPos.z }, transform_.pos_);
		animController_->Play(EnemyAnimation::Run, true);
	}
}

bool Enemy::Search(const Vector3 playerPos)
{
	bool hitFlag = false;
	VECTOR ePos = transform_.pos_.toVECTOR();
	ePos.y += SEARCH_OFFSET_Y;

	auto angle = transform_.rot_;

	float searchRad = Deg2RadF(SEARCH_ANGLE);

	VECTOR pos1 = transform_.pos_.toVECTOR();
	VECTOR pos2 = transform_.pos_.toVECTOR();
	VECTOR pos3 = transform_.pos_.toVECTOR();

	pos1.x += sinf(angle.y) * SEARCH_RANGE;
	pos1.y += SEARCH_OFFSET_Y;
	pos1.z += cosf(angle.y) * SEARCH_RANGE;

	pos2.x += sinf(angle.y - searchRad) * SEARCH_RANGE;
	pos2.y += SEARCH_OFFSET_Y;
	pos2.z += cosf(angle.y - searchRad) * SEARCH_RANGE;

	pos3.x += sinf(angle.y + searchRad) * SEARCH_RANGE;
	pos3.y += SEARCH_OFFSET_Y;
	pos3.z += cosf(angle.y + searchRad) * SEARCH_RANGE;

	MV1_COLL_RESULT_POLY lineHit{};

	//�ڂ̑O�̐�
	lineHit = MV1CollCheck_Line(lpModelMng.GetID("resource/model/stage/Coll3.mv1")[0], -1, ePos, pos1);
	if (lineHit.HitFlag)
	{
		pos1 = lineHit.HitPosition;
	}

	//���̐�
	lineHit = MV1CollCheck_Line(lpModelMng.GetID("resource/model/stage/Coll3.mv1")[0], -1, ePos, pos2);
	if (lineHit.HitFlag)
	{
		pos2 = lineHit.HitPosition;
	}

	//�E�̐�
	lineHit = MV1CollCheck_Line(lpModelMng.GetID("resource/model/stage/Coll3.mv1")[0], -1, ePos, pos3);
	if (lineHit.HitFlag)
	{
		pos3 = lineHit.HitPosition;
	}

	int hit = 0;

	//�G�̎��E�ɏՓ˂��Ă��邩�ǂ�������
	hit = HitCheck_Capsule_Triangle(
		playerCapsule_.topPos,
		playerCapsule_.downPos,
		playerCapsule_.radius,
		ePos,
		pos2,
		pos1);

	if (hit)
	{
		return true;
	}

	hit = HitCheck_Capsule_Triangle(
		playerCapsule_.topPos,
		playerCapsule_.downPos,
		playerCapsule_.radius,
		ePos,
		pos1,
		pos3);

	if (hit)
	{
		return true;
	}

	return false;
}

void Enemy::HuntPlayer(void)
{
	if (stepHuntTime_ < HUNT_LIMIT_TIME)
	{
		if (!Search(targetPos_))
		{
			stepHuntTime_ += lpSceneMng.GetDeltaTime();
		}
		else
		{
			stepHuntTime_ = 0.0;
		}
	}
	else
	{
		if (!Search(targetPos_))
		{
			huntFlag_ = false;
			animController_->Play(EnemyAnimation::Walk, true);
			dijkstra_->ConnectNearNode(transform_.pos_);
		}
		stepHuntTime_ = 0.0;
	}
}

void Enemy::DijkstraProcess(Vector3 playerPos)
{
	//�ړ�����
	Vector3 pos = dijkstra_->Update(huntFlag_, { playerPos.x,playerPos.y - PLAYER_OFFSET_Y,playerPos.z });
	Vector3 moveVec = (pos - transform_.pos_).Normalized();

	//Y����]����
	Quaternion rotY = AngleAxis(atan2(moveVec.x, moveVec.z), { 0.0f, 1.0f, 0.0f });

	//�p�x�������߂�
	double angleDeg = AngleDifference(rotY, targetRot_);

	//臒l����
	if (angleDeg >= 1.0)
	{
		stepRotTime_ = ROTATION_TIME;
		targetRot_ = rotY;
	}

	stepRotTime_ -= lpSceneMng.GetDeltaTime();

	transform_.quaRot_ = transform_.quaRot_.Slerp(targetRot_, (ROTATION_TIME - stepRotTime_) / ROTATION_TIME);
	transform_.pos_ = pos;
}

bool Enemy::CheckHitPlayer(void)
{
	//�G�ƃv���C���[���Փ˂����ꍇ�A�Q�[���I�[�o�[��
	auto hits = MV1CollCheck_Capsule(transform_.modelHandle_, -1, playerCapsule_.topPos, playerCapsule_.downPos, playerCapsule_.radius + PLAYER_CAPSULE_OFFSET_RADIUS);
	bool hitFlag = false;

	if (hits.HitNum > 0)
	{
		hitFlag = true;
	}
	MV1CollResultPolyDimTerminate(hits);
	return hitFlag;
}

void Enemy::DamageFlash(bool hitFlash)
{
	if (hitFlash)
	{
		stepTime_ = 0.0;
		stanFlag_ = true;
		animController_->Play(EnemyAnimation::Damage, false);
	}
}

void Enemy::RecoverFlash(void)
{
	if (stepTime_ > nowStanTime_)
	{
		huntFlag_ = false;
		stanFlag_ = false;
		animController_->Play(EnemyAnimation::Walk, true);

		//�X�^�����ԒZ�k
		ReduceStanTime();
	}
}

void Enemy::RecoverStanTime(void)
{
	nowStanTime_ += lpSceneMng.GetDeltaTime() / 6.0;
	if (nowStanTime_ > STAN_MAX_LIMIT_TIME)
	{
		nowStanTime_ = STAN_MAX_LIMIT_TIME;
	}
}

void Enemy::ReduceStanTime(void)
{
	nowStanTime_ -= STAN_RESUSTANCE_TIME;

	//�Œ�X�^�����Ԃ�0.5�b
	if (nowStanTime_ < STAN_MIN_LIMIT_TIME)
	{
		nowStanTime_ = STAN_MIN_LIMIT_TIME;
	}
}