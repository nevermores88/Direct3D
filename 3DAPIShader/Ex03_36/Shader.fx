float4x4 g_mtWorld;
float4x4 g_mtView;
float4x4 g_mtProj;
float4x4 g_mtRot;

float3	g_vCamPos;
float3	g_vLightDir;
float	g_fSharpness;

float4 g_vColor;

texture g_DiffuseTex;
sampler DiffuseSampler = sampler_state
{
	Texture = <g_DiffuseTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

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

texture g_SpecularTex;
sampler SpecularSampler = sampler_state
{
	Texture = <g_SpecularTex>;
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
};

struct VS_OUTPUT
{
	float4 mPosition : POSITION;
	float2 mTexCoord: TEXCOORD0;

	float3 mReflection : TEXCOORD1;
	float3 mEyeDir : TEXCOORD2;
	float3 mNormal : TEXCOORD3;
};

VS_OUTPUT VtxPrc(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float4 position = input.mPosition;
	position = mul(position, g_mtWorld);

	float3 normal = mul(input.mNormal, g_mtRot);
	float3 eyeDir = normalize(g_vCamPos - position);

	position = mul(position, g_mtView);
	position = mul(position, g_mtProj);

	float3 lightDir = g_vLightDir;
	float3 reflection = reflect(lightDir, normal);

	output.mPosition = position;
	output.mTexCoord = input.mTexCoord;
	output.mReflection = reflection;
	output.mEyeDir = eyeDir;
	output.mNormal = normal;

	return output;
}

struct PS_INPUT
{
	float2 mTexCoord: TEXCOORD0;

	float3 mReflection : TEXCOORD1;
	float3 mEyeDir : TEXCOORD2;
	float3 mNormal : TEXCOORD3;
};

struct PS_OUTPUT
{
	float4 mColor : COLOR;
};

PS_OUTPUT NormalPrc(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;

	float4 bump = { 1, 1, 1, 1 };
	float3 B = { 1, 0, 0 };
	float3 T = { 0, 1, 0 };
	float3 N = { 0, 0, 1 };

	float3 lightDir = normalize(-g_vLightDir);
	N = normalize(input.mNormal);
	T = normalize(cross(N, B));
	B = normalize(cross(T, N));

	float3 C1 = 0;
	float3 C = 2 * tex2D(NormalSampler, input.mTexCoord) - 1;
	C = normalize(C);

	C1.x = B.x * C.x + T.x * C.y + N.x * C.z;
	C1.y = B.y * C.x + T.y * C.y + N.y * C.z;
	C1.z = B.z * C.x + T.z * C.y + N.z * C.z;
	C1 = normalize(C1);

	bump = dot(C1, lightDir);
	//bump *= tex2D(DiffuseSampler, input.mTexCoord);
	//bump += 0.4f;

	output.mColor = bump;

	return output;
}

PS_OUTPUT PxlPrc(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;

	float4 hue = g_vColor;
	float3 lightDir = g_vLightDir;

	float4 diffuse = tex2D(DiffuseSampler, input.mTexCoord);
	float4 bump = NormalPrc(input).mColor;
	float4 specularTex = tex2D(SpecularSampler, input.mTexCoord);

	bump += 0.1f;
	bump *= 1.3f;
	bump = pow(bump, 3.0f);
	bump *= 1.5f;

	float3 reflection = normalize(input.mReflection);
	float3 eyeDir = normalize(input.mEyeDir);
	float4 specular = saturate(dot(reflection, eyeDir));

	specular = pow(specular, g_fSharpness);

	specularTex *= specular;
	specularTex *= 8.0f;

	diffuse *= bump;
	diffuse += specularTex;
	diffuse *= hue;

	output.mColor = diffuse;

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