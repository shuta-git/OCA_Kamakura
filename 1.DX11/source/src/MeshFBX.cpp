//===========================================================================
//!	@file	MeshFBX.cpp
//!	@brief	���b�V��FBX
//===========================================================================
#include"MeshFBX.h"

//---------------------------------------------------------------------------
//! ������
//---------------------------------------------------------------------------
bool MeshFBX::initialize(const ofbx::Mesh& mesh, const std::string& fbxFileName, f32 scale)
{
    const ofbx::Geometry& geometry = *mesh.getGeometry();

    auto                vertexCount = geometry.getVertexCount();   // ���_��
    const ofbx::Vec3*   vertices    = geometry.getVertices();      // ���_�z��̐擪
    std::vector<Vertex> vertexArray;
    //------------------------------------------------------------
    // ���_�̓ǂݍ���
    //------------------------------------------------------------
    for(u32 i = 0; i < vertexCount; ++i) {
        ofbx::Vec3 vertex = vertices[i];

        Vector3 v;
        v.x_ = vertex.x * scale;
        v.y_ = vertex.y * scale;
        v.z_ = vertex.z * scale;
        vertices_.emplace_back(std::move(v));
    }

    //--------------------------------------e----------------------
    //! ���_�C���f�N�b�X�̓ǂݍ���
    //------------------------------------------------------------
    for(s32 i = 0; i < geometry.getIndexCount(); ++i) {
        u32  index  = (geometry.getFaceIndices()[i] < 0) ? -geometry.getFaceIndices()[i] - 1 : geometry.getFaceIndices()[i];
        auto uv     = geometry.getUVs()[i];
        auto normal = geometry.getNormals()[i];

        //! ���_�C���f�b�N�X
        indices_.emplace_back(index);
        //! �@��
        normals_.emplace_back(Vector3((f32)normal.x, (f32)normal.y, (f32)normal.z));

        //! UV���W
        uvs_.emplace_back(Float2{ f32(uv.x), f32(1.0f - uv.y) });

        Vertex vertex;
        vertex.color_    = { 255, 255, 255, 255 };
        vertex.normal_   = { (float)normal.x, (float)normal.y, (float)normal.z };
        vertex.uv_       = { (float)uv.x, 1.0f - (float)uv.y };
        vertex.position_ = { vertices_[index].x_, vertices_[index].y_, vertices_[index].z_ };
        vertexArray.push_back(vertex);
    }

    vertexBuffer_ = new gpu::Buffer();
    indexBuffer_  = new gpu::Buffer();

    vertexBuffer_->initialize(sizeof(Vertex) * vertexArray.size(), D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, vertexArray.data());
    indexBuffer_->initialize(sizeof(u32) * indices_.size(), D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, indices_.data());

    for(s32 i = 0; i < geometry.getIndexCount() / 3; ++i) {
        auto materialIndex = 0;

        if(geometry.getMaterials()) {
            materialIndex = geometry.getMaterials()[i];
        }
        //! �}�e���A���ԍ�
        materialIndices_.emplace_back(materialIndex);
    }

    //------------------------------------------------------------
    //! �X�L���̓ǂݍ���
    //------------------------------------------------------------
    auto skin = geometry.getSkin();
    if(skin != nullptr) {
        for(s32 i = 0; i < skin->getClusterCount(); ++i) {
            auto             cluster = skin->getCluster(i);
            std::vector<u32> indices;
            for(s32 i = 0; i < cluster->getIndicesCount(); ++i) {
                indices.emplace_back(cluster->getIndices()[i]);
            }
            std::vector<f32> weights;
            for(s32 i = 0; i < cluster->getWeightsCount(); ++i) {
                weights.emplace_back(cluster->getWeights()[i]);
            }
            u32     id = cluster->getLink()->id;
            Cluster c;
            c.indices_ = indices;
            c.weights_ = weights;
            c.id_      = id;
            skin_.clusters_.emplace_back(c);
        }
    }

    //------------------------------------------------------------
    //! �}�e���A���̓ǂݍ���
    //------------------------------------------------------------
    for(s32 i = 0; i < mesh.getMaterialCount(); ++i) {
        auto m = mesh.getMaterial(i);
        auto t = m->getTexture(ofbx::Texture::TextureType::DIFFUSE);

        auto f  = t->getFileName();
        auto f2 = t->getRelativeFileName();

        //! �e�N�X�`���t�@�C�����̎擾
        char fileName[MAX_PATH];
        f.toString(fileName);

        char drive[MAX_PATH];
        char dir[MAX_PATH];
        char name[MAX_PATH];
        char ext[MAX_PATH];

        //! �e�N�X�`���t�@�C�����̕����������o
        _splitpath_s(fileName, drive, dir, name, ext);

        //! ���݂̃t�@�C���p�X�𕪉����ăp�X�����𒊏o
        _splitpath_s(fbxFileName.c_str(),
                     drive, sizeof(drive),
                     dir, sizeof(dir),
                     nullptr, 0,
                     nullptr, 0);

        //! FBX�t�@�C���̃p�X�ƃe�N�X�`���t�@�C����������
        _makepath_s(fileName, drive, dir, name, ".png");


        Material material;
        material.texture_[(s32)Material::TextureType::Albedo] = gpu::createTexture(fileName);

        //! ������"_A"�����Ă����ꍇ�O��
        if(strlen(name) > 2) {
            auto tail = name + strlen(name);
            if(strcmp(tail - 2, "_A") == 0) {
                *(tail - 2) = 0;
            }
            else {
                material.texture_[(s32)Material::TextureType::Normal]    = gpu::createTexture(fileName);
                material.texture_[(s32)Material::TextureType::Roughness] = gpu::createTexture(fileName);
                material.texture_[(s32)Material::TextureType::Metalness] = gpu::createTexture(fileName);
                material.texture_[(s32)Material::TextureType::Ao]        = gpu::createTexture(fileName);
                materials_.emplace_back(std::move(material));

                continue;
            }
        }

        // Normalmap
        {
            char file[MAX_PATH];
            strcpy(file, name);   // �x�[�X�̖��O��ǉ�
            strcat(file, "_N");   // ����������ǉ�

            _makepath_s(fileName, drive, dir, file, ext);
            material.texture_[(s32)Material::TextureType::Normal] = gpu::createTexture(fileName);
        }

        //! Roughness
        {
            char file[MAX_PATH];
            strcpy_s(file, name);   //! �x�[�X�̖��O
            strcat_s(file, "_R");   //! ����������ǉ�

            _makepath_s(fileName, drive, dir, file, ext);
            material.texture_[(s32)Material::TextureType::Roughness] = gpu::createTexture(fileName);
        }

        //! Metallness
        {
            char file[MAX_PATH];
            strcpy_s(file, name);   //! �x�[�X�̖��O
            strcat_s(file, "_M");   //! ����������ǉ�

            _makepath_s(fileName, drive, dir, file, ext);
            material.texture_[(s32)Material::TextureType::Metalness] = gpu::createTexture(fileName);
        }

        // AO
        {
            char file[MAX_PATH];
            strcpy(file, name);    // �x�[�X�̖��O��ǉ�
            strcat(file, "_AO");   // ����������ǉ�

            _makepath_s(fileName, drive, dir, file, ext);
            material.texture_[(s32)Material::TextureType::Ao] = gpu::createTexture(fileName);
        }

        materials_.emplace_back(std::move(material));
    }

    return true;
}