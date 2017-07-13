static int m_nType = 2;

struct TS_INPUT
{
	float2 pos : POSITION;
};

struct TS_OUTPUT
{
	vector diffuse : COLOR0;
};

TS_OUTPUT Main(TS_INPUT input)
{
	float2 Pos = input.pos;

	vector output = { 1, 1, 1, 1 };
	float2 P;

	// Circle
	if (0 == m_nType)
	{
		P = Pos;
		P -= float2(0.5f, 0.5f);

		float r = sqrt(P.x*P.x + P.y *P.y);
		if (r < 0.5f)
		{
			output.r = Pos.x; 
			output.g = Pos.y;
		}
	}
	// Rectangle
	else if (1 == m_nType)
	{
		float2 Delta = Pos;
			Delta = float2(0.5f, 0.5f) - Delta;
		Delta = (Delta + 1)*0.5f;

		output.rg = Delta.xy;
	}
	else if (2 == m_nType)
		output.rg = 1 - Pos.xy;

	TS_OUTPUT Out = (TS_OUTPUT)0;
	Out.diffuse = output;

	return Out;
}