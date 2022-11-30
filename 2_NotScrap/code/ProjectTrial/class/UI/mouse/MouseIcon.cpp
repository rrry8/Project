#include<array>
#include "MouseIcon.h"
#include"../../Scene/SceneManager.h"
#include "../../Common/ImageMng.h"

constexpr float ROD_MAX_LENGTH = 512.0f;	//�_�̍ő�̒���
constexpr int LOWER_LIMIT = 200;			//�X���̍Œᐔ�l�͂Q�O�O
constexpr float x = 960.0f;
constexpr float y = 670.0f;


MouseIcon::MouseIcon()
{
	Init();
}

MouseIcon::~MouseIcon()
{
	Release();
}

void MouseIcon::Init(void)
{
	colorChangeFlag_ = false;
	//�A�C�R���̃T�C�Y�擾
	int gx, gy;
	GetGraphSize(lpImageMng.GetID("./source/image/mouseIconWhite.png")[0], &gx, &gy);

	graphSize_.x = static_cast<float>(gx);
	graphSize_.y = static_cast<float>(gy);

	//�ύX���Ȃ����̂�������
	for (auto& v : verts_)
	{
		v.dif = GetColorU8(255, 255, 255, 255);
		v.spc = GetColorU8(255, 255, 255, 255);
		v.pos.z = 0.0f;
		v.rhw = 1.0f;
		v.su = 0.0f;
		v.sv = 0.0f;
	}


	pos_.x = lpSceneMng.GetScreenSize().x / 2.0f;
	pos_.y = lpSceneMng.GetScreenSize().y / 2.0f;
	//���_�X�V
	verts_[0].pos.x = pos_.x;
	verts_[0].pos.y = pos_.y;

	verts_[1].pos.x = pos_.x + graphSize_.x;
	verts_[1].pos.y = pos_.y;

	verts_[2].pos.x = pos_.x;
	verts_[2].pos.y = pos_.y + graphSize_.y;

	verts_[3].pos.x = pos_.x + graphSize_.x;
	verts_[3].pos.y = pos_.y + graphSize_.y;

	//UV���W
	verts_[0].u = 0.0f;
	verts_[0].v = 0.0f;

	verts_[1].u = 1.0f;
	verts_[1].v = 0.0f;

	verts_[2].u = 0.0f;
	verts_[2].v = 1.0f;

	verts_[3].u = 1.0f;
	verts_[3].v = 1.0f;


	//�V�F�[�_������
	shaderHandle_ = LoadPixelShader("./source/shader/PS_MouseIcon.pso");
	cbuff_ = CreateShaderConstantBuffer(sizeof(float) * 8);
	writer_ = static_cast<float*>(GetBufferShaderConstantBuffer(cbuff_));

	//��]���
	writer_[0] = 0.0f;
	//���C�g�J���[
	writer_[1] = 1.0f;
	writer_[2] = 1.0f;
	writer_[3] = 1.0f;
	writer_[4] = 0.0f;
}

void MouseIcon::Update(void)
{
	if (GetJoypadNum())
	{
		//PAD�ڑ���
		DINPUT_JOYSTATE rStick_Input;
		//�E�X�e�B�b�N�̓��͏�Ԃ��擾
		GetJoypadDirectInputState(DX_INPUT_PAD1, &rStick_Input);
		
		//�Q�[���V�[���̏ꍇ��������
		if (lpSceneMng.GetSceneID() == Scene::Game)
		{
			if ((rStick_Input.Rx != 0 || rStick_Input.Ry != 0) && 
				(abs(rStick_Input.Rx) >= LOWER_LIMIT || abs(rStick_Input.Ry) >= LOWER_LIMIT))
			{
				Vector2 vec = Vector2{ static_cast<float>(rStick_Input.Rx),static_cast<float>(rStick_Input.Ry) };
				vec.Normalize();
				Vector2 plus = vec * ROD_MAX_LENGTH;
				//���_�X�V
				verts_[0].pos.x = x + plus.x;
				verts_[0].pos.y = y + plus.y;

				verts_[1].pos.x = x + plus.x + graphSize_.x;
				verts_[1].pos.y = y + plus.y;

				verts_[2].pos.x = x + plus.x;
				verts_[2].pos.y = y + plus.y + graphSize_.y;

				verts_[3].pos.x = x + plus.x + graphSize_.x;
				verts_[3].pos.y = y + plus.y + graphSize_.y;
			}
		}
		else
		{
			//�ړ��ʌv�Z(���̂܂܎g���Ɗ��x�������̂Ŋ���)
			float moveX = rStick_Input.Rx / 70.0f;
			float moveY = rStick_Input.Ry / 70.0f;

			//�ʒu�X�V
			for (int v = 0; v < verts_.size(); v++)
			{
				verts_[v].pos.x += moveX;
				verts_[v].pos.y += moveY;
			}
			CorrectScreen();
		}

	}
	else
	{
		int mx, my;
		//PAD���ڑ�
		//�}�E�X�̈ʒu�X�V
		GetMousePoint(&mx, &my);

		pos_.x = static_cast<float>(mx);
		pos_.y = static_cast<float>(my);

		//���_�X�V
		verts_[0].pos.x = pos_.x;
		verts_[0].pos.y = pos_.y;

		verts_[1].pos.x = pos_.x + graphSize_.x;
		verts_[1].pos.y = pos_.y;

		verts_[2].pos.x = pos_.x;
		verts_[2].pos.y = pos_.y + graphSize_.y;

		verts_[3].pos.x = pos_.x + graphSize_.x;
		verts_[3].pos.y = pos_.y + graphSize_.y;
	}

	ChangeColor();

	SetShaderConstantBuffer(cbuff_, DX_SHADERTYPE_PIXEL, 5);
	writer_[0] += 0.04f;
	UpdateShaderConstantBuffer(cbuff_);

}

void MouseIcon::Draw(void)
{
	if (lpSceneMng.GetSceneID() == Scene::Game)
	{
		if (GetJoypadNum())
		{
			return;
		}
	}
	DrawMouseIcon(shaderHandle_, lpImageMng.GetID("./source/image/mouseIconWhite.png")[0], lpImageMng.GetID("./source/image/mouseIconNormal.png")[0]);
}

void MouseIcon::Release(void)
{
	DeleteShader(shaderHandle_);
	DeleteShaderConstantBuffer(cbuff_);
}

void MouseIcon::DrawMouseIcon(int shader,int icon,int norm)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	SetUsePixelShader(shader);
	SetUseTextureToShader(0, icon);
	SetUseTextureToShader(1, norm);
	DrawPrimitive2DToShader(verts_.data(), verts_.size(), DX_PRIMTYPE_TRIANGLESTRIP);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

void MouseIcon::ChangeColor(void)
{
	//�}�E�X�̓��͏��ŐF�ύX
	auto cntData_ = lpCntMng.GetController()->GetCntData();
	if (cntData_[InputID::BtnN][static_cast<int>(Trg::Now)])
	{
		writer_[1] = 1.0f;
		writer_[2] = 0.3f;
		writer_[3] = 0.3f;
	}
	else if (cntData_[InputID::BtnS][static_cast<int>(Trg::Now)])
	{
		writer_[1] = 0.3f;
		writer_[2] = 0.3f;
		writer_[3] = 1.0f;
	}
	else
	{
		writer_[1] = 1.0f;
		writer_[2] = 1.0f;
		writer_[3] = 1.0f;
		writer_[4] = 0.0f;

	}
	writer_[4] += 0.1f;
	if (writer_[4] > 1.0f)
	{
		writer_[4] = 1.0f;
	}
}

void MouseIcon::CorrectScreen(void)
{
	Vector2 screenSize = { static_cast<float>(lpSceneMng.GetScreenSize().x),static_cast<float>(lpSceneMng.GetScreenSize().y) };

	if (verts_[0].pos.x < 0)
	{
		verts_[0].pos.x = 0.0f;
		verts_[1].pos.x = graphSize_.x;
		verts_[2].pos.x = 0.0f;
		verts_[3].pos.x = graphSize_.x;
	}

	if (verts_[0].pos.y < 0)
	{
		verts_[0].pos.y = 0.0f;
		verts_[1].pos.y = 0.0f;
		verts_[2].pos.y = graphSize_.y;
		verts_[3].pos.y = graphSize_.y;
	}

	if (verts_[3].pos.x > screenSize.x)
	{
		verts_[0].pos.x = screenSize.x - graphSize_.x;
		verts_[1].pos.x = screenSize.x;
		verts_[2].pos.x = screenSize.x - graphSize_.x;
		verts_[3].pos.x = screenSize.x;
	}

	if (verts_[3].pos.y > screenSize.y)
	{
		verts_[0].pos.y = screenSize.y - graphSize_.y;
		verts_[1].pos.y = screenSize.y - graphSize_.y;
		verts_[2].pos.y = screenSize.y;
		verts_[3].pos.y = screenSize.y;
	}

	SetMousePoint(static_cast<int>(verts_[0].pos.x) + graphSize_.x / 2, static_cast<int>(verts_[0].pos.y) + graphSize_.y / 2);
}
