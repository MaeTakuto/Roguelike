#pragma once
#include "../manager/gm_manager.h"
#include "../base/character_base.h"
#include "../scene/scene_play.h"


class Node {
public:
	tnl::Vector2i pos_ = { 0, 0 };
	int cost_ = 0;
	bool is_enable_ = true;

};

class Enemy : public Character {
public:
	Enemy();
	~Enemy();

	void update(float delta_time) override;
	void draw(const std::shared_ptr<Camera> camera) override;

private:
	const int ACTION_ERROR_MAX = 3;

	// ScenePlay�̃C���X�^���X
	std::weak_ptr<ScenePlay> scene_play_;

	tnl::Sequence<Enemy> sequence_ = tnl::Sequence<Enemy>(this, &Enemy::seqIdle);

	tnl::Vector3 target_pos_ = { 0, 0, 0 };

	Node* nodes_ = nullptr;

	bool is_target_pos_ = false;

	int action_error_ = 0;
	int target_entrance_id_ = 0;

	bool seqIdle(const float delta_time);
	bool seqActionStundby(const float delta_time);
	bool seqMove(const float delta_time);

public:

	void init();

	// �Q�b�^�[�A�Z�b�^�[
	inline const tnl::Vector3& getPos() override { return pos_; }
	inline void setPos(const tnl::Vector3& pos) { pos_ = pos; }
	inline const tnl::Vector3& getNextPos() override { return next_pos_; }
	inline bool isAlive() override { return is_alive_; }
	inline const eActState& getActState() override { return act_state_; }
	inline int getAtk() override { return status_.getAtk(); }
	inline int getDef() override { return status_.getDef(); }

	// 
	inline void collisionProcess() override {

	}

	// �_���[�W���󂯂�
	inline void takeDamage(int damage) override {
		status_.takeDamage(damage);

		if (status_.getHP() == 0) {
			is_alive_ = false;
		}
	}

	// ���S����ɂ���
	inline void death() { 
		is_alive_ = false;
		is_target_pos_ = false;
		action_error_ = 0;
	}

	// �s�������߂�
	inline void desideAction() {

		auto scene_play = scene_play_.lock();
		if (scene_play == nullptr) return;

		if (scene_play->getPlace(pos_) == ePlace::ROAD) onRoadAction();
		else onRoomAction();
	}

	// �s�����J�n����
	inline void beginAction() { sequence_.immediatelyChange(&Enemy::seqMove); }

	// ���g���X�|�[��������
	inline void spawn(const tnl::Vector3& pos) {

		is_alive_ = true;
		pos_ = pos;
		next_pos_ = pos;

		status_.setStatus(1, 8, 5, 0, 5);
		dir_ = eDir_4::DOWN;
		act_state_ = eActState::IDLE;
	}

private:
	void onRoadAction();

	void onRoomAction();

	void moveToTarget();
	void findTargetInRoom();
	Node* getMinimunScoreNodeForEnabled();

	bool isEnableMapPosition(const tnl::Vector2i& pos) {

		if ( pos.x < 0 ) return false;
		if ( pos.y < 0 ) return false;
		if ( pos.x > GameManager::FIELD_WIDTH ) return false;
		if ( pos.y > GameManager::FIELD_HEIGHT ) return false;
		return true;
	}

	// =====================================================================================
	// �����Ă�������𔽑΂��擾����
	// =====================================================================================
	inline eDir_4 getOppositeDir(eDir_4 dir) {

		if (dir == eDir_4::UP)			return eDir_4::DOWN;
		else if (dir == eDir_4::DOWN)		return eDir_4::UP;
		else if (dir == eDir_4::LEFT)		return eDir_4::RIGHT;
		else if (dir == eDir_4::RIGHT)	return eDir_4::LEFT;
	}

	// =====================================================================================
	// �w�肵�������̂P�}�X��� next_pos_ �ɃZ�b�g
	// =====================================================================================
	inline void setNextPosInDir(eDir_4 dir) {

		switch (dir) {
		case eDir_4::UP:
			next_pos_.y -= 1;
			dir_ = dir;
			break;
		case eDir_4::DOWN:
			next_pos_.y += 1;
			dir_ = dir;
			break;
		case eDir_4::LEFT:
			next_pos_.x -= 1;
			dir_ = dir;
			break;
		case eDir_4::RIGHT:
			next_pos_.x += 1;
			dir_ = dir;
			break;
		default:
			return;
			break;
		}
	}

	// =====================================================================================
	// �w�肵���n�`�f�[�^���A�w�肵���ʒu�̎����4��������T���A���݂��������Ԃ��B
	// =====================================================================================
	inline std::vector<eDir_4> getNearbyMapData(const tnl::Vector3& pos, eMapData map_data) {
		
		auto scene_play = scene_play_.lock();		
		std::vector<eDir_4> directions;

		if (scene_play == nullptr) return directions;

		if (scene_play->getMapData(tnl::Vector3(pos.x, pos.y - 1, pos.z)) == map_data) directions.emplace_back(eDir_4::UP);
		if (scene_play->getMapData(tnl::Vector3(pos.x, pos.y + 1, pos.z)) == map_data) directions.emplace_back(eDir_4::DOWN);
		if (scene_play->getMapData(tnl::Vector3(pos.x - 1, pos.y, pos.z)) == map_data) directions.emplace_back(eDir_4::LEFT);
		if (scene_play->getMapData(tnl::Vector3(pos.x + 1, pos.y, pos.z)) == map_data) directions.emplace_back(eDir_4::RIGHT);

		return directions;
	}

	// =====================================================================================
	// �ړ��V�[�P���X�ɐ؂�ւ���B
	// =====================================================================================
	inline void changeToMoveAction() {

		auto scene_play = scene_play_.lock();
		if (scene_play == nullptr) return;

		scene_play->setMapData(pos_, scene_play->getTerrainData(pos_));
		scene_play->setMapData(next_pos_, eMapData::ENEMY);
		// sequence_.change(&Enemy::seqActionStundby);
		act_state_ = eActState::MOVE;
	}

};