#pragma once
#include "../../base/character_base.h"

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
	void drawEffect(const std::shared_ptr<Camera> camera) override;

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

	// �U��
	//tnl::Vector3 attack_dir_ = { 0, 0, 0 };


	// �I�𒆂̃Z���̉摜
	int select_cell_blue_gpc_hdl_ = 0;
	int select_cell_red_gpc_hdl_ = 0;

public:
	// �s�����J�n����
	void beginAction() override;
	// ���x���A�b�v�ł��邩����
	bool canLevelUp() override;
	// �U���J�n
	void startAttack() override;
	// ���x���A�b�v������
	void startLevelUp() override;

private:
	// �v���C���[�V�[�P���X
	bool seqIdle(const float delta_time);
	bool seqMove(const float delta_time);
	bool seqAttack(const float delta_time);
	bool seqDamage(const float delta_time);
	bool seqLevellUp(const float delta_time);

	// ====================== �֐��̃v���g�^�C�v�錾 ============================
	bool checkMapDataFromPos(const tnl::Vector3& pos, eMapData map_data);
	// �w�肵�������� next_pos_�Alooking_dir_�Adir_���Z�b�g
	void setNextPosInDir(eDir_8 dir);
	// �w�肵�������̃Z�����s���ł��邩����
	bool canActionToCell(eDir_8 dir);

};