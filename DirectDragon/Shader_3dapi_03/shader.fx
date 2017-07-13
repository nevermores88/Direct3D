float4 diffuse;

void Main(in float3 iPos : POSITION
	, out float4 oPos : POSITION
	, out float4 oD0 : COLOR) // float4 앞에 uniform을 붙이면 에러 없어짐
{
	oPos = float4(iPos, 1);
	oD0 = diffuse;
}