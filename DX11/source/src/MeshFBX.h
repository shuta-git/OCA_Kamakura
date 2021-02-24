//===========================================================================
//!	@file	MeshFBX.h
//!	@brief	���b�V��FBX
//===========================================================================
#pragma once

//===========================================================================
// �}�e���A��
//===========================================================================
struct Material
{
    enum class TextureType
    {
        Albedo,
        Normal,
        Roughness,
        Metalness,
        Ao,
        //----
        Max,
    };

    gpu::Texture* texture_[(s32)TextureType::Max] = {};
};

//===========================================================================
//! �N���X�^�[
//===========================================================================
struct Cluster
{
    std::vector<u32> indices_;   //! ���_�C���f�b�N�X�ԍ�
    std::vector<f32> weights_;   //! �E�F�C�g
    u32              id_;        //! id
};

//===========================================================================
//! �X�L��
//===========================================================================
struct Skin
{
    std::vector<Cluster> clusters_;   //! �N���X�^�[
};

//===========================================================================
//! ���b�V��
//===========================================================================
class MeshFBX
{
public:
    //! ������
    bool initialize(const ofbx::Mesh& mesh, const std::string& fbxFileName, f32 scale);

    //! ���_���擾
    const std::vector<Vector3>& getVertices() const { return vertices_; }

    //! �}�e���A���ꗗ���擾
    const std::vector<Material>& getMaterials() const { return materials_; }

    //! ���_�C���f�b�N�X�ԍ����擾
    const std::vector<u32>& getIndices() const { return indices_; }

    //! �@�����擾
    const std::vector<Vector3>& getNormals() const { return normals_; }

    //! UV���W���擾
    const std::vector<Float2>& getUVs() const { return uvs_; }

    //! �}�e���A���ԍ����擾
    const std::vector<s32>& getMaterialIndices() const { return materialIndices_; }

    //!  ���_�z��̃T�C�Y���擾
    const size_t getVertexArraySize() const { return vertexArray.size(); }

    //! ���_�o�b�t�@�̎擾
    gpu::Buffer* getVertexBuffer() { return vertexBuffer_; }

    //! �C���f�b�N�X�o�b�t�@�̎擾
    gpu::Buffer* getIndexBuffer() { return indexBuffer_; }

private:
    std::vector<Vector3> vertices_;   //!< ���_
    std::vector<u32>     indices_;    //!< ���_�C���f�b�N�X�ԍ�
    std::vector<Float2>  uvs_;        //!< �e�N�X�`��UV���W
    std::vector<Vector3> normals_;    //!< �@��

    //--------
    Skin skin_;   //!< �X�L��
    //-------
    gpu::Buffer* vertexBuffer_;   //!< ���_�o�b�t�@
    gpu::Buffer* indexBuffer_;    //!< �C���f�b�N�X�o�b�t�@

    std::vector<Vertex> vertexArray;    //!< ���_�z��

    std::vector<Material> materials_;         //!< �}�e���A��
    std::vector<s32>      materialIndices_;   //!< �}�e���A���ԍ�
};
