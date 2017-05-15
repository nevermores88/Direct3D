float4x4 g_WorldViewProj;
sampler DiffuseSampler : register(s0) = sampler_state
{
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VS_INPUT
{
	float4 m_Pos : POSITION;
	float2 m_Coord : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 m_Pos : POSITION;
	float2 m_Coord : TEXCOORD0;
};

VS_OUTPUT VtxPrc(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.m_Pos = mul(input.m_Pos, g_WorldViewProj);
	output.m_Coord = input.m_Coord;

	return output;
}

float4 g_Diffuse = { 1, 1, 1, 1 };

struct PS_INPUT
{
	float2 m_Coord : TEXCOORD0;
};

struct PS_OUTPUT
{
	float4 m_Color : COLOR0;
};

PS_OUTPUT PxlPrc(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;

	output.m_Color = tex2D(DiffuseSampler, input.m_Coord);
	output.m_Color *= g_Diffuse;

	return output;
}