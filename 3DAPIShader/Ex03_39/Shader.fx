float4x4 g_mtWorld;
float4x4 g_mtView;
float4x4 g_mtProj;

texture g_CubeTex;
sampler CubeSampler = sampler_state
{
	Texture = <g_CubeTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VS_INPUT
{
	float4 mPosition : POSITION;
	float3 mNormal : NORMAL;
};

struct VS_OUTPUT
{
	float4 mPosition : POSITION;
	float3 mReflection: TEXCOORD7;
};

VS_OUTPUT VtxPrc(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float3 normal = input.mNormal;
	float3 E = 1;
	float3 reflection = 1;

	normal = mul(normal, g_mtWorld);
	normal = mul(normal, g_mtView);
	normal = normalize(normal);

	output.mPosition = mul(input.mPosition, g_mtWorld);
	output.mPosition = mul(output.mPosition, g_mtView);

	E = -normalize(output.mPosition);

	output.mPosition = mul(output.mPosition, g_mtProj);

	reflection = 2.0 * dot(E, normal) * normal - E;

	output.mReflection = reflection;

	return output;
}

struct PS_INPUT
{
	float3 mReflection: TEXCOORD7;
};

struct PS_OUTPUT
{
	float4 mColor : COLOR0;
};

PS_OUTPUT PxlPrc(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;
	output.mColor = texCUBE(CubeSampler, input.mReflection);

	return output;
}

technique Tech0
{
	pass P0
	{
		LIGHTING = FALSE;
		//CULLMODE = NONE;

		VertexShader = compile vs_3_0 VtxPrc();
		PixelShader = compile ps_3_0 PxlPrc();
	}
};