#include "Base.h"

//---------------------------------------------
// コンストラクタ
//---------------------------------------------
Base::Base()
{
	pos = Vector3( 0.0f,0.0f,0.0f );
	old_pos = Vector3(0.0f, 0.0f, 0.0f);
	rot = Vector3( 0.0f,0.0f,0.0f );
	scale = Vector3(1.0f, 1.0f, 1.0f);

	mat = MGetIdent();
	mat_trans = MGetIdent();
	mat_rot = MGetIdent();

	speed = 0.0f;
}
//---------------------------------------------
// デストラクタ
//---------------------------------------------
Base::~Base()
{
}
void Base::MatrixInit()
{
	mat = MGetIdent();
	mat_trans = MGetIdent();
	mat_rot = MGetIdent();

}
void Base::RotMatrixInit()
{
	mat_rot = MGetIdent();
}
void Base::TransMatrixInit()
{
	mat_trans = MGetIdent();
}
//---------------------------------------------
// カプセルの上部分
//---------------------------------------------
VECTOR Base::GetCapsuleUp(VECTOR pos_,float size_) const
{
	VECTOR result;

	result.x = pos_.x;
	result.y = pos_.y + size_;
	result.z = pos_.z;

	return result;
}
//---------------------------------------------
// カプセルの下部分
//---------------------------------------------
VECTOR Base::GetCapsuleDown(VECTOR pos_) const
{
	VECTOR result;

	result.x = pos_.x;
	result.y = pos_.y + radius;
	result.z = pos_.z;

	return result;
}
//---------------------------------------------
// カプセルの前部分
//---------------------------------------------
VECTOR Base::GetCapsuleTip(VECTOR pos_, float size_) const
{
	VECTOR result;

	result.x = pos_.x;
	result.y = pos_.y;
	result.z = pos_.z + size_;

	return result;
}
//---------------------------------------------
// カプセルの後ろ部分
//---------------------------------------------
VECTOR Base::GetCapsuleRoot(VECTOR pos_) const
{
	VECTOR result;

	result.x = pos_.x;
	result.y = pos_.y;
	result.z = pos_.z;

	return result;
}
//---------------------------------------------
// エフェクトが止まっていたらデリート
//---------------------------------------------
void Base::DeleteEffect(int i)
{
	if (effect[i]->GetD()) {
		// 何番（GetPlayingEffect）のエフェクトが止まったかを判定
		if (IsEffekseer3DEffectPlaying(effect[i]->GetPlayingEffect()) == 0)
		{
			return;
		}
	}
	else {
		// 何番（GetPlayingEffect）のエフェクトが止まったかを判定
		if (IsEffekseer2DEffectPlaying(effect[i]->GetPlayingEffect()) == 0)
		{
			return;
		}
	}

	// 止まった番号のエフェクトをデリート、初期化
	delete effect[i];
	effect[i] = nullptr;

	// エフェクトが止まっていたら
	// 止まったエフェクト分サイズを縮める
	EraseEffect();
}
//---------------------------------------------
// エフェクトを停止
//---------------------------------------------
void Base::StopEffect(int i)
{
	if (effect[i]->GetD()) {
		StopEffekseer3DEffect(effect[i]->GetPlayingEffect());
	}
	else {
		StopEffekseer2DEffect(effect[i]->GetPlayingEffect());
	}
}
//---------------------------------------------
// vectorの中身がnullptrの時消去
//---------------------------------------------
void Base::EraseEffect()
{
	std::vector<Effect*>::iterator it = effect.begin();
	while (it != effect.end())
	{
		if (*it == nullptr) {
			it = effect.erase(it);
		}
		else {
			it++;
		}
	}
}


