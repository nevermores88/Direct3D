matrix  WorldMatrix;
matrix  ViewMatrix;
matrix  ProjMatrix;

matrix  RotMatrix;

vector  LightDirection;
vector  LightDiffuse;

static float SatA = 0.5f;
static float SatB = 0.5f;

struct VS_INPUT
{
	vector position : POSITION;
	vector normal : NORMAL0;
};

struct VS_OUTPUT
{
	vector position : POSITION;
	vector diffuse : COLOR0;
};

VS_OUTPUT Main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	vector p;
	vector n;

	p = input.position;
	p = mul(p, WorldMatrix);
	p = mul(p, ViewMatrix);
	p = mul(p, ProjMatrix);

	n = mul(input.normal, RotMatrix);

	vector light = -LightDirection;

	float reflectIntensity = saturate(SatA*dot(n, light) + SatB);
	//float reflectIntensity = saturate(dot(n, light));
	output.position = p;
	if (reflectIntensity < 0)
		output.diffuse.r = 1.0f;
	else
		output.diffuse = LightDiffuse * reflectIntensity;

	return output;
}