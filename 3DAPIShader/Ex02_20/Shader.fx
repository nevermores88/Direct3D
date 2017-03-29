sampler2D DiffuseSampler;

struct PS_INPUT
{
	float2 mTexCoord : TEXCOORD0;
};

struct PS_OUTPUT
{
	float4 mDiffuse : COLOR;
};

PS_OUTPUT PxlPrc(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;

	// 흐림 효과의 픽셀은 인접한 픽셀에 비중을 곱한 값을 누적해서 결정
	// 텍스처 좌표를 변화시키면서 인접한 픽셀을 샘플링
	// Gaussian 분포: exp (x*x/Delta) 값을 픽셀의 비중으로 설정
	for (int x = -4; x <= 4; x++)
	{
		float2 T = input.mTexCoord;
		T.x += (2.0f * x) / 1024.0f;			// 텍스처 좌표를 변화시킨다
		output.mDiffuse += tex2D(DiffuseSampler, T) * exp((-x*x) / 8.0f);		// 픽셀을 샘플링하고 가우스 분포 함수로 구한 비중 값을 곱한 후 더한다.
	}

	output.mDiffuse *= 0.24f;		// 전체 명도를 낮춤
	output.mDiffuse.a = 1.0f;			//불투명 픽셀로 설정

	return output;
}