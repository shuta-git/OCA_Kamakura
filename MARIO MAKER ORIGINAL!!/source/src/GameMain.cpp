#include "WinMain.h"

//-----------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------
GameMain::GameMain()
{
	pause = false;
	alive = false;
	clear = false;
	timer = 60.00;
	map = nullptr;
	player = nullptr;
	screen = nullptr;
}
//-----------------------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------------------
GameMain::~GameMain()
{
	Exit();
	if (map != nullptr) {
		delete map;
		map = nullptr;
	}
	if (player != nullptr) {
		delete player;
		player = nullptr;
	}
	if (screen != nullptr) {
		delete screen;
		screen = nullptr;
	}
	if (cur != nullptr) {
		delete cur;
		cur = nullptr;
	}
}

//-----------------------------------------------------------
//	������
//-----------------------------------------------------------
void GameMain::Init()
{
	if (cur == nullptr){
		cur = new Cursor();
	}
}
//-----------------------------------------------------------
//	�X�V
//-----------------------------------------------------------
void GameMain::Update(int number_, int mode_num_, int old_num_, int chara_num_)
{
	number = number_;
	old_num = old_num_;
	mode_num = mode_num_;
	chara_num = chara_num_;

	// pause;
	Pause();

	// �X�N���[��
	if (screen == nullptr) {
		screen = new Screen(number, chara_num);
	}
	if (screen != nullptr) {
		screen->Update(number, chara_num_);
	}


	if (number == GAMEOVER || number == CLEAR || old_num == TUTORIAL && mode_num == NOT_MODE) {
		ResetTimer();
		if (map != nullptr) {
			delete map;
			map = nullptr;
		}
	}
	//	�|�[�Y
	if (pause) {
		return;
	}

	if (old_num == PAUSE) {
		if (cur != nullptr) {
			cur->Move();
			if (CheckMouseInput()) {
				// ���X�|�[��
				if (cur->GetX() >= 255 && cur->GetY() >= 300 &&
					cur->GetX() <= 388 && cur->GetY() <= 325) {
					if (player != nullptr) {
						delete player;
						player = nullptr;
					}
					ResetTimer();
				}
			}
		}
		// �}�b�v�r���_�[
		if (number == START || number == NOT_SCREEN && mode_num == MAP_MODE ) {
			if (map != nullptr) {
				delete map;
				map = nullptr;
			}
			ResetTimer();
		}
	}

	// �}�b�v
	if (map == nullptr) {
		map = new MAP();
	}
	if (map != nullptr) {
		map->Update(number, mode_num, old_num);
	}

	 
	// �Q�[����  �`���[�g���A��
	if (mode_num == GAME_MODE || mode_num == TUTORIAL_MODE) {
		// �Q�[�����̃^�C�}�[
		if (timer <= 0.00) {
			alive = true;
		}
		if (player == nullptr) {
			player = new Player();
			map->Init();
		}
		if (number != TUTORIAL) {
			timer -= 0.02;
			if (player->GetDrawX() >= MAP_WIDTH_MAX * MAP_SIZE - 255) {
				clear = true;
				player->TimerScore(timer);
				if (screen != nullptr) {
					screen->SetPlayer(player, timer);
				}
			}
		}
		if (player != nullptr) {
			player->Update();
		}
		if (player->GetPosY() >= MAX_WINDOW_H) {
			alive = true;
		}
		if (map != nullptr) {
			if (player != nullptr) {
				map->SetPlayer(player);
				if (player->Check_Map_Range()) {
					map->ResetPosX();
					map->ResetMoveX();
				}
			}
		}
	}
	else{
		if (player != nullptr) {
			delete player;
			player = nullptr;
			alive = false;
			clear = false;
		}
	}
}
//-----------------------------------------------------------
//	�`��
//-----------------------------------------------------------
void GameMain::Draw()
{
	if (map != nullptr) {
		map->Draw();
	}
	if (mode_num == GAME_MODE || mode_num == TUTORIAL_MODE) {
		if (player != nullptr) {
			player->Draw();
		}
		if (number != TUTORIAL) {
			SetFontSize(30);
			DrawFormatString(280, 30, GetColor(255, 255, 255), "%0.2f", timer);
			SetFontSize(20);
			DrawFormatString(15, 15, GetColor(255, 255, 255), "�X�R�A �F %0.2f", player->GetScore());
		}
	}
	if (screen != nullptr) {
		screen->Draw();
	}
}
//-----------------------------------------------------------
//	�I��
//-----------------------------------------------------------
void GameMain::Exit()
{
	if (map != nullptr) {
		map->Exit();
	}
	if (player != nullptr) {
		player->Exit();
	}
	if (screen != nullptr) {
		screen->Exit();
	}
}
//-----------------------------------------------------------
// �摜�̓ǂݒ����֐�
//-----------------------------------------------------------
void GameMain::ReloadGraph()
{
	if (map != nullptr) {
		map->ReloadGraph();
	}
	if (player != nullptr) {
		player->ReloadGraph();
	}
}

//-----------------------------------------------------------
// pause
//-----------------------------------------------------------
void GameMain::Pause()
{
	// pause����
	if (number == PAUSE || mode_num == GAME_MODE && number == OPERATION || number == GAMEOVER || number == CLEAR) {
		pause = true;
	}
	else {
		pause = false;
	}
}
//-----------------------------------------------------------
// ���Z�b�g�^�C��
//-----------------------------------------------------------
void GameMain::ResetTimer()
{
	timer = 60.00;
}



