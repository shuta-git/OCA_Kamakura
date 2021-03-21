//===========================================================================
//!	@file	Map.cpp
//!	@brief	�}�b�v
//===========================================================================
#include "Map.h"

//---------------------------------------------------------------------------
//! �R���X�g���N�^
//---------------------------------------------------------------------------
Map::Map()
{
    initialize();
}

//---------------------------------------------------------------------------
//! �f�X�g���N�^
//---------------------------------------------------------------------------
Map::~Map()
{
    finalize();
}

//---------------------------------------------------------------------------
//! ������
//!	@retval	true	����I��	(����)
//!	@retval	false	�G���[�I��	(���s)
//---------------------------------------------------------------------------
bool Map::initialize()
{
    //-------------------------------------------------------------
    // �V�F�[�_�[������
    //-------------------------------------------------------------
    shaderVs_ = gpu::ShaderVs::create("shader/vsModel.fx");
    shaderPs_ = gpu::ShaderPs::create("shader/psModel.fx");

    //-------------------------------------------------------------
    // ���f��������
    //-------------------------------------------------------------
    // �X�J�C�h�[��
    skyDome_ = gpu::loadFBX("data/skydome/skydome7.FBX", 1600.0f);
    if(skyDome_ == nullptr)
    {
        MessageBox(nullptr, "fbx���f���̓ǂݍ��݂Ɏ��s���܂���.", nullptr, MB_OK);
    }
    skyDome_->setPos(Vector3(0,0.5f,0));


    //modelFbx_ = gpu::loadFBX("data/Forest/Forest.FBX", 20.0f);
    //modelFbx_ = gpu::loadFBX("data/dungeon/dungeon.fbx", 1.0f);
    // ��n
    cemetery_ = gpu::loadFBX("data/cemetery/cemetery_map.FBX", 1.0f);
    if(cemetery_ == nullptr) {
        MessageBox(nullptr, "fbx���f���̓ǂݍ��݂Ɏ��s���܂���.", nullptr, MB_OK);
    }
    cemetery_->setRotateX(radian(180.0f));

    // �t�@�C�����[�h
    loadFile();

    return true;
}

//---------------------------------------------------------------------------
//! �X�V
//---------------------------------------------------------------------------
void Map::update()
{

}

//---------------------------------------------------------------------------
//! �`��
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
    // �w�ʃJ�����O�����Ȃ��ꍇ
    rd.CullMode        = D3D11_CULL_NONE;

    // �J�����O���Ȃ�
    gpu::setCullMode(rd);

    skyDome_->render();

    rd.CullMode = D3D11_CULL_BACK;

    // �J�����O
    gpu::setCullMode(rd);
}

//---------------------------------------------------------------------------
//! ���
//---------------------------------------------------------------------------
void Map::finalize()
{
}

//---------------------------------------------------------------------------
//! imgui�`��
//---------------------------------------------------------------------------
void Map::imguiRender()
{
    ImGui::Begin(u8"�}�b�v");

    if(ImGui::BeginTabBar(u8"�}�b�v")) {

        // �R���C�_�[��\�����邩�ǂ���
        ImGui::Checkbox(u8"�R���C�_�[", &colliderRender);

        // �{�b�N�X�R���C�_�[�ǉ�
        if(ImGui::Button("AddBoxCollider")) {
            boxCollider_.push_back(gpu::createBoxCollider());
        }
        // �Z�[�u
        if(ImGui::Button(u8"�Z�[�u")) {
            writeFile();
        }
        // �Z�[�u���Ă���Ƃ���܂Ŗ߂�
        if(ImGui::Button(u8"�����[�h")) {
            boxCollider_.clear();
            loadFile();
        }

        // ��
		ImGui::NewLine();

        ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);

        for(int i = 0; i < boxCollider_.size(); ++i) {
            char name[16];
            sprintf(name, "map %d", i);
            if(ImGui::CollapsingHeader(name)) {
                // �{�b�N�X�R���C�_�[�̏��
                boxCollider_[i]->imguiRender();

                // �{�b�N�X�R���C�_�[����
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
//! �R���C�_�[�̐����擾
//---------------------------------------------------------------------------
size_t Map::getColliderSize() const
{
    return boxCollider_.size();
}

//---------------------------------------------------------------------------
//! �{�b�N�X�R���C�_�[���擾
//---------------------------------------------------------------------------
std::shared_ptr<gpu::BoxCollider> Map::getBoxCollider(int i) const
{
    return boxCollider_[i];
}

//---------------------------------------------------------------------------
//! �t�@�C�����[�h
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
//! �t�@�C����������
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
//! �C���[�X
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
