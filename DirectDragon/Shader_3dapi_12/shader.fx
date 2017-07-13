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

	// 정점의 뷰 변환 과정의 z/(Fog 끝 값 - Fog 시작 값)을 Fog Factor로 저장
	fFogFactor = p.z / (FogEnd - FogBegin);

	p = mul(p, ProjMatrix);

	//출력 Diffuse 색상 = Fog 색상 * Fog Factor + 정점 Diffuse * (1 - Fog Factor)
	vector Fog = FogColor * fFogFactor + input.diffuse * (1.0f - fFogFactor);

	output.diffuse = Fog;
	output.position = p;

	return output;
}