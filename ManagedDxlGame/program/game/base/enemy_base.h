#pragma once
#include "character_base.h"

class ScenePlay;

// =====================================================================================
// 移動マスのコスト、位置を管理するデータ
// =====================================================================================
struct CellCost {
	// セルの位置
	tnl::Vector2i pos_ = { 0, 0 };
	// 移動コスト
	int cost_ = 0;
	// 自身のセルが有効か判定
	bool is_enable_ = true;

};

// =====================================================================================
// 敵のベースとなるクラス
// =====================================================================================
class EnemyBase : public Character {
public:
	EnemyBase();
	virtual ~EnemyBase();

	// アップデート関数
	virtual void update(float delta_time) = 0;
	// 描画関数
	virtual void draw(std::shared_ptr<Camera> camera) = 0;
	// エフェクト描画関数
	virtual void drawEffect(const std::shared_ptr<Camera> camera) = 0;

protected:
	// 行動エラーの最大カウント（ 行動エラー時の対応を行う ）
	const int ACTION_ERROR_MAX = 3;
	const float ATTACK_TIME_MAX = 0.5f;

	// ScenePlayのインスタンス（ マップデータの取得や攻撃処理用に使うが、設計ミスによりあまり良くないと思っています。 ）
	std::weak_ptr<ScenePlay> scene_play_;

	// 目標がいる方向
	tnl::Vector3 target_pos_;
	// 周囲 8方向のセルの移動コストを計算する用
	CellCost cells_[static_cast<int>(eDir_8::MAX)];

	// 目標位置を見つけたか判定
	bool is_find_target_ = false;

	// プレイヤーを見つけたか判定
	bool is_find_player_ = false;

	// 行動出来なかった回数
	int action_error_ = 0;

	// 攻撃時間
	float attack_time_ = 0.0f;

public:
	// ================= 仮想関数 =================
	
	// クローンを生成する
	virtual std::shared_ptr<EnemyBase> createClone() const = 0;
	// 敵のレベルを設定する
	virtual void setEnemyLevel(int lv) = 0;
	// レベルが上がるが判定
	virtual bool canLevelUp() = 0;
	// 行動を決定する。
	virtual void decideAction() = 0;
	// 行動を開始する
	virtual void beginAction() = 0;
	// 攻撃開始
	virtual void startAttack() = 0;
	// レベルアップ処理を行う
	virtual void startLevelUp() = 0;
	// 敵をデスさせる
	virtual void death() = 0;

protected:
	// ================= 仮想関数 =================

	//----------------------------------------------------------------
	// 敵のレベルによってできる行動を変えようと思っていました。
	// 例・・・レベル1：近距離攻撃のみ, レベル２：遠距離攻撃もできる。
	
	//// レベル１モンスターの行動を決める
	//virtual void decideActionForLv_1() = 0;
	//
	//// レベル２モンスターの行動を決める
	//virtual void decideActionForLv_2() = 0;
	//----------------------------------------------------------------

	// プレイヤーを追跡する
	virtual void trackingPlayer() {}
	
	// 各セルの移動コスト、有効なセルかを更新
	virtual void updateCellsCost() = 0;

	// 各セルの移動コストから次の位置を決める
	virtual void decideNextPosFromCellCost() {}

	// 指定した方向のセルが行動できるか判定
	virtual bool canActionToCell(const tnl::Vector2i& pos) { return false; }
	
	// 指定した方向が攻撃可能か
	// arg...判定する方向
	virtual bool canAttackInDir(eDir_8 dir) { return false; }
	
	// 通路での行動
	virtual void actionInCorridor() {}
	
	// 部屋での行動
	virtual void actionInRoom() {}
	// ============= プロトタイプ宣言 =============

	// 周囲の上下左右のマスから、指定した "eMapData" が存在する方向を返す
	std::vector<eDir_4> getMapDataDir_4(eMapData map_data);

	// 周囲 8マスからプレイヤーがいるか確認し、その方向を返す。いなければ、"eDir_8::NONE" が返ってくる
	eDir_8 findPlayerDir_8();

	// プレイヤーにもっとも近い入口の位置を返す
	tnl::Vector3 getEntrancePosToNearestPlayer(int area_id) const;

	// プレイヤーが同じ部屋にいるか判定
	bool isSameRoomToPlayer() const;

	// 現在の部屋の入口をランダムに取得する
	tnl::Vector3 getRandomEntranceToNowRoom() const;

	// 移動コストが最も小さく有効な CellCostのインデックスを取得
	int getMinimunScoreCellIndex();

	// 攻撃行動に切り替える
	// args...攻撃方向
	void changeToAttackAction(eDir_8 dir);

	// 移動行動に切り替える
	void changeToMoveAction();
};

