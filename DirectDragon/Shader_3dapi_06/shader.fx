float4x4 mtWorld : register(c0);
float4 vcLight: register(c10);

vector diffuse : COLOR;

void Main(in float3 iposition : POSITION
	, in float4 idiffuse : COLOR
	, out float4 oposition : POSITION
	, out float4 odiffuse : COLOR)
{
	oposition = float4(iposition, 1);
	odiffuse = idiffuse;
	//odiffuse = diffuse;
}