#pragma once

//---------------------------------------------
// Enemy3�N���X �p��EnemyBase
//---------------------------------------------
class Enemy3 : public EnemyBase
{
public:
	Enemy3(Vector3 pos_ = { 0.0f,0.0f,0.0f }, Vector3 rot_ = { 0.0f,0.0f,0.0f },
		Vector3 scale_ = { 1.0f,1.0f,1.0f });
	~Enemy3();

	// �X�V
	void Update() override;
	// �`��
	void Draw() override;				

private:


};

