float4x4 g_mtWorld;
float4x4 g_mtView;
float4x4 g_mtProj;
float4x4 g_mtRot;
float3 g_vLightDir[4];
float4 g_vLightDif[4];
float3 g_vCamPos;
float g_fSharpness;
int g_bTexture;

texture g_Tex;
sampler DiffuseSampler = sampler_state
{
	Texture = <g_Tex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = WRAP;
	AddressV = WRAP;
};

struct VS_INPUT
{
	float4 mPosition : POSITION;
	float3 mNormal : NORMAL;
	float2 mTex : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 mPosition : POSITION;
	float2 mTex : TEXCOORD0;

	float3 mNormal : TEXCOORD1;
	float3 mViewDir : TEXCOORD2;
};

VS_OUTPUT VtxPrc(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float4 position = float4(input.mPosition);
		position = mul(position, g_mtWorld);

	float3 viewDir = normalize(g_vCamPos - position);
	float3 normal = normalize(mul(input.mNormal, g_mtRot));

	/*float3 lightDir = normalize(-g_vLightDir);
	float3 reflectionDir = normalize(2 * dot(normal, lightDir)*normal - lightDir);
	float4 specular = pow(max(0, dot(reflectionDir, viewDir)), g_fSharpness);*/

	position = mul(position, g_mtView);
	position = mul(position, g_mtProj);

	//specular.a = 1.0f;

	output.mPosition = position;
	output.mViewDir = viewDir;
	output.mNormal = normal;
	output.mTex = input.mTex;

	return output;
}

struct PS_INPUT
{
	float4 mPosition : POSITION;
	float2 mTex : TEXCOORD0;
	float3 mNormal : TEXCOORD1;
	float3 mViewDir : TEXCOORD2;
};

struct PS_OUTPUT
{
	float4 mColor : COLOR;
};

PS_OUTPUT PxlPrc(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;

	float3 viewDir = normalize(input.mViewDir);
	float3 normal = normalize(input.mNormal);

	float3 lightDir[3];
	float3 reflection[3];
	float4 diffuse = 0;
	float4 specular = 0;

	for (int i = 0; i < 3; i++)
	{
		lightDir[i] = normalize(-g_vLightDir[i]);
		reflection[i] = reflect(-lightDir[i], normal);
	}

	// Lambert = ¥Ò dot(N, L[i]) * Color[i]
	for (int i = 0; i < 3; i++)
	{
		diffuse += (0.5f * dot(normal, lightDir[i]) + 0.5) * g_vLightDif[i];
	}

	// Phong = ¥Ò pow( dot(E, R[i]), Sharpness) * Color[i]
	for (int i = 0; i < 3; i++)
	{
		specular += pow(max(0, dot(viewDir, reflection[i])), g_fSharpness)*g_vLightDif[i];
	}

	output.mColor = saturate(diffuse);
	if (g_bTexture == 1)
		output.mColor *= tex2D(DiffuseSampler, input.mTex);

	output.mColor += specular;
	output.mColor.a = 1;

	//float3 lightDir = normalize(-g_vLightDir);
	//float3 reflection = reflect(-lightDir, normal);
	//float3 specular = pow(max(0, dot(reflection, viewDir)), g_fSharpness);

	//output.mColor = tex2D(DiffuseSampler, input.mTex);
	//output.mColor *= 0.7f;
	//output.mColor += float4(specular, 1);

	return output;
}

technique Tech0
{
	pass P0
	{
		ALPHABLENDENABLE = FALSE;
		ALPHATESTENABLE = FALSE;

		VertexShader = compile vs_2_0 VtxPrc();
		PixelShader = compile ps_2_0 PxlPrc();
	}
}