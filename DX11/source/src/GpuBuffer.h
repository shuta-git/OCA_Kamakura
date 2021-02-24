//===========================================================================
//! @file   GpuBuffer.h
//!	@brief	GPU�o�b�t�@�[
//===========================================================================
#pragma once

namespace gpu {

class Buffer
{
public:
    //! �R���X�g���N�^
    Buffer();

    //! �f�X�g���N�^
    virtual ~Buffer();

    //! ������
    //!	@param	[in]	size		�o�b�t�@�T�C�Y(�P��:byte)
    //!	@param	[in]	usage		�p�r
    //!	@param	[in]	bindFlags	�ݒ�摮��
    //!	@param	[in]	initialData	�����f�[�^�[(nullptr�̏ꍇ�͂Ȃ�)
    //! @retval true	����(����I��)
    //! @retval false	���s(�G���[�I��)
    bool initialize(size_t size, D3D11_USAGE usage, D3D11_BIND_FLAG bindFlags, const void* initialData = nullptr);

    //! D3D�o�b�t�@���擾
    ID3D11Buffer* getD3DBuffer() const { return d3dBuffer_; }

protected:
    //! �������}�b�v�J�n
    void* map();

    //! �������}�b�v�I��
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

    //! �X�V�J�n
    T* begin()
    {
        // ������s���̓o�b�t�@���쐬
        if(!getD3DBuffer()) {
            create();
        }

        return reinterpret_cast<T*>(map());
    }

    // �X�V�I��
    void end()
    {
        // �o�b�t�@��unmap����
        unmap();
    };

private:
    // �o�b�t�@�쐬
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
