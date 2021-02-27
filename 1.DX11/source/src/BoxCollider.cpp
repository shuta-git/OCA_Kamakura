//===========================================================================
//!	@file	BoxCollider.cpp
//!	@brief	ボックスコライダー
//===========================================================================
#include "BoxCollider.h"

namespace gpu {

//===========================================================================
//! ボックスコライダー実装部
//===========================================================================
class BoxColliderImpl final : public gpu::BoxCollider
{
public:
    //! コンストラクタ
    BoxColliderImpl()
    {
        box_.min_ = Vector3(0,0,0);
        box_.max_ = Vector3(1.0f,1.0f,1.0f);
        isTrigger_ = false;
    }

    //! デストラクタ
    virtual ~BoxColliderImpl()
    {
        finalize();
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
    BoxColliderImpl(const BoxColliderImpl&) = delete;
    BoxColliderImpl(BoxColliderImpl&&)      = delete;
    BoxColliderImpl& operator=(const BoxColliderImpl&) = delete;
    BoxColliderImpl& operator=(BoxColliderImpl&&) = delete;

private:

    //! コライダー作成
    void createCollider();

private:

    Matrix matWorld;   //!< ワールド行列

    //-------------------------------------------------------------
    //!	@name	データ
    //-------------------------------------------------------------
    //@{

    int vertex_num = 24;
    int index_num  = 12;

    // @}
};

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
bool BoxColliderImpl::initialize()
{
    return false;
}

//---------------------------------------------------------------------------
//! 更新
//---------------------------------------------------------------------------
void BoxColliderImpl::update()
{
}

//---------------------------------------------------------------------------
//! 描画
//---------------------------------------------------------------------------
void BoxColliderImpl::render()
{
    matWorld = Matrix::rotateX(rotate_.x_) *
               Matrix::rotateY(rotate_.y_) *
               Matrix::rotateZ(rotate_.z_);

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
void BoxColliderImpl::finalize()
{
    
}

//---------------------------------------------------------------------------
//! imgui描画
//---------------------------------------------------------------------------
void BoxColliderImpl::imguiRender()
{
    ImGui::Checkbox("isTrigger", &isTrigger_);
    Vector3 min         = box_.min_;
    Vector3 max         = box_.max_;
    float*  pLocalMin[] = { &min.x_, &min.y_, &min.z_ };
    float*  pLocalMax[] = { &max.x_, &max.y_, &max.z_ };
    ImGui::DragFloat3("min", *pLocalMin, 0.03f, -FLT_MAX, FLT_MAX);
    ImGui::DragFloat3("max", *pLocalMax, 0.03f, -FLT_MAX, FLT_MAX);
    setAABB({ min, max });
    
    float* pos[]  = { &pos_.x_, &pos_.y_, &pos_.z_ };
    float  wight  = wigth_;
    float  height = height_;
    float  depth  = depth_;
    ImGui::SliderFloat3("pos", *pos, -12.0f, 12.0f);
    ImGui::SliderFloat("wigth", &wight, -16.0f, 16.0f);
    ImGui::SliderFloat("height", &height, -16.0f, 16.0f);
    ImGui::SliderFloat("depth", &depth, -16.0f, 16.0f);
    
    setPos(Vector3(*pos[0], *pos[1], *pos[2]));
    setWigth(wight);
    setHeight(height);
    setDepth(depth);
    
}

//============================================================================================================
// 内部実装
//============================================================================================================
//---------------------------------------------------------------------------
//! コライダー作成
//---------------------------------------------------------------------------
void BoxColliderImpl::createCollider()
{
    box_.min_.x_ = pos_.x_;
    box_.min_.y_ = pos_.y_;
    box_.min_.z_ = pos_.z_;
    
    box_.max_.x_ = pos_.x_ + wigth_;
    box_.max_.y_ = pos_.y_ + height_;
    box_.max_.z_ = pos_.z_ + depth_;

    // 正面
    // 上
    dxVertex3f(box_.min_.x_, box_.max_.y_, box_.min_.z_);
    dxVertex3f(box_.max_.x_, box_.max_.y_, box_.min_.z_);
    // 下
    dxVertex3f(box_.min_.x_, box_.min_.y_, box_.min_.z_);
    dxVertex3f(box_.max_.x_, box_.min_.y_, box_.min_.z_);
    // 右
    dxVertex3f(box_.max_.x_, box_.min_.y_, box_.min_.z_);
    dxVertex3f(box_.max_.x_, box_.max_.y_, box_.min_.z_);
    // 左
    dxVertex3f(box_.min_.x_, box_.min_.y_, box_.min_.z_);
    dxVertex3f(box_.min_.x_, box_.max_.y_, box_.min_.z_);

    // 背面
    // 上
    dxVertex3f(box_.min_.x_, box_.max_.y_, box_.max_.z_);
    dxVertex3f(box_.max_.x_, box_.max_.y_, box_.max_.z_);
    // 下
    dxVertex3f(box_.min_.x_, box_.min_.y_, box_.max_.z_);
    dxVertex3f(box_.max_.x_, box_.min_.y_, box_.max_.z_);
    // 右
    dxVertex3f(box_.max_.x_, box_.min_.y_, box_.max_.z_);
    dxVertex3f(box_.max_.x_, box_.max_.y_, box_.max_.z_);
    // 左
    dxVertex3f(box_.min_.x_, box_.min_.y_, box_.max_.z_);
    dxVertex3f(box_.min_.x_, box_.max_.y_, box_.max_.z_);

    // 側面（正面と背面をつなげる４辺を作る）
    // 左上
    dxVertex3f(box_.min_.x_, box_.max_.y_, box_.min_.z_);
    dxVertex3f(box_.min_.x_, box_.max_.y_, box_.max_.z_);
    // 左下
    dxVertex3f(box_.min_.x_, box_.min_.y_, box_.min_.z_);
    dxVertex3f(box_.min_.x_, box_.min_.y_, box_.max_.z_);
    // 右上
    dxVertex3f(box_.max_.x_, box_.max_.y_, box_.min_.z_);
    dxVertex3f(box_.max_.x_, box_.max_.y_, box_.max_.z_);
    // 右下
    dxVertex3f(box_.max_.x_, box_.min_.y_, box_.min_.z_);
    dxVertex3f(box_.max_.x_, box_.min_.y_, box_.max_.z_);

    // 対角線
    // 正面
    // 右上から左下
    dxVertex3f(box_.min_.x_, box_.max_.y_, box_.min_.z_);
    dxVertex3f(box_.max_.x_, box_.min_.y_, box_.min_.z_);
    // 左上から右下
    dxVertex3f(box_.min_.x_, box_.min_.y_, box_.min_.z_);
    dxVertex3f(box_.max_.x_, box_.max_.y_, box_.min_.z_);

    // 背面
    // 右上から左下
    dxVertex3f(box_.min_.x_, box_.max_.y_, box_.max_.z_);
    dxVertex3f(box_.max_.x_, box_.min_.y_, box_.max_.z_);
    // 左上から右下
    dxVertex3f(box_.min_.x_, box_.min_.y_, box_.max_.z_);
    dxVertex3f(box_.max_.x_, box_.max_.y_, box_.max_.z_);
    
    // 上面
    // 右上から左下
    dxVertex3f(box_.max_.x_, box_.max_.y_, box_.max_.z_);
    dxVertex3f(box_.min_.x_, box_.max_.y_, box_.min_.z_);
    // 左上から右下
    dxVertex3f(box_.min_.x_, box_.max_.y_, box_.max_.z_);
    dxVertex3f(box_.max_.x_, box_.max_.y_, box_.min_.z_);
   
    // 底面
    // 右上から左下
    dxVertex3f(box_.max_.x_, box_.min_.y_, box_.min_.z_);
    dxVertex3f(box_.max_.x_, box_.min_.y_, box_.max_.z_);
    // 左上から右下
    dxVertex3f(box_.max_.x_, box_.min_.y_, box_.min_.z_);
    dxVertex3f(box_.min_.x_, box_.min_.y_, box_.max_.z_);

    // 左側面
    // 右上から左下
    dxVertex3f(box_.min_.x_, box_.max_.y_, box_.min_.z_);
    dxVertex3f(box_.min_.x_, box_.min_.y_, box_.max_.z_);
    // 左上から右下
    dxVertex3f(box_.min_.x_, box_.max_.y_, box_.max_.z_);
    dxVertex3f(box_.min_.x_, box_.min_.y_, box_.min_.z_);

    // 右側面
    // 右上から左下
    dxVertex3f(box_.max_.x_, box_.max_.y_, box_.max_.z_);
    dxVertex3f(box_.max_.x_, box_.min_.y_, box_.min_.z_);
    // 左上から右下
    dxVertex3f(box_.max_.x_, box_.max_.y_, box_.min_.z_);
    dxVertex3f(box_.max_.x_, box_.min_.y_, box_.max_.z_);

}

//---------------------------------------------------------------------------
//! ボックスコライダーを作成
//---------------------------------------------------------------------------
std::shared_ptr<gpu::BoxCollider> createBoxCollider()
{
    //---------------------------------------------------------------------
    // SphereColliderの初期化
    //---------------------------------------------------------------------
    auto p = std::make_shared<gpu::BoxColliderImpl>();

    if(!p) {
        p.reset();
    }

    return p;
}

}   // namespace gpu