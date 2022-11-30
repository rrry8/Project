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

//定数
cbuffer buff : register(b0)
{
	float angle;
	float r;
	float g;
	float b;
	float height;
}

SamplerState sam;	//サンプラ
Texture2D tex : register(t0);
Texture2D norm : register(t1);

PS_OUTPUT main(PS_INPUT input)
{
	//960 727
	PS_OUTPUT output;
	float4 color = tex.Sample(sam, input.uv);

	//ライト定義
	float3 light = float3(cos(angle), sin(angle), 0.1f); //float3(960.0f-32.0f, 727.0f-32.0f,0.0f);
	light = normalize(light);
	float3 lightColor = float3(0.8f, 0.8f, 0.8f);

	//法線
	float3 normal = norm.Sample(sam, input.uv).rgb;
	normal = (normal * 2) - 1;
	normal = normalize(normal);
	normal.z = -normal.z;

	//拡散反射光:
	float t = saturate(saturate(dot(normal, -light)) + 0.25f);

	color.rgb *= t;

	if (input.uv.y < height)
	{
		color.r *= r;
		color.g *= g;
		color.b *= b;
	}

	output.color = color;

	return output;
}