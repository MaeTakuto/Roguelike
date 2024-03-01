#pragma once
#include "../manager/gm_manager.h"
#include "../base/character_base.h"
#include "../scene/scene_play.h"


// =====================================================================================
// �ړ��}�X�̃R�X�g�A�ʒu���Ǘ�����f�[�^
// =====================================================================================
struct Node {
	tnl::Vector2i pos_ = { 0, 0 };
	int cost_ = 0;
	bool is_enable_ = true;

};

// =====================================================================================
// �G�̃x�[�X�ƂȂ�N���X
// =====================================================================================
class Enemy : public Character {
public:
	Enemy();
	virtual ~Enemy();

	void update(float delta_time) override;
	void draw(const std::shared_ptr<Camera> camera) override;

private:
	// �s���G���[�̍ő�J�E���g�i �s���G���[���̑Ή����s�� �j
	const int ACTION_ERROR_MAX = 3;

	// ScenePlay�̃C���X�^���X�i �}�b�v�f�[�^�̎擾��U�������p�Ɏg�����A�݌v�~�X�ɂ�肠�܂�ǂ��Ȃ��Ǝv���Ă��܂��B �j
	std::weak_ptr<ScenePlay> scene_play_;

	// �G�̍s���V�[�P���X�̊Ǘ�
	tnl::Sequence<Enemy> sequence_ = tnl::Sequence<Enemy>(this, &Enemy::seqIdle);

	// �ڕW�̈ʒu
	tnl::Vector3 target_pos_ = { 0, 0, 0 };

	// ����̃}�X�̍œK�R�X�g��T���p
	Node* nodes_ = nullptr;

	// �ڕW�ʒu��������������
	bool is_find_target_ = false;

	// �v���C���[��������������
	bool is_find_player_ = false;

	// �s���o���Ȃ�������
	int action_error_ = 0;

	// �ڕW�̓����ԍ�
	int target_entrance_id_ = 0;

	float attack_time_ = 0.0f;

public:

	void init();

	// �Q�b�^�[�A�Z�b�^�[
	inline const tnl::Vector3& getPos() const override { return pos_; }
	inline void setPos(const tnl::Vector3& pos) { pos_ = pos; }
	inline const tnl::Vector3& getNextPos() const override { return next_pos_; }
	inline bool isAlive() const override { return is_alive_; }
	inline const eActState& getActState() const override { return act_state_; }
	inline const std::string& getName() const override { return name_; };
	inline const CharaStatus& getStatus() const override { return status_; }

	// �o���l���擾
	void addExp(int exp) override;

	// �s�����J�n����
	void beginAction() override;

	// �_���[�W���󂯂�
	void takeDamage(int damage) override;

	// ���S����ɂ���
	void death();

	// �s�������߂�
	void desideAction();

	// ���g���X�|�[��������
	void spawn(const tnl::Vector3& pos);

private:
	// �s���V�[�P���X
	bool seqIdle(const float delta_time);
	bool seqActionStundby(const float delta_time);
	bool seqMove(const float delta_time);
	bool seqAttack(const float delta_time);

	// �ʘH�ł̍s������
	void onRoadAction();

	// �����ł̍s������
	void onRoomAction();

	void moveToTarget();
	void findTargetInRoom();
	void findPlayer();
	Node* getMinimunScoreNodeForEnabled();

	bool isEnableMapPosition(const tnl::Vector2i& pos);

	// =====================================================================================
	// �����Ă�������𔽑΂��擾����
	// =====================================================================================
	eDir_4 getOppositeDir(eDir_4 dir);

	// =====================================================================================
	// �w�肵�������̂P�}�X��� next_pos_ �ɃZ�b�g
	// =====================================================================================
	inline void setNextPosInDir(eDir_4 dir) {

		switch (dir) {
		case eDir_4::UP:
			next_pos_.y -= 1;
			anim_dir_ = dir;
			break;
		case eDir_4::DOWN:
			next_pos_.y += 1;
			anim_dir_ = dir;
			break;
		case eDir_4::LEFT:
			next_pos_.x -= 1;
			anim_dir_ = dir;
			break;
		case eDir_4::RIGHT:
			next_pos_.x += 1;
			anim_dir_ = dir;
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
	inline void changeToMoveSeq() {

		auto scene_play = scene_play_.lock();
		if (scene_play == nullptr) return;

		scene_play->setMapData(pos_, scene_play->getTerrainData(pos_));
		scene_play->setMapData(next_pos_, eMapData::ENEMY);
		// sequence_.change(&Enemy::seqActionStundby);
		act_state_ = eActState::MOVE;
	}

	// =====================================================================================
	// �U���V�[�P���X�ɐ؂�ւ���B
	// =====================================================================================
	inline void changeToAttackSeq() {
		// sequence_.change(&Enemy::seqActionStundby);
		act_state_ = eActState::ATTACK;
	}

	// =====================================================================================
	// �v���C���[�� 8�����m�F���āA���݂��Ă����ꍇ���̕�����Ԃ��B
	// =====================================================================================
	inline eDir_8 findPlayerDir_8() {

		auto scene_play = scene_play_.lock();
		if (scene_play == nullptr) return eDir_8::NONE;

		for (int i = static_cast<int>(eDir_8::UP); i < static_cast<int>(eDir_8::MAX); ++i) {
			if (scene_play->getMapData(pos_ + DIR_POS[i]) == eMapData::PLAYER) return static_cast<eDir_8>(i);
		}
		//if (scene_play->getMapData(pos_ + DIR_POS[static_cast<int>(eDir_8::UP)]) == eMapData::PLAYER)				return eDir_8::UP;
		//else if (scene_play->getMapData(pos_ + DIR_POS[static_cast<int>(eDir_8::DOWN)]) == eMapData::PLAYER)		return eDir_8::DOWN;
		//else if (scene_play->getMapData(pos_ + DIR_POS[static_cast<int>(eDir_8::LEFT)]) == eMapData::PLAYER)		return eDir_8::LEFT;
		//else if (scene_play->getMapData(pos_ + DIR_POS[static_cast<int>(eDir_8::RIGHT)]) == eMapData::PLAYER)		return eDir_8::RIGHT;
		//else if (scene_play->getMapData(pos_ + DIR_POS[static_cast<int>(eDir_8::UP_LEFT)]) == eMapData::PLAYER)		return eDir_8::UP_LEFT;
		//else if (scene_play->getMapData(pos_ + DIR_POS[static_cast<int>(eDir_8::UP_RIGHT)]) == eMapData::PLAYER)	return eDir_8::UP_RIGHT;
		//else if (scene_play->getMapData(pos_ + DIR_POS[static_cast<int>(eDir_8::DOWN_LEFT)]) == eMapData::PLAYER)	return eDir_8::DOWN_LEFT;
		//else if (scene_play->getMapData(pos_ + DIR_POS[static_cast<int>(eDir_8::DOWN_RIGHT)]) == eMapData::PLAYER)	return eDir_8::DOWN_RIGHT;

		return eDir_8::NONE;
	}

	// =====================================================================================
	// �w�肵���ʒu�̃Z�����w�肵���}�b�v�f�[�^������
	// =====================================================================================
	inline bool checkMapDataFromPos(const tnl::Vector3& pos, eMapData map_data) {

		auto scene_play = scene_play_.lock();
		if (scene_play == nullptr) return false;

		return scene_play->getMapData(pos) == map_data;
	}

	// =====================================================================================
	// �w�肵���������L����
	// =====================================================================================
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

	// =====================================================================================
	// �w�肵���������L����
	// =====================================================================================
	bool checkAttackForPlayer() {
		eDir_8 dir = findPlayerDir_8();

		if (dir == eDir_8::NONE) return false;
		return isEnableDir(dir);
	}
};