float4x4 g_mtWorldViewProj;

texture	g_DiffuseTex;
sampler DiffuseSampler = sampler_state
{
	Texture = <g_DiffuseTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;
	AddressU = Wrap;
	AddressV = Wrap;
};

texture g_LightTex;
sampler LightSampler = sampler_state
{
	Texture = <g_LightTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VS_INPUT
{
	float4 mPosition : POSITION;
	float2 mTexCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 mPosition : POSITION;
	float2 mTexCoord : TEXCOORD0;
};

VS_OUTPUT VtxPrc(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.mPosition = mul(input.mPosition, g_mtWorldViewProj);
	output.mTexCoord = input.mTexCoord;

	return output;
}

struct PS_INPUT
{
	float2 mTexCoord : TEXCOORD0;
};

struct PS_OUTPUT
{
	float4 mColor : COLOR;
};

PS_OUTPUT PxlPrc(PS_INPUT input, uniform int iIntensity)
{
	PS_OUTPUT output = (PS_OUTPUT)0;

	float4 diffuseTex = tex2D(DiffuseSampler, input.mTexCoord);
		float4 lightTex = tex2D(LightSampler, input.mTexCoord);

		output.mColor = diffuseTex;

	if (iIntensity != 0)
		output.mColor *= lightTex * iIntensity;

	return output;
}

technique Tech0
{
	pass P0
	{
		VertexShader = compile vs_3_0 VtxPrc();
		PixelShader = compile ps_3_0 PxlPrc(0);
	}

	pass P1
	{
		VertexShader = compile vs_3_0 VtxPrc();
		PixelShader = compile ps_3_0 PxlPrc(1);
	}

	pass P2
	{
		VertexShader = compile vs_3_0 VtxPrc();
		PixelShader = compile ps_3_0 PxlPrc(3);
	}

	pass P3
	{
		VertexShader = compile vs_3_0 VtxPrc();
		PixelShader = compile ps_3_0 PxlPrc(6);
	}
}