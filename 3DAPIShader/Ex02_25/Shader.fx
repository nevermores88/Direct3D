static float m_fEspX = 4;
static float m_fEspY = 3;

struct TS_INPUT
{
	//������ �ؽ��Ŀ� ���� ���̴��� �ۼ��Ҷ��� TEXCOORD �� �ƴ� POSITION �ø�ƽ ���
	//�ȼ��� ��ǥ�� [0, 1] ����
	float3 mPosition : POSITION;
};

//�ؼ� ���μ���
float4 TxlPrc(TS_INPUT input) : COLOR0
{
	float4 output = { 1, 1, 1, 1 };

	int R = int(input.mPosition.x * m_fEspX);
	int G = int(input.mPosition.y * m_fEspY);

	output.r = 1 - float(R) / (m_fEspX - 1);
	output.g = 1 - float(G) / (m_fEspY - 1);

	return output;
}