float4x4	g_mtWorld;
float4x4	g_mtView;
float4x4	g_mtProj;

float4 g_vNormal;	
float4 g_vColor;

//pass0
struct VS_INPUT
{
	float4 m_Pos : POSITION;
	float3 m_Normal : NORMAL;
};

struct VS_OUTPUT
{
	float4 m_Pos : POSITION;
	float3 m_Normal : TEXCOORD7;	// Normal Vector
};

VS_OUTPUT vs_main0(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float4 P = 0;
		float3 N = 0;

	P = input.m_Pos;
	P = mul(P, g_mtWorld);
	P = mul(P, g_mtView);
	P = mul(P, g_mtProj);

	N = mul(input.m_Normal, (float3x3) g_mtWorld);

	output.m_Pos = P;
	output.m_Normal = N;

	return output;
}

struct PS_INPUT
{
	float4 m_Pos : POSITION;
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
	float3 L = normalize(g_vNormal);

	float4 D = 0;

	D = saturate(dot(N, L));
	D = pow(D, 10.0);
	D *= 1.5f;
	D *= g_vColor;

	output.m_Diffuse = D;
	//output.m_Diffuse = float4(0.5, 0.5, 0.9, 1.0);
	return output;
}


technique Tech0
{
	pass P0
	{
		LIGHTING = FALSE;
		/*CULLMODE = CW;

		ALPHABLENDENABLE = TRUE;
		ZWRITEENABLE = TRUE;
		SRCBLEND = ONE;
		DESTBLEND = ONE;*/

		VertexShader = compile vs_3_0 vs_main0();
		//PixelShader = compile ps_3_0 ps_main();
		PixelShader = compile ps_3_0 ps_main0();
	}

	pass P1
	{
		/*LIGHTING = FALSE;
		CULLMODE = CW;

		ALPHABLENDENABLE = TRUE;
		ZWRITEENABLE = FALSE;
		SRCBLEND = ONE;
		DESTBLEND = ONE;*/

		VertexShader = compile vs_3_0 vs_main0();
		PixelShader = compile ps_3_0 ps_main0();
	}
}
