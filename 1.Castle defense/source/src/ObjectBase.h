#pragma once


class ObjectBase : public CollisionBase
{
public:
	ObjectBase();
	~ObjectBase();

protected:
	// �e�N�X�`���[��K�p
	void ApplyTexture();
	// �e�N�X�`���[�̃p�X��ݒ�
	void SetTextureHandle(string str);
	// ���f���̃p�X��ݒ�
	void SetModelHandle(string str);
	// ���f����`��
	void DrawModel();
	// �l��ݒ�
	void SetValue();

	std::vector<int>model_handle;		// ���f���n���h��
	std::vector<int>texture_handle;		// �e�N�X�`���[�n���h��

private:

	
};

