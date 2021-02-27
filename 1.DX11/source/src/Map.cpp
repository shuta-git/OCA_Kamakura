//===========================================================================
//!	@file	Map.cpp
//!	@brief	マップ
//===========================================================================
#include "Map.h"

//---------------------------------------------------------------------------
//! コンストラクタ
//---------------------------------------------------------------------------
Map::Map()
{
    initialize();
}

//---------------------------------------------------------------------------
//! デストラクタ
//---------------------------------------------------------------------------
Map::~Map()
{
    finalize();
}

//---------------------------------------------------------------------------
//! 初期化
//!	@retval	true	正常終了	(成功)
//!	@retval	false	エラー終了	(失敗)
//---------------------------------------------------------------------------
bool Map::initialize()
{
    //-------------------------------------------------------------
    // シェーダー初期化
    //-------------------------------------------------------------
    shaderVs_ = gpu::ShaderVs::create("shader/vsModel.fx");
    shaderPs_ = gpu::ShaderPs::create("shader/psModel.fx");

    //-------------------------------------------------------------
    // モデル初期化
    //-------------------------------------------------------------
    // スカイドーム
    skyDome_ = gpu::loadFBX("data/skydome/skydome7.FBX", 1600.0f);
    if(skyDome_ == nullptr)
    {
        MessageBox(nullptr, "fbxモデルの読み込みに失敗しました.", nullptr, MB_OK);
    }
    skyDome_->setPos(Vector3(0,0.5f,0));


    //modelFbx_ = gpu::loadFBX("data/Forest/Forest.FBX", 20.0f);
    //modelFbx_ = gpu::loadFBX("data/dungeon/dungeon.fbx", 1.0f);
    // 墓地
    cemetery_ = gpu::loadFBX("data/cemetery/cemetery_map.FBX", 1.0f);
    if(cemetery_ == nullptr) {
        MessageBox(nullptr, "fbxモデルの読み込みに失敗しました.", nullptr, MB_OK);
    }
    cemetery_->setRotateX(radian(180.0f));

    // ファイルロード
    loadFile();

    return true;
}

//---------------------------------------------------------------------------
//! 更新
//---------------------------------------------------------------------------
void Map::update()
{

}

//---------------------------------------------------------------------------
//! 描画
//---------------------------------------------------------------------------
void Map::render()
{
    if(colliderRender) {
        for(int i = 0; i < boxCollider_.size(); ++i) {
            boxCollider_[i]->render();
        }
    }
    cemetery_->render();

    D3D11_RASTERIZER_DESC rd{};
    ZeroMemory(&rd, sizeof(rd));
    rd.FillMode = D3D11_FILL_SOLID;
    // 背面カリングをしない場合
    rd.CullMode        = D3D11_CULL_NONE;

    // カリングしない
    gpu::setCullMode(rd);

    skyDome_->render();

    rd.CullMode = D3D11_CULL_BACK;

    // カリング
    gpu::setCullMode(rd);
}

//---------------------------------------------------------------------------
//! 解放
//---------------------------------------------------------------------------
void Map::finalize()
{
}

//---------------------------------------------------------------------------
//! imgui描画
//---------------------------------------------------------------------------
void Map::imguiRender()
{
    ImGui::Begin(u8"マップ");

    if(ImGui::BeginTabBar(u8"マップ")) {

        // コライダーを表示するかどうか
        ImGui::Checkbox(u8"コライダー", &colliderRender);

        // ボックスコライダー追加
        if(ImGui::Button("AddBoxCollider")) {
            boxCollider_.push_back(gpu::createBoxCollider());
        }
        // セーブ
        if(ImGui::Button(u8"セーブ")) {
            writeFile();
        }
        // セーブしているところまで戻す
        if(ImGui::Button(u8"リロード")) {
            boxCollider_.clear();
            loadFile();
        }

        // 空白
		ImGui::NewLine();

        ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);

        for(int i = 0; i < boxCollider_.size(); ++i) {
            char name[16];
            sprintf(name, "map %d", i);
            if(ImGui::CollapsingHeader(name)) {
                // ボックスコライダーの情報
                boxCollider_[i]->imguiRender();

                // ボックスコライダー消去
                if(ImGui::Button("DestroyBoxCollider")) {
                    boxCollider_[i] = nullptr;
                    erase();
                }
            }
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

//---------------------------------------------------------------------------
//! コライダーの数を取得
//---------------------------------------------------------------------------
size_t Map::getColliderSize() const
{
    return boxCollider_.size();
}

//---------------------------------------------------------------------------
//! ボックスコライダーを取得
//---------------------------------------------------------------------------
std::shared_ptr<gpu::BoxCollider> Map::getBoxCollider(int i) const
{
    return boxCollider_[i];
}

//---------------------------------------------------------------------------
//! ファイルロード
//---------------------------------------------------------------------------
void Map::loadFile()
{
    file.open("data/txt/map.txt", std::ios::in);

    if(!file.is_open()) {
        return;
    }

    std::string str;
    while(getline(file, str)) {

        while(true) {
            std::string::size_type pos = str.find(',', 0);
            if(pos == std::string::npos)break;
            str.replace(pos, 1, " ");   
        }

        std::stringstream sstr(str);
        int               i = 0;
        float             x    = 0.0f;
        float             y    = 0.0f;
        float             z    = 0.0f;
        float             w    = 0.0f;
        float             h    = 0.0f;
        float             d    = 0.0f;
        sstr >> i >> x >> y >> z >> w >> h >> d;
        boxCollider_.push_back(gpu::createBoxCollider());
        boxCollider_[i]->setPos(Vector3(x,y,z));
        boxCollider_[i]->setWigth(w);
        boxCollider_[i]->setHeight(h);
        boxCollider_[i]->setDepth(d);

    }

    file.close();
}

//---------------------------------------------------------------------------
//! ファイル書き込み
//---------------------------------------------------------------------------
void Map::writeFile()
{
    file.open("data/txt/map.txt", std::ios::out);

    if(!file.is_open()) {
        return;
    }

    for(int i = 0; i < boxCollider_.size(); ++i) {
        file << i << ",";
        file << boxCollider_[i]->getPos().x_ << ",";
        file << boxCollider_[i]->getPos().y_ << ",";
        file << boxCollider_[i]->getPos().z_ << ",";
        file << boxCollider_[i]->getWigth()  << ",";
        file << boxCollider_[i]->getHeight() << ",";
        file << boxCollider_[i]->getDepth()  << ",";
        file << std::endl;
    }

    file.close();

}

//---------------------------------------------------------------------------
//! イレース
//---------------------------------------------------------------------------
void Map::erase()
{
    std::vector<std::shared_ptr<gpu::BoxCollider>>::iterator it = boxCollider_.begin();
    while(it != boxCollider_.end()) {
        if(*it == nullptr) {
            it = boxCollider_.erase(it);
        }
        else {
            it++;
        }
    }
}
