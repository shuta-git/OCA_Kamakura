#pragma once
#include <Siv3D.hpp>
#include <HamFramework.hpp>
#include <rnfs.h>


// �E�B���h�E�̃T�C�Y
#if ! defined(NDEBUG)
#define WINDOW_W 1920
#define WINDOW_H 1080
#else
#define WINDOW_W 1920
#define WINDOW_H 1080
#endif

// �u���b�N�̃T�C�Y
#define MAP_SIZE 40

// �u���b�N�̐�
#define BLOCK_W 100
#define BLOCK_H 25

// �}�b�v�̒[
#define MAX_W MAP_SIZE*BLOCK_W
#define MAX_H MAP_SIZE*BLOCK_H

// �J�����͈̔�
#define CAMERA_MIN_X 360
#define CAMERA_MAX_X 2360
#define CAMERA_MIN_Y 780

// �v���C���[
#define PLAYER_ROT 30
#define PLAYER_POS_X 200
#define PLAYER_PO_Y  100

// �h���b�v�g���b�v�̗����鑬�x
#define TRAP_DROP_SPEED 5.0

// �R���g���[���[
static XInput controller = XInput(0);


//----------------
// �^�X�N�p
//----------------
enum CallGroup
{
	CallGroup_Update,
	CallGroup_Draw,
};
enum CallPriority
{
	CallPriority_Map,     //�ŏ��ɕ`�悪�s����
	CallPriority_Trap,
	CallPriority_Player,    //���̌�ɕ`�悪�s����̂ŁA�v���C���[���B��Ȃ�

};

//----------------
// �g���b�v���
//----------------
enum Type_
{
	TRIANGLE_UP,
	TRIANGLE_DOWN,
	TRIANGLE_RIGHT,
	TRIANGLE_LEFT,
	TRIANGLE_OUT,
	TRIANGLE_DROP,
	BLOCK,
	TRIANGLE_OUT_1,
};

enum Scene_
{
	START_SCREEN,			// �X�^�[�g���
	SELECT_SCREEN,			// �Z���N�g���
	GAMEMAIN_SCREEN,		// �Q�[�����
	GAMEOVER_SCREEN,		// �Q�[���I�[�o�[���
	OPERATION_SCREEN,		// ����������
	CLEAR_SCREEN,			// �N���A���
	PAUSE_SCREEN,			// �|�[�Y���

	SCREEN_COUNT,
};

enum Map_Num_
{
	MAP_ONE,
	MAP_TWO,
	MAP_THREE,

	MAP_COUNT,
};

enum Select_
{
	SELECT_OPERATION,
	SELECT_EASY,
	SELECT_NORMAL,
	SELECT_HARD,

	SELECT_COUNT,
};

enum Pause_
{
	PAUSE_GAME,
	PAUSE_OPERATION,
	PAUSE_SPAWN,
	PAUSE_START,

	PAUSE_COUNT,
};