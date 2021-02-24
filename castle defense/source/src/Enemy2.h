#pragma once

//---------------------------------------------
// Enemy2クラス 継承EnemyBase
//---------------------------------------------
class Enemy2 : public EnemyBase
{
public:
	Enemy2( Vector3 pos_ = { 0.0f,0.0f,0.0f }, Vector3 rot_ = { 0.0f,0.0f,0.0f },
		Vector3 scale_ = { 1.0f,1.0f,1.0f });
	~Enemy2();
	// 更新
	void Update() override;		
	// 描画
	void Draw() override;	

private:

	void SetModelHandle(string str);
	void ApplyTexture();
	void SetAnim();
	void DrawModel();

	std::vector<int>model_type;
	int type;
	int old_type;


	enum Type
	{
		IDLE = 0,       
		WALK,
		ATTACK_FRONT,
		ATTACK_DIVEBOMB,
		ATTACK_FIREBALL,
		INTRO,
		DEATH,
	};
};

