#pragma once

class Base 
{
public:
	Base();						// �R���X�g���N�^
	~Base();					// �f�X�g���N�^

	virtual void Update() = 0;
	virtual void Draw() = 0;
	// �s������ׂď�����
	void MatrixInit();	
	// ��]�s���������
	void RotMatrixInit();		
	// ���W�s���������
	void TransMatrixInit();		

	//=======================================
	// Get�֐�
	//=======================================

	// ���W���擾
	Vector3 GetPos() const { return pos; }	
	// ��O�̍��W���擾
	Vector3 GetOldPos() const { return old_pos; }
 	// �������擾
	Vector3 GetRot() const { return rot; }		
	// �T�C�Y���擾
	Vector3 GetScale() const { return scale; }	
	// ���W���擾
	VECTOR GetVPos() const { return pos.GetV(); }
	// ��O�̍��W���擾
	VECTOR GetOldVPos() const { return old_pos.GetV(); }
	// �������擾
	// �������擾
	VECTOR GetVRot()const { return rot.GetV(); }		
	// �T�C�Y���擾
	VECTOR GetVScale() const{ return scale.GetV(); }			
	// �J�v�Z���̏㕔���̍��W���擾
	VECTOR GetCapsuleUp(VECTOR pos_,float size_) const;
	// �J�v�Z���̉������̍��W���擾
	VECTOR GetCapsuleDown(VECTOR pos_)const;
	// �J�v�Z���̐�[�����̍��W���擾
	VECTOR GetCapsuleTip(VECTOR pos_,float size_ = 0.0f)const;
	// �J�v�Z���̍��{�����̍��W���擾
	VECTOR GetCapsuleRoot(VECTOR pos_)const;


protected:

	Vector3 pos;				// Vector�N���X
	Vector3 old_pos;			// �P�t���[���O�̍��W
	Vector3 rot;				// ����
	Vector3 scale;				// �T�C�Y

	MATRIX mat;					// �s��
	MATRIX mat_trans;			// ���W�s��
	MATRIX mat_rot;				// ��]�s��

	float  radius;				// ���a
	float  speed;				// ���x
	
	std::vector<Effect*>effect;	// �G�t�F�N�g

	// �G�t�F�N�g������
	void DeleteEffect(int i);
	// �G�t�F�N�g���~�߂�
	void StopEffect(int i);

private:

	// effect�̃T�C�Y���k�߂�
	void EraseEffect();
};

