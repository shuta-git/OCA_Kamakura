//===========================================================================
//! @file   GpuBuffer.h
//!	@brief	GPUバッファー
//===========================================================================
#pragma once

namespace gpu {

class Buffer
{
public:
    //! コンストラクタ
    Buffer();

    //! デストラクタ
    virtual ~Buffer();

    //! 初期化
    //!	@param	[in]	size		バッファサイズ(単位:byte)
    //!	@param	[in]	usage		用途
    //!	@param	[in]	bindFlags	設定先属性
    //!	@param	[in]	initialData	初期データー(nullptrの場合はなし)
    //! @retval true	成功(正常終了)
    //! @retval false	失敗(エラー終了)
    bool initialize(size_t size, D3D11_USAGE usage, D3D11_BIND_FLAG bindFlags, const void* initialData = nullptr);

    //! D3Dバッファを取得
    ID3D11Buffer* getD3DBuffer() const { return d3dBuffer_; }

protected:
    //! メモリマップ開始
    void* map();

    //! メモリマップ終了
    void unmap();

private:
    ID3D11Buffer* d3dBuffer_;
};

template<typename T>
class ConstantBuffer final : public Buffer
{
public:
    ConstantBuffer() = default;

    ~ConstantBuffer() = default;

    //! 更新開始
    T* begin()
    {
        // 初回実行時はバッファを作成
        if(!getD3DBuffer()) {
            create();
        }

        return reinterpret_cast<T*>(map());
    }

    // 更新終了
    void end()
    {
        // バッファをunmapする
        unmap();
    };

private:
    // バッファ作成
    bool create()
    {
        auto size = sizeof(T);
        if(!initialize(size, D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER)) {
            return false;
        }
        return true;
    }
};

}   // namespace gpu
