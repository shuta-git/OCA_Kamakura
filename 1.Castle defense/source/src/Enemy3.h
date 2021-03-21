#pragma once

//---------------------------------------------
// Enemy3クラス 継承EnemyBase
//---------------------------------------------
class Enemy3 : public EnemyBase
{
public:
	Enemy3(Vector3 pos_ = { 0.0f,0.0f,0.0f }, Vector3 rot_ = { 0.0f,0.0f,0.0f },
		Vector3 scale_ = { 1.0f,1.0f,1.0f });
	~Enemy3();

	// 更新
	void Update() override;
	// 描画
	void Draw() override;				

private:


};

