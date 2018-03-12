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

#define	Pos	r0
#define	Nor	r1
#define	Lmb	r2
#define	Eye	r3
#define	Hlf		r4
#define	Bln	r5
#define Lgt	-c8
#define Cam	c16
#define Pow	c16.w
#define	Out	r0

vs_1_1

def c24, 1, 0.5, 0.1, 1.0

dcl_position v0
dcl_normal v1

m4x4 oPos, v0, c0
m4x4 Pos, v0, c4
m3x3 Nor, v1, c4

// Lambertian reflectance
dp3 Lmb.w, Nor, Lgt
add Lmb.w, Lmb.w, c24.x
mul Lmb.w, Lmb.w, c24.y
max Lmb.w, Lmb.w, c24.z
min Lmb.w, Lmb.w, c24.w

sub Eye, Cam, Pos
dp3 Eye.w, Eye, Eye
rsq Eye.w, Eye.w
mul Eye.xyz, Eye.xyz, Eye.www

// Determine Half Vector: H = E+L/|E+L|
add Hlf, Eye, Lgt
dp3 Hlf.w, Hlf, Hlf
rsq Hlf.w, Hlf.w
mul Hlf.xyz, Hlf.xyz, Hlf.www

dp3 Bln.w, Hlf, Nor
add Bln.w, Bln.w, c24.x
mul Bln.w, Bln.w, c24.y
max Bln.w, Bln.w, c24.z
min Bln.w, Bln.w, c24.w

// pow(r3, power)
// pow r3.x, r3.x, c16.w	// vs_2_0
// = exp (src1 * log(src0)).
log Bln.w, Bln.w
mul Bln.w, Bln.w, Pow
exp Bln.w, Bln.w

add Out, Lmb.w, Bln.w		//Output= Lambert + Phong
mul Out, c10, Out			//Color = Lighting Color * Output
mov Out.w, c24.w			//Color.w = 1.0f

mov oD0, Out