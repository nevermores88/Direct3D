matrix mtWorld;
matrix mtView;
matrix mtProj;


struct VS_INPUT
{
	vector position : POSITION;
	//vector diffuse : COLOR0;
	float2 tex0	: TEXCOORD0;
};

struct VS_OUTPUT
{
	vector position : POSITION;
	//vector diffuse : COLOR0;
	float2 tex0	: TEXCOORD0;
};

VS_OUTPUT Main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	vector p = input.position;

	p = mul(p, mtWorld);
	p = mul(p, mtView);
	p = mul(p, mtProj);

	output.position = p;
	//output.diffuse = input.diffuse;
	output.tex0 = input.tex0;

	return output;
}