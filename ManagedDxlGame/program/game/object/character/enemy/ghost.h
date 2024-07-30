#pragma once
#include "../../../base/enemy_base.h"


// =====================================================================================
// おばけかぼちゃ（敵）のクラス
// =====================================================================================
class Ghost final : public EnemyBase {
public:
	Ghost();
	~Ghost();

	void update(float delta_time) override;
	void draw(std::shared_ptr<Camera> camera) override;
	void drawEffect(const std::shared_ptr<Camera> camera) override;

private:
	// 敵の行動シーケンスの管理
	tnl::Sequence<Ghost> sequence_ = tnl::Sequence<Ghost>(this, &Ghost::seqIdle);

public:
	// ============= プロトタイプ宣言 =============

	// 次のレベルまでの必要経験値を取得
	int getExpToNextLevel() const override { return 0; }
	// クローンを生成する
	std::shared_ptr<EnemyBase> createClone() const override;
	// 敵のレベルを設定する
	void setEnemyLevel(int lv) override;
	// 指定した目標位置にキャラクターを移動させる
	void moveToTargetPos(const tnl::Vector3& target_pos) override;
	// レベルが上がるか判定
	bool canLevelUp() override;
	// 行動を決定する。
	void decideAction() override;
	// 攻撃を開始する
	void startAttack() override;
	// 行動を開始する
	void beginAction() override;
	// レベルアップ処理を行う
	void startLevelUp() override;
	// ダメージを受ける
	void takeDamage(int damage) override;
	// 敵をデスさせる
	void death() override;
	//// 
	//bool tryUseMagic(int magic_index) override { return false; };

private:
	// =================== 行動シーケンス ===================
	// 待機状態
	bool seqIdle(const float delta_time);
	// 移動状態
	bool seqMove(const float delta_time);
	// 攻撃状態
	bool seqAttack(const float delta_time);
	// ダメージを受けるシーケンス
	bool seqTakeDamage(const float delta_time);

	// ============= プロトタイプ宣言 =============

	// 目標の位置に向かって 1マス移動する
	void setNextPosToTarget();
	// 各セルの移動コスト、有効なセルかを更新
	void updateCellsCost() override;
	// 各セルの移動コストから次の位置を決める
	void decideNextPosFromCellCost() override;
	// 移動可能なセルからランダムに移動先を決める
	void decideNextPosFromRandom();
	
};