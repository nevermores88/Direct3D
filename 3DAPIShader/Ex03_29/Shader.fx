float4x4 g_mtWorld;
float4x4 g_mtView;
float4x4 g_mtProj;
float4x4 g_mtRot;
float3 g_vLightDir;

texture g_NormalTex;
sampler NormalSampler = sampler_state
{
	Texture = <g_NormalTex>;
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
	float2 mTexCoord : TEXCOORD0;
	float3 mTangent : TEXCOORD1;
	float3 mBinormal : TEXCOORD2;
};

struct VS_OUTPUT
{
	float4 mPosition : POSITION;
	float2 mTexCoord: TEXCOORD0;
	float3 mT : TEXCOORD1;
	float3 mB : TEXCOORD2;
	float3 mN : TEXCOORD3;
};

VS_OUTPUT VtxPrc(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float4 position = input.mPosition;

		position = mul(position, g_mtWorld);

	float3 N = mul(input.mNormal, g_mtRot);
	float3 T = mul(input.mTangent, g_mtRot);
	float3 B = mul(input.mBinormal, g_mtRot);

	position = mul(position, g_mtView);
	position = mul(position, g_mtProj);

	output.mPosition = position;
	output.mTexCoord = input.mTexCoord;
	output.mN = N;
	output.mT = T;
	output.mB = B;

	return output;
}

struct PS_INPUT
{
	float2 mTexCoord: TEXCOORD0;
	float3 mT : TEXCOORD1;
	float3 mB : TEXCOORD2;
	float3 mN : TEXCOORD3;
};

struct PS_OUTPUT
{
	float4 mColor : COLOR;
};

PS_OUTPUT PxlPrc(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;

	float4 bump = { 1, 1, 1, 1 };
	float3 B = { 1, 0, 0 };
	float3 T = { 0, 1, 0 };
	float3 N = { 0, 0, 1 };

	float3 lightDir = normalize(-g_vLightDir);
	N = normalize(input.mN);
	T = normalize(input.mT);
	B = normalize(input.mB);

	float3 C1 = 0;
	float3 C = 2 * tex2D(NormalSampler, input.mTexCoord) - 1;
	C = normalize(C);

	C1.x = B.x * C.x + T.x * C.y + N.x * C.z;
	C1.y = B.y * C.x + T.y * C.y + N.y * C.z;
	C1.z = B.z * C.x + T.z * C.y + N.z * C.z;
	C1 = normalize(C1);

	bump = dot(C1, lightDir);

	output.mColor = bump;

	return output;
}

technique Tech0
{
	pass P0
	{
		VertexShader = compile vs_3_0 VtxPrc();
		PixelShader = compile ps_3_0 PxlPrc();
	}
}