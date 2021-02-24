//---------------------------------------------
// �萔�̐錾
//---------------------------------------------
const float MIN_CAMERA_POS_Y = 0.6f;	// �J�����̃|�W�V����Y�̍ŏ��l
const float MAX_LENGTH = -15.0f;		// length�̍ő�l
const float MIN_LENGTH = -2.0f;			// length�̍ŏ��l
const float TARGET_POS_Y_ADD = 5.0f;	// �^�[�Q�b�g�̃|�W�V��������Y���Ƀv���X����l

//---------------------------------------------
// �R���X�g���N�^
//---------------------------------------------
Camera::Camera()
{
	// �s��̏�����
	MatrixInit();
	// �}�E�X���W�̎擾
	GetMousePoint(&mausex, &mausey);
	// ��]�p
	rot.Set(0.0f, 0.0f, 0.0f);
	// �J�����|�W�V����
	pos.Set(0.0f, 0.0f, 0.0f);
	// �^�[�Q�b�g�|�W�V����
	look_pos.Set(0.0f, 0.0f, 0.0f);

	// �}�E�X�̉�]�p�̕ϐ�������
	Mause_VRot = 0.0f;
	Mause_HRot = 0.0f;
	Old_Mause_VRot = 0.0f;
	Old_Mause_HRot = 0.0f;

	// �^�[�Q�b�g����J�����̋������w��
	length = MAX_LENGTH;
}
//---------------------------------------------
// �f�X�g���N�^
//---------------------------------------------
Camera::~Camera()
{
}
//---------------------------------------------
// �X�V
//---------------------------------------------
void Camera::Update()
{
	// Old�̕ϐ�����
	OldSet();

	// �}�E�X���W�̎擾
	GetMousePoint(&mausex, &mausey);


	if (GetPadInput().ThumbRX == 32767) {
		Mause_HRot+= (DX_PI_F / 45.0f);
	}
	if (GetPadInput().ThumbRX == -32768) {
		Mause_HRot -= (DX_PI_F / 45.0f);
	}
	if (GetPadInput().ThumbRY == 32767) {
		Mause_VRot -= (DX_PI_F / 45.0f);
	}
	if (GetPadInput().ThumbRY == -32768) {
		Mause_VRot += (DX_PI_F / 45.0f);
	}


	// �}�E�X���W�����S����ǂꂾ�����������擾
	// �}�E�XX�̒l
	Mause_HRot += (mausex - WINDOW_W / 2) / 1000.0f;
	// �}�E�XY�̒l
	Mause_VRot += (mausey - WINDOW_H / 2) / 1000.0f;

	// �}�E�X���W�𒆐S�ɏC��
	SetMousePoint(WINDOW_W / 2, WINDOW_H / 2);


	// �J�������W�Ƀ^�[�Q�b�g���W��ݒ�
	pos.Set(look_pos);

	// �J�����̃��Z�b�g
	if (IsKeyOn(KEY_INPUT_R) || IsPadRepeat(PAD_L)) {
		Mause_HRot = player_rot.y;
	}

	// �}�E�X�̓�����͈͂��w��
	MauseMoveRange();
	
	// Y�����ɉ�]
	rot.y = Mause_HRot;
	// X�����ɉ�]
	rot.x = Mause_VRot;

	
	// �J�����̈ړ�
	pos.x += length * sinf(rot.y);
	pos.z += length * cosf(rot.y);
	pos.y -= length * sinf(rot.x);

	// �J�����̃|�W�V����Y�� 0.6f �揬�����Ȃ�����
	if (pos.y <= MIN_CAMERA_POS_Y) {
		Mause_VRot = Old_Mause_VRot;
		pos.y = MIN_CAMERA_POS_Y;
		old_pos.y = pos.y;
	}


}
//---------------------------------------------
// �`��
//---------------------------------------------
void Camera::Draw()
{
	// �J������ݒ�
	SetCameraPositionAndTarget_UpVecY(pos.GetV(), look_pos.GetV());

#if(NDEBUG)
	//printfDx("Mause_HRot  %f \n" , Mause_HRot);
	//printfDx("Mause_VRot  %f \n", Mause_VRot);
	//printfDx("pos.y  %f \n", pos.y);
	//printfDx("length %f \n", length);
#endif
}
//---------------------------------------------
// �^�[�Q�b�g�ݒ�
//---------------------------------------------
void Camera::SetTarget(Vector3 target_pos)
{
	look_pos.Set(target_pos.x,target_pos.y + TARGET_POS_Y_ADD,target_pos.z);
}
//---------------------------------------------
// Y���̒l��ݒ�
//---------------------------------------------
void Camera::SetRot(Vector3 rot_)
{
	player_rot = rot_;
}
//---------------------------------------------
// �}�E�X�̓�����͈͂��w��
//---------------------------------------------
void Camera::MauseMoveRange()
{
	if (Mause_VRot > (DX_PI_F / 2) || Mause_VRot < -(DX_PI_F / 2)) {
		Mause_VRot = Old_Mause_VRot;
	}
	if (Mause_VRot > (DX_PI_F * 2) || Mause_VRot < -(DX_PI_F * 2)) {
		Mause_VRot = 0.0f;
	}
}
//---------------------------------------------
// Old�̕ϐ�����
// old_pos,Old_Mause_VRot,Old_Mause_HRot
//---------------------------------------------
void Camera::OldSet()
{
	// �O�̃t���[�����W��ۑ�
	old_pos = pos;
	old_length = length;
	Old_Mause_VRot = Mause_VRot;
	Old_Mause_HRot = Mause_HRot;
}
