sampler SampDif : register(s0);

struct PS_INPUT
{
	float2 base : TEXCOORD0;
};

struct PS_OUTPUT
{
	vector diffuse : COLOR0;
};

PS_OUTPUT Main(PS_INPUT input)
{
	float4 Out = 0.0f;
	float4 MonoColor = { 0.5f, 1.0f, 2.0f, 1.0f };				// 단색화 색상
	float4 MonoWeight = { 0.299f, 0.587f, 0.114f, 0.0f };	// 단색화 비중

	Out = tex2D(SampDif, input.base);	// 샘플링

	Out = dot(Out, MonoWeight);	// 내적(dot)로 단색화
	Out *= MonoColor;					// 단색화 색상을 곱함

	Out *= 3.0f;							// Aux: 전체 밝기를 올림
	Out.a = 1.0f;							// 불투명 픽셀로 설정

	PS_OUTPUT ret;
	ret.diffuse = Out;//float4(1.0f, 0.0f, 0.0f, 1.0f);
	return ret;
}