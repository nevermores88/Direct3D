sampler2D DiffuseSampler;

struct PS_INPUT
{
	float2 mTexCoord : TEXCOORD0;
};

struct PS_OUTPUT
{
	float4 mDiffuse : COLOR0;
};

PS_OUTPUT PxlPrc(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;

	float4 MonoColor = { 0.5f, 1.0f, 2.0f, 1.0f };	//�ܻ�ȭ ����
	float4 MonoWeight = { 0.299f, 0.587f, 0.114f, 0.0f };	//�ܻ�ȭ ����

	output.mDiffuse = tex2D(DiffuseSampler, input.mTexCoord);

	output.mDiffuse = dot(output.mDiffuse, MonoWeight);		//�������� �ܻ�ȭ�� ���� ���
	output.mDiffuse *= MonoColor;		//�ܻ�ȭ ������ ����

	output.mDiffuse *= 3.0f;
	output.mDiffuse *= 1.0f;

	return output;

}