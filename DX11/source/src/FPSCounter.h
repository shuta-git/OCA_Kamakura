//===========================================================================
//!	@file	FPSCounter.h
//!	@brief	FPS�J�E���^�[
//===========================================================================
#pragma once
#pragma comment(lib, "winmm.lib")

#define FPSCOUNTER_QUERYPER_COUNTER 1
#define FPSCOUNTER_TIMEGETTIME 2

//===========================================================================
//!�@FPS�J�E���^�[
//===========================================================================
class FPSCounter
{
public:
    //! �R���X�g���N�^
    //! @parm [in] smp    �T���v����
    FPSCounter(unsigned int smp = 10);

    //! �f�X�g���N�^
    virtual ~FPSCounter();

    //! FPS�l���擾
    double getFPS();

    //! �T���v������ύX
    //! @parm [in] smp    �T���v����
    void setSampleNum(unsigned int smp);

protected:

    //! ���݂̎������擾
    double getCurDefTime();

    //! FPS���X�V
    //! @parm [in] Def ����
    double updateFPS(double def);

private:

    int               counterFlag_;    //!< �g�p����v���֐��̔���t���O
    LARGE_INTEGER     counter_;         //!< �N���b�N�J�E���g��
    double            perSecondCount_;           //!< 1�b������N���b�N�J�E���g���i���g���j
    LONGLONG          prevCount_;    //!< �ȑO�̃N���b�N�J�E���g��
    DWORD             prevMilliSecondCount_;   //!< �ȑO�̎����i�~���b�j
    std::list<double> timeList_;    //!< ���ԃ��X�g
    UINT              dataNum_;           //!< �ړ����όv�Z���̃f�[�^��
    double            sumTimes_;      //!< ���ʕ����̍��v�l

};

double getFPS();

f32 getDeltaTime();
