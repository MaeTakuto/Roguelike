#pragma once
#include "../base/character_base.h"

class Camera;

// �ő僌�x��
const int MAX_LEVEL = 6;
// �o���l�e�[�u��
const int LEVEL_TABLE[MAX_LEVEL - 1] = { 12, 30, 55, 80, 110 };

// =====================================================================================
// �v���C���[�N���X
// =====================================================================================
class Player : public Character {
public:
	Player();
	~Player();

	void update(float delta_time) override;
	void draw(const std::shared_ptr<Camera> camera) override;

	// �s�����J�n����
	inline void beginAction() override { act_state_ = eActState::IDLE; }

	// �Փˏ���
	inline void collisionProcess() {

		act_state_ = eActState::IDLE;
		sequence_.immediatelyChange(&Player::seqIdle);
		next_pos_ = pos_;
	}

private:
	// ���^�[���񕜂����� HP
	int regenerating_hp_ = 1;

	// �v���C���[�̍s���V�[�P���X
	tnl::Sequence<Player> sequence_ = tnl::Sequence<Player>(this, &Player::seqIdle);

	// ���݌��Ă������
	eDir_8 looking_dir_ = eDir_8::DOWN;

	// �I�𒆂̃Z���̉摜
	int select_cell_blue_gpc_hdl_ = 0;
	int select_cell_red_gpc_hdl_ = 0;

	// �v���C���[�V�[�P���X
	bool seqIdle(const float delta_time);
	bool seqMove(const float delta_time);
	bool seqAttack(const float delta_time);

	// ====================== �֐��̃v���g�^�C�v�錾 ============================
	bool checkMapDataFromPos(const tnl::Vector3& pos, eMapData map_data);

	// ============================= �C�����C���֐� =============================

	// �w�肵�������� next_pos_�Alooking_dir_�Adir_���Z�b�g
	inline void setNextPosInDir(eDir_8 dir) {

		int index = static_cast<int>(dir);
		if (index < 0 || index >= static_cast<int>(eDir_8::MAX)) {
			return;
		}

		anim_dir_ = ANIM_DIR[index];
		looking_dir_ = dir;
		next_pos_ += DIR_POS[static_cast<int>(dir)];
	}

	// �w�肵���������L����
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