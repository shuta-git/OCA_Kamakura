//===========================================================================
//!	@file   ModelFBX.h
//!	@brief  FBX���f��
//===========================================================================
#pragma once
#include "../OpenFBX/src/ofbx.h"
#include "MeshFBX.h"

namespace gpu {

//===========================================================================
//!	FBX���f��
//===========================================================================
class ModelFBX
{
public:

    virtual ~ModelFBX() = default;

    //! �`��
    void render();

    //! ImGui�`��
    void imguiRender();

    //-------------------------------------------------------------
    //!	@name	�Q��
    //-------------------------------------------------------------
    //@{

    //! �ʒu��ݒ�
    void setPos(Vector3 pos) { pos_ = pos; }

    //! ��]��ݒ�
    void setRotate(Vector3 rotate) { rotate_ = rotate; }

    //! X����]��ݒ�
    void setRotateX(float x) { rotate_.x_ = x; }

    //! Y����]��ݒ�
    void setRotateY(float y) { rotate_.y_ = y; }

    //! Z����]��ݒ�
    void setRotateZ(float z) { rotate_.z_ = z; }

    //@}

    //-------------------------------------------------------------
    //!	@name	�擾
    //-------------------------------------------------------------
    //@{

    //! �ʒu���擾
    Vector3 getPos() const { return pos_; }

    //! ��]���擾
    Vector3 getRotate() const
    {
        return rotate_;
    }

    //! X����]���擾
    float getRotateX() const { return rotate_.x_ ; }

    //! Y����]���擾
    float getRotateY() const { return rotate_.y_ ; }

    //! Z����]���擾
    float getRotateZ() const { return rotate_.z_ ; }

    //!< ���[���h���W
    Matrix getMatWorld() const { return matWorld; }

    //@}

protected:

    ModelFBX() = default;

    std::vector<MeshFBX> meshes_;   //!< ���b�V��

    std::unique_ptr<gpu::ShaderVs> shaderVs_;          //!< ���_�V�F�[�_�[
    std::unique_ptr<gpu::ShaderPs> shaderPs_;          //!< �s�N�Z���V�F�[�_�[
    std::unique_ptr<gpu::ShaderPs> shaderPsTexture_;   //!< �e�N�X�`���s�N�Z���V�F�[�_�[

    char* filePath_;   //!< �t�@�C���p�X
    char  fileName_[MAX_PATH];  //!< �t�@�C���l�[��
    ofbx::IScene* scene_ = nullptr;
    f32           scale_;   //!< �T�C�Y

    double animStart = 0;
    double animEnd = 0;
    double timeCount = 0;
    double frameTime = 0;


private:

    Vector3 rotate_ = Vector3(0, 0, 0);   //!< ����
    Vector3 pos_    = Vector3(0, 0, 0);    //!< ���W

    Matrix matWorld;    //!< ���[���h�s��
};

}   // namespace gpu

namespace fbx {

//! FBX���t�@�C������쐬
//! @parm [in] filePath �t�@�C���p�X
//! @parm [in] scale    �T�C�Y
std::shared_ptr<gpu::ModelFBX> loadFBX(char* filePath, f32 scale = 1.0f);

}