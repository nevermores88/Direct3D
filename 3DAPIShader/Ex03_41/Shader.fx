float4x4 g_mtWorld;
float4x4 g_mtView;
float4x4 g_mtProj;
int			g_iEnvOpt;

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
	float3 mEye : TEXCOORD6;
	float3 mNormal : TEXCOORD7;
};

VS_OUTPUT VtxPrc(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float3 normal = input.mNormal;
	float3 eye = 1;
	float3 reflection = 1;

	normal = mul(normal, g_mtWorld);
	normal = mul(normal, g_mtView);
	normal = normalize(normal);

	output.mPosition = mul(input.mPosition, g_mtWorld);
	output.mPosition = mul(output.mPosition, g_mtView);

	eye = -normalize(output.mPosition);

	output.mPosition = mul(output.mPosition, g_mtProj);

	output.mEye = eye;
	output.mNormal = normal;
	//reflection = 2.0 * dot(E, normal) * normal - E;

	//output.mReflection = reflection;

	return output;
}

static float n1 = 1;
static float n2 = 1.02;

struct PS_INPUT
{
	float3 mEye : TEXCOORD6;
	float3 mNormal : TEXCOORD7;
};

struct PS_OUTPUT
{
	float4 mColor : COLOR0;
};

PS_OUTPUT PxlPrc(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;
	float3 eye = normalize(input.mEye);
	float3 normal = normalize(input.mNormal);
	float3 reflection = 2.0 * dot(eye, normal) * normal - eye;
	float3 F = 0;

	float3 X = dot(eye, normal) * normal - eye;
	float sin_theta1 = length(-X);
	float k = n1 / n2;
	float sin_theta2 = k*sin_theta1;
	float cos_theta2 = sqrt(1.0 - sin_theta2 * sin_theta2);

	X = normalize(X);
	F = (-normal) * cos_theta2 + X * sin_theta2;

	if (g_iEnvOpt == 1)
		output.mColor = texCUBE(CubeSampler, reflection);
	else
		output.mColor = texCUBE(CubeSampler, F);
	//output.mColor = texCUBE(CubeSampler, input.mReflection);

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
};