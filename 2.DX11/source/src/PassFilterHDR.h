//===========================================================================
//!	@file   PassFilterHDR.h
//!	@brief	HDRフィルターパス
//===========================================================================
#pragma once

class PassFilterHDR : public Pass
{
public:
    PassFilterHDR() = default;

    //! 初期化
    virtual bool initialize() override;

    //! 解放
    virtual void finalize() override;

    //! 開始
    virtual void begin() override;

    //! 実行
    virtual void execute() override;

    //! 終了
    virtual void end() override;

private:
    // コピー禁止
    PassFilterHDR(const PassFilterHDR&)  = delete;
    PassFilterHDR(const PassFilterHDR&&) = delete;
    void operator=(const PassFilterHDR&) = delete;
    void operator=(const PassFilterHDR&&) = delete;

private:
    std::unique_ptr<gpu::ShaderVs> shaderVs_integrate_;
    std::unique_ptr<gpu::ShaderPs> shaderPs_integrate_;

    // グレアフィルター
    FilterGlare filterGlare_;

    // トーンマッピングシェーダー
    std::unique_ptr<gpu::ShaderVs> shaderTonemappingVs_;   //!< 頂点シェーダー
    std::unique_ptr<gpu::ShaderPs> shaderTonemappingPs_;   //!< ピクセルシェーダー




};
