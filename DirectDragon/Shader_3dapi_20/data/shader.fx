sampler SampDif : register(s0);

struct PS_INPUT
{
	float2 base : TEXCOORD0;
};

struct PS_OUTPUT
{
	vector diffuse : COLOR0;
};

PS_OUTPUT Main(PS_INPUT input)
{
	PS_OUTPUT Out = (PS_OUTPUT)0;

	// �帲 ȿ���� �ȼ��� ������ �ȼ��� ������ ���� ���� �����ؼ� ����
	// �ؽ�ó ��ǥ�� ��ȭ��Ű�鼭 ������ �ȼ��� ���ø�
	// Gaussian ����: exp (x*x/Delta) ���� �ȼ��� �������� ����
	for (int x = -4; x <= 4; ++x)
	{
		float2 T = input.base;
		T.x += (2.f * x) / 1024.f;					// �ؽ�ó ��ǥ�� ��ȭ��Ų��
		Out.diffuse += tex2D(SampDif, T) * exp((-x*x) / 8.f);		// �ȼ��� ���ø��ϰ� ���콺 ���� �Լ��� ���� ���� ���� ���� �� ���Ѵ�.
	}

	Out.diffuse *= 0.24f;				// ��ü ���� ����
	Out.diffuse.a = 1.0f;				// ������ �ȼ��� ����

	return Out;
}