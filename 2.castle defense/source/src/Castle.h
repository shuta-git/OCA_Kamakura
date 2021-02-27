#pragma once

class Castle : public ObjectBase
{
public:
	Castle();
	~Castle();
	void Update() override;
	void Draw() override;

	// 城の当たり判定用の座標1
	VECTOR GetCastlePos1()const { return castle_pos1; }
	// 城の当たり判定用の座標2
	VECTOR GetCastlePos2()const { return castle_pos2; }
	// 最大HP
	int GetMaxHp()const;

private:
	
	//=======================================
	// 変数
	//=======================================

	VECTOR castle_pos1;		// 城の当たり判定用の座標
	VECTOR castle_pos2;		// 城の当たり判定用の座標
};

