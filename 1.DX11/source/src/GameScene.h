//===========================================================================
//!	@file	GameScene.h
//!	@brief	�Q�[���V�[��
//===========================================================================
#pragma once

//===========================================================================
//! �Q�[���V�[��
//===========================================================================
class GameScene : public Scene
{
public:
    //! �R���X�g���N�^
    GameScene();

    //! �f�X�g���N�^
    virtual ~GameScene();

     //! ������
    virtual bool initialize() override;

    //! �X�V
    virtual void update() override;

    //! �`��
    virtual void render(RenderMode mode) override;

    //! ���
    virtual void finalize() override;

    //! imgui�`��
    virtual void imguiRender() override;

private:

    //! �����蔻��
    void collisionDetection();

private:

    std::unique_ptr<gpu::ShaderCs> shaderCs_;   //!< ���Z�V�F�[�_�[

    std::unique_ptr<Player> player_;        //!< �v���C���[
    std::unique_ptr<EnemyManager> enemy_;   //!< �G�l�~�[
    std::unique_ptr<Map> map_;              //!< �}�b�v

    f32         textScale_;
    std::string text_;

    f32 time_;

};
