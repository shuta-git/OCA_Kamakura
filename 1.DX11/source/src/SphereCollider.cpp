//===========================================================================
//!	@file	SphereCollider.cpp
//!	@brief	スフィアコライダー
//===========================================================================
#include "SphereCollider.h"

namespace gpu {

//===========================================================================
//! スフィアコライダー実装部
//===========================================================================
class SphereColliderImpl final : public gpu::SphereCollider
{
public:
    //! コンストラクタ
    SphereColliderImpl()
    {
    }

    //! デストラクタ
    virtual ~SphereColliderImpl()
    {
    }

    //! 初期化
    virtual bool initialize() override;

    //! 更新
    virtual void update() override;

    //! 描画
    virtual void render() override;

    //! 解放
    virtual void finalize() override;

    //! imgui描画
    virtual void imguiRender() override;

private:
    // コピー禁止/move禁止
    SphereColliderImpl(const SphereColliderImpl&) = delete;
    SphereColliderImpl(SphereColliderImpl&&)      = delete;
    SphereColliderImpl& operator=(const SphereColliderImpl&) = delete;
    SphereColliderImpl& operator=(SphereColliderImpl&&) = delete;

private:
    //! コライダー作成
    void createCollider();

private:
    Matrix matWorld;   //!< ワールド行列

    //-------------------------------------------------------------
    //!	@name	データ
    //-------------------------------------------------------------
    //@{

    const int u_max = 15;   //!< 縦線の数
    const int v_max = 15;   //!< 横線の数

    //@}
};

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
bool SphereColliderImpl::initialize()
{
    return false;
}

//---------------------------------------------------------------------------
//! 更新
//---------------------------------------------------------------------------
void SphereColliderImpl::update()
{
}

//---------------------------------------------------------------------------
//! 描画
//---------------------------------------------------------------------------
void SphereColliderImpl::render()
{
    matWorld = Matrix::rotateX(rotate_.x_) *
               Matrix::rotateY(rotate_.y_) *
               Matrix::rotateZ(rotate_.z_);

    matWorld = matWorld * Matrix::translate(pos_);
    // 行列設定
    dxLoadMatrixf(matWorld);

    if(!isTrigger_) {
        dxColor4ub(60, 255, 60, 255);
    }
    else {
        dxColor4ub(255, 60, 60, 255);
    }
    dxBegin(PT_LINES);
    {
        createCollider();
    }
    dxEnd();

    // 元に戻す
    dxLoadMatrixf(Matrix::identity());
}

//---------------------------------------------------------------------------
//! 解放
//---------------------------------------------------------------------------
void SphereColliderImpl::finalize()
{
}

//---------------------------------------------------------------------------
//! imgui描画
//---------------------------------------------------------------------------
void SphereColliderImpl::imguiRender()
{
}

//============================================================================================================
// 内部実装
//============================================================================================================
//---------------------------------------------------------------------------
//! コライダー作成
//---------------------------------------------------------------------------
void SphereColliderImpl::createCollider()
{
    // 縦線
    for(int u = 0; u < u_max; u++) {
        for(int v = 0; v <= v_max; v++) {
            double theta = XMConvertToRadians(180.0f * v / v_max);
            double phi   = XMConvertToRadians(360.0f * u / u_max);

            double x = sin(theta) * cos(phi) * radius_;
            double y = cos(theta) * radius_;
            double z = sin(theta) * sin(phi) * radius_;

            dxVertex3f(x, y, z);

            theta = XMConvertToRadians(180.0f * (v + 1) / v_max);

            x = sin(theta) * cos(phi) * radius_;
            y = cos(theta) * radius_;
            z = sin(theta) * sin(phi) * radius_;

            dxVertex3f(x, y, z);
        }
    }

    // 横線
    for(int v = 0; v <= v_max; v++) {
        for(int u = 0; u < u_max; u++) {
            double theta = XMConvertToRadians(180.0f * v / v_max);
            double phi   = XMConvertToRadians(360.0f * u / u_max);

            double x = sin(theta) * cos(phi) * radius_;
            double y = cos(theta) * radius_;
            double z = sin(theta) * sin(phi) * radius_;

            dxVertex3f(x, y, z);

            phi = XMConvertToRadians(360.0f * (u + 1) / u_max);

            x = sin(theta) * cos(phi) * radius_;
            y = cos(theta) * radius_;
            z = sin(theta) * sin(phi) * radius_;

            dxVertex3f(x, y, z);
        }
    }
}

//---------------------------------------------------------------------------
//! スフィアコライダーを作成
//---------------------------------------------------------------------------
std::shared_ptr<gpu::SphereCollider> createSphereCollider()
{
    //---------------------------------------------------------------------
    // SphereColliderの初期化
    //---------------------------------------------------------------------
    auto p = std::make_shared<gpu::SphereColliderImpl>();

    if(!p) {
        p.reset();
    }

    return p;
}

}   // namespace gpu