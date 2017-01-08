//
// Shader Example.
// Copyleft (c) 2007 Heesung Oh(Galic Studio). All rights Not reserved.
//
////////////////////////////////////////////////////////////////////////////////


float	m_fDeviation;


texture m_Tex;
sampler sampTex =
sampler_state
{
	Texture = <m_Tex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Wrap;
	AddressV = Wrap;
};


VertexShader VtxPrc = asm
{
	vs_3_0
	dcl_position v0
	dcl_color0   v1
	dcl_texcoord v2

	mov	oPos, v0
	mov oD0, v1
	mov oT0, v2
};


float4 PxlPrc0(	float4 Dif : COLOR0
			,	float2 Tx0 : TEXCOORD0
				) : COLOR
{
	float4	Out= tex2D(sampTex, Tx0);
	Out	*=0.5f;
	return Out;
}


float4 PxlPrc1(	float4 Dif : COLOR0
			,	float2 Tx0 : TEXCOORD0
				) : COLOR
{
	int		i=0;
	int		iMax=4;
	float4	Out= 0.0f;

	for(i=-iMax; i<=iMax; ++i)
	{
		float2 Tx	= Tx0;
		Tx.x	+= (i *m_fDeviation)/1024.0f;

		float4	d = tex2D(sampTex, Tx);
		float	e = i*i;

		e = -e/16.0f;
		Out += d * 1.0f* exp( e );
	}


	for(i=-iMax; i<=iMax; ++i)
	{
		float2 Tx	= Tx0;
		Tx.y	+= (i *m_fDeviation)/1024.0f;

		float4	d = tex2D(sampTex, Tx);
		float	e = i*i;

		e = -e/16.0f;
		Out += d * 1.0f* exp( e );
	}

	Out	  *=0.3f;

	float	d = Out.r * 0.288 + Out.g * 0.588 + Out.b * 0.114;

//	d	= pow(d, 1.0f);
	d	*=.4f;

	Out.r	= d* 1.0f;
	Out.g	= d* 1.0f;
	Out.b	= d* 1.0f;
	Out.a	 = 1.0f;

	return Out;
}


technique Tech
{
	pass P0
	{
		FogEnable	= FALSE;
		LIGHTING	= FALSE;

		AlphablendEnable= FALSE;

		ADDRESSU[0] = CLAMP;
		ADDRESSV[0] = CLAMP;
		ADDRESSW[0] = CLAMP;

		MINFILTER[0]= POINT;
		MAGFILTER[0]= POINT;
		MIPFILTER[0]= POINT;

		VertexShader = (VtxPrc);
		PixelShader  = compile ps_3_0 PxlPrc0();
	}

	pass P1
	{
		FogEnable	= FALSE;
		LIGHTING	= FALSE;

		AlphablendEnable= TRUE;
		SRCBLEND	= ONE;
		DESTBLEND	= ONE;

		ADDRESSU[0] = CLAMP;
		ADDRESSV[0] = CLAMP;
		ADDRESSW[0] = CLAMP;

		MINFILTER[0]= POINT;
		MAGFILTER[0]= POINT;
		MIPFILTER[0]= POINT;

		VertexShader = (VtxPrc);
		PixelShader  = compile ps_3_0 PxlPrc1();
	}
}


