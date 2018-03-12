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
#define Hlf r3
#define Bln r4
#define Lgt -c8
#define Cam c16
#define Pow c16.w

vs_1_1

def c24, 1, 0.5, 0.1, 1.0

dcl_position v0
dcl_normal v1

m4x4 oPos, v0, c0
m4x4 Pos, v0, c4
m3x3 Nor, v1, c4

sub Eye, Cam, Pos
dp3 Eye.w, Eye, Eye					//Eye vector ¡§±‘»≠
rsq Eye.w, Eye.w
mul Eye.xyz, Eye.xyz, Eye.w

add Hlf, Eye, Lgt
dp3 Hlf.w, Hlf, Hlf
rsq Hlf.w, Hlf.w
mul Hlf.xyz, Hlf.xyz, Hlf.www

dp3 Bln.w, Hlf, Nor
add Bln.w, Bln.w, c24.w			//limit[0, 1]
mul Bln.w, Bln.w, c24.y
max Bln.w, Bln.w, c24.z
min Bln.w, Bln.w, c24.w

log Bln.w, Bln.w
mul Bln.w, Bln.w, Pow
exp Bln.w, Bln.w

mul Bln, c10, Bln.w
mov Bln.w, c24.w
mov oD0, Bln