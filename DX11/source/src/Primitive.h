//===========================================================================
//!	@file   Primitive.h
//!	@brief	�v���~�e�B�u�`��
//===========================================================================
#pragma once

//! ������
bool PRIM_Initialize();

//! ���
void PRIM_Finalize();

//===========================================================================
// OpenGL�݊��֐�
//===========================================================================


//! �s��̐ݒ�
void dxLoadMatrixf(const Matrix& matWorld);

//! �s��X�^�b�N�֑ޔ�
void dxPushMatrix();

//! �s��X�^�b�N���畜�A
void dxPopMatrix();


//! �`��J�n
//!	@param	[in]	type	�v���~�e�B�u�̎��
void dxBegin(PRIMITIVE_TYPE type);

//! ���_�L�b�N (Kick)
void dxVertex3f(f32 x, f32 y, f32 z);
void dxVertex3fv(const Vector3& v);

//! �J���[u8
void dxColor4ub(u8 r, u8 g, u8 b, u8 a);

//! �J���[f32
void dxColor4f(f32 r, f32 g, f32 b, f32 a);

//! �e�N�X�`�����W
void dxTexCoord2f(f32 u, f32 v);

//! �@��
void dxNormal3fv(const Vector3& v);

//! �ڃx�N�g��
void dxTangent3fv(const Vector3& v);

//! �]�@���x�N�g��
void dxBinormal3fv(const Vector3& v);

//! �`��I��
void dxEnd();

//! �e�N�X�`���̐ݒ�
void dxTexture(ptr<gpu::Texture> texture);
