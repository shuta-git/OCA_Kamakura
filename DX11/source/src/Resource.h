//===========================================================================
//!	@file   Resource.h
//!	@brief	���\�[�X�Ǘ�
//===========================================================================
#pragma once

//===========================================================================
//! ���\�[�X���
//===========================================================================
class Resource
{
public:
	//! �R���X�g���N�^
	Resource();

	//! �f�X�g���N�^
	virtual ~Resource();

	//-------------------------------------------------------------
	//!	@name �p���������K�v�Ȋ֐�
	//-------------------------------------------------------------
	//@{

	//! ���\�[�X�̃����[�h
	virtual	bool	reload()=0;

	//@}
	//-------------------------------------------------------------
	//!	@name �A�N�Z�T
	//-------------------------------------------------------------
	//@{

	//! ���\�[�X�p�X�̐ݒ�
	void	setPath(const std::wstring& path) { path_ = path; }

	//! ���\�[�X�p�X�̎擾
	const std::wstring&	getPath() const { return path_; }

	//! �g���q�̐ݒ�
	void	setExt(const std::wstring& ext) { ext_=ext; }

	//! �g���q�̎擾
	const std::wstring&	getExt() const { return ext_;}

	//@}
private:
	std::wstring	path_;	// ���\�[�X�p�X
	std::wstring	ext_;	// �t�@�C���̊g���q

};

//! �t�@�C���p�X���烊�\�[�X�p�X����
std::wstring makeResourcePath(const char* fileName);

//! ���\�[�X�p�X���烊�\�[�X������
//! @return ���łɓǂݍ��܂�Ă��郊�\�[�X�̃|�C���^(nullptr�Ȃ猟�o���ꂸ)
//Resource*	findResource(const std::wstring& resourcePath);

//! ���\�[�X�������[�h
void	reloadResource(const std::wstring& resourcePath);
