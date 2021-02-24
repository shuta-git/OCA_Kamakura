//===========================================================================
//!	@file   PassGbuffer.h
//!	@brief	Gbuffer生成パス
//===========================================================================
#pragma once

class PassGbuffer : public Pass
{
public:
    PassGbuffer() = default;

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
    PassGbuffer(const PassGbuffer&)  = delete;
    PassGbuffer(PassGbuffer&&) = delete;
    void operator=(const PassGbuffer&) = delete;
    void operator=(PassGbuffer&&) = delete;

private:
};
