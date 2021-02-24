//===========================================================================
//!	@file	FPSCounter.cpp
//!	@brief	FPS�J�E���^�[
//===========================================================================
#include "FPSCounter.h"

//---------------------------------------------------------------------------
//! �R���X�g���N�^
//! @parm [in] smp    �T���v����
//---------------------------------------------------------------------------
FPSCounter::FPSCounter(unsigned int smp)
{
    // �T���v�����̐ݒ�
    setSampleNum(smp);

    // �v�����鎞�v�̑I��
    if(QueryPerformanceCounter(&counter_) != 0) {
        // QueryPerformanceCounter�֐����g���t���O
        counterFlag_ = FPSCOUNTER_QUERYPER_COUNTER;
        prevCount_ = counter_.QuadPart;   // �������̎����i�N���b�N���j���擾
        LARGE_INTEGER freq;
        QueryPerformanceFrequency(&freq);   // 1�b������N���b�N�����擾
        perSecondCount_ = (double)freq.QuadPart;
    }
    else {
        // timeGetTime�֐����g���t���O
        counterFlag_ = FPSCOUNTER_TIMEGETTIME;

        // ���x���グ��
        timeBeginPeriod(1);

        // �������̎����i�~���b�j���擾
        prevMilliSecondCount_ = timeGetTime();
    }

    // �v��
    getFPS();
}

//---------------------------------------------------------------------------
//! �f�X�g���N�^
//---------------------------------------------------------------------------
FPSCounter::~FPSCounter(void)
{
    if(counterFlag_ == FPSCOUNTER_TIMEGETTIME)
        timeEndPeriod(1);   // �^�C�}�[�̐��x��߂�
}

//---------------------------------------------------------------------------
// FPS�l���擾
//---------------------------------------------------------------------------
double FPSCounter::getFPS()
{
    double def = getCurDefTime();
    if(def == 0) {
        // �v�Z�ł��Ȃ��̂ł�Ԃ�
        return 0;
    }

    return updateFPS(def);
}

//---------------------------------------------------------------------------
//! ���݂̍����������~���b�P�ʂŎ擾
//---------------------------------------------------------------------------
double FPSCounter::getCurDefTime()
{
    // �������Ԃ��v��
    if(counterFlag_ == FPSCOUNTER_QUERYPER_COUNTER) {
        QueryPerformanceCounter(&counter_);                  
        // ���݂̎������擾���A�����J�E���^�����Z�o����B
        LONGLONG longDef = counter_.QuadPart - prevCount_;
        double   dDef    = (double)longDef;   
        prevCount_   = counter_.QuadPart;
        
        // �������Ԃ��~���b�P�ʂŕԂ�
        return dDef * 1000 / perSecondCount_;                
    }

    DWORD curTime   = timeGetTime();
    DWORD curDef          = curTime - prevMilliSecondCount_;   
    prevMilliSecondCount_ = curTime;                          
    return curDef;
}

//---------------------------------------------------------------------------
// FPS���X�V
//! @parm [in] def ����
//---------------------------------------------------------------------------
double FPSCounter::updateFPS(double def)
{
    // ����
    timeList_.pop_front();
    timeList_.push_back(def);

    // FPS�Z�o
    double FPS;
    double AveDef = (sumTimes_ + def) / dataNum_;
    if(AveDef != 0) {
        FPS = 1000.0 / AveDef;
    }

    // ���ʉ��Z�����̍X�V
    sumTimes_ += def - *timeList_.begin();

    return FPS;
}

//---------------------------------------------------------------------------
// �T���v������ύX
//! @parm [in] smp    �T���v����
//---------------------------------------------------------------------------
void FPSCounter::setSampleNum(unsigned int smp)
{
    dataNum_ = smp;                        
    timeList_.resize(dataNum_, 0.0);   
    sumTimes_ = 0;
}