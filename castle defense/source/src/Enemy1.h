#pragma once

//---------------------------------------------
// Enemy1�N���X �p��EnemyBase
//---------------------------------------------
class Enemy1 : public EnemyBase
{
public:
	Enemy1(Vector3 pos_ = { 0.0f,0.0f,0.0f }, Vector3 rot_ = { 0.0f,0.0f,0.0f },
		Vector3 scale_ = {0.05f,0.05f,0.05f});
	~Enemy1();

	// �X�V
	void Update() override;		
	// �`��
	void Draw() override;				

private:

	//=======================================
	// �񋓑�
	//=======================================

	// �A�j���[�V�����̎��
	enum Anim {
		ATTACK = 0,	// 0
		DIE,		// 1
		DIE2,		// 2
		FALL,		// 4
		JUMP,		// 3
		NORMAL,		// 4	
		RUN_BACK,	// 6
		RUN_VOR,    // 7   
		RUN_LEFT,	// 8
		RUN_RIGHT,	// 9	
		WAIK_BACK,	// 10	
		WAIK_VOR,	// 11	
		WAIK_LEFT,	// 12	
		WAIK_RIGHT,	// 13	
		POSE,		// 14	

		MAX,
	};
};

