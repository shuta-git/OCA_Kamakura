//===========================================================================
//!	@file   FilterGlare.h
//!	@brief	グレアフィルター
//===========================================================================
#pragma once

//===========================================================================
//! グレアフィルター
//===========================================================================
class FilterGlare
{
public:
    //-------------------------------------------------------------
    //!	@name	初期化
    //-------------------------------------------------------------
    //@{

    //! コンストラクタ
    FilterGlare() = default;

    //! デストラクタ
    virtual ~FilterGlare() = default;

    //@}
    //-------------------------------------------------------------
    //!	@name	更新
    //-------------------------------------------------------------
    //@{

    //! 初期化
    bool initialize();

    // 描画
    void render();

    //@}
private:
    struct CBFilterGlare
    {
        f32 stride_;
		f32	attenuation_;
		Vector4	dir_;
    };

    std::unique_ptr<gpu::ShaderVs> shaderVs_;

    std::unique_ptr<gpu::ShaderPs> shaderPs_Luminance_;       //!< 輝度抽出
    std::unique_ptr<gpu::ShaderPs> shaderPs_GaussianBlurH_;   //!< ガウスぼかし(水平)
    std::unique_ptr<gpu::ShaderPs> shaderPs_GaussianBlurV_;   //!< ガウスぼかし(垂直)
    std::unique_ptr<gpu::ShaderPs> shaderPs_Texture_;         //!< テクスチャ貼り
    std::unique_ptr<gpu::ShaderPs> shaderPs_;                 //!< 最終合成

    std::unique_ptr<gpu::ShaderPs> shaderPs_Glare_;   //!< グレア光芒

    gpu::ConstantBuffer<CBFilterGlare> cbFilter_;   //!< 定数バッファ
};
