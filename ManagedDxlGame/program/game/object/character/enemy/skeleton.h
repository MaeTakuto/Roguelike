#pragma once
#include "../../../base/enemy_base.h"

class Projectile;

// =====================================================================================
// ダンジョン内に出現する、"がいこつ"のクラス
// =====================================================================================
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

	//eDir_8 bone_throw_dir_;
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
	// 指定した目標位置にキャラクターを移動させる
	void moveToTargetPos(const tnl::Vector3& target_pos) override;
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
	// ダメージを受ける
	void takeDamage(int damage) override;
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
	// ダメージを受けるシーケンス
	bool seqTakeDamage(const float delta_time);

	// ============= プロトタイプ宣言 =============
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
	// 指定した方向が攻撃可能か判定をする
	bool canAttackInDir(eDir_8 dir) override;
	// 通路での行動処理をする
	void actionInCorridor() override;
	// 部屋での行動処理をする
	void actionInRoom() override;

};