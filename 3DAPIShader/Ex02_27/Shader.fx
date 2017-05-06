//static int m_nType = 2;
//
//struct TS_INPUT
//{
//	//������ �ؽ��Ŀ� ���� ���̴��� �ۼ��Ҷ��� TEXCOORD �� �ƴ� POSITION �ø�ƽ ���
//	//�ȼ��� ��ǥ�� [0, 1] ����
//	float2 mPosition : POSITION;
//};
//
//float4 TxlPrc(TS_INPUT input) : COLOR0
//{
//	float4 output = { 1, 1, 1, 1 };
//	float2 pos;
//
//	//Circle
//	if (0 == m_nType)
//	{
//		pos = input.mPosition;
//		pos -= float2(0.5f, 0.5f);
//
//		float r = sqrt(pos.x * pos.x + pos.y + pos.y);
//		if (r < 0.5f)
//		{
//			output.r = input.mPosition.x;
//			output.g = input.mPosition.y;
//		}
//	}
//	//Rectangle
//	else if (1 == m_nType)
//	{
//		float2 Delta = input.mPosition;
//		Delta = float2(0.5f, 0.5f) - Delta;
//		Delta = (Delta + 1) * 0.5f;
//
//		output.rg = Delta.xy;
//	}
//	else if (2 == m_nType)
//	{
//		output.rg = 1 - input.mPosition.xy;
//	}
//
//	return output;
//}

static int	m_nType = 2;

// �ؼ� ���μ���
//////////////////////////////////////////////////////
float4 TxlPrc(float2 Pos : POSITION) : COLOR0
{
	float4 Out = { 1, 1, 1, 1 };
	float2 P;

	// Circle
	if (0 == m_nType)
	{
		P = Pos;
		P -= float2(0.5F, 0.5F);

		float r = sqrt(P.x * P.x + P.y * P.y);
		if (r<.5f)
		{
			Out.r = Pos.x;
			Out.g = Pos.y;
		}
	}

	// Rectangle
	else if (1 == m_nType)
	{
		float2 Delta = Pos;
			Delta = float2(0.5F, 0.5F) - Delta;
		Delta = (Delta + 1) * 0.5F;

		Out.rg = Delta.xy;
	}

	// etc
	else if (2 == m_nType)
		Out.rg = 1 - Pos.xy;

	return Out;
}
