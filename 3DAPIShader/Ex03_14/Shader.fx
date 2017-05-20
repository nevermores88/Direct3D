float4x4 g_mtWorldViewProj;
float3 g_vLightDir;

struct VS_INPUT
{
	float4 mPosition : POSITION;
	float4 mNormal : NORMAL;
};

struct VS_OUTPUT
{
	float4 mPosition : POSITION;
	float4 mColor : COLOR0;
};

VS_OUTPUT VtxPrc(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float4 position = mul(input.mPosition, g_mtWorldViewProj);
	////float3 normal = mul(input.mNormal, m_mtRot)	회전 안시킬거니깐 상관없음
	float3 lightDir = -g_vLightDir;
	float4 color;

	color = (0.5f + dot(input.mNormal, lightDir)) * 0.6667f;
	//color = dot(input.mNormal, lightDir);
	color.a = 1.0f;

	output.mPosition = position;
	output.mColor = color;

	return output;
}

technique Tech0
{
	pass P0
	{
		VertexShader = compile vs_2_0 VtxPrc();
	}
}

//float4x4	m_mtWVP;	// World * View * Projection
//float4x4	m_mtRot;	// Rotation Matrix
//float3		m_vcLgt;	// Lighting Direction
//
//
//// Output Vertex Processing Structure
//struct SVsOut
//{
//	float4 Pos	: POSITION;
//	float4 Dff	: COLOR0;
//};
//
//
//// Vertex Shader Processing
//SVsOut VtxPrc(float4 Pos : POSITION,	// Local Position
//	float4 Nor : NORMAL		// Normal Vector
//	)
//{
//	SVsOut Out = (SVsOut)0;						// Initialize to Zero
//
//	float4 P = mul(Pos, m_mtWVP);				// Transform Rotation, Scaling, Translation of Vertex Position
//		float3 N = mul(Nor, (float3x3)m_mtRot);		// Rotation Normal Vector
//		float3 L = -m_vcLgt;						// Lighting Direction Vector
//		float4 D;									// Output Color
//
//	D = (0.5f + dot(N, L)) * 0.6667f;			// Lambert
//	D.a = 1.f;
//
//	Out.Pos = P;		// Output Position
//	Out.Dff = D;		// Output Color
//
//	return Out;
//}
//
//technique Tech0
//{
//	pass P0
//	{
//		VertexShader = compile vs_2_0 VtxPrc();
//	}
//}
