//����
struct PS_INPUT {
	float4 pos    : SV_POSITION;
	float4 diffuse     : COLOR0;
	float2 uv  : TEXCOORD0;
};
//�o��
struct PS_OUTPUT
{
	float4 color      : SV_TARGET;
};

//�萔
cbuffer buff : register(b8)
{
	float power;
	float time;
}

float ScanLine(float2 uv)
{
	float scanLine = clamp(0.85f + 0.15f * cos(3.1415f * (uv.y + 0.016f * time) * 240.0f), 0.0f, 1.0f);

	//float grille = 0.85f + 0.15f * clamp(1.5f * cos(3.1415f * uv.x * 640.0f * 1.0f), 0.0f, 1.0f);

	return scanLine  * 1.2f;
}

//�e�N�X�`��
SamplerState sam     : register(s0);
Texture2D    screen  : register(t0);
Texture2D    noise  : register(t1);

PS_OUTPUT main(PS_INPUT input)
{
	PS_OUTPUT output;

	//����̃G�t�F�N�g
	float4 offset = noise.Sample(sam, input.uv);
	//���C���X�N���[��
	float4 color = screen.Sample(sam, input.uv);

	//�ΐF�Ɋ�点��
	color.rgb = saturate(color.rgb * float3(0.6f, 1.25f, 0.8f));

	//��������
	color.rgb *= ScanLine(input.uv);

	color.rgb = pow(color.rgb, 0.8f);

	//��Z�Ŏ��R�Ȍ`�ɂ���
	offset = pow(offset, 2.5f);

	//����
	color.rgb += offset.rgb;

	//���邭����
	color += color;

	//1~0�ɗ}����
	color = saturate(color);

	//�t���b�V�����͔��𑫂�
	if (power > 0.0f)
	{
		color = saturate(color + float4(1.0f, 1.0f, 1.0f, 1.0f) * power);
	}

	output.color = color;

	return output;
}
