//===========================================================================
//!	@file	CapsuleCollider.cpp
//!	@brief	カプセルコライダー
//===========================================================================
#include "CapsuleCollider.h"

namespace gpu {

//===========================================================================
//! 球コライダー実装部
//===========================================================================
class CapsuleColliderImpl final : public gpu::CapsuleCollider
{
public:
    //! コンストラクタ
    CapsuleColliderImpl()
    {
    }

    //! デストラクタ
    virtual ~CapsuleColliderImpl()
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
    CapsuleColliderImpl(const CapsuleColliderImpl&) = delete;
    CapsuleColliderImpl(CapsuleColliderImpl&&)      = delete;
    CapsuleColliderImpl& operator=(const CapsuleColliderImpl&) = delete;
    CapsuleColliderImpl& operator=(CapsuleColliderImpl&&) = delete;

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
bool CapsuleColliderImpl::initialize()
{
    return false;
}

//---------------------------------------------------------------------------
//! 更新
//---------------------------------------------------------------------------
void CapsuleColliderImpl::update()
{
}

//---------------------------------------------------------------------------
//! 描画
//---------------------------------------------------------------------------
void CapsuleColliderImpl::render()
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
void CapsuleColliderImpl::finalize()
{
}

//---------------------------------------------------------------------------
//! imgui描画
//---------------------------------------------------------------------------
void CapsuleColliderImpl::imguiRender()
{
}

//---------------------------------------------------------------------------
//! コライダー作成
//---------------------------------------------------------------------------
void CapsuleColliderImpl::createCollider()
{
    //-------------------------------------------------------------
    // 円柱の頂点データ作成
    //-------------------------------------------------------------
    // 縦線
    for(int u = 0; u < u_max; u++) {
        for(int v = 0; v <= v_max; v++) {
            double theta  = XMConvertToRadians(180.0f * v / v_max);
            double dTheta = 2.0f * PI / u_max;

            float x = cos(u * dTheta) * radius_;
            float y = cos(theta) * height_;
            float z = sin(u * dTheta) * radius_;

            dxVertex3f(x, y, z);

            theta = XMConvertToRadians(180.0f * (v + 1) / v_max);

            x = cos(u * dTheta) * radius_;
            y = cos(theta) * height_;
            z = sin(u * dTheta) * radius_;

            dxVertex3f(x, y, z);
        }
    }
    // 横線
    for(int v = 0; v <= v_max; v++) {
        for(int u = 0; u < u_max; u++) {
            double theta  = XMConvertToRadians(180.0f * v / v_max);
            double dTheta = 2.0f * PI / u_max;

            float x = cos(u * dTheta) * radius_;
            float y = cos(theta) * height_;
            float z = sin(u * dTheta) * radius_;

            dxVertex3f(x, y, z);

            x = cos((u + 1) * dTheta) * radius_;
            y = cos(theta) * height_;
            z = sin((u + 1) * dTheta) * radius_;

            dxVertex3f(x, y, z);
        }
    }

    //-------------------------------------------------------------
    // 上の半球の頂点データー作成
    //-------------------------------------------------------------
    // 縦線
    for(int u = 0; u < u_max; u++) {
        for(int v = 0; v <= v_max; v++) {
            double theta = XMConvertToRadians(90.0f * v / v_max);
            double phi   = XMConvertToRadians(360.0f * u / u_max);

            double x = sin(theta) * cos(phi) * radius_;
            double y = cos(theta) * sphereHeight_ + height_;
            double z = sin(theta) * sin(phi) * radius_;

            dxVertex3f(x, y, z);

            theta = XMConvertToRadians(90.0f * (v + 1) / v_max);

            x = sin(theta) * cos(phi) * radius_;
            y = cos(theta) * sphereHeight_ + height_;
            z = sin(theta) * sin(phi) * radius_;

            dxVertex3f(x, y, z);
        }
    }
    // 横線
    for(int v = 0; v <= v_max; v++) {
        for(int u = 0; u < u_max; u++) {
            double theta = XMConvertToRadians(90.0f * v / v_max);
            double phi   = XMConvertToRadians(360.0f * u / u_max);

            double x = sin(theta) * cos(phi) * radius_;
            double y = cos(theta) * sphereHeight_ + height_;
            double z = sin(theta) * sin(phi) * radius_;

            dxVertex3f(x, y, z);

            phi = XMConvertToRadians(360.0f * (u + 1) / u_max);

            x = sin(theta) * cos(phi) * radius_;
            y = cos(theta) * sphereHeight_ + height_;
            z = sin(theta) * sin(phi) * radius_;

            dxVertex3f(x, y, z);
        }
    }

    //-------------------------------------------------------------
    // 下の半球の頂点データー作成
    //-------------------------------------------------------------
    // 縦線
    for(int u = 0; u < u_max; u++) {
        for(int v = 0; v <= v_max; v++) {
            double theta = XMConvertToRadians(90.0f * v / v_max);
            double phi   = XMConvertToRadians(360.0f * u / u_max);
            double x     = sin(theta) * cos(phi) * radius_;
            double y     = cos(theta) * -sphereHeight_ - height_;
            double z     = sin(theta) * sin(phi) * radius_;

            dxVertex3f(x, y, z);

            theta = XMConvertToRadians(90.0f * (v + 1) / v_max);

            x = sin(theta) * cos(phi) * radius_;
            y = cos(theta) * -sphereHeight_ - height_;
            z = sin(theta) * sin(phi) * radius_;

            dxVertex3f(x, y, z);
        }
    }
    // 横線
    for(int v = 0; v <= v_max; v++) {
        for(int u = 0; u < u_max; u++) {
            double theta = XMConvertToRadians(90.0f * v / v_max);
            double phi   = XMConvertToRadians(360.0f * u / u_max);
            double x     = sin(theta) * cos(phi) * radius_;
            double y     = cos(theta) * -sphereHeight_ - height_;
            double z     = sin(theta) * sin(phi) * radius_;

            dxVertex3f(x, y, z);

            phi = XMConvertToRadians(360.0f * (u + 1) / u_max);

            x = sin(theta) * cos(phi) * radius_;
            y = cos(theta) * -sphereHeight_ - height_;
            z = sin(theta) * sin(phi) * radius_;

            dxVertex3f(x, y, z);
        }
    }
}

//---------------------------------------------------------------------------
//! CapsuleColliderを作成
//---------------------------------------------------------------------------
std::shared_ptr<gpu::CapsuleCollider> createCapsuleCollider()
{
    //---------------------------------------------------------------------
    // CapsuleColliderの初期化
    //---------------------------------------------------------------------
    auto p = std::make_shared<gpu::CapsuleColliderImpl>();

    if(!p) {
        p.reset();
    }

    return p;
}

}   // namespace gpu