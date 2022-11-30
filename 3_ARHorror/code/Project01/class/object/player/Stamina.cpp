#include "Stamina.h"
#include "../../scene/SceneManager.h"
#include "../../input/PadInput.h"
#include "../../input/KeyInput.h"
#include "../../../_debug/_DebugDispOut.h"

constexpr float DASH_TIME_LIMIT = 9.0f;	//�_�b�V���\�Ȏ���(��7�b)
constexpr float DASH_ADD_TIME = 5.0f;		//�_�b�V�����Ԃ��߂����ꍇ�ɒǉ�����b��
constexpr float DASH_COEFFIECIENT = 2.75f;	//�_�b�V�������ꍇ�̑��x�{��(2.5???)

//�X�^�~�i�Q�[�W�֌W
constexpr int STAMINA_GAUGE_HEIGHT = 12;	//�Q�[�W�̍���
constexpr int STAMINA_GAUGE_WIDTH = 300;	//�Q�[�W�̕�

constexpr int STAMINA_GAUGE_COLOR_GREEN  = 0x44ff44;	//�X�^�~�i���������̐F(��)
constexpr int STAMINA_GAUGE_COLOR_YELLOW = 0xffaa22;	//�X�^�~�i�������Ă������̐F(��)
constexpr int STAMINA_GAUGE_COLOR_RED    = 0xff4444;	//�X�^�~�i���قƂ�ǂȂ����̐F(��)

constexpr int STAMINA_GAUGE_OFFSET_Y = 100;			//�X�^�~�i�̏c�̃I�t�Z�b�g
constexpr int STAMINA_GAUGE_ALPHA_MAX = 255;		//�X�^�~�i�̃A���t�@�ő�l
constexpr int STAMINA_GAUGE_ALPHA_MINUS = 5;		//�X�^�~�i�̖��t���[���̃}�C�i�X�l
constexpr double STAMINA_GAUGE_TRANS_LIMIT = 0.3;	//�Q�[�W�����߂��n�߂鎞��

Stamina::Stamina(ControllerType type)
{
	//�R���g���[������
	if (type == ControllerType::Pad)
	{
		controller_ = std::make_unique<PadInput>();
	}
	else
	{
		controller_ = std::make_unique<KeyInput>();
	}
	Init();
}

Stamina::~Stamina()
{
}

void Stamina::Init(void)
{
	//�ړ��W��
	moveCoefficient_ = 1.0f;
	//�_�b�V������
	dashTime_ = 0.0;
	//������Ԃɂ���
	gaugeAlpha_ = 0;
	//�����͗ΐF
	gaugeColor = STAMINA_GAUGE_COLOR_GREEN;
}

void Stamina::Release(void)
{
}

float Stamina::Update(void)
{
	controller_->Update();

	moveCoefficient_ = 1.0f;

	//���l����
	CalcStaminaGaugeAlpha(true);

	//�F�ύX����
	ChangeStaminaGaugeColor();

	//�ړ��W���Ǝ��ԏ���
	CalcCoefficientAndTime();

	return moveCoefficient_;
}

void Stamina::Draw(void)
{
	//��J���͂��΂炭�\�����Ȃ�
	int lineWidth = DASH_TIME_LIMIT - dashTime_ >= 0.0f ? static_cast<int>(STAMINA_GAUGE_WIDTH * ((DASH_TIME_LIMIT - dashTime_) / DASH_TIME_LIMIT)) : 0;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, gaugeAlpha_);

	//�X�^�~�i�Q�[�W(�E)
	DrawLine(
		lpSceneMng.GetScreenSize().x / 2,
		lpSceneMng.GetScreenSize().y - STAMINA_GAUGE_OFFSET_Y,
		lpSceneMng.GetScreenSize().x / 2 + lineWidth,
		lpSceneMng.GetScreenSize().y - STAMINA_GAUGE_OFFSET_Y,
		gaugeColor,
		STAMINA_GAUGE_HEIGHT
	);

	//�X�^�~�i�Q�[�W(��)
	DrawLine(
		lpSceneMng.GetScreenSize().x / 2,
		lpSceneMng.GetScreenSize().y - STAMINA_GAUGE_OFFSET_Y,
		lpSceneMng.GetScreenSize().x / 2 - lineWidth,
		lpSceneMng.GetScreenSize().y - STAMINA_GAUGE_OFFSET_Y,
		gaugeColor,
		STAMINA_GAUGE_HEIGHT
	);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, STAMINA_GAUGE_ALPHA_MAX);

	//�f�o�b�O
	//_dbgDrawFormatString(500, 0, 0xffffff, "�_�b�V�����ԁF%.2f", dashTime_);
}

void Stamina::CalcCoefficientAndTime(void)
{
	//��J(fatigue)���Ă��邩�ǂ���
	bool fatigueFlag = DASH_TIME_LIMIT - dashTime_ < 0.0f;

	//��J���Ă���ꍇ�A��莞�ԑ���x������
	if (fatigueFlag)
	{
		moveCoefficient_ *= 0.4f;
	}

	//�X�^�~�i�񕜌W��
	double healSpeed = DASH_TIME_LIMIT - dashTime_ >= 0.0 ? 2.5 : 1.0;

	//�ړ����Ă��Ȃ��ꍇ�̓X�^�~�i�񕜂��ĕԂ�
	if (!controller_->CheckLongInputKey(KeyID::Up) &&
		!controller_->CheckLongInputKey(KeyID::Down) &&
		!controller_->CheckLongInputKey(KeyID::Right) &&
		!controller_->CheckLongInputKey(KeyID::Left)
		)
	{
		//�ړ����Ă��Ȃ��̂ŃX�^�~�i�񕜗ʂQ�{
		dashTime_ -= lpSceneMng.GetDeltaTime() / healSpeed * 2.0;
		if (dashTime_ < 0.0)
		{
			dashTime_ = 0.0;
		}
		return;
	}

	//�_�b�V�������Ă���ꍇ�͌W����傫������
	if (controller_->CheckLongInputKey(KeyID::Dash) && !fatigueFlag)
	{
		dashTime_ += lpSceneMng.GetDeltaTime();		//�_�b�V�����ԃJ�E���g

		//�X�^�~�i��\��������
		CalcStaminaGaugeAlpha(false);

		if (dashTime_ <= DASH_TIME_LIMIT)
		{
			moveCoefficient_ *= DASH_COEFFIECIENT;
		}
		else
		{
			//�X�^�~�i���؂ꂽ�ꍇ�͉񕜂���܂ł̎��Ԃ�����������
			dashTime_ = DASH_TIME_LIMIT + DASH_ADD_TIME;
		}
	}
	else
	{
		//�_�b�V�����Ă��Ȃ��ꍇ�̓X�^�~�i���񕜂�����
		dashTime_ -= lpSceneMng.GetDeltaTime() / healSpeed;	
		if (dashTime_ < 0.0)
		{
			dashTime_ = 0.0;
		}
	}
}

void Stamina::CalcStaminaGaugeAlpha(bool transFlag)
{
	//true�̏ꍇ�͓��߂���
	if (transFlag)
	{
		if (dashTime_ < STAMINA_GAUGE_TRANS_LIMIT)
		{
			//�X�^�~�i��������x�񕜂�����Q�[�W�����X�ɓ��߂�����
			if (gaugeAlpha_ > 0)
			{
				gaugeAlpha_ -= STAMINA_GAUGE_ALPHA_MINUS;
			}
		}
	}
	else
	{
		gaugeAlpha_ = STAMINA_GAUGE_ALPHA_MAX;
	}
}

void Stamina::ChangeStaminaGaugeColor(void)
{
	//�_�b�V���ł���c��̎��Ԃ������ȏ�̏ꍇ�͗΁A������艺�̏ꍇ�͉��F�A�S���̂P��艺�̏ꍇ�͐ԐF�ɂ���
	if (DASH_TIME_LIMIT - dashTime_ >= DASH_TIME_LIMIT / 2.0f)
	{
		gaugeColor = STAMINA_GAUGE_COLOR_GREEN;
	}
	else if(DASH_TIME_LIMIT - dashTime_ >= DASH_TIME_LIMIT / 4.0f)
	{
		gaugeColor = STAMINA_GAUGE_COLOR_YELLOW;
	}
	else
	{
		gaugeColor = STAMINA_GAUGE_COLOR_RED;
	}
}
