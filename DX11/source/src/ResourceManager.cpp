//===========================================================================
//!	@file	ResourceManager.cpp
//!	@brief	���\�[�X�}�l�[�W���[
//===========================================================================
#include "ResourceManager.h"

namespace {

	std::unordered_map<char*, std::shared_ptr<gpu::ModelFBX>> fbxModel;

}

namespace gpu {

std::shared_ptr<ModelFBX> loadFBX(char* filePath, f32 scale)
{
    int x = 0;

    // ���łɓǂݍ��񂾂��̂���f�[�^��������
    if(fbxModel[filePath] != 0) {
        // ���̂܂܂��̃f�[�^��n��
        return fbxModel[filePath];
    }

    // �Ⴄ��������
    // �V�K�f�[�^��ǂݍ���
    //std::make_shared<ModelFBX>h; 
    std::shared_ptr<ModelFBX> handle = fbx::loadFBX(filePath, scale);
    // �f�[�^��ۑ�
    fbxModel[filePath] = handle;

    return handle;
}


}   // namespace gpu
