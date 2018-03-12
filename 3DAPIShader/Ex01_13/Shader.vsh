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
#define Eye r2
#define Rfc r3
#define Phn r4
#define Lgt -c8
#define Cam c16
#define Pow c16.w

vs_1_1

def c24, 1, 0.5, 0.1, 1.0	//정점과 빛의 내적을 위한 상수

dcl_position v0
dcl_normal v1

m4x4 oPos, v0, c0
m4x4 Pos, v0, c4				//월드변환
m3x3 Nor, v1, c4				//법선 벡터는 회전만 적용

sub Eye, Cam, Pos
dp3 Eye.w, Eye, Eye						//E : Normalize Process. E Square = dot(r3, r3)
rsq Eye.w, Eye.w							//1/sqrt(Esq)
mul Eye.xyz, Eye.xyz, Eye.www		// Eye.x * 1/sqrt(Esq), Eye.y * 1/sqrt(Esq), Eye.z * 1/sqrt(Esq)	==> Unit Vector

// Determine Reflection Vector: R = 2 * dot(N, L)* N - L
dp3 Rfc.w, Nor, Lgt						//dot(N, L)
add Rfc.w, Rfc.w, Rfc.w					//*2
mul Rfc.xyz, Nor, Rfc.www				//*N
sub Rfc, Rfc, Lgt							//-L

dp3 Phn.w, Eye, Rfc						//dot(E, R)
add Phn.w, Phn.w, c24.x				//limit [0, 1]
mul Phn.w, Phn.w, c24.y
max Phn.w, Phn.w, c24.z
min Phn.w, Phn.w, c24.w

//pow(r3, power)
//pow r3.x, r3.x, c16.w //vs_2_0
// = exp (src1 * log(src0))
log Phn.w, Phn.w
mul Phn.w, Phn.w, Pow
exp Phn.w, Phn.w

mul Phn, c10, Phn.w						//Color = Phong 반사 * 빛의 색상

mov Phn.w, c24.w						//Color.w = 1.0f
mov oD0, Phn								//Output Diffuse Color