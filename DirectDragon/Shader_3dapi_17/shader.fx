int ProcType;

vector PixelInverse(vector input)
{
	return 1 - input;
}

vector PixelMonotone(vector input)
{
	vector output = 0.0f;
	vector d = vector(0.299, 0.587, 0.114, 0);
	vector t = input;

	output = dot(d, t);
	output.w = 1;

	return output;
}

struct PS_INPUT
{
	vector diffuse : COLOR;
};

struct PS_OUTPUT
{
	vector diffuse : COLOR;
};

PS_OUTPUT Main(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;
	//vector output = vector(0, 0, 0, 1);

	if (ProcType == 1)
		output.diffuse = PixelInverse(input.diffuse);
	else if (ProcType == 2)
		output.diffuse = PixelMonotone(input.diffuse);
	else
		output.diffuse = input.diffuse;

	return output;
}