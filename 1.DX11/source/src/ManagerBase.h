//===========================================================================
//!	@file	ManagerBase.h
//!	@brief	マネージャー基底
//===========================================================================
#pragma once

//===========================================================================
//! マネジャー基底
//===========================================================================
class ManagerBase
{
public:
    ManagerBase()   = default;
    virtual ~ManagerBase() = default;

    //! 初期化
    virtual bool initialize() = 0;

    //! 更新
    virtual void update() = 0;

    //! 描画
    virtual void render() = 0;

    //! 解放
    virtual void finalize() = 0;

    //! imgui描画
    virtual void imguiRender() = 0;

};
