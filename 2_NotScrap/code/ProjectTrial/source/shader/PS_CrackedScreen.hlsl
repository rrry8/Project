struct PS_INPUT
{
	float4 diffuse : COLOR0;
	float4 specular : COLOR1;
	float2 uv : TEXCOORD0;
};

struct PS_OUTPUT
{
	float4 color : SV_TARGET0;
};

SamplerState sam;
Texture2D tex : register(t0);
Texture2D nor : register(t1);

cbuffer ConstantBuffer : register(b0)
{
	float angle;
}

PS_OUTPUT main(PS_INPUT input)
{
	PS_OUTPUT output;
	float2 normal = nor.Sample(sam, input.uv).xy;
	normal = normal * 2 - 1;//-1~1

	float2 crack = normal.xy * 0.3f;
	//crack = float2(cos(angle) * crack.x + sin(angle) * crack.y,
	//	-sin(angle) * crack.x + cos(angle) * crack.y);

	output.color = tex.Sample(sam, input.uv + crack);

	return output;
}