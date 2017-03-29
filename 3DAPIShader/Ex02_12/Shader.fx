float4x4 m_mtWorld;	// World Matrix
float4x4 m_mtView;	// View Matrix
float4x4 m_mtProj;	// Projection Matrix

float4	m_FogColor;	// Fog 색상
float	m_FogEnd;	// Fog 끝 값
float	m_FogBgn;	// Fog 시작 값

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

	// 정점의 변환: 월드, 뷰
	vcOut = float4(In.Pos, 1);
	vcOut = mul(vcOut, m_mtWorld);
	vcOut = mul(vcOut, m_mtView);

	// 정점의 뷰 변환 과정의 z/(Fog 끝 값 - Fog 시작 값)을 Fog Factor로 저장
	FogFactor = vcOut.z / (m_FogEnd - m_FogBgn);

	if (FogFactor > 1.0f)
		FogFactor = 1.0f;

	// 정점의 정규 변환
	vcOut = mul(vcOut, m_mtProj);

	// 출력 Diffuse를 정점 Diffuse와 포그 색상과 혼합
	float4 Fog = m_FogColor * FogFactor + In.Dif * (1.0 - FogFactor);

		// 혼합 값을 출력 색상으로 지정
	Out.Dif = Fog;


	Out.Pos = vcOut;


	return Out;
}
