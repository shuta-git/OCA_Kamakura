//===========================================================================
//!	@file	MeshFBX.cpp
//!	@brief	メッシュFBX
//===========================================================================
#include"MeshFBX.h"

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
bool MeshFBX::initialize(const ofbx::Mesh& mesh, const std::string& fbxFileName, f32 scale)
{
    const ofbx::Geometry& geometry = *mesh.getGeometry();

    auto                vertexCount = geometry.getVertexCount();   // 頂点数
    const ofbx::Vec3*   vertices    = geometry.getVertices();      // 頂点配列の先頭
    std::vector<Vertex> vertexArray;
    //------------------------------------------------------------
    // 頂点の読み込み
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
    //! 頂点インデクッスの読み込み
    //------------------------------------------------------------
    for(s32 i = 0; i < geometry.getIndexCount(); ++i) {
        u32  index  = (geometry.getFaceIndices()[i] < 0) ? -geometry.getFaceIndices()[i] - 1 : geometry.getFaceIndices()[i];
        auto uv     = geometry.getUVs()[i];
        auto normal = geometry.getNormals()[i];

        //! 頂点インデックス
        indices_.emplace_back(index);
        //! 法線
        normals_.emplace_back(Vector3((f32)normal.x, (f32)normal.y, (f32)normal.z));

        //! UV座標
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
        //! マテリアル番号
        materialIndices_.emplace_back(materialIndex);
    }

    //------------------------------------------------------------
    //! スキンの読み込み
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
    //! マテリアルの読み込み
    //------------------------------------------------------------
    for(s32 i = 0; i < mesh.getMaterialCount(); ++i) {
        auto m = mesh.getMaterial(i);
        auto t = m->getTexture(ofbx::Texture::TextureType::DIFFUSE);

        auto f  = t->getFileName();
        auto f2 = t->getRelativeFileName();

        //! テクスチャファイル名の取得
        char fileName[MAX_PATH];
        f.toString(fileName);

        char drive[MAX_PATH];
        char dir[MAX_PATH];
        char name[MAX_PATH];
        char ext[MAX_PATH];

        //! テクスチャファイル名の部分だけ抽出
        _splitpath_s(fileName, drive, dir, name, ext);

        //! 現在のファイルパスを分解してパス部分を抽出
        _splitpath_s(fbxFileName.c_str(),
                     drive, sizeof(drive),
                     dir, sizeof(dir),
                     nullptr, 0,
                     nullptr, 0);

        //! FBXファイルのパスとテクスチャファイル名を結合
        _makepath_s(fileName, drive, dir, name, ".png");


        Material material;
        material.texture_[(s32)Material::TextureType::Albedo] = gpu::createTexture(fileName);

        //! 末尾に"_A"がついていた場合外す
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
            strcpy(file, name);   // ベースの名前を追加
            strcat(file, "_N");   // 属性文字を追加

            _makepath_s(fileName, drive, dir, file, ext);
            material.texture_[(s32)Material::TextureType::Normal] = gpu::createTexture(fileName);
        }

        //! Roughness
        {
            char file[MAX_PATH];
            strcpy_s(file, name);   //! ベースの名前
            strcat_s(file, "_R");   //! 属性文字を追加

            _makepath_s(fileName, drive, dir, file, ext);
            material.texture_[(s32)Material::TextureType::Roughness] = gpu::createTexture(fileName);
        }

        //! Metallness
        {
            char file[MAX_PATH];
            strcpy_s(file, name);   //! ベースの名前
            strcat_s(file, "_M");   //! 属性文字を追加

            _makepath_s(fileName, drive, dir, file, ext);
            material.texture_[(s32)Material::TextureType::Metalness] = gpu::createTexture(fileName);
        }

        // AO
        {
            char file[MAX_PATH];
            strcpy(file, name);    // ベースの名前を追加
            strcat(file, "_AO");   // 属性文字を追加

            _makepath_s(fileName, drive, dir, file, ext);
            material.texture_[(s32)Material::TextureType::Ao] = gpu::createTexture(fileName);
        }

        materials_.emplace_back(std::move(material));
    }

    return true;
}