//===========================================================================
//!	@file   GBuffer.h
//!	@brief	GBuffer管理
//===========================================================================
#pragma once

//===========================================================================
//!	GBuffer管理
//===========================================================================
class GBuffer
{
public:
    GBuffer()  = default;
    ~GBuffer() = default;

    //! 初期化
    bool initialize(u32 w, u32 h);

    //! 解放
    void finalize();

    //! 開始
    void begin();

    //! 終了
    void end();

	//! Imgui描画
    void imguiRender();

    //! シャドウテクスチャの取得
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
	
	std::array< std::unique_ptr<gpu::Texture>, 4> gbuffer_;   //!< GBuffer用のテクスチャ
};
