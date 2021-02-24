//===========================================================================
//!	@file	ResourceManager.cpp
//!	@brief	リソースマネージャー
//===========================================================================
#include "ResourceManager.h"

namespace {

	std::unordered_map<char*, std::shared_ptr<gpu::ModelFBX>> fbxModel;

}

namespace gpu {

std::shared_ptr<ModelFBX> loadFBX(char* filePath, f32 scale)
{
    int x = 0;

    // すでに読み込んだこのあるデータだったら
    if(fbxModel[filePath] != 0) {
        // そのままそのデータを渡す
        return fbxModel[filePath];
    }

    // 違うかったら
    // 新規データを読み込み
    //std::make_shared<ModelFBX>h; 
    std::shared_ptr<ModelFBX> handle = fbx::loadFBX(filePath, scale);
    // データを保存
    fbxModel[filePath] = handle;

    return handle;
}


}   // namespace gpu
