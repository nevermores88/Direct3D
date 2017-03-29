sampler2D DiffuseSampler;

struct PS_INPUT
{
	float2 mTexCoord : TEXCOORD0;
};

struct PS_OUTPUT
{
	float4 mDiffuse : COLOR;
};

PS_OUTPUT PxlPrc(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;

	// �帲 ȿ���� �ȼ��� ������ �ȼ��� ������ ���� ���� �����ؼ� ����
	// �ؽ�ó ��ǥ�� ��ȭ��Ű�鼭 ������ �ȼ��� ���ø�
	// Gaussian ����: exp (x*x/Delta) ���� �ȼ��� �������� ����
	for (int x = -4; x <= 4; x++)
	{
		float2 T = input.mTexCoord;
		T.x += (2.0f * x) / 1024.0f;			// �ؽ�ó ��ǥ�� ��ȭ��Ų��
		output.mDiffuse += tex2D(DiffuseSampler, T) * exp((-x*x) / 8.0f);		// �ȼ��� ���ø��ϰ� ���콺 ���� �Լ��� ���� ���� ���� ���� �� ���Ѵ�.
	}

	output.mDiffuse *= 0.24f;		// ��ü ���� ����
	output.mDiffuse.a = 1.0f;			//������ �ȼ��� ����

	return output;
}