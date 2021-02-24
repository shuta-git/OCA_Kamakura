//===========================================================================
//!	@file	FPSCounter.h
//!	@brief	FPSカウンター
//===========================================================================
#pragma once
#pragma comment(lib, "winmm.lib")

#define FPSCOUNTER_QUERYPER_COUNTER 1
#define FPSCOUNTER_TIMEGETTIME 2

//===========================================================================
//!　FPSカウンター
//===========================================================================
class FPSCounter
{
public:
    //! コンストラクタ
    //! @parm [in] smp    サンプル数
    FPSCounter(unsigned int smp = 10);

    //! デストラクタ
    virtual ~FPSCounter();

    //! FPS値を取得
    double getFPS();

    //! サンプル数を変更
    //! @parm [in] smp    サンプル数
    void setSampleNum(unsigned int smp);

protected:

    //! 現在の時刻を取得
    double getCurDefTime();

    //! FPSを更新
    //! @parm [in] Def 差分
    double updateFPS(double def);

private:

    int               counterFlag_;    //!< 使用する計測関数の判定フラグ
    LARGE_INTEGER     counter_;         //!< クロックカウント数
    double            perSecondCount_;           //!< 1秒当たりクロックカウント数（周波数）
    LONGLONG          prevCount_;    //!< 以前のクロックカウント数
    DWORD             prevMilliSecondCount_;   //!< 以前の時刻（ミリ秒）
    std::list<double> timeList_;    //!< 時間リスト
    UINT              dataNum_;           //!< 移動平均計算時のデータ数
    double            sumTimes_;      //!< 共通部分の合計値

};

double getFPS();

f32 getDeltaTime();
