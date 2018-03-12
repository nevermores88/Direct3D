//N : Vertex Normal Vector
//L : (-)Light Direction
//E : Direction Of (Eye - Vertex Position)
//R : Reflection Vector
//H : Half Vector

//Phong Shading
//R = 2*dot(N, L)*N - L
//I = dot(R, E)^power

//Blinn-Phong Shading
//H = (E+L)/|E+L|
//I = dot(N, H) ^ power

//c0 : World * View * Projection Matrix
//c4 : Rotation Matrix
//c8 : Lighting Direction
//c10 : Lighting Color
//c16 : Camera Eye

//Application Phong Shading

#define Pos r0
#define Nor r1
#define Lmb r2
#define Eye r3
#define Rfc r4
#define Phn r5
#define Lgt -c8
#define Cam c16
#define Pow c16.w
#define Out r0

vs_1_1

def c24, 1, 0.5, 0.1, 1.0

dcl_position v0
dcl_normal v1

m4x4 oPos, v0, c0					// 출력 위치 = 입력 위치 * c0에 입력된 행렬
m4x4 Pos, v0, c4						// 월드 변환
m3x3 Nor, v1, c4						// 법선 벡터는 회전만 적용

//Lambertian reflectance
dp3 Lmb.w, Nor, Lgt					// dot(N, -L)
add Lmb.w, Lmb.w, c24.x			// (Dot + 1)* 0.5로 설정해서 0~1.0사이의 값으로 조정
mul Lmb.w, Lmb.w, c24.y
max Lmb.w, Lmb.w, c24.z
min Lmb.w, Lmb.w, c24.w

sub Eye, Cam, Pos					// Determine E = Camera Pos - Vertex Postion(Transformed)
dp3 Eye.w, Eye, Eye
rsq Eye.w, Eye.w
mul Eye.xyz, Eye.xyz, Eye.www

// Determine Reflection Vector: R = 2 * dot(N, L)* N - L
dp3 Rfc.w, Nor, Lgt					//dot(N, L)
add Rfc.w, Rfc.w, Rfc.w				//*2
mul Rfc.xyz, Nor, Rfc.www			//*N
sub Rfc, Rfc, Lgt						//-L

dp3 Phn.w, Eye, Rfc					//dot(E, R)
add Phn.w, Phn.w, c24.x			//limit[0, 1]
mul Phn.w, Phn.w, c24.y
max Phn.w, Phn.w, c24.z
min Phn.w, Phn.w, c24.w

//pow(r3, power)
//pow r3.x, r3.x, c16.w				//vs_2_0
//= exp(src1 * log(src0))	
log Phn.w, Phn.w
mul Phn.w, Phn.w, Pow
exp Phn.w, Phn.w

add Out, Lmb.w, Phn.w				//Output = Lambert + Phong
mul Out, c10, Out					//Color = Lighting * Output
mov Out.w, c24.w					//Color.w = 1.0f;

mov oD0, Out		