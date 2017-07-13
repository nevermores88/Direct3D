static float m_Delta = 4.0F;
//static float m_Delta = 2000.0F;

// ≈ÿºø «¡∑ŒººΩÃ
float4 Main(float2 Pos : POSITION) : COLOR0
{
	float4 Out = (float4)0;

	Out.r = noise((Pos + 0) * m_Delta);
	Out.g = noise((Pos + 1) * m_Delta);
	Out.b = noise((Pos + 2) * m_Delta);

	Out = normalize(Out);

	Out = (Out + 1) * 0.5F;
	Out.w = 1;

	return Out;
}




////static float m_Delta = 4.0f;
//static float m_Delta = 2000.0f;
//
//struct TS_INPUT
//{
//	float2 pos : POSITION;
//};
//
//struct TS_OUTPUT
//{
//	vector diffuse : COLOR0;
//};
//
//TS_OUTPUT Main(TS_INPUT input)
//{
//	TS_OUTPUT output = (TS_OUTPUT)0;
//
//	output.diffuse.r = noise((input.pos + 0) * m_Delta);
//	output.diffuse.g = noise((input.pos + 1)*m_Delta);
//	output.diffuse.b = noise((input.pos + 2)*m_Delta);
//
//	output.diffuse = normalize(output.diffuse);
//
//	output.diffuse = (output.diffuse + 1) * 0.5f;
//	output.diffuse.w = 1;
//
//	return output;
//}