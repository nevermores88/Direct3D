static float m_fEspX = 4;
static float m_fEspY = 3;

struct TS_INPUT
{
	//절차적 텍스쳐에 대한 쉐이더를 작성할때는 TEXCOORD 가 아닌 POSITION 시멘틱 사용
	//픽셀의 좌표는 [0, 1] 범위
	float3 mPosition : POSITION;
};

//텍셀 프로세싱
float4 TxlPrc(TS_INPUT input) : COLOR0
{
	float4 output = { 1, 1, 1, 1 };

	int R = int(input.mPosition.x * m_fEspX);
	int G = int(input.mPosition.y * m_fEspY);

	output.r = 1 - float(R) / (m_fEspX - 1);
	output.g = 1 - float(G) / (m_fEspY - 1);

	return output;
}