#pragma once

//---------------------------------------------
// �萔
//---------------------------------------------

#define PI		 3.141592653589793
#define RadToDeg 57.29577951f
#define R2D(rad) rad * RadToDeg

#define DegToRad 0.017453293f
#define D2R(deg) deg * DegToRad

#define BLACK	GetColor(0, 0, 0)
#define BLACKGRAY GetColor(90,90,90)
#define GRAY	GetColor(125,125,125)
#define WHITE	GetColor(255, 255, 255)
#define RED		GetColor(255, 0, 0)
#define GREEN	GetColor(0, 255, 0)
#define BLUE	GetColor(0, 0, 255)
#define YELLOW	GetColor(255, 255, 0)
#define PINK	GetColor(255, 0, 255)
#define SKYBLUE	GetColor(0, 255, 255)


#define WINDOW_MODE	TRUE
#define MOUSE_FLG	TRUE

#define WINDOW_W	1920
#define WINDOW_H	1080

//#if (NDEBUG)
//#define WINDOW_W	960
//#define WINDOW_H	540
//#else
//#define WINDOW_W	1920
//#define WINDOW_H	1080
//#endif


// �E	90�x
#define DIR_RIGHT (DX_PI_F / 2)
// ��	270�x
#define DIR_LEFT  (DX_PI_F + (DX_PI_F / 2))
// ��	0�x
#define DIR_UP1 0.0f
// ��	360�x
#define DIR_UP2 (DX_PI_F * 2)
// ��	180�x
#define DIR_DOWN DX_PI_F 
// �E�� 45�x
#define DIR_RIGHT_UP (DX_PI_F / 4)
// �E�� 135�x
#define DIR_RIGHT_DOWN (DX_PI_F - (DX_PI_F / 4))
// ���� 315�x
#define DIR_LEFT_UP ((DX_PI_F * 2 ) - (DX_PI_F / 4))
// ���� 225�x
#define DIR_LEFT_DOWN (DX_PI_F + (DX_PI_F / 4))



enum ModelType
{
	PLAYER_MODEL = 0,	// �v���C���[
	//GROUND_MODEL,		// �O���E���h
	CASTLE_MODEL,		// ��
	SPIDER_MODEL,		// �w�(�G�l�~�[1)
	ENEMY2_MODEL,		// (�G�l�~�[2)
	ENEMY3_MODEL,		// (�G�l�~�[3)
};

enum SceneType {
	TITLE = 0,	// �^�C�g��
	SELECT,		// �Z���N�g	
	LOAD,		// ���[�h
	GAME_MAIN,	// �Q�[�����C��
	OPERATION,	// �������
	RANKING,	// �����L���O
	PAUSE,		// �|�[�Y
	RESULT,		// ���U���g
};
