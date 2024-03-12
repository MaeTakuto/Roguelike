#pragma once
#include "../../base/character_base.h"

class Camera;

// 最大レベル
const int MAX_LEVEL = 6;
// 経験値テーブル
const int LEVEL_TABLE[MAX_LEVEL - 1] = { 12, 30, 55, 80, 110 };

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
	// 毎ターン回復させる HP
	int regenerating_hp_ = 1;

	// プレイヤーの行動シーケンス
	tnl::Sequence<Player> sequence_ = tnl::Sequence<Player>(this, &Player::seqIdle);

	// 現在見ている方向
	eDir_8 looking_dir_ = eDir_8::DOWN;

	// 攻撃
	//tnl::Vector3 attack_dir_ = { 0, 0, 0 };


	// 選択中のセルの画像
	int select_cell_blue_gpc_hdl_ = 0;
	int select_cell_red_gpc_hdl_ = 0;

public:
	// 行動を開始する
	void beginAction() override;
	// レベルアップできるか判定
	bool canLevelUp() override;
	// 攻撃開始
	void startAttack() override;
	// レベルアップさせる
	void startLevelUp() override;

private:
	// プレイヤーシーケンス
	bool seqIdle(const float delta_time);
	bool seqMove(const float delta_time);
	bool seqAttack(const float delta_time);
	bool seqDamage(const float delta_time);
	bool seqLevellUp(const float delta_time);

	// ====================== 関数のプロトタイプ宣言 ============================
	bool checkMapDataFromPos(const tnl::Vector3& pos, eMapData map_data);
	// 指定した方向の next_pos_、looking_dir_、dir_をセット
	void setNextPosInDir(eDir_8 dir);
	// 指定した方向のセルが行動できるか判定
	bool canActionToCell(eDir_8 dir);

};