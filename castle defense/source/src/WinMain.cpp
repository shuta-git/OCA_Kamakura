
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int GradTexHandle;
	int PixelShaderHandle;			// �s�N�Z���V�F�[�_�[
	int VertexShaderHandle;			// ���_�V�F�[�_�[

	SetOutApplicationLogValidFlag(FALSE);			// ���O�o�� ��
	ChangeWindowMode(TRUE);							// �E�B���h�E���[�h ��
	SetGraphMode(WINDOW_W, WINDOW_H, 32);			// ��ʃT�C�Y
	SetMainWindowText("3D");	// �E�B���h�E�^�C�g��
	SetZBufferBitDepth(24);

	// Effekseer���g�p����ɂ͕K���ݒ肷��B
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	if (DxLib_Init() == -1)		return -1;

	// Effekseer������������B
	// �����ɂ͉�ʂɕ\������ő�p�[�e�B�N������ݒ肷��B
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
		return -1;
	}

	// �t���X�N���[���E�C���h�E�̐؂�ւ��Ń��\�[�X��������̂�h���B
	// Effekseer���g�p����ꍇ�͕K���ݒ肷��B
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
	// DX���C�u�����̃f�o�C�X���X�g�������̃R�[���o�b�N��ݒ肷��B
	// �E�C���h�E�ƃt���X�N���[���̐؂�ւ�����������ꍇ�͕K�����s����B
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	// Effekseer��2D�`��̐ݒ������B
	Effekseer_Set2DSetting(WINDOW_W, WINDOW_H);

	SetMouseDispFlag(MOUSE_FLG);
	SetMousePoint(WINDOW_W / 2, WINDOW_H / 2);

	SetDrawScreen(DX_SCREEN_BACK);
	SetBackgroundColor(100, 100, 100);

	// �J�����̕`�拗��
	SetCameraNearFar(0.1f, 2000.0f);

	// ---------------
	// ����������
	// ---------------
	KeyInputInit();
	PadInputInit();
	MouseInputInit();
	FpsInit();

	// Z�o�b�t�@��L���ɂ���B
	// Effekseer���g�p����ꍇ�A2D�Q�[���ł�Z�o�b�t�@���g�p����B
	SetUseZBuffer3D(TRUE);

	// Z�o�b�t�@�ւ̏������݂�L���ɂ���B
	// Effekseer���g�p����ꍇ�A2D�Q�[���ł�Z�o�b�t�@���g�p����B
	SetWriteZBuffer3D(TRUE);

	// �}�E�X�J�[�\�����\��(true �ŕ\��)
	SetMouseDispFlag(false);

	// �V�[���}�l�W���[���Ă�
	SceneManager* scene = new SceneManager();



	// ���_�V�F�[�_�[��ǂݍ���
	VertexShaderHandle = LoadVertexShader("shader/SkinMesh4_DirLight_ToonVS.vso");
	// �s�N�Z���V�F�[�_�[��ǂݍ���
	PixelShaderHandle = LoadPixelShader("shader/SkinMesh4_DirLight_ToonPS.pso");
	// �O���f�[�V�����e�N�X�`����ǂݍ���
	GradTexHandle = LoadGraph("shader/data/GradTex.bmp");

	// ���f���̕`��ɃI���W�i���V�F�[�_�[���g�p����ݒ���n�m�ɂ���
	//MV1SetUseOrigShader(TRUE);
	// �g�p����e�N�X�`���P�ɃO���f�[�V�����e�N�X�`�����Z�b�g����
	SetUseTextureToShader(1, GradTexHandle);
	// �g�p���钸�_�V�F�[�_�[���Z�b�g
	SetUseVertexShader(VertexShaderHandle);
	// �g�p����s�N�Z���V�F�[�_�[���Z�b�g
	SetUsePixelShader(PixelShaderHandle);

	// �Q�[�����C�����[�v
	while (!ProcessMessage() && CheckHitKey(KEY_INPUT_ESCAPE) == 0 && KeyInputUpdate() == 0)
	{
		clsDx();
		ClearDrawScreen();

		// ---------------
		// �X�V����
		// ---------------
		FpsUpdate();
		if (scene != nullptr) {
			scene->Update();
		}
		// DX���C�u�����̃J������Effekseer�̃J�����𓯊�����B
		Effekseer_Sync3DSetting();

		KeyInputUpdate();
		MouseInputUpdate();
		PadInputUpdate();
	


		FpsDraw();
		if (scene != nullptr) {
			scene->Draw();
		}
		ScreenFlip();

		ScreenFlip();

		// Fps�ҋ@
		FpsWait();
	}

	// ---------------
	// �I������
	// ---------------
	KeyInputExit();
	PadInputExit();
	MouseInputExit();
	FpsExit();

	// �g�p����e�N�X�`������O���f�[�V�����e�N�X�`�����O��
	SetUseTextureToShader(1, -1);
	// �O���f�[�V�����e�N�X�`�����폜
	DeleteGraph(GradTexHandle);
	// �ǂݍ��񂾒��_�V�F�[�_�[�̍폜
	DeleteShader(VertexShaderHandle);
	// �ǂݍ��񂾃s�N�Z���V�F�[�_�[�̍폜
	DeleteShader(PixelShaderHandle);

	delete scene;

	// Effekseer���I������B
	Effkseer_End();

	DxLib_End();

	return 0;
}
