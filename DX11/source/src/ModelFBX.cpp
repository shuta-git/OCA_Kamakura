//===========================================================================
//! @file   ModelFBX.cpp
//!	@brief  FBX���f��
//===========================================================================
#include "ModelFBX.h"
#include "ImGuiFBX.h"
#include <inttypes.h>

std::unordered_map<const ofbx::Mesh*, MeshFBX*> meshTable;

namespace gpu {

ofbx::IScene* gScene_ = nullptr;

//===========================================================================
//! FBX������
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
        //! �e�N�X�`���t�@�C�����̕����������o
        _splitpath_s(filePath_, drive, dir, fileName_, ext);

        //-------------------------------------------------------------
        // �V�F�[�_�[�̓ǂݍ���
        //-------------------------------------------------------------
        shaderVs_ = gpu::ShaderVs::create("shader/vsModel.fx");
        //shaderPs_ = gpu::ShaderPs::create("shader/psModel.fx");
        shaderPs_ = gpu::ShaderPs::create("shader/psGBuffer.fx");
        // �e�X�g�p
        //shaderPsTexture_ = gpu::ShaderPs::create("shader/psTexture.fx");

        loadMesh();
        //loadAnimation();
    }

    virtual ~ModelFBXImpl()
    {
    }

    // ������
    bool initialize()
    {
        return true;
    }

private:
    // �R�s�[�֎~/move�֎~
    ModelFBXImpl(const ModelFBXImpl&) = delete;
    ModelFBXImpl(ModelFBXImpl&&)      = delete;
    ModelFBXImpl& operator=(const ModelFBXImpl&) = delete;
    ModelFBXImpl& operator=(ModelFBXImpl&&) = delete;

private:
    //!�@���b�V���ǂݍ���
    void loadMesh();

    //!  �A�j���[�V�����ǂݍ���
    void loadAnimation();

};

//---------------------------------------------------------------------------
//!�@�`��
//---------------------------------------------------------------------------
void ModelFBX::render()
{

    matWorld = Matrix::rotateX(rotate_.x_) *
               Matrix::rotateY(rotate_.y_) *
               Matrix::rotateZ(rotate_.z_) *
               Matrix::scale(Vector3(1.0f, -1.0f, 1.0f)) *
               Matrix::scale(Vector3(0.01f, 0.01f, 0.01f));

    matWorld = matWorld * Matrix::translate(pos_);

    // �s��ݒ�
    dxLoadMatrixf(matWorld);

    // ���b�V���`��
    for(auto& mesh : meshes_) {
        auto materialIndices = mesh.getMaterialIndices();
        auto materials       = mesh.getMaterials();

        // �V�F�[�_�[
        gpu::setShader(shaderVs_, shaderPs_);
        // �e�N�X�`��
        gpu::setTexture(4, materials[0].texture_[(s32)Material::TextureType::Albedo]);
        gpu::setTexture(5, materials[0].texture_[(s32)Material::TextureType::Albedo]);
        gpu::setTexture(6, materials[0].texture_[(s32)Material::TextureType::Albedo]);
        gpu::setTexture(7, materials[0].texture_[(s32)Material::TextureType::Albedo]);
        gpu::setTexture(8, materials[0].texture_[(s32)Material::TextureType::Albedo]);

        // �o�b�t�@
        gpu::setVertexBuffer(mesh.getVertexBuffer(), sizeof(Vertex));   //! ���_�o�b�t�@
        gpu::setIndexBuffer(mesh.getIndexBuffer());                     //! �C���f�b�N�X�o�b�t�@�@
        
        // �`��
        gpu::drawIndexed(PT_TRIANGLES, (u32)mesh.getIndices().size());
    }

    // ������
    dxTexture(nullptr);

    gpu::setTexture(0, nullptr);
    gpu::setTexture(1, nullptr);
    gpu::setTexture(2, nullptr);
    gpu::setTexture(3, nullptr);
    gpu::setTexture(4, nullptr);

    // ���ɖ߂�
    dxLoadMatrixf(Matrix::identity());
}

//---------------------------------------------------------------------------
//! ImGui�\��
//---------------------------------------------------------------------------
void ModelFBX::imguiRender()
{
    //! ImGui�`��
    gpu::imguiRender(fileName_, scene_);
}

//============================================================================================================
// ��������
//============================================================================================================
//---------------------------------------------------------------------------
//! ���b�V���ǂݍ���
//---------------------------------------------------------------------------
void ModelFBXImpl::loadMesh()
{
    auto& scene = *scene_;

    //-------------------------------------------------------------
    // �f�t�H���g���b�V���̓ǂݍ���
    //-------------------------------------------------------------
    auto meshCount = scene.getMeshCount();   //! ���b�V����
    for(s32 i = 0; i < meshCount; ++i) {
        const ofbx::Mesh& fbxMesh = *scene.getMesh(i);
    
        MeshFBX mesh;
        if(mesh.initialize(fbxMesh, filePath_, scale_) == false) {
            continue;
        }
        meshes_.emplace_back(std::move(mesh));
    }

    //auto meshCount = scene.getMeshCount();   //! ���b�V����
    //for (s32 i = 0; i < meshCount; ++i) {
    //    const ofbx::Mesh& fbxMesh = *scene.getMesh(i);
    //    MeshFBX mesh;
    //    // ���łɓǂݍ��񂾂��̂���f�[�^��������
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
//! �A�j���[�V�����ǂݍ���
//---------------------------------------------------------------------------
void ModelFBXImpl::loadAnimation()
{
    auto& scene = *scene_;

    //-------------------------------------------------------------
    //! �A�j���[�V�����̎擾
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

    // �A�j���[�V�����X�V�����F�{���͂����ɏ����������Ȃ�
    //f32 a = TIME::getDeltaTime();
    //timeCount += (double)TIME::getDeltaTime();
    //if(timeCount > animEnd)
    //    timeCount = animStart;
    //auto& scene = *scene_;
    //auto meshCount = scene.getMeshCount();   //! ���b�V����
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
    //    // �e���_�Ɋ|���邽�߂̍ŏI�I�ȍs��̔z��
    //    ofbx::Matrix* clusterDeformation = new ofbx::Matrix[meshes_[i].getVertexArraySize()];
    //    memset(clusterDeformation, 0, sizeof(ofbx::Matrix) * meshes_[i].getVertexArraySize());
    //    // �e�N���X�^����e���_�ɉe����^���邽�߂̍s��쐬
    //    for(int clusterIndex = 0; clusterIndex < clusterCount; clusterIndex++) {
    //        // �N���X�^(�{�[��)�̎��o��
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
    //            // �{�[���m�F�p
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
//! FBX�ǂݍ���
//! @parm [in] filePath �t�@�C���p�X
//! @parm [in] scale    �T�C�Y
//---------------------------------------------------------------------------
std::shared_ptr<gpu::ModelFBX> loadFBX(char* filePath, f32 scale)
{
    FILE* fp;
    if(fopen_s(&fp, filePath, "rb")) {
        return false;
    }

    //-------------------------------------------------------------
    // FBX�o�C�i���ǂݍ���
    //-------------------------------------------------------------
    fseek(fp, 0, SEEK_END);
    auto fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    std::vector<u8> fileImage;
    fileImage.resize(fileSize);
    fread(fileImage.data(), fileSize, 1, fp);
    fclose(fp);

    // FBX�ǂݍ���
    u64 loadFlags = (u64)ofbx::LoadFlags::TRIANGULATE;


    gpu::gScene_  = ofbx::load(fileImage.data(), fileSize, loadFlags);

    if(!&gpu::gScene_) {
        OutputDebugString(ofbx::getError());
        return false;
    }

    //---------------------------------------------------------------------
    // FBX�̏�����
    //---------------------------------------------------------------------
    auto p = std::make_shared<gpu::ModelFBXImpl>(filePath, scale, gpu::gScene_);

    if(!p) {
        p.reset();
    }

    return p;
}

}   // namespace fbx