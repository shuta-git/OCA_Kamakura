
static int gStartCounter;	// 開始カウンター
static int gCounter;		// カウンター
static float gFps;			// fps

static const int N = 60;	// 平均を取るサンプル数
static const int FPS = 60;	// 設定したFPS(60フレーム)

// 初期化
void FpsInit()
{
	gStartCounter = GetNowCount();
	gCounter = 0;
	gFps = 0.f;
}

// 更新
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

// 描画
void FpsDraw()
{
#if defined(NDEBUG)

	//printfDx("%.1f\n", gFps);

#endif 
}

// 待機
void FpsWait()
{
	int tTime = GetNowCount() - gStartCounter;
	int wTime = gCounter * 1000 / FPS - tTime;
	if (wTime > 0) {
		Sleep(wTime);
	}
}

// 終了
void FpsExit()
{
	;
}