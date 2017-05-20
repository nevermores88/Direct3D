int m_iMono;
float4 m_Color;

sampler smp0 : register(s0);
sampler smp1 : register(s1);

struct VS_INPUT
{
	float4 mPos : POSITION;
	float2 mTexCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 mPos : POSITION;
	float2 mTexCoord : TEXCOORD0;
};

VS_OUTPUT VtxProc(VS_INPUT input)
{
	VS_OUTPUT output;

	output.mPos = input.mPos;
	output.mTexCoord = input.mTexCoord;

	return output;
}

struct PS_INPUT
{
	float4 mPos : POSITION;
	float2 mTexCoord : TEXCOORD0;
};

struct PS_OUTPUT
{
	float4 mColor : COLOR0;
};

PS_OUTPUT PxlProc(PS_INPUT input)
{
	PS_OUTPUT output;

	output.mColor = tex2D(smp0, input.mTexCoord);
	output.mColor *= m_Color;

	if (0 != m_iMono)
	{
		output.mColor.a *= m_Color.a;
		output.mColor.r = m_Color.r;
		output.mColor.g = m_Color.g;
		output.mColor.b = m_Color.b;
	}

	return output;
}

technique Tech
{
	pass P0
	{
		VertexShader = compile vs_1_1 VtxProc();
		PixelShader = compile ps_2_0 PxlProc();
	}
}