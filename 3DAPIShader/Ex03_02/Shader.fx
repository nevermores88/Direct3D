texture gTex01;
sampler SamplerTex01 : register (s0) = 
sampler_state
{
	Texture = (gTex01);
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = WRAP;
	AddressV = WRAP;
};

texture	gTex02;
sampler SamplerTex02 :register(s1) =
sampler_state
{
	Texture = (gTex02);
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = WRAP;
	AddressV = WRAP;
};

technique Tech0
{
	pass P0
	{
		// Setup Vertex Processing Constant of Rendering Machine
		FOGENABLE = FALSE;
		LIGHTING = FALSE;
		CULLMODE = CCW;

		Sampler[0] = (SamplerTex01);

		ColorOp[0] = ADDSIGNED;
		ColorArg1[0] = TEXTURE;
		ColorArg2[0] = DIFFUSE;

		AlphaOp[0] = DISABLE;
		ColorOp[1] = DISABLE;
	}

	pass P1
	{
		// Setup Vertex Processing Constant of Rendering Machine
		FOGENABLE = FALSE;
		LIGHTING = FALSE;
		CULLMODE = CCW;

		Texture[0] = (gTex02);

		ColorOp[0] = SELECTARG1;
		ColorArg1[0] = TEXTURE;
		ColorArg2[0] = DIFFUSE;

		AlphaOp[0] = DISABLE;
		ColorOp[1] = DISABLE;
	}

	pass P2
	{
		FOGENABLE = FALSE;
		LIGHTING = FALSE;
		CULLMODE = CCW;

		Sampler[0] = (SamplerTex01);
		Sampler[1] = (SamplerTex02);

		TEXCOORDINDEX[1] = 0;

		ColorOp[0] = MODULATE;
		ColorArg1[0] = TEXTURE;
		ColorArg2[0] = DIFFUSE;
		AlphaOp[0] = DISABLE;

		ColorOp[1] = ADD;
		ColorArg1[1] = CURRENT;
		ColorArg2[1] = TEXTURE;

		ColorOp[2] = DISABLE;
	}
};