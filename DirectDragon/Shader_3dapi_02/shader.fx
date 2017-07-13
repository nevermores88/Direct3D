float4 diffuse = float4(0, 1, 1, 1);

void Main(in float3 iPos : POSITION
	, out float4 oPos : POSITION
	, out float4 oD0 : COLOR)
{
	float4 Dif = float4(1.0f, 1.0f, 0.0f, 1.0f);
		oPos = float4(iPos, 1);
	oD0 = Dif;
}