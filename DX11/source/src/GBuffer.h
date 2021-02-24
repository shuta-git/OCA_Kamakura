//===========================================================================
//!	@file   GBuffer.h
//!	@brief	GBuffer�Ǘ�
//===========================================================================
#pragma once

//===========================================================================
//!	GBuffer�Ǘ�
//===========================================================================
class GBuffer
{
public:
    GBuffer()  = default;
    ~GBuffer() = default;

    //! ������
    bool initialize(u32 w, u32 h);

    //! ���
    void finalize();

    //! �J�n
    void begin();

    //! �I��
    void end();

	//! Imgui�`��
    void imguiRender();

    //! �V���h�E�e�N�X�`���̎擾
    gpu::Texture* getGBuffer(u32 index) const { return gbuffer_[index].get(); }

private:
	//          R        G        B        A
	//     +--------+--------+--------+--------+
	// RT0 |        Albedo.RGB        |EmsvMetal| R8G8B8A8_Unorm
	//     +--------+--------+--------+--------+
	// RT1 |  WorldNormal.xy | Rough  |   AO   | R8G8B8A8_Unorm
	//     +--------+--------+--------+--------+
	// RT2 |    velocity.xy  |  ----  |  ----  | R16G16_Float
	//     +--------+--------+--------+--------+
	//
	//     +--------+--------+--------+--------+
	//DEPTH|           fp32 depth              | D32_Float
	//     +--------+--------+--------+--------+
	
	std::array< std::unique_ptr<gpu::Texture>, 4> gbuffer_;   //!< GBuffer�p�̃e�N�X�`��
};
