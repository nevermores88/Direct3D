float g_fDeviation;

texture g_Tex;
sampler TexSampler = 
sampler_state
{
	Texture = g_Tex;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Wrap;
	AddressV = Wrap;
};

struct VS_OUTPUT
{
	float4 mPosition : POSITION;
	float4 mDiffuse : COLOR0;
	float2 mTexCoord : TEXCOORD0;
};

struct VS_INPUT
{
	float4 mPosition :POSITION;
	float4 mDiffuse : COLOR0;
	float2 mTexCoord : TEXCOORD0;
};

VS_OUTPUT vs_main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.mPosition = input.mPosition;
	output.mDiffuse = input.mDiffuse;
	output.mTexCoord = input.mTexCoord;

	return output;
}

//VertexShader vs_main = asm
//{
//	vs_2_0
//	dcl_position v0
//	dcl_color0   v1
//	dcl_texcoord v2
//
//	mov	oPos, v0
//	mov oD0, v1
//	mov oT0, v2
//};

struct PS_INPUT
{
	float4 mDiffuse : COLOR0;
	float2 mTexCoord : TEXCOORD0;
};

struct PS_OUTPUT
{
	float4 mDiffuse : COLOR0;
};

PS_OUTPUT ps_main0(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;
	output.mDiffuse = tex2D(TexSampler, input.mTexCoord);
	output.mDiffuse *= 0.5f;
	
	return output;
}

float4 ps_main1(float4 Dif : COLOR0
	, float2 Tx0 : TEXCOORD0
	) : COLOR
{
	int		i = 0;
	int		iMax = 4;
	float4	Out = 0.0f;

		for (i = -iMax; i <= iMax; ++i)
		{
			float2 Tx = Tx0;
				Tx.x += (i *g_fDeviation) / 1024.0f;

			float4	d = tex2D(TexSampler, Tx);
				float	e = i*i;

			e = -e / 16.0f;
			Out += d * 1.0f* exp(e);
		}


	for (i = -iMax; i <= iMax; ++i)
	{
		float2 Tx = Tx0;
			Tx.y += (i *g_fDeviation) / 1024.0f;

		float4	d = tex2D(TexSampler, Tx);
			float	e = i*i;

		e = -e / 16.0f;
		Out += d * 1.0f* exp(e);
	}

	Out *= 0.3f;

	float	d = Out.r * 0.288 + Out.g * 0.588 + Out.b * 0.114;

	//	d	= pow(d, 1.0f);
	d *= .4f;

	Out.r = d* 1.0f;
	Out.g = d* 1.0f;
	Out.b = d* 1.0f;
	Out.a = 1.0f;

	return Out;
}

technique Tech
{
	pass P0
	{
		FogEnable = FALSE;
		LIGHTING = FALSE;

		AlphablendEnable = FALSE;

		ADDRESSU[0] = CLAMP;
		ADDRESSV[0] = CLAMP;
		ADDRESSW[0] = CLAMP;

		MINFILTER[0] = POINT;
		MAGFILTER[0] = POINT;
		MIPFILTER[0] = POINT;

		VertexShader = compile vs_3_0 vs_main();
		PixelShader = compile ps_3_0 ps_main0();
	}

	pass P1
	{
		FogEnable = FALSE;
		LIGHTING = FALSE;

		AlphablendEnable = TRUE;
		SRCBLEND = ONE;
		DESTBLEND = ONE;

		ADDRESSU[0] = CLAMP;
		ADDRESSV[0] = CLAMP;
		ADDRESSW[0] = CLAMP;

		MINFILTER[0] = POINT;
		MAGFILTER[0] = POINT;
		MIPFILTER[0] = POINT;

		//VertexShader = (vs_main);
		VertexShader = compile vs_3_0 vs_main();
		PixelShader = compile ps_3_0 ps_main1();
	}
}
