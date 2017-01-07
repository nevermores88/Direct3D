float4x4 g_mtWorld0;
float4x4 g_mtView;
float4x4 g_mtProj;
float3x3 g_mtRot;
float4 g_vLightDir;
float4 g_vCamZ;

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
	float2 m_Tex : TEXCOORD0;			// Diffuse Map Coordinate
	float3 m_Normal : TEXCOORD7;	// Normal Vector
};

VS_OUTPUT vs_main0(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float4 P = mul(input.m_Pos, g_mtWorld0);
	P = mul(P, g_mtView);
	P = mul(P, g_mtProj);

	float3 N = mul(input.m_Normal, (float3x3)g_mtRot);
	output.m_Pos = P;
	output.m_Normal = N;
	output.m_Tex = input.m_Tex;

	return output;
}

struct PS_INPUT
{
	float4 m_Pos : POSITION;
	float4 m_Tex : TEXCOORD0;			// Diffuse Map Coordinate
	float3 m_Normal : TEXCOORD7;	// Normal Vector
};

struct PS_OUTPUT
{
	float4 m_Diffuse : COLOR;
};

PS_OUTPUT ps_main0(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;
	
	float3 N = normalize(input.m_Normal);
	float3 L = normalize(g_vLightDir);

	output.m_Diffuse = (1 + dot(N, L)) * 0.5f;
	output.m_Diffuse *= tex2D(g_DiffuseSampler, input.m_Tex);

	return output;
}

//pass1
VS_OUTPUT vs_main1(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float4 P = mul(input.m_Pos, g_mtWorld0);
	float3 N = mul(input.m_Normal, (float3x3)g_mtRot);

	P += float4(N, 0) * 4.0f;
	P = mul(P, g_mtView);
	P = mul(P, g_mtProj);

	output.m_Pos = P;
	output.m_Normal = N;
	output.m_Tex = input.m_Tex;

	return output;
}

PS_OUTPUT ps_main1(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;
	
	float3 N = normalize(input.m_Normal);
	float4 G = float4(2.0f, 2.0f, 0.2f, 1.0f);
	float3 A = float3(-1.0f, 0.0f, 0.0f);		//GlowAxis
	float Power = 0;

	A = normalize(g_vCamZ);

	Power = saturate(dot(N, A) + 1);
	Power *= 0.9f;
	Power = pow(Power, 4.0f);
	output.m_Diffuse = G * Power;

	return output;
}

PS_OUTPUT ps_main2(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;

	output.m_Diffuse = tex2D(g_DiffuseSampler, input.m_Tex);

	return output;
}

technique Tech0
{
	//Model Rendering
	pass P0
	{
		LIGHTING = FALSE;
		CULLMODE = CCW;
		ALPHABLENDENABLE = FALSE;
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