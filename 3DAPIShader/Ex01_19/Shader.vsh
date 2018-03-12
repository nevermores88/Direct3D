// c4:	View Matrix
// c8:  Projection Matrix
// c12:	matWorld0
// c16:	matWorld1

vs_1_1

def	c0, 1, 0, 0, 0

dcl_position v0
dcl_blendweight v1
dcl_color v4
dcl_texcoord v5

m4x4 r0, v0, c12				//���� ��ġ
m4x4 r1, v0, c16				//��ȯ�� ��ġ

// Vertex Blending
// pos = r0 * weight + r1* (1-weight)

mul r0, r0, v1.x		//r0 = r0 * weight
add r2, c0.x, -v1.x	//r2.xyzw = 1 - weight
mad r2, r1, r2, r0		//pos = (1-weight)*v1 + v0*weight

//���� ��ġ ���
m4x4 r0, r2, c4				//�� ��ȯ
m4x4 oPos, r0, c8			//���� ��ȯ

//m4x4 oPos, v0, c20

mov oD0, v4					//Diffuse ���
mov oT0, v5					//Texture ��ǥ ���