matrix WorldMatrix;
matrix ViewMatrix;
matrix ProjMatrix;
vector FogColor;
float FogEnd;
float FogBegin;

struct VS_INPUT
{
	vector position : POSITION;
	vector diffuse : COLOR0;
};

struct VS_OUTPUT
{
	vector position : POSITION;
	vector diffuse : COLOR0;
};

VS_OUTPUT Main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	vector p = input.position;

	p = mul(p, WorldMatrix);
	p = mul(p, ViewMatrix);

	float fFogFactor;

	// ������ �� ��ȯ ������ z/(Fog �� �� - Fog ���� ��)�� Fog Factor�� ����
	fFogFactor = p.z / (FogEnd - FogBegin);

	p = mul(p, ProjMatrix);

	//��� Diffuse ���� = Fog ���� * Fog Factor + ���� Diffuse * (1 - Fog Factor)
	vector Fog = FogColor * fFogFactor + input.diffuse * (1.0f - fFogFactor);

	output.diffuse = Fog;
	output.position = p;

	return output;
}