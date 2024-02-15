#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "../common/enum.h"
#include "../common/chara_status.h"

class Camera;

const int DIR_MAX = 8;

// キャラの行動状態
enum class eActState {
	IDLE,
	MOVE,
	ATTACK,
	END
};

class Character {
public:
	Character() {};
	virtual ~Character() {};

	virtual void update(float delta_time) = 0;
	virtual void draw(const std::shared_ptr<Camera> camera) = 0;

	virtual const tnl::Vector3& getPos() = 0;
	virtual void setPos(const tnl::Vector3&) = 0;
	
	virtual const tnl::Vector3& getNextPos() = 0;

	virtual bool isAlive() = 0;

	virtual const eActState& getActState() = 0;

	virtual const std::string& getName() = 0;

	virtual CharaStatus& getStatus() = 0;

	virtual void takeDamage(int damage) = 0;

protected:
	const float MOVE_SPEED = 0.25f;

	std::vector< std::vector<int> > chara_gpc_hdl_;
	std::vector< std::vector<tnl::CsvCell> > gpc_hdl_data_;

	std::string name_ = "";
	CharaStatus status_ = CharaStatus();

	tnl::Vector3 pos_ = { 0, 0, 0 };
	tnl::Vector3 next_pos_ = { 0, 0, 0 };
	tnl::Vector3 attack_dir_ = { 0, 0, 0 };

	eDir_4 dir_ = eDir_4::DOWN;
	eActState act_state_ = eActState::IDLE;

	bool is_alive_ = true;;
	bool is_collision_ = false;


};