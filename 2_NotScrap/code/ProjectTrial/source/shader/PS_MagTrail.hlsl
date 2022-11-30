//入力
struct PS_INPUT
{
	float4 pos    : SV_POSITION;
	float4 diffuse     : COLOR0;
	float2 uv  : TEXCOORD0;
};

//出力
struct PS_OUTPUT
{
	float4 color      : SV_TARGET;
};

//定数
cbuffer buff : register(b5)
{
	float curve;
}

//テクスチャ
SamplerState sam       : register(s0);
Texture2D    tex       : register(t0);
Texture2D    ptn       : register(t1);

PS_OUTPUT main(PS_INPUT input)
{
	PS_OUTPUT output;

	//ノイズをサンプリング。uvを回転させて歪みを移動させる
	float2 p = ptn.Sample(sam, float2(input.uv.r + cos(curve) * 0.05f, input.uv.g + sin(curve) * 0.05f)).xy;

	//-1~1にする
	p.xy = (p.xy * 2.0 - 1.0f) * 0.05f;

	//背景をサンプリングして加算
	output.color = tex.Sample(sam, input.uv + p.xy);
	output.color.rgb *= input.diffuse.rgb;
	output.color.a = input.diffuse.a;

	return output;
}
