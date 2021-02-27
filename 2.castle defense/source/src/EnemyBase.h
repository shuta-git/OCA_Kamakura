#pragma once

//---------------------------------------------
// EnemyBaseクラス 継承Base
//---------------------------------------------
class EnemyBase : public CollisionBase
{
public:
	EnemyBase();
	~EnemyBase();

	// 死亡アニメーションフラグを取得
	bool GetDeathAnim() { return death_anim; }
	// 目標地点についた時の処理
	bool GetGoal();
	// 範囲用の半径を取得
	float GetRangeRadius() const { return range_radius; }
	// 攻撃範囲の半径を取得
	float GetAttackRangeRadius() const { return attack_range_radius; }
	// プレイヤー座標取得
	void SetPlayerPos(VECTOR player_pos_);

protected:

	// プレイヤー座標を取得
	VECTOR GetPlayerPos() { return player_pos; }
	// old変数設定
	void SetOld();	
	// 近づく
	void Approach(VECTOR goal_);	
	// ゴールチェック
	void CheckGoal();
	// モデルハンドルを設定
	void SetModelHandle(string str);

	//=======================================
	// アニメーション
	//=======================================

	// アニメーションの番号を設定
	void SetAnimNum(int num) { anim_num = num; } 
	// アニメーションを設定
	void SetAnim();			
	// アニメーションの更新
	void UpdateAnim();							 

	//=======================================
	// テクスチャー
	//=======================================

	 // テクスチャーのパスを取得
	void SetTextureHandle(string str);		
	// テクスチャーを適用
	void ApplyTexture();									

	//=======================================
	// 変数
	//=======================================
	int	   recast_time;			// リキャストタイム
	float  range_radius;		// 範囲用の半径
	float  attack_range_radius; // 攻撃範囲の半径

	VECTOR player_pos;
	VECTOR goal_pos;					// 目標地点の座標
	std::vector<int>texture_handle;		// テクスチャーハンドル

	bool death_anim;	// 死亡アニメーション
	bool goal;			// ゴール


};

