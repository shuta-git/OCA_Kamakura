#include "SceneBase.h"

//---------------------------------------------
// コンストラクタ
//---------------------------------------------
SceneBase::SceneBase()
{
	x = 0;
	y = 0;
	size = 1.0f;
}
//---------------------------------------------
// デストラクタ
//---------------------------------------------
SceneBase::~SceneBase()
{
}
//---------------------------------------------
// シーンの種類を取得
//---------------------------------------------
int SceneBase::GetSceneType() const
{
	return scene_type;
}
//---------------------------------------------
// シーンの種類を設定
//---------------------------------------------
void SceneBase::SetSceneType(int scene_type_)
{
	scene_type = scene_type_;
}
//---------------------------------------------
// グラフィックのパスを設定
// "data/"　ここからパスを入力
//---------------------------------------------
void SceneBase::SetGraphHandle(string str, int x_, int y_, float size_)
{
	// パスを設定
	string data = "data/scene/";
	string path = data + str;
	// グラフィックハンドルを入れていく
	graph_handle.push_back(resource::LoadGraph(path.c_str()));
	// ハンドルの設定
	handle_path.push_back(path);
	// 座標設定
	v_x.push_back(x_);
	v_y.push_back(y_);
	// サイズの設定
	v_size.push_back(size_);
	// 描画する
	v_active.push_back(true);
	// 透明度を有効にするか
	v_trans.push_back(TRUE);
}
//---------------------------------------------
// 画像の座標を設定
//---------------------------------------------
void SceneBase::DrawSetPosition(string str, int x_, int y_)
{
	// パスを設定
	string data = "data/scene/";
	string path = data + str;
	for (int i = 0; i < graph_handle.size(); i++) {
		if (handle_path[i] == path) {
			v_x[i] = x_;
			v_y[i] = y_;
		}
	}
}
//---------------------------------------------
// 透明度の設定
//---------------------------------------------
void SceneBase::SetTrans(string str, bool trans)
{
	// パスを設定
	string data = "data/scene/";
	string path = data + str;
	for (int i = 0; i < graph_handle.size(); i++) {
		if (handle_path[i] == path) {
			v_trans[i] = trans;
		}
	}
}
//---------------------------------------------
// 画像を描画するか指定
//---------------------------------------------
void SceneBase::DrawSetActive(string str, bool active)
{
	// パスを設定
	string data = "data/scene/";
	string path = data + str;
	for (int i = 0; i < graph_handle.size(); i++) {
		if (handle_path[i] == path) {
			v_active[i] = active;
		}
	}
}
//---------------------------------------------
// グラフィックハンドルの描画
//---------------------------------------------
void SceneBase::DrawGraphHandle()
{
	for (int i = 0; i < graph_handle.size(); i++) {
		if (v_active[i]) {
			DrawRotaGraph(v_x[i], v_y[i], v_size[i], 0, graph_handle[i], v_trans[i], FALSE);
		}
	}
}
//---------------------------------------------
// エフェクトが止まっていたらデリート
//---------------------------------------------
void SceneBase::DeleteEffect(int i)
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
void SceneBase::StopEffect(int i)
{
	if (effect[i]->GetD()) {
		StopEffekseer3DEffect(effect[i]->GetPlayingEffect());
	}
	else {
		StopEffekseer2DEffect(effect[i]->GetPlayingEffect());
	}
}
//---------------------------------------------
// 中央揃えで描画
// どの位置から中央揃えするか　Y座標　
// 出力したい文字 フォントハンドル 色
//---------------------------------------------
void SceneBase::DrawCenterString(int x_,int y_, string str_, unsigned int color_, int font_handle_)
{
	int x = GetDrawStringWidthToHandle(str_.c_str(), (int)strlen(str_.c_str()), font_handle_) / 2;

	DrawStringToHandle(x_ - x,y_,str_.c_str(), color_,font_handle_);
}
//---------------------------------------------
// 中央揃えでフォーマット指定描画
// どの位置から中央揃えするか　Y座標　
// 色 フォントハンドル 出力したい文字 変数
//---------------------------------------------
void SceneBase::DrawCenterFormatString(int x_, int y_, unsigned int color_, int font_handle_, string str_, int num)
{
	int x = GetDrawStringWidthToHandle(str_.c_str(), (int)strlen(str_.c_str()), font_handle_) / 2;

	DrawFormatStringToHandle(x_ - x, y_, color_,font_handle_,str_.c_str(),num);
}
//---------------------------------------------
// 右詰めで描画
// Y座標　出力したい文字
// フォントハンドル 色
//---------------------------------------------
void SceneBase::DrawRightString(int y_, string str_, unsigned int color_, int font_handle_)
{
	int x = GetDrawStringWidthToHandle(str_.c_str(), (int)strlen(str_.c_str()), font_handle_);

	DrawStringToHandle( WINDOW_W - x, y_, str_.c_str(), color_, font_handle_);
}
//---------------------------------------------
// 右詰めてフォーマット指定描画
// Y座標　色 フォントハンドル
// 出力したい文字 変数
//---------------------------------------------
void SceneBase::DrawRightFormatString(int y_, unsigned int color_, int font_handle_, string str_, int num)
{
	int x = GetDrawStringWidthToHandle(str_.c_str(), (int)strlen(str_.c_str()), font_handle_);

	DrawFormatStringToHandle(WINDOW_W - x, y_, color_, font_handle_, str_.c_str(), num);
}
//---------------------------------------------
// 文字の先頭のX座標を取得
//---------------------------------------------
int SceneBase::GetStringPosX(int x_, string str_, int font_handle_)
{
	int x = GetDrawStringWidthToHandle(str_.c_str(), (int)strlen(str_.c_str()), font_handle_) / 2;

	return x_ - x;
}
//---------------------------------------------
// フォントハンドルを消去
//---------------------------------------------
void SceneBase::DeleteFontHandle()
{
	for (int i = 0; i < font_handle.size();i++) {
		DeleteFontToHandle(font_handle[i]);
	}
}
//---------------------------------------------
// vectorの中身がnullptrの時消去
//---------------------------------------------
void SceneBase::EraseEffect()
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