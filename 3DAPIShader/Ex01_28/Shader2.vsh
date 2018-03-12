//  c0: World * View * Projection matrix
//	c9: color
//	c27: Scale

#define	scale c27

vs_1_1

def c25, 0.0, 0.0, 0.0, 1.0

dcl_position v0
dcl_normal v1

mov r0, v1					//¹ý¼± º¤ÅÍ
mad r0, r0, scale, v0	//À±°û¼± À§Ä¡ = ¹ý¼± º¤ÅÍ * ½ºÄÉÀÏ + À§Ä¡
mov r0.w, c25.w			//À±°û¼± À§Ä¡ w = 1.0

m4x4 oPos, r0, c0

mov oD0, c9