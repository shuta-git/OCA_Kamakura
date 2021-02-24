//===========================================================================
//! @file   GpuBuffer.cpp
//!	@brief	GPU�o�b�t�@�[
//===========================================================================
namespace gpu {

//---------------------------------------------------------------------------
//! �R���X�g���N�^
//---------------------------------------------------------------------------
Buffer::Buffer()
: d3dBuffer_(nullptr)
{
}

//---------------------------------------------------------------------------
//! �f�X�g���N�^
//---------------------------------------------------------------------------
Buffer::~Buffer()
{
    if(d3dBuffer_) {
        d3dBuffer_->Release();
    }
}

//---------------------------------------------------------------------------
//! ������
//---------------------------------------------------------------------------
bool Buffer::initialize(size_t size, D3D11_USAGE usage, D3D11_BIND_FLAG bindFlags, const void* initialData)
{
    D3D11_BUFFER_DESC desc{};
    desc.ByteWidth      = (size + 15) & ~(16ul - 1);   // �o�b�t�@�̃T�C�Y(16�̔{��)
    desc.Usage          = usage;                       // �o�b�t�@�̔z�u�ꏊ(VRAM��)
    desc.BindFlags      = bindFlags;                   // ���_�o�b�t�@�Ƃ��Đݒ�
    desc.CPUAccessFlags = (usage == D3D11_USAGE_DYNAMIC) ? D3D11_CPU_ACCESS_WRITE : 0;

    // �o�b�t�@�̏����f�[�^��n���Ē��_�o�b�t�@���쐬
    D3D11_SUBRESOURCE_DATA initData{};
    initData.pSysMem = initialData;   // �����f�[�^�̐擪�A�h���X

    if(FAILED(D3DDevice()->CreateBuffer(&desc, initialData ? &initData : nullptr, &d3dBuffer_))) {
        return false;
    }
    return true;
}

//---------------------------------------------------------------------------
//! �������}�b�v�J�n
//---------------------------------------------------------------------------
void* Buffer::map()
{
    // �o�b�t�@��map����
    auto                     d3dBuffer = getD3DBuffer();
    D3D11_MAPPED_SUBRESOURCE data;
    D3DContext()->Map(d3dBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);

    return data.pData;
}

//---------------------------------------------------------------------------
//! �������}�b�v�I��
//---------------------------------------------------------------------------
void Buffer::unmap()
{
    // �o�b�t�@��unmap����
    auto d3dBuffer = getD3DBuffer();
    D3DContext()->Unmap(d3dBuffer, 0);
}

}   // namespace gpu
