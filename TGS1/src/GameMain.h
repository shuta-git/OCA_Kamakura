#pragma once
#include"Map.h"
#include"Block.h"
#include"Main.h"

class GameMain
{
private:
	// �u���b�N�̈ʒu�p
	Vec2 b_pos;
	// �g���b�v�̈ʒu�p
	Vec2 t_pos;
	Vec2 t_drop_pos;
	// �J����
	CameraBox2D camera;
	// �}�b�v
	TaskKeep<Map> map;
	// MAP�e�L�X�g�p
	String MAP_text;
	// Trap�e�L�X�g�p
	String Trap_text;
	// �g���b�v�̎��
	int type;
	// �}�b�v�I��ϐ�
	int map_num;
	// �S�[������
	bool goal;

	// TRIANGLE_OUT �p�̕ϐ�
	int tout_cnt;		// �t�@�C���̒��̉��ڂ�����p
	int tout_cnt1;		// �t�@�C���̒��̐��𐔂���
	int tout_add;		// num���������������𐔂���
	int tout_add1;		// �N���G�C�g���ꂽ���𐔂���

	// �t�H���g�p
	const Font m_font;

public:
	GameMain(int num);
	~GameMain();

	//----------------
	// �K��֐�
	//----------------
	void Init();
	void Update();
	void Draw();
	void Exit();
	// �擾�������ԍ��ƍ��W�̎O�p�`���擾
	Triangle GetTriangle(int num, double x,double y) const;

	//----------------
	// Get�֐�
	//----------------
	bool GetIsPlayerDeath()const { return map->GetPDeath(); }
	bool GetIsGoal()const { return goal; }
	RectF GetRectGoal() const { return RectF(MAX_W - 100, MAP_SIZE, MAP_SIZE, MAP_SIZE * 3); }
	double GetPlayerPosX()const { return map->GetPlayerPosX(); }

private:
	// �e�L�X�g�t�@�C����ǂݍ���Ńu���b�N�����
	void TextReadCreatBlock();
	// MAP�e�L�X�g��ǂݍ���
	void ReadTextMAP();
	// MAP�e�L�X�g�ɏ�������
	void WriterTextMAP();
	// MAP�e�L�X�g�̒��g�̏�������
	void ChangeTextBlock();

	// �ŏ������Ă΂��֐�( Init �� Trap2 , DropTrap2 )
	void InitTextCreatTrap();
	// �����̃e�L�X�g��Trap�e�L�X�g���㏑���ۑ�( Trap , DropTrap �� Init ) 
	void InitWriterTextTrap();
	// �g���b�v�e�L�X�g��ǂݍ��݃g���b�v�쐬( Trap )
	void TextReadCreatTrap();
	// �g���b�v�e�L�X�g��ǂݍ��ݔ�яo��g���b�v���쐬( Trap )
	void TextReadCreatOutTrap1();

	// �g���b�v�e�L�X�g�ɏ�������( Trap2 �� Trap )
	void WriterTextTrap();
	// �g���b�v2�e�L�X�g�ɏ�������( Trap �� Trap2 )
	void WriterText2Trap();
	// �g���b�v������( Trap �� Trap2 )( DropTrap �� DropTrap2)
	void DeleteTrap();
	// �g���b�v���`�F���W����( Trap �� Trap2 )
	void ChangeTrap();
	// �h���b�v�g���b�v�e�L�X�g��ǂݍ���( DropTrap )
	void TextReadCreatDropTrap();
	// �g���b�v�e�L�X�g�ɏ�������( DropTrap2 �� DropTrap )
	void WriterTextDropTrap();
	// �g���b�v�e�L�X�g�ɏ�������( DropTrap �� DropTrap2 )
	void WriterText2DropTrap();
	

	// �g���b�v�̎�ނ��L�[�Ő؂�ւ�
	void KeyChangeTrapType();
};

