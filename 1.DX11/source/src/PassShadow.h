//===========================================================================
//!	@file   PassShadow.h
//!	@brief	シャドウ生成パス
//===========================================================================
#pragma once

class PassShadow : public Pass
{
public:
    PassShadow() = default;

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
    PassShadow(const PassShadow&)  = delete;
    PassShadow(const PassShadow&&) = delete;
    void operator=(const PassShadow&) = delete;
    void operator=(const PassShadow&&) = delete;

private:
};
