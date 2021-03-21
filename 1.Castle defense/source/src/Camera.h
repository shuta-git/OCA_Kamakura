#pragma once

//---------------------------------------------
// �N���X
//---------------------------------------------
class Camera : public Base
{
public:
	Camera();
	~Camera();

	// �X�V
	void Update() override;	
	// �`��
	void Draw() override;						

	//=======================================
	// Set�֐�
	//=======================================

	// �^�[�Q�b�g�̐ݒ�
	void SetTarget(Vector3 target_pos);	
	// Y���̒l��ݒ�
	void SetRot(Vector3 rot_);

	//=======================================
	// Get�֐�
	//=======================================

	// �|�W�V�����̎擾
	Vector3 GetPos()const { return pos; }		
	// �����̎擾
	Vector3 GetRot()const { return rot; }		
	// �J�����̍s����擾
	MATRIX GetMat()const { return mat; }		

private:

	// �}�E�X�̓�����͈͂��w��
	void MauseMoveRange();			
	// Old�̕ϐ�����
	void OldSet();								

	//=======================================
	// �ϐ�
	//=======================================
	int mausex, mausey;			// �}�E�X���W

	Vector3 look_pos;			// �^�[�Q�b�g���W
	Vector3 player_rot;			// �v���C���[�̊p�x

	float Camera_VRot;			// �J�������������p
	float Camera_HRot;			// �J�������������p
	float Camera_TRot;			// �J�����P������p

	float move_camera = 2.0f;	// �J�����̑��x

	float Mause_VRot;			// �}�E�X�̐�������
	float Mause_HRot;			// �}�E�X�ɐ�������
	float Old_Mause_VRot;		// �P�t���[���O�̒l��ۑ�����ϐ�
	float Old_Mause_HRot;		// �P�t���[���O�̒l��ۑ�����ϐ�

	float length;				// �v���C���[����J�����̋���
	float old_length;			// �P�t���[���O�̒l��ۑ�
};

