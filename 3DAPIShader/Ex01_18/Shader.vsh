//  c0: World * View * Projection Matrix
//  c4: Rotation Matrix
//  c8: Lighting Direction
// c10: Lighting Color
// c16: Camera Eye


// Application Lambert + Blinn-Phong Shading

#define	Pos	r0
#define	Nor	r1
#define	Lmb	r2
#define	Eye	r3
#define	Hlf	r4
#define	Bln	r5
#define Lgt	-c8
#define Cam	c16
#define Pow c16.w

vs_1_1

def c24, 1.0f, 0.5f, 0.1f, 1.0f

dcl_position v0
dcl_normal v1
dcl_color v2
dcl_texcoord v3

m4x4 oPos, v0, c0
m4x4 Pos, v0, c4
m3x3 Nor, v1, c4

// Lambertian reflectance
dp3 Lmb.w, Nor, Lgt
add Lmb.w, Lmb.w, c24.x
mul Lmb.w, Lmb.w, c24.y
max Lmb.w, Lmb.w, c24.z
min Lmb.w, Lmb.w, c24.w
mul Lmb, c10, Lmb.wwww
mov Lmb.w, c24.w

sub Eye, Cam, Pos
dp3 Eye.w, Eye, Eye
rsq Eye.w, Eye.w
mul Eye.xyz, Eye.xyz, Eye.www

add Hlf, Eye, Lgt
dp3 Hlf.w, Hlf, Hlf
rsq Hlf.w, Hlf.w
mul Hlf.xyz, Hlf.xyz, Hlf.www

dp3 Bln.w, Hlf, Nor
add Bln.w, Bln.w, c24.x
mul Bln.w, Bln.w, c24.y
max Bln.w, Bln.w, c24.z
min Bln.w, Bln.w, c24.w

log Bln.w, Bln.w
mul Bln.w, Bln.w, Pow
exp Bln.w, Bln.w

mul Bln, c10, Bln.w
mov Bln.w, c24.w

mul oD0, Lmb, v2			// Output Diffuse: Lambert * Vertex Diffuse
mov oD1, Bln					// Output Specular: Blinn-Phong Reflectance

mov oT0, v3					// Output UV = Input UV