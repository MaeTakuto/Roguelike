#pragma once
#include "../../../base/enemy_base.h"

class Projectile;

class Skeleton final : public EnemyBase {
public:
	Skeleton();
	~Skeleton();

	void update(float delta_time) override;
	void draw(const std::shared_ptr<Camera> camera) override;
	void drawEffect(const std::shared_ptr<Camera> camera) override;

private:
	// 敵の行動シーケンスの管理
	tnl::Sequence<Skeleton> sequence_;

	// 投擲物
	std::shared_ptr<Projectile> bone_;

	// 投擲物の画像
	int bone_gpc_hdl_;

public:
	// ============= プロトタイプ宣言 =============

	// クローンを生成する
	std::shared_ptr<EnemyBase> createClone() const override;
	// 敵のレベルを設定する
	void setEnemyLevel(int lv) override;
	// レベルが上がるが判定
	bool canLevelUp() override;
	// 行動を決定する。
	void decideAction() override;
	// 攻撃を開始する
	void startAttack() override;
	// 行動を開始する
	void beginAction() override;
	// レベルアップ処理を行う
	void startLevelUp() override;
	// 敵をデスさせる
	void death() override;

private:
	// =================== 行動シーケンス ===================
	// 待機状態
	bool seqIdle(const float delta_time);
	// 移動状態
	bool seqMove(const float delta_time);
	// 攻撃状態
	bool seqAttack(const float delta_time);

	// ============= プロトタイプ宣言 =============
	// レベル１モンスターの行動を決める
	void decideActionForLv_1() override;
	// レベル２モンスターの行動を決める
	void decideActionForLv_2() override;
	// 目標の位置に向かって 1マス移動する
	void setNextPosToTarget();
	// プレイヤーを追跡する
	void trackingPlayer() override;
	// 各セルの移動コスト、有効なセルかを更新
	void updateCellsCost() override;
	// 各セルの移動コストから次の位置を決める
	void decideNextPosFromCellCost() override;
	// 指定した方向のセルが行動できるか判定
	bool canActionToCell(const tnl::Vector2i& pos) override;
	// 骨を投げる攻撃ができるか判定し、可能であれば "dir" に方向を格納する
	bool tryCanBoneAttack(eDir_8& dir);
	// プレイヤーがいるか確認する。
	bool isPlayerDir(const tnl::Vector3& pos, eDir_8 dir, bool is_loop = false) const;
	// 指定した方向が攻撃可能か
	bool canAttackInDir(eDir_8 dir) override;
	// 通路での行動
	void actionInCorridor() override;
	// 部屋での行動
	void actionInRoom() override;

};