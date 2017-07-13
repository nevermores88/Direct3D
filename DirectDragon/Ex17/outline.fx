extern matrix WorldViewMatrix;
extern matrix ProjMatrix;

static vector Black = { 0.0f, 0.0f, 0.0f, 0.0f };

struct VS_INPUT
{
	vector position : POSITION;
	vector normal : NORMAL0;
	vector faceNormal1 : NORMAL1;
	vector faceNormal2 : NORMAL2;
};

struct VS_OUTPUT
{
	vector position : POSITION;
	vector diffuse : COLOR;
};

VS_OUTPUT Main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	input.position = mul(input.position, WorldViewMatrix);

	// Compute a vector in the direction of the vertex
	// from the eye.  Recall the eye is at the origin
	// in view space - eye is just camera position.
	vector eyeToVertex = input.position;

	// transform normals to view space.  Set w
	// components to zero since we're transforming vectors.
	// Assume there are no scalings in the world
	// matrix as well.
	input.normal.w = 0.0f;
	input.faceNormal1.w = 0.0f;
	input.faceNormal2.w = 0.0f;

	input.normal = mul(input.normal, WorldViewMatrix);
	input.faceNormal1 = mul(input.faceNormal1, WorldViewMatrix);
	input.faceNormal2 = mul(input.faceNormal2, WorldViewMatrix);

	float dot0 = dot(eyeToVertex, input.faceNormal1);
	float dot1 = dot(eyeToVertex, input.faceNormal2);

	if ((dot0*dot1) < 0.0f)
	{
		input.position += 0.1f * input.normal;
	}

	output.position = mul(input.position, ProjMatrix);
	output.diffuse = Black;

	return output;
}