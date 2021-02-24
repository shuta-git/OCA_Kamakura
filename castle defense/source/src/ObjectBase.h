#pragma once


class ObjectBase : public CollisionBase
{
public:
	ObjectBase();
	~ObjectBase();

protected:
	// テクスチャーを適用
	void ApplyTexture();
	// テクスチャーのパスを設定
	void SetTextureHandle(string str);
	// モデルのパスを設定
	void SetModelHandle(string str);
	// モデルを描画
	void DrawModel();
	// 値を設定
	void SetValue();

	std::vector<int>model_handle;		// モデルハンドル
	std::vector<int>texture_handle;		// テクスチャーハンドル

private:

	
};

