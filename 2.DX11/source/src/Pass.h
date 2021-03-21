//===========================================================================
//!	@file   Pass.h
//!	@brief	描画パス
//===========================================================================
#pragma once

class Pass
{
public:
    Pass() = default;

    //! 初期化
    //!	@retval	true	成功 (正常終了)
    //!	@retval	false	失敗 (エラー終了)
    virtual bool initialize() = 0;

    //! 解放
    virtual void finalize() = 0;

    //! 開始
    virtual void begin() = 0;

    //! 実行
    virtual void execute() = 0;

    //! 終了
    virtual void end() = 0;

private:
    // コピー禁止
    Pass(const Pass&)  = delete;
    Pass(const Pass&&) = delete;
    void operator=(const Pass&) = delete;
    void operator=(const Pass&&) = delete;

private:
};
