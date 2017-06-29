float4 g_dwTFactor;

texture g_DiffuseTex;
sampler DiffuseSampler : register(s0) = sampler_state
{
	Texture = <g_DiffuseTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

texture g_NormalTex;
sampler NormalSampler : register(s1) = sampler_state
{
	Texture = <g_NormalTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

texture g_SpecularTex;
sampler SpecularSampler : register(s2) = sampler_state
{
	Texture = <g_SpecularTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

technique Tech0
{
	pass P0
	{
		sampler[0] = (DiffuseSampler);

		COLORARG1[0] = TEXTURE;
		COLORARG2[0] = DIFFUSE;
		COLOROP[0] = SELECTARG1;
		ALPHAOP[0] = DISABLE;

		COLOROP[1] = DISABLE;
	}

	pass P1
	{
		sampler[0] = (NormalSampler);
		
		TEXTUREFACTOR = (g_dwTFactor);

		COLORARG1[0] = TFACTOR;
		COLORARG2[0] = TEXTURE;
		COLOROP[0] = DOTPRODUCT3;
		ALPHAOP[0] = DISABLE;

		COLOROP[1] = DISABLE;
	}

	pass P2
	{
		sampler[0] = (NormalSampler);
		sampler[1] = (DiffuseSampler);
		sampler[2] = (SpecularSampler);

		TEXTUREFACTOR = (g_dwTFactor);

		COLORARG1[0] = TFACTOR;
		COLORARG2[0] = TEXTURE;
		COLOROP[0] = DOTPRODUCT3;
		ALPHAOP[0] = DISABLE;
		RESULTARG[0] = CURRENT;

		TEXCOORDINDEX[1] = 0;					// 0 stage texture coordinate.

		COLORARG1[1] = CURRENT;
		COLORARG2[1] = TEXTURE;
		COLOROP[1] = MODULATE;

		TEXCOORDINDEX[2] = 0;					// 0 stage texture coordinate.

		COLORARG1[2] = CURRENT;
		COLORARG2[2] = TEXTURE;
		COLOROP[2] = ADD;

		COLOROP[3] = DISABLE;
	}

	pass P3
	{
		sampler[0] = (NormalSampler);
		sampler[1] = (DiffuseSampler);
		sampler[2] = (SpecularSampler);

		TEXTUREFACTOR = (g_dwTFactor);

		COLORARG1[0] = TFACTOR;
		COLORARG2[0] = TEXTURE;
		COLOROP[0] = DOTPRODUCT3;
		ALPHAOP[0] = DISABLE;
		RESULTARG[0] = CURRENT;

		TEXCOORDINDEX[1] = 0;					// 0 stage texture coordinate.

		COLORARG1[1] = CURRENT;
		COLORARG2[1] = TEXTURE;
		COLOROP[1] = MODULATE2X;

		TEXCOORDINDEX[2] = 0;					// 0 stage texture coordinate.

		COLORARG1[2] = CURRENT;
		COLORARG2[2] = TEXTURE;
		COLOROP[2] = ADD;

		COLOROP[3] = DISABLE;
	}
}