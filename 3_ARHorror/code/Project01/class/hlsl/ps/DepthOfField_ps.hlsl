struct PS_INPUT
{
    float4 pos : POSITION;
    float4 diffuse : COLOR0;
    float2 uv : TEXCOORD0;
};
cbuffer BaseCBuffer : register(b1)
{
    matrix AntiViewportM; //4x4�r���[�|�[�g�t�s��
    matrix ProjectionM; //4x4�v���W�F�N�V�����s��
    float4x3 ViewM; //4x3(�r���[�s��)
    float4x3 LocalM; //4x3(��]�g�k���s�ړ�)
    float4 ToonOutLineSize; // �g�D�[���̗֊s���̑傫��
    float DiffuseSource; // �f�B�t���[�Y�J���[( 0.0f:�}�e���A��  1.0f:���_ )
    float SpecularSource; // �X�y�L�����J���[(   0.0f:�}�e���A��  1.0f:���_ )
    float MulSpecularColor; // �X�y�L�����J���[�l�ɏ�Z����l( �X�y�L�������������Ŏg�p )
    float Padding; //�l�ߕ�(����)
}

cbuffer Cbuf_DOF_Focus : register(b8)
{
    float3 DOF_Focus;
}

struct PS_OUTPUT
{
    float4 color : SV_TARGET;
};

SamplerState sam:register(s0);
Texture2D<float4> ColorMapTexture0 : register(t0);
Texture2D<float4> ColorMapTexture1 : register(t1);
Texture2D<float4> ColorMapTexture2 : register(t2);
Texture2D<float4> DepthMapTexture : register(t3); // �[�x�}�b�v�e�N�X�`��

PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;
    float4 Color1, Color2;
    float BlendRate;
    float Fade;
    float Depth;
    
    Depth = DepthMapTexture.Sample(sam, input.uv);
    
    if (Depth < DOF_Focus.x)
    {
        Fade = 1.0f - Depth / DOF_Focus.x;
    }
    else if (Depth < DOF_Focus.y)
    {
        Depth = 0.0f;
    }
    else
    {
        Depth = (Depth - DOF_Focus.y) / (1.0f - DOF_Focus.y);
    }
    
    if(Fade < 0.5f)
    {
        Color1 = ColorMapTexture0.Sample(sam, input.uv);
        Color2 = ColorMapTexture1.Sample(sam, input.uv);
        BlendRate = Fade / 0.5f;
    }
    else
    {
        Color1 = ColorMapTexture1.Sample(sam, input.uv);
        Color2 = ColorMapTexture2.Sample(sam, input.uv);
        BlendRate = (Fade - 0.5f) / 0.5f;
    }
    
    output.color = lerp(Color1, Color2, BlendRate);
    return output;

}