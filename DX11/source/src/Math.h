//===========================================================================
//!	@file	Math.cpp
//!	@brief	���w�֐�
//===========================================================================
#pragma once

namespace {

// �~����
static const f32 PI = 3.141592f;

//---------------------------------------------------------------------------
// ���W�A���p�ϊ�
//!	@param	[in]    deg     �p�x
//---------------------------------------------------------------------------
f32 radian(f32 deg)
{
    return deg / 180.0f * PI;
}


}   // namespace