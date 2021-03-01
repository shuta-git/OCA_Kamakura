 //---------------------------------------------------------------------------
//!	@file   gpu_ModelFBX.cpp
//!	@brief	GPU FBX
//---------------------------------------------------------------------------

#define GM_ASSERT_MESSAGE(a, b) assert(a&& b)

#include"OpenFBX/src/ofbx.h"

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
    
    gpu::Texture*     texture_[(s32)TextureType::Max] = {};
};

namespace gpu {

struct FBXDesc
{
    const char*     fileName;   //!< モデルパス
    int fileSize;
    std::vector<u8> fileImage;
};


//===========================================================================
//! FBX実装部
//===========================================================================
class FBXImpl final : public gpu::FBX
{
public:
    //! コンストラクタ
    FBXImpl(const FBXDesc& desc)
    {
        desc_ = desc;
        //FBX読み込み
        ofbx::u64 loadFlags = (ofbx::u64)ofbx::LoadFlags::TRIANGULATE;

        ofbx::IScene* scene_ = ofbx::load(desc_.fileImage.data(), desc_.fileSize, loadFlags);   // FBXシーンデータ

        if(!&scene_) {
            OutputDebugString(ofbx::getError());
            assert(0);
        }

        //-------------------------------------------------------------
        // メッシュの読み込み
        //-------------------------------------------------------------
        auto& scene = *scene_;

        //auto meshCount = scene.getMeshCount();   // メッシュ数
        //for(u32 i = 0; i < meshCount; ++i) {
        //    const ofbx::Mesh& fbxMesh = *scene.getMesh(i);

        //    Mesh mesh;
        //    if(mesh.initialize(fbxMesh, desc_.fileName) == false) {
        //        continue;
        //    }
        //    meshes_.emplace_back(std::move(mesh));
        //}

        //auto animationStackCount = scene.getAnimationStackCount();
        //for(u32 i = 0; i < animationStackCount; ++i) {
        //    auto animationStack = scene.getAnimationStack(i);
        //    auto animationLayer = animationStack->getLayer(0);

        //    s32 c = 0;
        //    while(auto animationCurveNode = animationLayer->getCurveNode(c)) {
        //        auto bone = animationCurveNode->getBone();
        //        c++;
        //    }

        //    printf("test");
        //}
    }

    //! デストラクタ
    virtual ~FBXImpl()
    {
    }

    // 初期化
    bool initialize()
    {
        return true;
    }

private:
    // コピー禁止/move禁止
    FBXImpl(const FBXImpl&) = delete;
    FBXImpl(FBXImpl&&)      = delete;
    FBXImpl& operator=(const FBXImpl&) = delete;
    FBXImpl& operator=(FBXImpl&&) = delete;

private:

    //----------------------------------------------------------
    //! @name 参照
    //----------------------------------------------------------
    //@{

    FBXDesc desc_ = {};

    //@}

};

//===========================================================================
// メッシュ
//===========================================================================
class Mesh
{
public:
    //! 初期化
    //bool initialize(const ofbx::Mesh& mesh, const std::string& fbxFileName);

    //! 頂点を取得
    const std::vector<Vector3>& getVertices() const { return vertices_; }

    //! マテリアル一覧を取得
    const std::vector<Material>& getMaterials() const { return materials_; }

    //! 頂点インデックス番号を取得
    const std::vector<u32>& getIndices() const { return indices_; }

    //! 法線を取得
    const std::vector<Vector3>& getNormals() const { return normals_; }

    //! 接ベクトルを取得
    const std::vector<Vector3>& getTangents() const { return tangents_; }

    //! 従法線ベクトルを取得
    const std::vector<Vector3>& getBinormals() const { return binormals_; }

    //! UV座標を取得
    const std::vector<Float2>& getUVs() const { return uvs_; }

    //! マテリアル番号を取得
    const std::vector<s32>& getMaterialIndices() const { return materialIndices_; }

private:
    std::vector<Vector3>  vertices_;    // 頂点
    std::vector<Material> materials_;   // マテリアル

    std::vector<u32>     indices_;           // 頂点インデックス番号
    std::vector<Float2>  uvs_;               // テクスチャUV座標
    std::vector<Vector3> normals_;           // 法線
    std::vector<Vector3> tangents_;          // 接ベクトル
    std::vector<Vector3> binormals_;         // 従法線ベクトル
    std::vector<s32>     materialIndices_;   // マテリアル番号
};

std::vector<Mesh> meshes_;
u32               indexCount_;

//---------------------------------------------------------------------------
//! PMDをファイルから作成
//---------------------------------------------------------------------------
std::shared_ptr<gpu::FBX> createFBXFromFile(const char* fileName)
{
   FILE* fp;
   if(fopen_s(&fp, fileName, "rb")) {
       return false;
   }

   //-------------------------------------------------------------
   // FBXバイナリ読み込み
   //-------------------------------------------------------------
   fseek(fp, 0, SEEK_END);
   int fileSize = ftell(fp);
   fseek(fp, 0, SEEK_SET);

   std::vector<u8> fileImage(fileSize);
   fread(fileImage.data(), fileSize, 1, fp);
   fclose(fp);

   FBXDesc desc{};
   desc.fileName  = fileName;
   desc.fileSize = fileSize;
   desc.fileImage = fileImage;

   auto p = std::make_shared<gpu::FBXImpl>(desc);
   if(!p->initialize()) {
       p.reset();
   }

   return p;
}

//---------------------------------------------------------------------------
// 三角形の座標とUV座標から接ベクトルを求める
//!	@return 第1パラメーター : 接ベクトル
//!	@return 第2パラメーター : 従法線ベクトル
//---------------------------------------------------------------------------
std::tuple<Vector3, Vector3> calcTangent(const Vector3& p0, const Vector3& p1, const Vector3& p2,
                                         const Float2& uv0, const Float2& uv1, const Float2& uv2)
{
    // 5次元→3次元頂点に
    Vector3 cp0[3] = {
        { p0.x_, uv0.x_, uv0.y_ },
        { p0.y_, uv0.x_, uv0.y_ },
        { p0.z_, uv0.x_, uv0.y_ },
    };
    Vector3 cp1[3] = {
        { p1.x_, uv1.x_, uv1.y_ },
        { p1.y_, uv1.x_, uv1.y_ },
        { p1.z_, uv1.x_, uv1.y_ },
    };
    Vector3 cp2[3] = {
        { p2.x_, uv2.x_, uv2.y_ },
        { p2.y_, uv2.x_, uv2.y_ },
        { p2.z_, uv2.x_, uv2.y_ },
    };

    // 平面パラメータからUV軸座標算出
    float u[3], v[3];
    for(int i = 0; i < 3; ++i) {
        Vector3 v1  = cp1[i] - cp0[i];
        Vector3 v2  = cp2[i] - cp1[i];
        Vector3 abc = Vector3::cross(v1, v2);

        if(abc.x_ == 0.0f) {
            GM_ASSERT_MESSAGE(false, "三角形かUV座標が縮退しています.");
            return std::make_tuple(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f));
        }

        u[i] = -abc.y_ / abc.x_;
        v[i] = -abc.z_ / abc.x_;
    }

    Vector3 tangent  = Vector3{ u[0], u[1], u[2] }.normalize();
    Vector3 binormal = Vector3{ v[0], v[1], v[2] }.normalize() * -1.0f;

    return std::make_tuple(tangent, binormal);
}

}   // namespace gpu