//===========================================================================
//!	@file   Shadowmap.h
//!	@brief	�V���h�E�}�b�v�Ǘ�
//===========================================================================
#pragma once

//===========================================================================
//!	�V���h�E�}�b�v�Ǘ�
//===========================================================================
class Shadowmap
{
public:
    Shadowmap()  = default;
    ~Shadowmap() = default;

    //! ������
    bool initialize(s32 resolution = 2048);

    //! ���
    void finalize();

    //! �V���h�E�p�X�̊J�n
    void begin();

    //! �V���h�E�p�X�̏I��
    void end();

    //! Imgui�`��
    void imguiRender();

    //! �V���h�E�e�N�X�`���̎擾
    gpu::Texture* getShadowTexture() const { return shadowDepthTexture_.get(); }

    //! �����p�r���[�s��
    Matrix getViewMatrix() const { return matLightView_; }

    //! �����p���e�s��
    Matrix getProjMatrix() const { return matLightProj_; }

private:
    s32                           resolution_ = 2048;    //!< �V���h�E�}�b�v�̉𑜓x
    std::unique_ptr<gpu::Texture> shadowDepthTexture_;   //!< �e�p�̃f�v�X�e�N�X�`��
    Camera                        lightCamera_;          //!< �����p�J����
    Matrix                        matLightView_;         //!< �����p�r���[�s��
    Matrix                        matLightProj_;         //!< �����p���e�s��
};
