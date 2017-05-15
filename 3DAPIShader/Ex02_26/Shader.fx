static float m_Delta = 4.0F;
//static float m_Delta = 2000.0F;

struct TS_INPUT
{
	//절차적 텍스쳐에 대한 쉐이더를 작성할때는 TEXCOORD 가 아닌 POSITION 시멘틱 사용
	//픽셀의 좌표는 [0, 1] 범위
	float2 mPosition : POSITION;
};

//텍셀 프로세싱
float4 TxlPrc(TS_INPUT input) : COLOR0
{
	float4 output = (float4)0;

	output.r = noise((input.mPosition + 0) * m_Delta);
	output.g = noise((input.mPosition + 1) * m_Delta);
	output.b = noise((input.mPosition + 2) * m_Delta);

	output = normalize(output);

	//output = (output + 1) * 0.5f;
	//output.w = 1;

	return output;
}
