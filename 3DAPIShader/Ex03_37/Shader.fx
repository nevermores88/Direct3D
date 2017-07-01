float4x4 g_mtWorld;
float4x4 g_mtView;
float4x4 g_mtProj;
float4x4 g_mtRot;

float3	g_vCamPos;


texture g_EnvTex;
sampler EnvSampler = sampler_state
{
	Texture = <g_EnvTex>;
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
	float2 mTexCoord: TEXCOORD0;
};

VS_OUTPUT VtxPrc(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float4 position = input.mPosition;
	position = mul(position, g_mtWorld);
	position = mul(position, g_mtView);
	position = mul(position, g_mtProj);

	float3 normal = input.mNormal;
	normal = mul(normal, g_mtRot);
	normal = mul(normal, g_mtView);

	float2 texCoord = 1;

	texCoord = normal.xy;
	texCoord.y = -texCoord.y;
	texCoord = texCoord*0.5 + 0.5;

	output.mPosition = position;
	output.mTexCoord = texCoord;

	return output;
}

struct PS_INPUT
{
	float2 mTexCoord: TEXCOORD0;
};

struct PS_OUTPUT
{
	float4 mColor : COLOR;
};

PS_OUTPUT PxlPrc(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;

	output.mColor = tex2D(EnvSampler, input.mTexCoord);
	output.mColor.a = 1;

	return output;
}


technique Tech0
{
	pass P0
	{
		LIGHTING = FALSE;
		CULLMODE = NONE;

		VertexShader = compile vs_3_0 VtxPrc();
		PixelShader = compile ps_3_0 PxlPrc();
	}
}