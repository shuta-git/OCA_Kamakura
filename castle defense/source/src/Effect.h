#pragma once

//---------------------------------------------
// Effectクラス　継承Base
//---------------------------------------------
class Effect
{
public:
	Effect();
	Effect(string str, Vector3 pos_ , bool d);
	~Effect();

	// 更新処理
	void Update();

	
	// 座標の設定
	void SetPos(Vector3 pos_);
	// 再生速度に設定
	void SetSpeed(float speed_);
	// サイズの設定
	void SetScale(Vector3 scale_);
	// 向きの設定
	void SetRot(Vector3 rot_);

	// 描画
	void Draw();				

	// パスを確認
	bool CheckPath(string str);
	// エフェクトが再生中か取得
	// 0で再生中   -1で再生されていない : 再生終了
	int IsEffectPlaying(string str);
	//　再生中のエフェクトのパスを取得
	int GetPlayingEffect() const { return playingEffectHandle; }
	// 何次元か取得
	bool GetD()const { return D; }

private:

	Vector3 pos;
	Vector3 rot;
	Vector3 scale;

	float speed;

	string save_path;			// パスを保存

	bool D;


	int handle;					// 画像ハンドル
	int playingEffectHandle;	// 再生中のエフェクトのハンドルを初期化する。

};

