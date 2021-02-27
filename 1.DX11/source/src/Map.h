//===========================================================================
//!	@file	Map.h
//!	@brief	�}�b�v
//===========================================================================
#pragma once

//===========================================================================
//!�@�}�b�v
//===========================================================================
class Map : public CollisionBase
{
public:
    //! �R���X�g���N�^
    Map();

    //! �f�X�g���N�^
    virtual ~Map();

    //! ������
    virtual bool initialize() override;

    //! �X�V
    virtual void update() override;

    //! �`��
    virtual void render() override;

    //! ���
    virtual void finalize() override;

    //! imgui�`��
    virtual void imguiRender() override;

    //-------------------------------------------------------------
    //!	@name	�擾
    //-------------------------------------------------------------
    //@{

    //! �R���C�_�[�̐����擾
    size_t getColliderSize() const;

    //! �{�b�N�X�R���C�_�[���擾
    std::shared_ptr<gpu::BoxCollider> getBoxCollider(int i) const;

    //@}

private:

    //! �t�@�C���ǂݍ���
    void loadFile();

    //! �t�@�C�������o��
    void writeFile();

    //! �C���[�X
    void erase();

private:

    std::unique_ptr<gpu::ShaderVs> shaderVs_;   //!< ���_�V�F�[�_�[
    std::unique_ptr<gpu::ShaderPs> shaderPs_;   //!< �s�N�Z���V�F�[�_�[

    std::shared_ptr<gpu::ModelFBX> cemetery_;  //!< ��
    std::shared_ptr<gpu::ModelFBX> skyDome_;   //!< �X�J�C�h�[��

    std::vector<std::shared_ptr<gpu::BoxCollider>> boxCollider_;   //!< �{�b�N�X�R���C�_�[

    std::fstream file; //! �t�@�C���p

};
