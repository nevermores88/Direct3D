static float m_Delta = 4.0F;
//static float m_Delta = 2000.0F;

struct TS_INPUT
{
	//������ �ؽ��Ŀ� ���� ���̴��� �ۼ��Ҷ��� TEXCOORD �� �ƴ� POSITION �ø�ƽ ���
	//�ȼ��� ��ǥ�� [0, 1] ����
	float2 mPosition : POSITION;
};

//�ؼ� ���μ���
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
