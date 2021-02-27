//===========================================================================
//!	@file   ModelFBX.h
//!	@brief  FBXモデル
//===========================================================================
#pragma once
#include "../OpenFBX/src/ofbx.h"
#include "MeshFBX.h"

namespace gpu {

//===========================================================================
//!	FBXモデル
//===========================================================================
class ModelFBX
{
public:

    virtual ~ModelFBX() = default;

    //! 描画
    void render();

    //! ImGui描画
    void imguiRender();

    //-------------------------------------------------------------
    //!	@name	参照
    //-------------------------------------------------------------
    //@{

    //! 位置を設定
    void setPos(Vector3 pos) { pos_ = pos; }

    //! 回転を設定
    void setRotate(Vector3 rotate) { rotate_ = rotate; }

    //! X軸回転を設定
    void setRotateX(float x) { rotate_.x_ = x; }

    //! Y軸回転を設定
    void setRotateY(float y) { rotate_.y_ = y; }

    //! Z軸回転を設定
    void setRotateZ(float z) { rotate_.z_ = z; }

    //@}

    //-------------------------------------------------------------
    //!	@name	取得
    //-------------------------------------------------------------
    //@{

    //! 位置を取得
    Vector3 getPos() const { return pos_; }

    //! 回転を取得
    Vector3 getRotate() const
    {
        return rotate_;
    }

    //! X軸回転を取得
    float getRotateX() const { return rotate_.x_ ; }

    //! Y軸回転を取得
    float getRotateY() const { return rotate_.y_ ; }

    //! Z軸回転を取得
    float getRotateZ() const { return rotate_.z_ ; }

    //!< ワールド座標
    Matrix getMatWorld() const { return matWorld; }

    //@}

protected:

    ModelFBX() = default;

    std::vector<MeshFBX> meshes_;   //!< メッシュ

    std::unique_ptr<gpu::ShaderVs> shaderVs_;          //!< 頂点シェーダー
    std::unique_ptr<gpu::ShaderPs> shaderPs_;          //!< ピクセルシェーダー
    std::unique_ptr<gpu::ShaderPs> shaderPsTexture_;   //!< テクスチャピクセルシェーダー

    char* filePath_;   //!< ファイルパス
    char  fileName_[MAX_PATH];  //!< ファイルネーム
    ofbx::IScene* scene_ = nullptr;
    f32           scale_;   //!< サイズ

    double animStart = 0;
    double animEnd = 0;
    double timeCount = 0;
    double frameTime = 0;


private:

    Vector3 rotate_ = Vector3(0, 0, 0);   //!< 向き
    Vector3 pos_    = Vector3(0, 0, 0);    //!< 座標

    Matrix matWorld;    //!< ワールド行列
};

}   // namespace gpu

namespace fbx {

//! FBXをファイルから作成
//! @parm [in] filePath ファイルパス
//! @parm [in] scale    サイズ
std::shared_ptr<gpu::ModelFBX> loadFBX(char* filePath, f32 scale = 1.0f);

}