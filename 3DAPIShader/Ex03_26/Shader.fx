float4x4 g_mtWorld;
float4x4 g_mtView;
float4x4 g_mtProj;
float4x4 g_mtRot;
float3 g_vLightDir;
float g_fHatchingWidth;

sampler smp0 : register(s0) = sampler_state{ MinFilter = LINEAR; MagFilter = LINEAR; MipFilter = LINEAR; AddressU = Wrap; AddressV = Wrap; };
sampler smp1 : register(s1) = sampler_state{ MinFilter = LINEAR; MagFilter = LINEAR; MipFilter = LINEAR; AddressU = Wrap; AddressV = Wrap; };
sampler smp2 : register(s2) = sampler_state{ MinFilter = LINEAR; MagFilter = LINEAR; MipFilter = LINEAR; AddressU = Wrap; AddressV = Wrap; };
sampler smp3 : register(s3) = sampler_state{ MinFilter = LINEAR; MagFilter = LINEAR; MipFilter = LINEAR; AddressU = Wrap; AddressV = Wrap; };
sampler smp4 : register(s4) = sampler_state{ MinFilter = LINEAR; MagFilter = LINEAR; MipFilter = LINEAR; AddressU = Wrap; AddressV = Wrap; };
sampler smp5 : register(s5) = sampler_state{ MinFilter = LINEAR; MagFilter = LINEAR; MipFilter = LINEAR; AddressU = Wrap; AddressV = Wrap; };

texture g_DiffuseTex;
sampler DiffuseSampler = sampler_state
{
	Texture = (g_DiffuseTex);
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
	float4 mColor : COLOR0;
	float2 mTexCoord : TEXCOORD0;
};

VS_OUTPUT VtxPrc(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float4 position = input.mPosition;
	
	position = mul(position, g_mtWorld);
	position = mul(position, g_mtView);
	position = mul(position, g_mtProj);

	float3 normal = normalize(mul(input.mNormal, g_mtRot));
	float3 lightDir = normalize(-g_vLightDir);

	float4 diffuse = (1.0f + dot(normal, lightDir)) * 0.5f;
	diffuse.a = 1.0f;

	output.mPosition = position;
	output.mColor = diffuse;
	output.mTexCoord = input.mTexCoord;

	return output;
}

struct PS_INPUT
{
	float4 mPosition : POSITION;
	float4 mColor : COLOR0;
	float2 mTexCoord : TEXCOORD0;
};

struct PS_OUTPUT
{
	float4 mColor : COLOR;
};

PS_OUTPUT PxlPrc(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;

	//Hatching Level
	float fHatchLvl = input.mColor * 5.3f;

	//Hatching Weight
	float fHatch0 = 0;
	float fHatch1 = 0;
	float fHatch2 = 0;
	float fHatch3 = 0;
	float fHatch4 = 0;
	float fHatch5 = 0;

	// map tone to quantized palette
	if (fHatchLvl > 5.0)
	{
		fHatch0 = 1.0;
	}
	else if (fHatchLvl > 4.0)
	{
		fHatch0 = 1.0 - (5.0 - fHatchLvl);
		fHatch1 = 1.0 - fHatch0;
	}
	else if (fHatchLvl > 3.0)
	{
		fHatch1 = 1.0 - (4.0 - fHatchLvl);
		fHatch2 = 1.0 - fHatch1;
	}
	else if (fHatchLvl > 2.0)
	{
		fHatch2 = 1.0 - (3.0 - fHatchLvl);
		fHatch3 = 1.0 - fHatch2;
	}
	else if (fHatchLvl > 1.0)
	{
		fHatch3 = 1.0 - (2.0 - fHatchLvl);
		fHatch4 = 1.0 - fHatch3;
	}
	else
	{
		fHatch4 = 1.0 - (1.0 - fHatchLvl);
		fHatch5 = 1.0 - fHatch4;
	}

	float4 t0 = tex2D(smp0, input.mTexCoord * g_fHatchingWidth) * fHatch0;
	float4 t1 = tex2D(smp1, input.mTexCoord * g_fHatchingWidth) * fHatch1;
	float4 t2 = tex2D(smp2, input.mTexCoord * g_fHatchingWidth) * fHatch2;
	float4 t3 = tex2D(smp3, input.mTexCoord * g_fHatchingWidth) * fHatch3;
	float4 t4 = tex2D(smp4, input.mTexCoord * g_fHatchingWidth) * fHatch4;
	float4 t5 = tex2D(smp5, input.mTexCoord * g_fHatchingWidth) * fHatch5;

	output.mColor = t0 + t1 + t2 + t3 + t4 + t5;
	output.mColor.a = 1.0f;

	output.mColor = pow(output.mColor, 0.6f) * 1.5f;
	output.mColor *= tex2D(DiffuseSampler, input.mTexCoord);

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