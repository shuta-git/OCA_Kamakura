#pragma once

//---------------------------------------------
// CollisionBaseクラス　継承 Base
//---------------------------------------------
class CollisionBase : public Base
{
public:
	CollisionBase();
	~CollisionBase();

	//=======================================
	// Get関数
	//=======================================
	// モデルハンドルを取得
	int GetModel() const { return model; }
	// ダメージを取得
	int GetDamage() const { return damage; }
	// HPを取得
	int GetHp()const { return hp; }
	// 一つ前のHPを取得
	int GetOldHp()const { return old_hp; }
	// スコアを取得
	int GetScore()const { return score; }
	// 半径を取得
	float GetRadius() const { return radius; }
	// 死んだかどうか
	bool GetDeath()const { return death; }					
	// 範囲に入っているか
	bool GetRange()const { return range; }
	// 攻撃範囲に入っているかどうか
	bool GetAttackRange()const { return attack_range; }
	// 無敵かどうか
	bool GetInvincible()const { return invincible;}
	// 攻撃フラグを取得
	bool GetAttack() const { return attack; }

	//=======================================
	// Set関数
	//=======================================
	// death フラグを立てる
	void SetIsHitDeath() { death = true; }
	// range フラグを立てる
	void SetIsHitRange() { range = true; }
	// attack_rangeフラグを立てる
	void SetIs_HitAttackRange() { attack_range = true; }
	// invincible フラグを立てる(無敵)
	void SetInvincible() { invincible = true; }	
	// range を falseにする
	void SetFalseRange() { range = false; }
	// attack_range を false にする
	void SetFalseAttackRange() { attack_range = false; }
	// HP の処理
	void HpProcessing(int hp_);
	// 座標に代入
	void SetPos(Vector3 v) { pos = v; }
	// attack フラグを falseにする
	void SetFalseAttack() { attack = false; }
	// HPを0にする
	void SetHPInit() { hp = 0; }

protected:
	//=======================================
	// 変数
	//=======================================
	float  length;			// 長さ
	float  anim_time;		// アニメーションの時間
	float  play_anim;		// アニメーションの再生
	float  play_anim_speed; // アニメーション速度

	int    hp;				// HP
	int    old_hp;			// 一つ前のHP
	int	   damage;			// ダメージ
	int	   old_anim_num;	// 前のアニメーション番号
	int	   anim_num;		// アニメーション番号
	int    attach_anim;		// アッタッチしたアニメデータ
	int	   model;			// モデルハンドル
	int	   score;			// スコア

	bool   death;			// 死亡フラグ
	bool   range;			// 範囲フラグ
	bool   attack_range;	// 攻撃範囲フラグ
	bool   invincible;		// 無敵フラグ
	bool   attack;			// 攻撃用のフラグ

	//=======================================
	// Get関数
	//=======================================
	// 無敵時間を取得
	int GetInvincibleTimer()const { return invincible_timer; }	
	// 無敵処理
	void Invincible();											

private:
	//=======================================
	// 定数
	//=======================================
	const int MAX_INVUNCIBLE_TIME = 60;		// 最大無敵時間(60)
	int invincible_timer;					// 無敵時間をはかる用

};

