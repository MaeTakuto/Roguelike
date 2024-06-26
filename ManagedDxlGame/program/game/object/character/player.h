#pragma once
#include "../../base/character_base.h"

class Camera;
class Projectile;

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
	// ���^�[���񕜂����� MP
	int regenerating_mp_;

	// �v���C���[�̍s���V�[�P���X
	tnl::Sequence<Player> sequence_;

	// ���͉\������
	bool can_operation_input_;

	// ------------------ ���@�֘A ---------------------------------

	// ���@�r���G�t�F�N�g
	std::shared_ptr<Animation> magic_chanting_effect_;
	// ���@���g�p���Ă��邩����
	bool is_use_magic_;
	// �g�p���閂�@�̃��X�g�ԍ�
	int use_magic_index_;

	// -------------------------------------------------------------

	// �I�𒆂̃Z���̉摜
	int select_cell_blue_gpc_hdl_;
	int select_cell_red_gpc_hdl_;

public:
	// ������͂ł���悤�ɂ��邩�ݒ�
	inline void setOperationInput(bool can_operation_input) { can_operation_input_ = can_operation_input; }
	// �s�����J�n����
	void beginAction() override;
	// �w�肵���ڕW�ʒu�ɃL�����N�^�[���ړ�������
	void moveToTargetPos(const tnl::Vector3& target_pos) override;
	// ���x���A�b�v�ł��邩����
	bool canLevelUp() override;
	// �U���J�n
	void startAttack() override;
	// ���@�̎g�p����������
	void setupMagic();
	// ���@�̎g�p�J�n
	void startMagic();
	// ���x���A�b�v������
	void startLevelUp() override;
	
	// ���@���g���邩����
	bool tryUseMagic(int magic_index);
	
private:
	// ===================== �v���C���[�V�[�P���X =======================
	// �ҋ@�A���̓V�[�P���X
	bool seqIdle(const float delta_time);
	// �ړ��V�[�P���X
	bool seqMove(const float delta_time);
	// �U���V�[�P���X
	bool seqAttack(const float delta_time);
	// ���x���A�b�v�V�[�P���X
	bool seqLevellUp(const float delta_time);
	// ���@�r���V�[�P���X
	bool seqMagicChanting(const float delta_time);
	// ���@�U���V�[�P���X
	bool seqUseMagic(const float delta_time);

	// ====================== �֐��̃v���g�^�C�v�錾 ============================
	bool checkMapDataFromPos(const tnl::Vector3& pos, eMapData map_data);
	// �w�肵�������� next_pos_�Alooking_dir_�Adir_���Z�b�g
	void setNextPosInDir(eDir_8 dir);
	// �w�肵�������̃Z�����s���ł��邩����
	bool canActionToCell(eDir_8 dir);

};