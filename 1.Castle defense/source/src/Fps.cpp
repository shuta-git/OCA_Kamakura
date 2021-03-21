
static int gStartCounter;	// �J�n�J�E���^�[
static int gCounter;		// �J�E���^�[
static float gFps;			// fps

static const int N = 60;	// ���ς����T���v����
static const int FPS = 60;	// �ݒ肵��FPS(60�t���[��)

// ������
void FpsInit()
{
	gStartCounter = GetNowCount();
	gCounter = 0;
	gFps = 0.f;
}

// �X�V
void FpsUpdate()
{
	if (gCounter == 0) {
		gStartCounter = GetNowCount();
	}
	if (gCounter == N) {
		int t = GetNowCount();
		gFps = 1000.f / ((t - gStartCounter) / (float)N);
		gCounter = 0;
		gStartCounter = t;
	}
	++gCounter;
}

// �`��
void FpsDraw()
{
#if defined(NDEBUG)

	//printfDx("%.1f\n", gFps);

#endif 
}

// �ҋ@
void FpsWait()
{
	int tTime = GetNowCount() - gStartCounter;
	int wTime = gCounter * 1000 / FPS - tTime;
	if (wTime > 0) {
		Sleep(wTime);
	}
}

// �I��
void FpsExit()
{
	;
}