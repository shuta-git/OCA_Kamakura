//===========================================================================
//!	DirectX11�`��V�X�e���Ǘ�
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
    BLEND_MODE_OFF,     //!< ����
    BLEND_MODE_BLEND,   //!< �ʏ�u�����h
    BLEND_MODE_ADD,     //!< ���Z
    //----
    BLEND_MODE_MAX,   //!< �u�����h���[�h�̌�
};

// �V�[���P�ʂōX�V����萔�o�b�t�@
struct CBCamera
{
    Matrix matView_;   //!< �r���[�s��
    Matrix matProj_;   //!< ���e�s��

    Matrix matLightView_;
    Matrix matLightProj_;

    Float3 cameraPosition_;   //!< �J�����̃��[���h���W
    f32    dummy_;            //!< �A���C�����g�����낦��_�~�[(���g�p)
};

//===========================================================================
//!	�����_�����O�V�X�e��
//===========================================================================
class SystemRender
{
public:
    //-------------------------------------------------------------
    //!	@name	������
    //-------------------------------------------------------------
    //@{

    //! �R���X�g���N�^
    SystemRender();

    //! �f�X�g���N�^
    virtual ~SystemRender();

    //! ������
    //!	@param	[in]	width	��
    //!	@param	[in]	height	����
    //!	@param	[in]	hWnd	�`��Ώۂ̃E�B���h�E�n���h��
    //!	@retval	true	����I��	�i�����j
    //!	@retval	false	�G���[�I��	�i���s�j
    bool setup(u32 width, u32 height, HWND hWnd);

    //! ���
    void cleanup();

    //@}
    //-------------------------------------------------------------
    //!	@name	����
    //-------------------------------------------------------------
    //@{

    //! �`��J�n
    void begin();

    //! �`��I��
    void end();

    //!	��ʍX�V
    //! @param	[in]	interval	��ʍX�V���� 1:60fps 2:30fps 3:20fps 4:15fps
    void swap(s32 interval);

    //@}

    //! �V�[���̊J�n
    void beginScene();

    //! �V�[���̏I��
    void endScene();

    //! Imgui�`��
    void imguiRender();

    //! �J�����̐ݒ�
    void setCamera(const Camera* camera);

    //! �J�����̐ݒ�(�s��w��)
    void setCamera(const Matrix& view, const Matrix& proj);

    // �`��o�b�t�@���擾
    //!	index=0: ���݂̃o�b�t�@
    //!	index=1: ������̃��[�N�o�b�t�@
    gpu::Texture* getRenderBuffer(u32 index = 0) const
    {
        // HDR�`���Ԃ̏ꍇ��HDR�e�N�X�`���ASDR�`���Ԃ�SDR�e�N�X�`��
        u32 x = currentBuffer_ ^ index;
        return (isHDR_) ? hdrBuffer_[x].get() : sdrBuffer_[x].get();
    }

    void swapRenderBuffer()
    {
        currentBuffer_ ^= 1;
    }

    // �o�b�N�o�b�t�@���擾
    gpu::Texture* getBackBuffer() const
    {
        return backBuffer_.get();
    }

    // �f�v�X�X�e���V�����擾
    gpu::Texture* getDepthStencil() const
    {
        return depthStencil_.get();
    }

    //! HDR���[�h�̐ݒ�
	void setHDR(bool enable) { isHDR_ = enable; }

    //! �S��ʕ`��
    void put();

    //! �V���h�E�}�b�v�̎擾
    Shadowmap* getShadowmap() { return &shadowmap_; }

	//! �f�B�t���[�Y�e�N�X�`�����擾
    gpu::Texture* getDiffuseTexture() const { return diffuseTexture_.get(); }
	//! �X�y�L�����e�N�X�`�����擾
    gpu::Texture* getSpecularTexture() const { return specularTexture_.get(); }

    //! ���[�N�pHDR�o�b�t�@���擾
    gpu::Texture* getHDRWorkBuffer(u32 index, u32 mip = 0) const { return hdrWorkBuffer_[index][mip].get(); }

    // �u�����h�X�e�[�g���擾
    ID3D11BlendState* getBlendState(BLEND_MODE mode) const
    {
        return d3dBlendState_[mode].Get();
    }

private:
    D3D_DRIVER_TYPE              driverType_;            //!< �쐬���ꂽ�f�o�C�X�̎��	      (HW, WARP, REF)
    D3D_FEATURE_LEVEL            featureLevel_;          //!< �쐬���ꂽ�f�o�C�X�̋@�\���x�� (DX9,DX10,DX11)
    com_ptr<ID3D11Device>        d3dDevice_;             //!< D3D�f�o�C�X
    com_ptr<ID3D11DeviceContext> d3dImmediateContext_;   //!< 3D�C�~�f�B�G�C�g�R���e�L�X�g
    com_ptr<IDXGISwapChain>      dxgiSwapChain_;         //!< �X���b�v�`�F�C��

    std::unique_ptr<gpu::Texture> backBuffer_;     //!< �o�b�N�o�b�t�@
    std::unique_ptr<gpu::Texture> depthStencil_;   //!< �f�v�X�o�b�t�@
    std::unique_ptr<gpu::Texture> hdrBuffer_[2];   //!< �`��pHDR�o�b�t�@
    std::unique_ptr<gpu::Texture> sdrBuffer_[2];   //!< �`��pSDR�o�b�t�@

    std::unique_ptr<gpu::Texture> hdrWorkBuffer_[2][7];   //!< ���[�N�pHDR�o�b�t�@

    u32                           currentBuffer_ = 0;   //!< ���݂̃o�b�t�@�̃C���f�b�N�X�ԍ�
    Shadowmap                     shadowmap_;           //!< �V���h�E�}�b�v�Ǘ�
    gpu::ConstantBuffer<CBCamera> cbCamera_;            //!< �萔�o�b�t�@(�J�����P��)

    GBuffer                       gbuffer_;
    std::unique_ptr<gpu::Texture> diffuseTexture_;    //!< �f�B�t���[�Y
    std::unique_ptr<gpu::Texture> specularTexture_;   //!< �X�y�L�����[

    bool isHDR_ = false;   //!< HDR�����_�����O�����ǂ���

    // �u�����h�X�e�[�g
    com_ptr<ID3D11BlendState> d3dBlendState_[BLEND_MODE_MAX];

    std::unique_ptr<gpu::ShaderVs> shaderCopyVs_;          //!< ���_�V�F�[�_�[
    std::unique_ptr<gpu::ShaderPs> shaderCopyPs_;          //!< �s�N�Z���V�F�[�_�[

    std::unique_ptr<gpu::InputLayout> inputLayout_;                   //!< ���̓��C�A�E�g
    com_ptr<ID3D11SamplerState>       d3dSamplerState_LinearWrap_;    //!< �T���v���[
    com_ptr<ID3D11SamplerState>       d3dSamplerState_LinearClamp_;   //!< �T���v���[

    std::unique_ptr<gpu::Texture> iblDiffuse_;    //!< IBL�f�B�t���[�Y�e�N�X�`��
    std::unique_ptr<gpu::Texture> iblSpecular_;   //!< IBL�X�y�L�����[�e�N�X�`��
};

//! �V�X�e���p�e�N�X�`�����擾
gpu::Texture* getSystemTexture(SYSTEM_TEXTURE type);

//! �����_�����O�V�X�e�����擾
SystemRender* GmRender();

namespace gpu {

//! D3D�f�o�C�X���擾
ID3D11Device* D3DDevice();

//! D3D�f�o�C�X�R���e�L�X�g���擾
ID3D11DeviceContext* D3DContext();

}   // namespace gpu
