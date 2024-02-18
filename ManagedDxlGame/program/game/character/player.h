#pragma once
#include "../base/character_base.h"

class Camera;


class Player : public Character {
public:
	Player();
	~Player();

	void update(float delta_time) override;
	void draw(const std::shared_ptr<Camera> camera) override;

	inline const tnl::Vector3& getPos() override { return pos_; }
	inline void setPos(const tnl::Vector3& pos) override { pos_ = pos; next_pos_ = pos_; }

	inline const tnl::Vector3& getNextPos() override { return next_pos_; }
	
	inline bool isAlive() { return is_alive_; }

	inline const eActState& getActState() override { return act_state_; }

	inline const std::string& getName() override { return name_; };

	inline CharaStatus& getStatus() override { return status_; }

	inline void beginAct() { act_state_ = eActState::IDLE; };

	// 衝突処理
	inline void collisionProcess() {

		act_state_ = eActState::IDLE;
		is_collision_ = false;
		sequence_.immediatelyChange(&Player::seqIdle);
		next_pos_ = pos_;
	}

	inline void takeDamage(int damage) override {
		status_.takeDamage(damage);

		if (status_.getHP() == 0) {
			is_alive_ = false;
		}
	}

private:
	const int MAX_LEVEL = 6;
	const int LEVEL_TABLE[5] = { 12, 30, 55, 80, 110 };

	tnl::Sequence<Player> sequence_ = tnl::Sequence<Player>(this, &Player::seqIdle);

	// 現在見ている方向
	eDir_8 looking_dir_ = eDir_8::DOWN;

	// 選択中のセルの画像
	int select_cell_blue_gpc_hdl_ = 0;
	int select_cell_red_gpc_hdl_ = 0;

	// プレイヤーシーケンス
	bool seqIdle(const float delta_time);
	bool seqCheckCollision(const float delta_time);
	bool seqMove(const float delta_time);
	bool seqAttack(const float delta_time);

	// 関数のプロトタイプ宣言
	bool checkMapDataFromPos(const tnl::Vector3& pos, eMapData map_data);

	// インライン関数
	inline void setNextPosInDir(eDir_8 dir) {

		if (dir == eDir_8::UP) {
			dir_ = eDir_4::UP;
			looking_dir_ = dir;
			next_pos_ += getPosFromDir(dir);
		}
		else if (dir == eDir_8::DOWN) {
			dir_ = eDir_4::DOWN;
			looking_dir_ = dir;
			next_pos_ += getPosFromDir(dir);
		}
		else if (dir == eDir_8::LEFT) {
			dir_ = eDir_4::LEFT;
			looking_dir_ = dir;
			next_pos_ += getPosFromDir(dir);
		}
		else if (dir == eDir_8::RIGHT) {
			dir_ = eDir_4::RIGHT;
			looking_dir_ = dir;
			next_pos_ += getPosFromDir(dir);
		}
		else if (dir == eDir_8::UP_LEFT) {
			dir_ = eDir_4::UP;
			looking_dir_ = dir;
			next_pos_ += getPosFromDir(dir);
		}
		else if (dir == eDir_8::UP_RIGHT) {
			dir_ = eDir_4::UP;
			looking_dir_ = dir;
			next_pos_ += getPosFromDir(dir);
		}
		else if (dir == eDir_8::DOWN_LEFT) {
			dir_ = eDir_4::DOWN;
			looking_dir_ = dir;
			next_pos_ += getPosFromDir(dir);
		}
		else if (dir == eDir_8::DOWN_RIGHT) {
			dir_ = eDir_4::DOWN;
			looking_dir_ = dir;
			next_pos_ += getPosFromDir(dir);
		}

	}

	// 指定した方向が有効か
	inline bool isEnableDir(eDir_8 dir) {

		if (dir == eDir_8::UP_LEFT) {
			if (checkMapDataFromPos(pos_ + getPosFromDir(dir), eMapData::WALL)) return false;
			if (checkMapDataFromPos(pos_ + getPosFromDir(eDir_8::UP), eMapData::WALL)) return false;
			if (checkMapDataFromPos(pos_ + getPosFromDir(eDir_8::LEFT), eMapData::WALL)) return false;
			return true;
		}
		else if (dir == eDir_8::UP_RIGHT) {
			if (checkMapDataFromPos(pos_ + getPosFromDir(dir), eMapData::WALL)) return false;
			if (checkMapDataFromPos(pos_ + getPosFromDir(eDir_8::UP), eMapData::WALL)) return false;
			if (checkMapDataFromPos(pos_ + getPosFromDir(eDir_8::RIGHT), eMapData::WALL)) return false;
			return true;
		}
		else if (dir == eDir_8::DOWN_LEFT) {
			if (checkMapDataFromPos(pos_ + getPosFromDir(dir), eMapData::WALL)) return false;
			if (checkMapDataFromPos(pos_ + getPosFromDir(eDir_8::DOWN), eMapData::WALL)) return false;
			if (checkMapDataFromPos(pos_ + getPosFromDir(eDir_8::LEFT), eMapData::WALL)) return false;
			return true;
		}
		else if (dir == eDir_8::DOWN_RIGHT) {
			if (checkMapDataFromPos(pos_ + getPosFromDir(dir), eMapData::WALL)) return false;
			if (checkMapDataFromPos(pos_ + getPosFromDir(eDir_8::DOWN), eMapData::WALL)) return false;
			if (checkMapDataFromPos(pos_ + getPosFromDir(eDir_8::RIGHT), eMapData::WALL)) return false;
			return true;
		}
		return !checkMapDataFromPos(pos_ + getPosFromDir(dir), eMapData::WALL);
	}

};