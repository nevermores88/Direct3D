struct VS_INPUT
{
	vector position : POSITION;
	vector normal : NORMAL;
	vector diffuse : COLOR;
	float2 uvCoords : TEXCOORD;
};

struct VS_OUTPUT
{
	vector position : POSITION;
	vector normal : TEXCOORD7; // ���� ���ʹ� ��� �������Ͱ� �����Ƿ� TEXCOORD �̿�
	vector diffuse : COLOR;
	float2 uvCoords : TEXCOORD0;
};

VS_OUTPUT Main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.position = input.position;
	output.diffuse = input.diffuse;

	return output;
}