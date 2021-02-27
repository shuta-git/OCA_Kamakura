//===========================================================================
//!	@file	Pumpkin.h
//!	@brief	�v���C���[
//===========================================================================
#pragma once

//===========================================================================
//!�@�G�l�~�[
//===========================================================================
class Pumpkin : public EnemyBase
{
public:
    //! �R���X�g���N�^
    Pumpkin(Vector3 pos);

    //! �f�X�g���N�^
    virtual ~Pumpkin();

    //! ������
    virtual bool initialize() override;

    //! �X�V
    virtual void update() override;

    //! �X�V
    virtual void update(Vector3 playerPos) override;

    //! �`��
    virtual void render() override;

    //! ���
    virtual void finalize() override;

    //! imgui�`��
    virtual void imguiRender() override;

private:

};
