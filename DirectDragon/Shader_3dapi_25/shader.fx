

static float m_fEspX = 4;
static float m_fEspY = 3;

struct TS_INPUT
{
	vector pos : POSITION;
};

struct TS_OUTPUT
{
	vector diffuse : COLOR0;
};


TS_OUTPUT Main(TS_INPUT input)
{
	TS_OUTPUT output = (TS_OUTPUT)0;

	output.diffuse.r = 1;
	output.diffuse.g = 1;
	output.diffuse.b = 1;
	output.diffuse.a = 1;

	int R = int(input.pos.x * m_fEspX);
	int G = int(input.pos.y*m_fEspY);

	output.diffuse.r = 1 - float(R)/(m_fEspX - 1);
	output.diffuse.g = 1 - float(G)/(m_fEspY - 1);

	return output;
}