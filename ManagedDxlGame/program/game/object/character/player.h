#pragma once
#include "../../base/character_base.h"

class Camera;
class Projectile;

// =====================================================================================
// プレイヤークラス
// =====================================================================================
class Player : public Character {
public:
	Player();
	~Player();

	void update(float delta_time) override;
	void draw(const std::shared_ptr<Camera> camera) override;
	void drawEffect(const std::shared_ptr<Camera> camera) override;

	// 衝突処理
	inline void collisionProcess() {

		act_state_ = eActState::IDLE;
		sequence_.immediatelyChange(&Player::seqIdle);
		next_pos_ = pos_;
	}

private:
	// 毎ターン回復させる MP
	int regenerating_mp_;

	// プレイヤーの行動シーケンス
	tnl::Sequence<Player> sequence_;

	// 入力可能か判定
	bool can_operation_input_;

	// ------------------ 魔法関連 ---------------------------------

	// 魔法詠唱エフェクト
	std::shared_ptr<Animation> magic_chanting_effect_;
	// 魔法を使用しているか判定
	bool is_use_magic_;
	// 使用する魔法のリスト番号
	int use_magic_index_;

	// -------------------------------------------------------------

	// 選択中のセルの画像
	int select_cell_blue_gpc_hdl_;
	int select_cell_red_gpc_hdl_;

public:
	// 操作入力できるようにするか設定
	inline void setOperationInput(bool can_operation_input) { can_operation_input_ = can_operation_input; }
	// 行動を開始する
	void beginAction() override;
	// 指定した目標位置にキャラクターを移動させる
	void moveToTargetPos(const tnl::Vector3& target_pos) override;
	// レベルアップできるか判定
	bool canLevelUp() override;
	// 攻撃開始
	void startAttack() override;
	// 魔法の使用準備をする
	void setupMagic();
	// 魔法の使用開始
	void startMagic();
	// レベルアップさせる
	void startLevelUp() override;
	
	// 魔法を使えるか試す
	bool tryUseMagic(int magic_index);
	
private:
	// ===================== プレイヤーシーケンス =======================
	// 待機、入力シーケンス
	bool seqIdle(const float delta_time);
	// 移動シーケンス
	bool seqMove(const float delta_time);
	// 攻撃シーケンス
	bool seqAttack(const float delta_time);
	// レベルアップシーケンス
	bool seqLevellUp(const float delta_time);
	// 魔法詠唱シーケンス
	bool seqMagicChanting(const float delta_time);
	// 魔法攻撃シーケンス
	bool seqUseMagic(const float delta_time);

	// ====================== 関数のプロトタイプ宣言 ============================
	bool checkMapDataFromPos(const tnl::Vector3& pos, eMapData map_data);
	// 指定した方向の next_pos_、looking_dir_、dir_をセット
	void setNextPosInDir(eDir_8 dir);
	// 指定した方向のセルが行動できるか判定
	bool canActionToCell(eDir_8 dir);

};