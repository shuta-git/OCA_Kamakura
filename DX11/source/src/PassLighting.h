//===========================================================================
//!	@file   PassLighting.h
//!	@brief	照明パス
//===========================================================================
#pragma once

class PassLighting : public Pass
{
public:
    PassLighting() = default;

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
    PassLighting(const PassLighting&)  = delete;
    PassLighting(const PassLighting&&) = delete;
    void operator=(const PassLighting&) = delete;
    void operator=(const PassLighting&&) = delete;

private:
    std::unique_ptr<gpu::ShaderVs> shaderVs_;
    std::unique_ptr<gpu::ShaderPs> shaderPs_;
};
