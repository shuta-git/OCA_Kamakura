//===========================================================================
//! @file   ModelMQO.h
//!	@brief  FQO���f��
//===========================================================================

#pragma once

//===========================================================================
//!	���f��
//===========================================================================
class ModelMQO
{
public:
    // �ʏ��
    struct Face
    {
        s32     vertexIndex_[3];   //!< ���_�ԍ�
        s32     material_;         //!< �}�e���A���ԍ�(-1�̏ꍇ�̓}�e���A���Ȃ�)
        Vector3 normal_[3];        //!< �@��
        f32     u_[3];             //!< �e�N�X�`�����WU
        f32     v_[3];             //!< �e�N�X�`�����WV
    };

    // �p�[�c�I�u�W�F�N�g
    struct Object
    {
        std::string          name_;       //!< ���O
        std::vector<Vector3> vertices_;   //!< ���_
        std::vector<Face>    faces_;      //!< �t�F�C�X

        Vector4 color_;   //!< �I�u�W�F�N�g�J���[
        f32     facet_ = 60.0f;
        //! �œK��
        void optimize();
    };

    // �}�e���A��
    struct Material
    {
        gpu::Texture* texture_;
        Vector4 color_;   //!< �}�e���A���J���[
    };

    //! �R���X�g���N�^
    ModelMQO();

    //! �f�X�g���N�^
    ~ModelMQO();

    //! �ǂݍ���
    bool load(const char* fileName, f32 scale = 1.0f);

    //! �`��
    void render();

    //! Object�̌����擾
    size_t getObjectCount() const { return objects_.size(); }

    //! Object�̎擾
    Object* getObject(u32 index) const { return objects_[index]; }

private:
    std::vector<Object*>           objects_;
    std::vector<Material>          materials_;
    std::unique_ptr<gpu::ShaderVs> shaderVs_;   //!< ���_�V�F�[�_�[
    std::unique_ptr<gpu::ShaderPs> shaderPs_;   //!< �s�N�Z���V�F�[�_�[
};
