#pragma once
#include "Cursor.h"

//-----------------------------------------------------------
// �萔
//-----------------------------------------------------------
#define MAP_WIDTH_MAX   200        // �}�b�v�̍ő剡��
#define MAP_HEIGHT_MAX  15         // �}�b�v�̍ő�c����
#define MAP_MAX         1          // �}�b�v�̐�
#define MAX_MAP_IMG     10
#define MAP_SPEED  6.0F

//-----------------------------------------------------------
// �N���X
//-----------------------------------------------------------
class MAP : public BaseObj{
public:
	MAP();

	~MAP();

	Cursor* cur = nullptr;

	void Init();
	void Exit();
	void Update(int num_,int mode_num_,int old_num_);
	void Draw();
	void ReloadGraph();
	// �����蔻��
	void SetPlayer(Player* player);
	void CheckHitPlayer(Player* player);
	void ResetPosX() { draw_x = old_x; }// ���Z�b�gX
	void SetMoveX() { draw_old_x = move_x; }
	void ResetMoveX() { move_x = draw_old_x; }

	void InitDrawLeftX() { draw_x = 0; }
	void InitDrawRightX();

	float GetDrawX() const { return draw_x; }

	

private:

	
	int number;					// �X�e�[�W�i���o�[
	int mode_num;
	int old_num;

	int images[MAX_MAP_IMG];
	int BlockData[MAP_HEIGHT_MAX][MAP_WIDTH_MAX];
	
	int image_count = 0;
	bool start;

	void ImageChange();			// �u���b�N�̃C���[�W�`�F���W
	
	void InitFileRead();
	void TutorialFileRead();
	void TutorialFileSave();
	void OldFileRead();
	void OldFileSave();
	void FileRead();			// �t�@�C���̓ǂݍ���
	void FileSave();			// �t�@�C���Z�[�u

	void Move();
	void DrawMoveX();

	bool MoveCheckRangeLeft();					// ������͈͓����`�F�b�N
	bool MoveCheckRangeRight();					// ������͈͓����`�F�b�N
								
	void DrawMoveRange();		// �}�b�v�r���_�[���̃X�N���[���͈�

};


