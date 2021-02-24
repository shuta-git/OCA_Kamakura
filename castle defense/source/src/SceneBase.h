#pragma once

//---------------------------------------------
// SceneBaseクラス
//---------------------------------------------
class SceneBase
{
public:
	SceneBase();
	~SceneBase();

	virtual void Update() = 0;
	virtual void Draw() = 0;

	// シーンの種類を取得
	int GetSceneType() const;
	// シーンの種類を設定
	void SetSceneType(int scene_type_);
	// フォントハンドルを消去
	void DeleteFontHandle();

protected:

	int arrow_pos_x;	// 矢印のX座標
	int arrow_pos_y;	// 矢印のY座標
	int scene_type;		// シーンの種類
	int old_scene;		// 一つ前のシーン
	int x, y;			// 座標
	float size;			// サイズ
	Vector3 pos;		// Vectorクラス
	Vector3 scale;		// サイズ
	std::vector<Effect*>effect;		// エフェクト
	std::vector<int>font_handle;	// フォントデータ

	// 画像のパスを設定
	void SetGraphHandle(string str,int x_ = 0,int y_ = 0, float size_ = 1.0f);
	// 画像の座標を設定
	void DrawSetPosition(string str,int x_ = 0,int y_ = 0);
	// 透明度の設定
	void SetTrans(string str, bool trans);
	// 画像を描画するか指定
	void DrawSetActive(string str,bool active);
	// グラフィックハンドルの描画
	void DrawGraphHandle();
	// エフェクトを消す
	void DeleteEffect(int i);
	// エフェクトを止める
	void StopEffect(int i);
	// 中央揃えで描画
	// どの位置から中央揃えするか　Y座標　
	// 出力したい文字 フォントハンドル色
	void DrawCenterString(int x_, int y_,string str_, unsigned int color_,int font_handle_);
	// 中央揃えでフォーマット指定描画
	// どの位置から中央揃えするか　Y座標　
    // 色 フォントハンドル 出力したい文字 変数
	void DrawCenterFormatString(int x_,int y_, unsigned int color_,int font_handle_,string str_,int num);
	// 右詰めで描画
	// Y座標　出力したい文字
    // フォントハンドル 色
	void DrawRightString(int y_,string str_,unsigned int color_, int font_handle_);
	// 右詰めてフォーマット指定描画
	// Y座標　色 フォントハンドル
	// 出力したい文字 変数
	void DrawRightFormatString(int y_, unsigned int color_, int font_handle_, string str_, int num);
	// 文字の先頭のX座標を取得
	int GetStringPosX(int x_,string str_,int font_handle_);
	

private:
	std::vector<string>handle_path;	// ハンドルのパス
	std::vector<int>graph_handle;	// 画像ハンドル
	std::vector<int>v_x;			// x座標
	std::vector<int>v_y;			// y座標
	std::vector<float>v_size;		// サイズ
	std::vector<bool>v_active;		// 描画するか
	std::vector<bool>v_trans;		// 透明度を有効にするか

	// effectのサイズを縮める
	void EraseEffect();
};

