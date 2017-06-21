float4x4 g_mtWorld;
float4x4 g_mtView;
float4x4 g_mtProj;
float4x4 g_mtRot;
float3 g_vLightDir;
float4 g_vToonColor;

texture g_Tex;
sampler ToonSampler = sampler_state
{
	Texture = <g_Tex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	//Clamp 적용이 제대로 안되는듯
	AddressU = Clamp;
	AddressV = Clamp;
};

struct VS_INPUT
{
	float4 mPosition : POSITION;
	float3 mNormal : NORMAL;
};

struct VS_OUTPUT
{
	float4 mPosition : POSITION;
	float mTex : TEXCOORD7;
};

VS_OUTPUT VtxPrc(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float4 position = float4(input.mPosition);
	position = mul(position, g_mtWorld);
	position = mul(position, g_mtView);
	position = mul(position, g_mtProj);

	float3 normal = normalize(mul(input.mNormal, g_mtRot));
	float3 lightDir = normalize(-g_vLightDir);

	float4 diffuse = 0.4f + 0.6f * dot(normal, lightDir);

	output.mPosition = position;
	//output.mTex = diffuse;
	output.mTex = diffuse;

	return output;
}

struct PS_INPUT
{
	float4 mPosition : POSITION;
	float mTex : TEXCOORD7;
};

struct PS_OUTPUT
{
	float4 mColor : COLOR;
};

PS_OUTPUT PxlPrc(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;
	//float diffuse = input.mTex;//saturate(input.mTex);
	output.mColor = tex2D(ToonSampler, saturate(input.mTex)) * g_vToonColor;
	//diffuse = ceil(diffuse * 5) / 5.0f;
	//output.mColor = float4(g_vToonColor * diffuse.xyz, 1);
	return output;
}

technique Tech0
{
	pass P0
	{
		ALPHABLENDENABLE = FALSE;
		ALPHATESTENABLE = FALSE;
		LIGHTING = FALSE;
		ALPHABLENDENABLE = FALSE;
		CULLMODE = CCW;
		ZWRITEENABLE = TRUE;

		VertexShader = compile vs_3_0 VtxPrc();
		PixelShader = compile ps_3_0 PxlPrc();
	}
}