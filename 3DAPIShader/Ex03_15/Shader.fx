float4x4 g_mtWorldViewProj;
float3x3 g_mtRot;
float3 g_vLightDir;

texture g_DiffuseTex;
sampler DiffuseSampler = sampler_state
{
	Texture = g_DiffuseTex;

	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = WRAP;
	AddressV = WRAP;
};

struct VS_INPUT
{
	float4 mPosition : POSITION;
	float4 mNormal : NORMAL;
	float2 mTexCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 mPosition : POSITION;
	float2 mTexCoord : TEXCOORD0;
	float3 mNormal : TEXCOORD1;
};

VS_OUTPUT VtxPrc(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float4 position = mul(input.mPosition, g_mtWorldViewProj);
	float3 normal = mul(input.mNormal, g_mtRot);

	output.mPosition = position;
	output.mNormal = normal;
	output.mTexCoord = input.mTexCoord;

	return output;
}

struct PS_INPUT
{
	float4 mPosition : POSITION;
	float2 mTexCoord : TEXCOORD0;
	float3 mNormal : TEXCOORD1;
};

struct PS_OUTPUT
{
	float4 mColor : COLOR;
};

PS_OUTPUT PxlPrc(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;

	float3 normal = normalize(input.mNormal);
	float3 lightDir = -g_vLightDir;

	float4 color = (0.5f + dot(lightDir, normal)) * 0.6f;
	color *= tex2D(DiffuseSampler, input.mTexCoord);
	color.a = 1.0f;

	output.mColor = color;

	return output;
}

technique Tech0
{
	pass P0
	{
		VertexShader = compile vs_2_0 VtxPrc();
		PixelShader = compile ps_2_0 PxlPrc();
	}
}