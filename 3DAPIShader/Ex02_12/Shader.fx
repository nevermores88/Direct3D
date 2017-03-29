float4x4 m_mtWorld;	// World Matrix
float4x4 m_mtView;	// View Matrix
float4x4 m_mtProj;	// Projection Matrix

float4	m_FogColor;	// Fog ����
float	m_FogEnd;	// Fog �� ��
float	m_FogBgn;	// Fog ���� ��

struct VS_INPUT
{
	float3 Pos : POSITION;	// dcl_position0
	float4 Dif : COLOR0;	// dcl_color0
};

// For Vertex Processing Output
struct VS_OUTPUT
{
	float4 Pos : POSITION;	// oPos
	float4 Dif : COLOR0;	// oD0
};

VS_OUTPUT VtxPrc(VS_INPUT In)
{
	float4	vcOut;
	float FogFactor;

	VS_OUTPUT Out = (VS_OUTPUT)0;				// Initialized to 0

	// ������ ��ȯ: ����, ��
	vcOut = float4(In.Pos, 1);
	vcOut = mul(vcOut, m_mtWorld);
	vcOut = mul(vcOut, m_mtView);

	// ������ �� ��ȯ ������ z/(Fog �� �� - Fog ���� ��)�� Fog Factor�� ����
	FogFactor = vcOut.z / (m_FogEnd - m_FogBgn);

	if (FogFactor > 1.0f)
		FogFactor = 1.0f;

	// ������ ���� ��ȯ
	vcOut = mul(vcOut, m_mtProj);

	// ��� Diffuse�� ���� Diffuse�� ���� ����� ȥ��
	float4 Fog = m_FogColor * FogFactor + In.Dif * (1.0 - FogFactor);

		// ȥ�� ���� ��� �������� ����
	Out.Dif = Fog;


	Out.Pos = vcOut;


	return Out;
}
