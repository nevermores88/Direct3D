//
// Shader Example.
// Copyleft (c) 2007 Heesung Oh(Galic Studio). All rights Not reserved.
////////////////////////////////////////////////////////////////////////////

sampler	SampDif : register(s0);

float4 PxlPrc(float2 Tx0 : TEXCOORD0 /* Texture Coordinate */) : COLOR
{
	float4	Out=0.0F;
	float4	MonoColor ={0.5F, 1.0F, 2.0F, 1.0F};		// �ܻ�ȭ ����
	float4	MonoWeight={0.299F, 0.587F, 0.114F, 0.0F};	// �ܻ�ȭ ����

	Out = tex2D( SampDif, Tx0 );	// ���ø�

	Out = dot(Out, MonoWeight);		// ����(dot)�� �ܻ�ȭ
	Out *= MonoColor;				// �ܻ�ȭ ������ ����

	Out *= 3.0F;					// Aux: ��ü ��⸦ �ø�
	Out.a = 1.0F;					// ������ �ȼ��� ����

	return Out;
}

