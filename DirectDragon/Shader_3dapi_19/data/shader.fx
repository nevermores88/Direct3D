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
	float4 Out = 0.0f;
	float4 MonoColor = { 0.5f, 1.0f, 2.0f, 1.0f };				// �ܻ�ȭ ����
	float4 MonoWeight = { 0.299f, 0.587f, 0.114f, 0.0f };	// �ܻ�ȭ ����

	Out = tex2D(SampDif, input.base);	// ���ø�

	Out = dot(Out, MonoWeight);	// ����(dot)�� �ܻ�ȭ
	Out *= MonoColor;					// �ܻ�ȭ ������ ����

	Out *= 3.0f;							// Aux: ��ü ��⸦ �ø�
	Out.a = 1.0f;							// ������ �ȼ��� ����

	PS_OUTPUT ret;
	ret.diffuse = Out;//float4(1.0f, 0.0f, 0.0f, 1.0f);
	return ret;
}