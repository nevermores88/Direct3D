
matrix WorldMatrix;
matrix ViewMatrix;
matrix ProjMatrix; 

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
	vector p;

	p = input.position;

	p = mul(p, WorldMatrix);
	p = mul(p, ViewMatrix);
	p = mul(p, ProjMatrix);

	output.position = p;
	output.diffuse = input.diffuse;

	return output;
}

//matrix WorldMatrix;
//matrix ViewMatrix;
//matrix ProjMatrix;
//
//
//struct VS_INPUT
//{
//	vector position : POSITION;
//	vector diffuse : COLOR0;
//};
//
//struct VS_OUTPUT
//{
//	vector position : POSITION;
//	vector diffuse : COLOR0;
//};
//
//VS_OUTPUT Main(VS_INPUT input)
//{
//	VS_OUTPUT output = (VS_OUTPUT)0;
//
//	vector p = input.position;
//
//	p = mul(p, WorldMatrix);
//	p = mul(p, ViewMatrix);
//	p = mul(p, ProjMatrix);
//
//	output.position = p;
//	output.diffuse = input.diffuse;
//
//	return output;
//}