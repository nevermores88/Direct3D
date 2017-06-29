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

texture g_LTex;
sampler LSampler : register(s1) = sampler_state
{
	Texture = <g_LTex>;
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
		sampler[0] = (LSampler);
		sampler[1] = (DiffuseSampler);
		sampler[2] = (SpecularSampler);

		LIGHTING = FALSE;
		ZENABLE = TRUE;
		ZFUNC = LESS;
		ZWRITEENABLE = TRUE;

		TEXTUREFACTOR = 0x00304050;
		NORMALIZENORMALS = TRUE;
		WRAP0 = U | V;
		CULLMODE = NONE;

		COLORARG1[0] = TFACTOR;
		COLORARG2[0] = TEXTURE;
		COLOROP[0] = ADD;
		ALPHAOP[0] = DISABLE;

		TEXTURETRANSFORMFLAGS[0] = COUNT3;

		TEXCOORDINDEX[1] = 0;

		COLORARG1[1] = TEXTURE;
		COLORARG2[1] = CURRENT;
		COLOROP[1] = MODULATE;

		/*TEXTURETRANSFORMFLAGS[1] = DISABLE;

		COLORARG1[2] = TEXTURE;
		COLORARG2[2] = CURRENT;
		COLOROP[2] = ADD;

		TEXTURETRANSFORMFLAGS[2] = COUNT3;
		COLOROP[3] = DISABLE;*/
	}
}