//
// Shader Example.
// Copyleft (c) 2007 Heesung Oh(Galic Studio). All rights Not reserved.
////////////////////////////////////////////////////////////////////////////

// c0: World * View * Projection
// c10: Color

vs_1_1

dcl_position 	v0
dcl_color		v2
dcl_texcoord	v3

m4x4 oPos, v0,	c0		// ���� ��ȯ

mul  r0, v2,	c10		// ro = ������ ���� * ���(c10) ��

mov  oD0, r0			// ��� ��ǻ�� ����
mov oT0, v3				// ��� UV ��ǥ = �Է� UV ��ǥ

