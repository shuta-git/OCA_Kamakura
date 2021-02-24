//===========================================================================
//! @file   ModelFBX.cpp
//!	@brief  FBXモデル
//===========================================================================
#include "ModelFBX.h"
#include "ImGuiFBX.h"
#include <inttypes.h>

std::unordered_map<const ofbx::Mesh*, MeshFBX*> meshTable;

namespace gpu {

ofbx::IScene* gScene_ = nullptr;

//===========================================================================
//! FBX実装部
//===========================================================================
class ModelFBXImpl final : public gpu::ModelFBX
{
public:

    ModelFBXImpl(char* filePath, f32 scale, ofbx::IScene* scene)
    {
        filePath_ = filePath;
        scale_    = scale;
        scene_    = scene;

        char drive[MAX_PATH];
        char dir[MAX_PATH];
        char ext[MAX_PATH];
        //! テクスチャファイル名の部分だけ抽出
        _splitpath_s(filePath_, drive, dir, fileName_, ext);

        //-------------------------------------------------------------
        // シェーダーの読み込み
        //-------------------------------------------------------------
        shaderVs_ = gpu::ShaderVs::create("shader/vsModel.fx");
        //shaderPs_ = gpu::ShaderPs::create("shader/psModel.fx");
        shaderPs_ = gpu::ShaderPs::create("shader/psGBuffer.fx");
        // テスト用
        //shaderPsTexture_ = gpu::ShaderPs::create("shader/psTexture.fx");

        loadMesh();
        //loadAnimation();
    }

    virtual ~ModelFBXImpl()
    {
    }

    // 初期化
    bool initialize()
    {
        return true;
    }

private:
    // コピー禁止/move禁止
    ModelFBXImpl(const ModelFBXImpl&) = delete;
    ModelFBXImpl(ModelFBXImpl&&)      = delete;
    ModelFBXImpl& operator=(const ModelFBXImpl&) = delete;
    ModelFBXImpl& operator=(ModelFBXImpl&&) = delete;

private:
    //!　メッシュ読み込み
    void loadMesh();

    //!  アニメーション読み込み
    void loadAnimation();

};

//---------------------------------------------------------------------------
//!　描画
//---------------------------------------------------------------------------
void ModelFBX::render()
{

    matWorld = Matrix::rotateX(rotate_.x_) *
               Matrix::rotateY(rotate_.y_) *
               Matrix::rotateZ(rotate_.z_) *
               Matrix::scale(Vector3(1.0f, -1.0f, 1.0f)) *
               Matrix::scale(Vector3(0.01f, 0.01f, 0.01f));

    matWorld = matWorld * Matrix::translate(pos_);

    // 行列設定
    dxLoadMatrixf(matWorld);

    // メッシュ描画
    for(auto& mesh : meshes_) {
        auto materialIndices = mesh.getMaterialIndices();
        auto materials       = mesh.getMaterials();

        // シェーダー
        gpu::setShader(shaderVs_, shaderPs_);
        // テクスチャ
        gpu::setTexture(4, materials[0].texture_[(s32)Material::TextureType::Albedo]);
        gpu::setTexture(5, materials[0].texture_[(s32)Material::TextureType::Albedo]);
        gpu::setTexture(6, materials[0].texture_[(s32)Material::TextureType::Albedo]);
        gpu::setTexture(7, materials[0].texture_[(s32)Material::TextureType::Albedo]);
        gpu::setTexture(8, materials[0].texture_[(s32)Material::TextureType::Albedo]);

        // バッファ
        gpu::setVertexBuffer(mesh.getVertexBuffer(), sizeof(Vertex));   //! 頂点バッファ
        gpu::setIndexBuffer(mesh.getIndexBuffer());                     //! インデックスバッファ　
        
        // 描画
        gpu::drawIndexed(PT_TRIANGLES, (u32)mesh.getIndices().size());
    }

    // 初期化
    dxTexture(nullptr);

    gpu::setTexture(0, nullptr);
    gpu::setTexture(1, nullptr);
    gpu::setTexture(2, nullptr);
    gpu::setTexture(3, nullptr);
    gpu::setTexture(4, nullptr);

    // 元に戻す
    dxLoadMatrixf(Matrix::identity());
}

//---------------------------------------------------------------------------
//! ImGui表示
//---------------------------------------------------------------------------
void ModelFBX::imguiRender()
{
    //! ImGui描画
    gpu::imguiRender(fileName_, scene_);
}

//============================================================================================================
// 内部実装
//============================================================================================================
//---------------------------------------------------------------------------
//! メッシュ読み込み
//---------------------------------------------------------------------------
void ModelFBXImpl::loadMesh()
{
    auto& scene = *scene_;

    //-------------------------------------------------------------
    // デフォルトメッシュの読み込み
    //-------------------------------------------------------------
    auto meshCount = scene.getMeshCount();   //! メッシュ数
    for(s32 i = 0; i < meshCount; ++i) {
        const ofbx::Mesh& fbxMesh = *scene.getMesh(i);
    
        MeshFBX mesh;
        if(mesh.initialize(fbxMesh, filePath_, scale_) == false) {
            continue;
        }
        meshes_.emplace_back(std::move(mesh));
    }

    //auto meshCount = scene.getMeshCount();   //! メッシュ数
    //for (s32 i = 0; i < meshCount; ++i) {
    //    const ofbx::Mesh& fbxMesh = *scene.getMesh(i);
    //    MeshFBX mesh;
    //    // すでに読み込んだこのあるデータだったら
    //    if (meshTable[&fbxMesh] != 0) {
    //        mesh = *meshTable[&fbxMesh];
    //    }
    //    else {
    //        if (mesh.initialize(fbxMesh, filePath_, scale_) == false) {
    //            continue;
    //        }
    //        meshTable[&fbxMesh] = &mesh;
    //        int x = 0;
    //    }
    //    meshes_.emplace_back(std::move(mesh));
    //}

}

//---------------------------------------------------------------------------
//! アニメーション読み込み
//---------------------------------------------------------------------------
void ModelFBXImpl::loadAnimation()
{
    auto& scene = *scene_;

    //-------------------------------------------------------------
    //! アニメーションの取得
    //-------------------------------------------------------------
    auto animationStackCount = scene.getAnimationStackCount();
    for(s32 i = 0; i < animationStackCount; ++i) {
        auto animationStack = scene.getAnimationStack(i);

        const auto& takeinfo = scene.getTakeInfo(animationStack->name);
        animStart            = takeinfo->local_time_from;
        animEnd              = takeinfo->local_time_to;

        scene.getGlobalSettings()->TimeMode;
        timeCount = animStart;
    }

    // アニメーション更新処理：本来はここに処理を書かない
    //f32 a = TIME::getDeltaTime();
    //timeCount += (double)TIME::getDeltaTime();
    //if(timeCount > animEnd)
    //    timeCount = animStart;
    //auto& scene = *scene_;
    //auto meshCount = scene.getMeshCount();   //! メッシュ数
    //for(s32 i = 0; i < meshCount; ++i) {
    //    const ofbx::Mesh&     fbxMesh  = *scene.getMesh(i);
    //    const ofbx::Geometry& geometry = *fbxMesh.getGeometry();
    //    
    //    if(!geometry.getSkin())
    //        continue;
    //    int clusterCount = geometry.getSkin()->getClusterCount();
    //    ofbx::Matrix gt0 = fbxMesh.getGlobalTransform();
    //    ofbx::Vec3   t0  = fbxMesh.getLocalTranslation();
    //    ofbx::Vec3   r0  = fbxMesh.getLocalRotation();
    //    ofbx::Vec3   s0  = fbxMesh.getLocalScaling();
    //    // 各頂点に掛けるための最終的な行列の配列
    //    ofbx::Matrix* clusterDeformation = new ofbx::Matrix[meshes_[i].getVertexArraySize()];
    //    memset(clusterDeformation, 0, sizeof(ofbx::Matrix) * meshes_[i].getVertexArraySize());
    //    // 各クラスタから各頂点に影響を与えるための行列作成
    //    for(int clusterIndex = 0; clusterIndex < clusterCount; clusterIndex++) {
    //        // クラスタ(ボーン)の取り出し
    //        const ofbx::Skin*    skin    = geometry.getSkin();
    //        const ofbx::Cluster* cluster = skin->getCluster(clusterIndex);
    //        const ofbx::Object* linkObject = cluster->getLink();
    //        //(&cluster->getScene(), linkObject->element);
    //        ofbx::IScene* clusterScene = &scene;
    //        int           objectCount  = scene.getAllObjectCount();
    //        const ofbx::Object*   object  = clusterScene->getRoot();
    //        const ofbx::IElement* element = clusterScene->getRootElement();
    //        auto animationStackCount = clusterScene->getAnimationStackCount();
    //        for(int i = 0; i < animationStackCount; ++i) {
    //            const ofbx::AnimationStack*     animationStack     = clusterScene->getAnimationStack(i);
    //            const ofbx::AnimationLayer*     animationLayer     = animationStack->getLayer(0);
    //            const ofbx::AnimationCurveNode* animationCurveNode = animationLayer->getCurveNode(0);
    //            // ボーン確認用
    //            auto bone = animationCurveNode->getBone();
    //            ofbx::Vec3 boneVec3 = bone->getLocalTranslation();
    //            ofbx::Vec3 nodeLocalTrasform = animationCurveNode->getNodeLocalTransform(timeCount);
    //        }
    //        ofbx::Matrix vertexTransformMatrix;
    //        ofbx::Matrix referenceGlobalInitPosition;
    //        ofbx::Matrix clusterGlobalInitPosition;
    //        ofbx::Matrix clusterGlobalCurrentPosition;
    //        ofbx::Matrix clusterRelativeInitPosition;
    //        ofbx::Matrix clusterRelativeCurrentPositionInverse;
    //    }
    //}
}


}   // namespace gpu

namespace fbx {

std::unordered_map<ofbx::IScene*, std::shared_ptr<gpu::ModelFBXImpl>> modelTable;

//---------------------------------------------------------------------------
//! FBX読み込み
//! @parm [in] filePath ファイルパス
//! @parm [in] scale    サイズ
//---------------------------------------------------------------------------
std::shared_ptr<gpu::ModelFBX> loadFBX(char* filePath, f32 scale)
{
    FILE* fp;
    if(fopen_s(&fp, filePath, "rb")) {
        return false;
    }

    //-------------------------------------------------------------
    // FBXバイナリ読み込み
    //-------------------------------------------------------------
    fseek(fp, 0, SEEK_END);
    auto fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    std::vector<u8> fileImage;
    fileImage.resize(fileSize);
    fread(fileImage.data(), fileSize, 1, fp);
    fclose(fp);

    // FBX読み込み
    u64 loadFlags = (u64)ofbx::LoadFlags::TRIANGULATE;


    gpu::gScene_  = ofbx::load(fileImage.data(), fileSize, loadFlags);

    if(!&gpu::gScene_) {
        OutputDebugString(ofbx::getError());
        return false;
    }

    //---------------------------------------------------------------------
    // FBXの初期化
    //---------------------------------------------------------------------
    auto p = std::make_shared<gpu::ModelFBXImpl>(filePath, scale, gpu::gScene_);

    if(!p) {
        p.reset();
    }

    return p;
}

}   // namespace fbx