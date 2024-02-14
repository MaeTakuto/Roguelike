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

	inline int getAtk() override { return status_.getAtk(); }

	inline int getDef() override { return status_.getDef(); }

	inline void beginAct() { act_state_ = eActState::IDLE; };

	// 衝突処理
	inline void collisionProcess() override {

		act_state_ = eActState::IDLE;
		is_collision_ = false;
		sequence_.immediatelyChange(&Player::seqIdle);
		next_pos_ = pos_;
	}

	inline void takeDamage(int damage) override {}

private:

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
	bool isEnableDir(eDir_8 dir);

	// インライン関数
	inline const tnl::Vector3& getPosFromDir(eDir_8 dir) {
		tnl::Vector3 next;

		if (dir == eDir_8::UP)				next.y -= 1;
		else if(dir == eDir_8::DOWN)		next.y += 1;
		else if(dir == eDir_8::LEFT)		next.x -= 1;
		else if(dir == eDir_8::RIGHT)		next.x += 1;
		else if(dir == eDir_8::UP_LEFT)		next += tnl::Vector3(-1, -1, 0 );
		else if(dir == eDir_8::UP_RIGHT)	next += tnl::Vector3( 1, -1, 0 );
		else if(dir == eDir_8::DOWN_LEFT)	next += tnl::Vector3(-1,  1, 0 );
		else if(dir == eDir_8::DOWN_RIGHT)	next += tnl::Vector3( 1,  1, 0 );
		return next;
	}

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

};