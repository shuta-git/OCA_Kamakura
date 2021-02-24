#include "Effect.h"

//---------------------------------------------
// コンストラクタ
//---------------------------------------------
Effect::Effect()
{

}
//---------------------------------------------
// コンストラクタ
// ファイルパス　ポジション　true３D　false2D
//---------------------------------------------
Effect::Effect(string str, Vector3 pos_, bool d)
{
	playingEffectHandle = -1;

	pos = pos_;
	rot = Vector3(0.0f, 0.0f, 0.0f);
	scale = Vector3(1.0f, 1.0f, 1.0f);
	speed = 1.0f;

	// パスを設定
	string data = "data/effect/";
	string path = data + str;
	save_path = path;
	// ハンドルにパスを設定
	handle = resource::LoadEffekseerEffect(path.c_str());
	// ３Dか２D
	D = d;

	if (D) {
		// エフェクトを再生する。
		playingEffectHandle = PlayEffekseer3DEffect(handle);
	}
	else {
		// エフェクトを再生する。
		playingEffectHandle = PlayEffekseer2DEffect(handle);
	}
}
//---------------------------------------------
// デストラクタ
//---------------------------------------------
Effect::~Effect()
{
}
//---------------------------------------------
// 更新
//---------------------------------------------
void Effect::Update()
{
	if (D) {
		// ポジションを設定
		SetPosPlayingEffekseer3DEffect(playingEffectHandle, pos.x, pos.y, pos.z);
		// 向きの設定
		SetRotationPlayingEffekseer3DEffect(playingEffectHandle, rot.x, rot.y, rot.z);
		// サイズの設定
		SetScalePlayingEffekseer3DEffect(playingEffectHandle, scale.x, scale.y, scale.z);
		// 再生速度
		SetSpeedPlayingEffekseer3DEffect(playingEffectHandle, speed);
	}
	else {
		// ポジションを設定
		SetPosPlayingEffekseer2DEffect(playingEffectHandle, pos.x, pos.y, pos.z);
		// 向きの設定
		SetRotationPlayingEffekseer2DEffect(playingEffectHandle, rot.x, rot.y, rot.z);
		// サイズの設定
		SetScalePlayingEffekseer2DEffect(playingEffectHandle, scale.x, scale.y, scale.z);
		// 再生速度
		SetSpeedPlayingEffekseer2DEffect(playingEffectHandle, speed);
	}
}
//---------------------------------------------
// エフェクトが再生中か取得
//---------------------------------------------
int  Effect::IsEffectPlaying(string str)
{
	if (CheckPath(str)) {
		return IsEffekseer3DEffectPlaying(playingEffectHandle);
	}
}
//---------------------------------------------
// 座標の設定
//---------------------------------------------
void Effect::SetPos(Vector3 pos_)
{
	pos = pos_;
}
//---------------------------------------------
// 速度の設定
//---------------------------------------------
void Effect::SetSpeed(float speed_)
{
	speed = speed_;
}
//---------------------------------------------
// スケールの設定
//---------------------------------------------
void Effect::SetScale(Vector3 scale_)
{
	scale = scale_;
}
//---------------------------------------------
// 向きの設定
//---------------------------------------------
void Effect::SetRot(Vector3 rot_)
{
	rot = rot_;
}
//---------------------------------------------
// 描画
//---------------------------------------------
void Effect::Draw()
{
	if (D) {
		// Effekseerにより再生中のエフェクトを描画する。
		DrawEffekseer3D();
	}
	else {
		// Effekseerにより再生中のエフェクトを描画する。
		DrawEffekseer2D();
	}
}
//---------------------------------------------
// パスを確認
//---------------------------------------------
bool Effect::CheckPath(string str)
{
	string data = "data/effect/";
	string path = data + str;
	if (save_path == path) {
		return true;
	}
	return false;
}
