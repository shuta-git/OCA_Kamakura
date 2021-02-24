//===========================================================================
//!	@file	Map.h
//!	@brief	マップ
//===========================================================================
#pragma once

//===========================================================================
//!　マップ
//===========================================================================
class Map : public CollisionBase
{
public:
    //! コンストラクタ
    Map();

    //! デストラクタ
    virtual ~Map();

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

    //-------------------------------------------------------------
    //!	@name	取得
    //-------------------------------------------------------------
    //@{

    //! コライダーの数を取得
    size_t getColliderSize() const;

    //! ボックスコライダーを取得
    std::shared_ptr<gpu::BoxCollider> getBoxCollider(int i) const;

    //@}

private:

    //! ファイル読み込み
    void loadFile();

    //! ファイル書き出し
    void writeFile();

    //! イレース
    void erase();

private:

    std::unique_ptr<gpu::ShaderVs> shaderVs_;   //!< 頂点シェーダー
    std::unique_ptr<gpu::ShaderPs> shaderPs_;   //!< ピクセルシェーダー

    std::shared_ptr<gpu::ModelFBX> cemetery_;  //!< 墓
    std::shared_ptr<gpu::ModelFBX> skyDome_;   //!< スカイドーム

    std::vector<std::shared_ptr<gpu::BoxCollider>> boxCollider_;   //!< ボックスコライダー

    std::fstream file; //! ファイル用

};
