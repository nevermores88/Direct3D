matrix WorldMatrix;
matrix ViewMatrix;
matrix ProjMatrix;
matrix RotMatrix;

vector LightDirection;
vector LightDiffuse;
vector Camera;
float Sharp;

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
	vector x;
	vector n;

	p = input.position;
	p = mul(p, WorldMatrix);
	x = p;
	p = mul(p, ViewMatrix);
	p = mul(p, ProjMatrix);

	n = mul(input.normal, RotMatrix);

	vector L = -LightDirection;
	vector E = normalize(Camera - x);
	vector R = reflect(LightDirection, n);

	float Phong = saturate(dot(R, E));

	Phong = pow(Phong, Sharp);

	output.position = p;
	output.diffuse = LightDiffuse * Phong;

	return output;
}