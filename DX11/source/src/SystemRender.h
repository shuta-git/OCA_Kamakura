//===========================================================================
//!	DirectX11描画システム管理
//===========================================================================
#pragma once

namespace gpu {
class Texture;
class ShaderVs;
class ShaderPs;
};   // namespace gpu

enum SYSTEM_TEXTURE
{
    SYSTEM_TEXTURE_NULL_WHITE,
    SYSTEM_TEXTURE_NULL_BLACK,
    //----
    SYSTEM_TEXTURE_MAX,
};

enum BLEND_MODE
{
    BLEND_MODE_OFF,     //!< 無効
    BLEND_MODE_BLEND,   //!< 通常ブレンド
    BLEND_MODE_ADD,     //!< 加算
    //----
    BLEND_MODE_MAX,   //!< ブレンドモードの個数
};

// シーン単位で更新する定数バッファ
struct CBCamera
{
    Matrix matView_;   //!< ビュー行列
    Matrix matProj_;   //!< 投影行列

    Matrix matLightView_;
    Matrix matLightProj_;

    Float3 cameraPosition_;   //!< カメラのワールド座標
    f32    dummy_;            //!< アライメントをそろえるダミー(未使用)
};

//===========================================================================
//!	レンダリングシステム
//===========================================================================
class SystemRender
{
public:
    //-------------------------------------------------------------
    //!	@name	初期化
    //-------------------------------------------------------------
    //@{

    //! コンストラクタ
    SystemRender();

    //! デストラクタ
    virtual ~SystemRender();

    //! 初期化
    //!	@param	[in]	width	幅
    //!	@param	[in]	height	高さ
    //!	@param	[in]	hWnd	描画対象のウィンドウハンドル
    //!	@retval	true	正常終了	（成功）
    //!	@retval	false	エラー終了	（失敗）
    bool setup(u32 width, u32 height, HWND hWnd);

    //! 解放
    void cleanup();

    //@}
    //-------------------------------------------------------------
    //!	@name	制御
    //-------------------------------------------------------------
    //@{

    //! 描画開始
    void begin();

    //! 描画終了
    void end();

    //!	画面更新
    //! @param	[in]	interval	画面更新周期 1:60fps 2:30fps 3:20fps 4:15fps
    void swap(s32 interval);

    //@}

    //! シーンの開始
    void beginScene();

    //! シーンの終了
    void endScene();

    //! Imgui描画
    void imguiRender();

    //! カメラの設定
    void setCamera(const Camera* camera);

    //! カメラの設定(行列指定)
    void setCamera(const Matrix& view, const Matrix& proj);

    // 描画バッファを取得
    //!	index=0: 現在のバッファ
    //!	index=1: もう一つのワークバッファ
    gpu::Texture* getRenderBuffer(u32 index = 0) const
    {
        // HDR描画区間の場合はHDRテクスチャ、SDR描画区間はSDRテクスチャ
        u32 x = currentBuffer_ ^ index;
        return (isHDR_) ? hdrBuffer_[x].get() : sdrBuffer_[x].get();
    }

    void swapRenderBuffer()
    {
        currentBuffer_ ^= 1;
    }

    // バックバッファを取得
    gpu::Texture* getBackBuffer() const
    {
        return backBuffer_.get();
    }

    // デプスステンシルを取得
    gpu::Texture* getDepthStencil() const
    {
        return depthStencil_.get();
    }

    //! HDRモードの設定
	void setHDR(bool enable) { isHDR_ = enable; }

    //! 全画面描画
    void put();

    //! シャドウマップの取得
    Shadowmap* getShadowmap() { return &shadowmap_; }

	//! ディフューズテクスチャを取得
    gpu::Texture* getDiffuseTexture() const { return diffuseTexture_.get(); }
	//! スペキュラテクスチャを取得
    gpu::Texture* getSpecularTexture() const { return specularTexture_.get(); }

    //! ワーク用HDRバッファを取得
    gpu::Texture* getHDRWorkBuffer(u32 index, u32 mip = 0) const { return hdrWorkBuffer_[index][mip].get(); }

    // ブレンドステートを取得
    ID3D11BlendState* getBlendState(BLEND_MODE mode) const
    {
        return d3dBlendState_[mode].Get();
    }

private:
    D3D_DRIVER_TYPE              driverType_;            //!< 作成されたデバイスの種類	      (HW, WARP, REF)
    D3D_FEATURE_LEVEL            featureLevel_;          //!< 作成されたデバイスの機能レベル (DX9,DX10,DX11)
    com_ptr<ID3D11Device>        d3dDevice_;             //!< D3Dデバイス
    com_ptr<ID3D11DeviceContext> d3dImmediateContext_;   //!< 3Dイミディエイトコンテキスト
    com_ptr<IDXGISwapChain>      dxgiSwapChain_;         //!< スワップチェイン

    std::unique_ptr<gpu::Texture> backBuffer_;     //!< バックバッファ
    std::unique_ptr<gpu::Texture> depthStencil_;   //!< デプスバッファ
    std::unique_ptr<gpu::Texture> hdrBuffer_[2];   //!< 描画用HDRバッファ
    std::unique_ptr<gpu::Texture> sdrBuffer_[2];   //!< 描画用SDRバッファ

    std::unique_ptr<gpu::Texture> hdrWorkBuffer_[2][7];   //!< ワーク用HDRバッファ

    u32                           currentBuffer_ = 0;   //!< 現在のバッファのインデックス番号
    Shadowmap                     shadowmap_;           //!< シャドウマップ管理
    gpu::ConstantBuffer<CBCamera> cbCamera_;            //!< 定数バッファ(カメラ単位)

    GBuffer                       gbuffer_;
    std::unique_ptr<gpu::Texture> diffuseTexture_;    //!< ディフューズ
    std::unique_ptr<gpu::Texture> specularTexture_;   //!< スペキュラー

    bool isHDR_ = false;   //!< HDRレンダリング中かどうか

    // ブレンドステート
    com_ptr<ID3D11BlendState> d3dBlendState_[BLEND_MODE_MAX];

    std::unique_ptr<gpu::ShaderVs> shaderCopyVs_;          //!< 頂点シェーダー
    std::unique_ptr<gpu::ShaderPs> shaderCopyPs_;          //!< ピクセルシェーダー

    std::unique_ptr<gpu::InputLayout> inputLayout_;                   //!< 入力レイアウト
    com_ptr<ID3D11SamplerState>       d3dSamplerState_LinearWrap_;    //!< サンプラー
    com_ptr<ID3D11SamplerState>       d3dSamplerState_LinearClamp_;   //!< サンプラー

    std::unique_ptr<gpu::Texture> iblDiffuse_;    //!< IBLディフューズテクスチャ
    std::unique_ptr<gpu::Texture> iblSpecular_;   //!< IBLスペキュラーテクスチャ
};

//! システム用テクスチャを取得
gpu::Texture* getSystemTexture(SYSTEM_TEXTURE type);

//! レンダリングシステムを取得
SystemRender* GmRender();

namespace gpu {

//! D3Dデバイスを取得
ID3D11Device* D3DDevice();

//! D3Dデバイスコンテキストを取得
ID3D11DeviceContext* D3DContext();

}   // namespace gpu
