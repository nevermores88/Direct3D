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

technique Tech0
{
	pass P0
	{
		sampler [0] = DiffuseSampler;

		COLORARG1[0] = TEXTURE;
		COLOROP[0] = SELECTARG1;
		ALPHAOP[0] = DISABLE;

		COLOROP[1] = DISABLE;
	}

	pass P1
	{
		sampler [0] = DiffuseSampler;
		sampler [1] = LightSampler;

		COLORARG1[0] = TEXTURE;
		COLORARG2[0] = DIFFUSE;
		COLOROP[0] = SELECTARG1;

		ALPHAARG1[0] = TEXTURE;
		ALPHAOP[0] = SELECTARG1;

		TEXCOORDINDEX[1] = 0;				// 0 stage texture coordinate.
		COLORARG1[1] = TEXTURE;
		COLORARG2[1] = CURRENT;
		COLOROP[1] = MODULATE2X;

		ALPHAARG1[1] = TEXTURE;
		ALPHAARG2[1] = CURRENT;
		ALPHAOP[1] = MODULATE;

		COLOROP[2] = DISABLE;
		ALPHAOP[2] = DISABLE;
	}

	pass P2
	{
		sampler [0] = DiffuseSampler;
		sampler [1] = LightSampler;

		COLORARG1[0] = TEXTURE;
		COLORARG2[0] = DIFFUSE;
		COLOROP[0] = SELECTARG1;

		ALPHAARG1[0] = TEXTURE;
		ALPHAOP[0] = SELECTARG1;

		TEXCOORDINDEX[1] = 0;		// 0 stage texture coordinate.
		COLORARG1[1] = TEXTURE;
		COLORARG2[1] = CURRENT;
		COLOROP[1] = MODULATE4X;

		ALPHAARG1[1] = TEXTURE;
		ALPHAARG2[1] = CURRENT;
		ALPHAOP[1] = MODULATE;

		COLOROP[2] = DISABLE;
		ALPHAOP[2] = DISABLE;
	}
}