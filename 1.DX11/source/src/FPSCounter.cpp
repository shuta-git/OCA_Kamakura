//===========================================================================
//!	@file	FPSCounter.cpp
//!	@brief	FPSカウンター
//===========================================================================
#include "FPSCounter.h"

//---------------------------------------------------------------------------
//! コンストラクタ
//! @parm [in] smp    サンプル数
//---------------------------------------------------------------------------
FPSCounter::FPSCounter(unsigned int smp)
{
    // サンプル数の設定
    setSampleNum(smp);

    // 計測する時計の選択
    if(QueryPerformanceCounter(&counter_) != 0) {
        // QueryPerformanceCounter関数を使うフラグ
        counterFlag_ = FPSCOUNTER_QUERYPER_COUNTER;
        prevCount_ = counter_.QuadPart;   // 生成時の時刻（クロック数）を取得
        LARGE_INTEGER freq;
        QueryPerformanceFrequency(&freq);   // 1秒当たりクロック数を取得
        perSecondCount_ = (double)freq.QuadPart;
    }
    else {
        // timeGetTime関数を使うフラグ
        counterFlag_ = FPSCOUNTER_TIMEGETTIME;

        // 精度を上げる
        timeBeginPeriod(1);

        // 生成時の時刻（ミリ秒）を取得
        prevMilliSecondCount_ = timeGetTime();
    }

    // 計測
    getFPS();
}

//---------------------------------------------------------------------------
//! デストラクタ
//---------------------------------------------------------------------------
FPSCounter::~FPSCounter(void)
{
    if(counterFlag_ == FPSCOUNTER_TIMEGETTIME)
        timeEndPeriod(1);   // タイマーの精度を戻す
}

//---------------------------------------------------------------------------
// FPS値を取得
//---------------------------------------------------------------------------
double FPSCounter::getFPS()
{
    double def = getCurDefTime();
    if(def == 0) {
        // 計算できないのでを返す
        return 0;
    }

    return updateFPS(def);
}

//---------------------------------------------------------------------------
//! 現在の差分時刻をミリ秒単位で取得
//---------------------------------------------------------------------------
double FPSCounter::getCurDefTime()
{
    // 差分時間を計測
    if(counterFlag_ == FPSCOUNTER_QUERYPER_COUNTER) {
        QueryPerformanceCounter(&counter_);                  
        // 現在の時刻を取得し、差分カウンタ数を算出する。
        LONGLONG longDef = counter_.QuadPart - prevCount_;
        double   dDef    = (double)longDef;   
        prevCount_   = counter_.QuadPart;
        
        // 差分時間をミリ秒単位で返す
        return dDef * 1000 / perSecondCount_;                
    }

    DWORD curTime   = timeGetTime();
    DWORD curDef          = curTime - prevMilliSecondCount_;   
    prevMilliSecondCount_ = curTime;                          
    return curDef;
}

//---------------------------------------------------------------------------
// FPSを更新
//! @parm [in] def 差分
//---------------------------------------------------------------------------
double FPSCounter::updateFPS(double def)
{
    // 消去
    timeList_.pop_front();
    timeList_.push_back(def);

    // FPS算出
    double FPS;
    double AveDef = (sumTimes_ + def) / dataNum_;
    if(AveDef != 0) {
        FPS = 1000.0 / AveDef;
    }

    // 共通加算部分の更新
    sumTimes_ += def - *timeList_.begin();

    return FPS;
}

//---------------------------------------------------------------------------
// サンプル数を変更
//! @parm [in] smp    サンプル数
//---------------------------------------------------------------------------
void FPSCounter::setSampleNum(unsigned int smp)
{
    dataNum_ = smp;                        
    timeList_.resize(dataNum_, 0.0);   
    sumTimes_ = 0;
}