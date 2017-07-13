//
// Shader Example.
// Copyleft (c) 2007 Heesung Oh(Galic Studio). All rights Not reserved.
////////////////////////////////////////////////////////////////////////////

sampler	SampDif : register(s0);

float4 PxlPrc(float2 Tx0 : TEXCOORD0 /* Texture Coordinate */) : COLOR
{
	float4	Out=0.0F;
	float4	MonoColor ={0.5F, 1.0F, 2.0F, 1.0F};		// 단색화 색상
	float4	MonoWeight={0.299F, 0.587F, 0.114F, 0.0F};	// 단색화 비중

	Out = tex2D( SampDif, Tx0 );	// 샘플링

	Out = dot(Out, MonoWeight);		// 내적(dot)로 단색화
	Out *= MonoColor;				// 단색화 색상을 곱함

	Out *= 3.0F;					// Aux: 전체 밝기를 올림
	Out.a = 1.0F;					// 불투명 픽셀로 설정

	return Out;
}

