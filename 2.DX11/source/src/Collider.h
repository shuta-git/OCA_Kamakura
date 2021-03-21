//===========================================================================
//!	@file	Collider.h
//!	@brief	コライダー基底
//===========================================================================
#pragma once

namespace gpu {

//===========================================================================
//!　コライダー基底
//===========================================================================
class Collider : public Base
{
public:

    virtual ~Collider() = default;

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

    //-------------------------------------------------------------
    //!	@name	参照
    //-------------------------------------------------------------
    //@{

    void setTrigger(bool isTrigger) { isTrigger_ = isTrigger; }

    //@}

    bool getTrigger() { return isTrigger_; }

protected:

    bool isTrigger_;

};

}   // namespace gpu