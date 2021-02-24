//===========================================================================
//!	@file	EnemyManager.h
//!	@brief	�G�l�~�[�}�l�[�W���[
//===========================================================================
#pragma once

//===========================================================================
//! �G�l�~�[�}�l�W���[
//===========================================================================
class EnemyManager : public ManagerBase
{
public:
    //! �R���X�g���N�^
    EnemyManager();

    //! �f�X�g���N�^
    virtual ~EnemyManager();

    //! ������
    virtual bool initialize() override;

    //! �X�V
    virtual void update() override;

    void update(Vector3 playerPos);

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

    //! �G�l�~�[�̐����擾
    size_t getSize() const;				

    //! �{�b�N�X�R���C�_�[���擾
    std::shared_ptr<gpu::BoxCollider> getBoxCollider(int i) const;

    //! ���W���擾
    Vector3 getPos(int i) const;

    //@}

    void setPos(int i, Vector3 pos);

private:

    //! �C���[�X����
    void erase();

    //! �X�|�[��
    void spawn();

private:

	std::vector<std::unique_ptr<EnemyBase>> enemies;   // �G�l�~�[

    f32 time_;   //!< �o�ߎ���(s)
};
