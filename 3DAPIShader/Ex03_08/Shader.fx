float4x4 gWorld0;
float4x4 gWorld1;
float4x4 gView;
float4x4 gProj;
float4 gLightDir;

texture g_DiffuseTex;
sampler g_DiffuseSampler = sampler_state
{
	Texture = <g_DiffuseTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Wrap;
	AddressV = Wrap;
};

//pass0
struct VS_INPUT
{
	float4 m_Pos : POSITION;
	float4 m_Normal : NORMAL;
	float2 m_Tex : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 m_Pos : POSITION;
	float4 m_Diffuse : COLOR0;
	float2 m_Tex : TEXCOORD0;
};

VS_OUTPUT vs_main0(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float4 P = 0;
	float3 N = 0;
	float3 L = normalize(gLightDir);

	P = float4(input.m_Pos);
	P = mul(P, gWorld0);
	P = mul(P, gView);
	P = mul(P, gProj);

	N = mul(input.m_Normal, (float3x3)gWorld0);		//Rotation Normal Vector

	float D = 0;

	D = 0.5f * dot(N, L) + 0.5f;			//Lambert I = (V*L + 1) /2
	D = pow(D, 1.3f);

	output.m_Pos = P;
	output.m_Diffuse = D;
	output.m_Tex = input.m_Tex;

	return output;
}

struct PS_INPUT
{
	float4 m_Pos : POSITION;
	float4 m_Diffuse : COLOR0;
	float2 m_Tex : TEXCOORD0;
};

struct PS_OUTPUT
{
	float4 m_Diffuse : COLOR;
};

PS_OUTPUT ps_main0(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;
	output.m_Diffuse = input.m_Diffuse * tex2D(g_DiffuseSampler, input.m_Tex);
	return output;
}

//pass1
float4 g_vGlowColor;		// Glow Color
float3 g_vGlowAxis;		// Glow Axis
float  g_fGlowThick;		// Glow Thick

VS_OUTPUT vs_main1(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float4 P = input.m_Pos;
	float3 N = normalize(input.m_Normal);

	P += float4(N, 0) * g_fGlowThick;
	P = mul(P, gWorld1);
	P = mul(P, gView);
	P = mul(P, gProj);

	float Power = 0.0f;

	Power = (1 + dot(N, g_vGlowAxis)) * 0.6666f;		//Glow Color Intensity
	Power = pow(Power, 4.0f);

	output.m_Pos = P;
	output.m_Diffuse = g_vGlowColor * Power;
	output.m_Tex = input.m_Tex;

	return output;
}

PS_OUTPUT ps_main1(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;
	output.m_Diffuse = input.m_Diffuse;

	return output;
}

technique Tech0
{
	//Model Rendering
	pass P0
	{
		LIGHTING = FALSE;
		CULLMODE = NONE;

		ALPHABLENDENABLE = False;
		ZWRITEENABLE = TRUE;

		VertexShader = compile vs_3_0 vs_main0();
		PixelShader = compile ps_3_0	ps_main0();
	}

	//Glow Effect Rendering
	pass P1
	{
		LIGHTING = FALSE;
		CULLMODE = CW;

		ALPHABLENDENABLE = TRUE;
		SRCBLEND = SRCALPHA;
		DESTBLEND = DESTALPHA;
		ZWRITEENABLE = FALSE;

		VertexShader = compile vs_3_0 vs_main1();
		PixelShader = compile ps_3_0	ps_main1();
	}
}