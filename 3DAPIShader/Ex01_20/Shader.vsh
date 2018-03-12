// c4:	View Matrix
// c8:  Projection Matrix
// c12:	World Matrix Palette

#define MATRIX_OFFSET	12

vs_2_0

def	c0, 4.0, 0.0, 0.0, 1.0

dcl_position v0
dcl_blendweight v1
dcl_blendindices v2
dcl_normal	v3
dcl_color v4
dcl_texcoord v5

mul r0, v2, c0.x		//����� 4x4 �̹Ƿ� ��� ���������� ��ġ�� ���ϱ� ���� �ε����� 4�� ���Ѵ�.
mova a0, r0

m4x4 r0, v0, c[MATRIX_OFFSET + a0.x]			//��� �ε����� ���� ������ ��ȯ�Ѵ�.
m4x4 r1, v0, c[MATRIX_OFFSET + a0.y]
m4x4 r2, v0, c[MATRIX_OFFSET + a0.z]
m4x4 r3, v0, c[MATRIX_OFFSET + a0.w]

//���� ���
//������ ������ 1 - (v1.x + v1.y + v1.z)
mov r4, v1
add r4.w, r4.x, r4.y
add r4.w, r4.w, r4.z
add r4.w, c0.w, -r4.w

//��ȯ�� ������ ��ġ�� �� ������ ���Ѵ�.
mul r0, r0, r4.x
mul r1, r1, r4.y
mul r2, r2, r4.z
mul r3, r3, r4.w

//������ ������ �� ��ġ�� ���Ѵ�.
add r0, r0, r1
add r0, r0, r2
add r0, r0, r3

//������ ��ġ�� ��, ���� ��ȯ �����ϰ� ��� �������Ϳ� ����Ѵ�.
m4x4 r1, r0, c4			//�� ��ȯ
m4x4 oPos, r1, c8		//���� ��ȯ

//��Ÿ ���
mov oD0, v4				//Diffuse ���
mov oT0, v5				//Texture ��ǥ ���