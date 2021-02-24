//===========================================================================
//!	@file	EnemyManager.h
//!	@brief	エネミーマネージャー
//===========================================================================
#pragma once

//===========================================================================
//! エネミーマネジャー
//===========================================================================
class EnemyManager : public ManagerBase
{
public:
    //! コンストラクタ
    EnemyManager();

    //! デストラクタ
    virtual ~EnemyManager();

    //! 初期化
    virtual bool initialize() override;

    //! 更新
    virtual void update() override;

    void update(Vector3 playerPos);

    //! 描画
    virtual void render() override;

    //! 解放
    virtual void finalize() override;

    //! imgui描画
    virtual void imguiRender() override;

    //-------------------------------------------------------------
    //!	@name	取得
    //-------------------------------------------------------------
    //@{

    //! エネミーの数を取得
    size_t getSize() const;				

    //! ボックスコライダーを取得
    std::shared_ptr<gpu::BoxCollider> getBoxCollider(int i) const;

    //! 座標を取得
    Vector3 getPos(int i) const;

    //@}

    void setPos(int i, Vector3 pos);

private:

    //! イレース処理
    void erase();

    //! スポーン
    void spawn();

private:

	std::vector<std::unique_ptr<EnemyBase>> enemies;   // エネミー

    f32 time_;   //!< 経過時間(s)
};
