#pragma once
#include "../base/character_base.h"

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

	// 行動を開始する
	inline void beginAction() override { act_state_ = eActState::IDLE; }

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

	// 選択中のセルの画像
	int select_cell_blue_gpc_hdl_ = 0;
	int select_cell_red_gpc_hdl_ = 0;

	// プレイヤーシーケンス
	bool seqIdle(const float delta_time);
	bool seqMove(const float delta_time);
	bool seqAttack(const float delta_time);

	// ====================== 関数のプロトタイプ宣言 ============================
	bool checkMapDataFromPos(const tnl::Vector3& pos, eMapData map_data);

	// ============================= インライン関数 =============================

	// 指定した方向の next_pos_、looking_dir_、dir_をセット
	inline void setNextPosInDir(eDir_8 dir) {

		int index = static_cast<int>(dir);
		if (index < 0 || index >= static_cast<int>(eDir_8::MAX)) {
			return;
		}

		anim_dir_ = ANIM_DIR[index];
		looking_dir_ = dir;
		next_pos_ += DIR_POS[static_cast<int>(dir)];
	}

	// 指定した方向が有効か
	inline bool isEnableDir(eDir_8 dir) {

		if (dir == eDir_8::UP_LEFT) {
			if (checkMapDataFromPos(pos_ + DIR_POS[static_cast<int>(dir)], eMapData::WALL)) return false;
			if (checkMapDataFromPos(pos_ + DIR_POS[static_cast<int>(eDir_8::UP)], eMapData::WALL)) return false;
			if (checkMapDataFromPos(pos_ + DIR_POS[static_cast<int>(eDir_8::LEFT)], eMapData::WALL)) return false;
			return true;
		}
		else if (dir == eDir_8::UP_RIGHT) {
			if (checkMapDataFromPos(pos_ + DIR_POS[static_cast<int>(dir)], eMapData::WALL)) return false;
			if (checkMapDataFromPos(pos_ + DIR_POS[static_cast<int>(eDir_8::UP)], eMapData::WALL)) return false;
			if (checkMapDataFromPos(pos_ + DIR_POS[static_cast<int>(eDir_8::RIGHT)], eMapData::WALL)) return false;
			return true;
		}
		else if (dir == eDir_8::DOWN_LEFT) {
			if (checkMapDataFromPos(pos_ + DIR_POS[static_cast<int>(dir)], eMapData::WALL)) return false;
			if (checkMapDataFromPos(pos_ + DIR_POS[static_cast<int>(eDir_8::DOWN)], eMapData::WALL)) return false;
			if (checkMapDataFromPos(pos_ + DIR_POS[static_cast<int>(eDir_8::LEFT)], eMapData::WALL)) return false;
			return true;
		}
		else if (dir == eDir_8::DOWN_RIGHT) {
			if (checkMapDataFromPos(pos_ + DIR_POS[static_cast<int>(dir)], eMapData::WALL)) return false;
			if (checkMapDataFromPos(pos_ + DIR_POS[static_cast<int>(eDir_8::DOWN)], eMapData::WALL)) return false;
			if (checkMapDataFromPos(pos_ + DIR_POS[static_cast<int>(eDir_8::RIGHT)], eMapData::WALL)) return false;
			return true;
		}
		return !checkMapDataFromPos(pos_ + DIR_POS[static_cast<int>(dir)], eMapData::WALL);
	}

};