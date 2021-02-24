//===========================================================================
//!	@file   Resource.cpp
//!	@brief	���\�[�X�Ǘ�
//===========================================================================
#include "Resource.h"

namespace {
std::set<Resource*> resourceList;   // �ǂݍ��܂ꂽ���\�[�X�ꗗ
}

//---------------------------------------------------------------------------
//! �R���X�g���N�^
//---------------------------------------------------------------------------
Resource::Resource()
{
    resourceList.insert(this);
}

//---------------------------------------------------------------------------
//! �f�X�g���N�^
//---------------------------------------------------------------------------
Resource::~Resource()
{
    resourceList.erase(this);
}

//---------------------------------------------------------------------------
//! �t�@�C���p�X���烊�\�[�X�p�X����
//---------------------------------------------------------------------------
std::wstring makeResourcePath(const char* fileName)
{
    std::wstring resourcePath;

    // �K�v�ȕ����������O�ɃJ�E���g
    auto size = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, fileName, -1, nullptr, 0);
    resourcePath.resize(size);   // null�I�[���܂߂�������
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, fileName, -1, resourcePath.data(), resourcePath.size());

    // ���ׂď������ɕύX
    _wcslwr_s(resourcePath.data(), resourcePath.size());   // ������

    // �o�b�N�X���b�V�����X���b�V���ɕύX
    for(auto& x : resourcePath) {
        if(x == '\\')
            x = '/';
    }

    return resourcePath;
}

//---------------------------------------------------------------------------
//! ���\�[�X�������[�h
//---------------------------------------------------------------------------
void reloadResource(const std::wstring& resourcePath)
{
    for(auto& x : resourceList) {
        if(x->getPath() != resourcePath)
            continue;
        x->reload();
    }
}
