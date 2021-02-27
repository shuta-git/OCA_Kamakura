#pragma once


//===========================================================================
// マテリアル
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
//! クラスター
//===========================================================================
struct Cluster
{
    std::vector<u32> indices_;   //! 頂点インデックス番号
    std::vector<f32> weights_;   //! ウェイト
    u32              id_;        //! id
};

//===========================================================================
//! スキン
//===========================================================================
struct Skin
{
    std::vector<Cluster> clusters_;   //! クラスター
};

//===========================================================================
//! メッシュ
//===========================================================================
class MeshFBX
{
public:
    //! 初期化
    bool initialize(const ofbx::Mesh& mesh, const std::string& fbxFileName, f32 scale);

    //! 頂点を取得
    const std::vector<Vector3>& getVertices() const { return vertices_; }

    //! マテリアル一覧を取得
    const std::vector<Material>& getMaterials() const { return materials_; }

    //! 頂点インデックス番号を取得
    const std::vector<u32>& getIndices() const { return indices_; }

    //! 法線を取得
    const std::vector<Vector3>& getNormals() const { return normals_; }

    //! UV座標を取得
    const std::vector<Float2>& getUVs() const { return uvs_; }

    //! マテリアル番号を取得
    const std::vector<s32>& getMaterialIndices() const { return materialIndices_; }

    //! 頂点バッファの取得
    gpu::Buffer* getVertexBuffer() { return vertexBuffer_; }

    //! インデックスバッファの取得
    gpu::Buffer* getIndexBuffer() { return indexBuffer_; }

private:
    std::vector<Vector3> vertices_;   //!< 頂点
    std::vector<u32>     indices_;    //!< 頂点インデックス番号
    std::vector<Float2>  uvs_;        //!< テクスチャUV座標
    std::vector<Vector3> normals_;    //!< 法線

    //--------
    Skin skin_;   //!< スキン
    //-------
    gpu::Buffer* vertexBuffer_;   //!< 頂点バッファ
    gpu::Buffer* indexBuffer_;    //!< インデックスバッファ

    std::vector<Material> materials_;         //!< マテリアル
    std::vector<s32>      materialIndices_;   //!< マテリアル番号
};
