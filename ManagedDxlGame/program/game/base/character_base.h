#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "../common/enum.h"
#include "../common/chara_status.h"
#include "../common/animation.h"

class Camera;

// const int DIR_MAX = 8;

// �L�����̍s�����
enum class eActState {
	IDLE,
	MOVE,
	ATTACK,
	END
};

// =====================================================================================
// �L�����N�^�[�̃x�[�X�N���X
// =====================================================================================
class Character {
public:
	Character();
	virtual ~Character() {};

	virtual void update(float delta_time) = 0;
	virtual void draw(const std::shared_ptr<Camera> camera) = 0;
	virtual void drawEffect(const std::shared_ptr<Camera> camera) = 0;

protected:
	// �L�����̉摜���A�`�b�v�T�C�Y�i �L�����`�b�v�̐؂��� �j
	const int CHARA_GPC_X_NUM = 4;
	const int CHARA_GPC_Y_NUM = 1;
	const int CHARA_GPC_MAX_NUM = CHARA_GPC_X_NUM * CHARA_GPC_Y_NUM;

	// �ړ�������X�s�[�h
	const float MOVE_SPEED = 0.25f;

	// �e�����̕\���摜�̕���
	const eDir_4 ANIM_DIR[static_cast<int>(eDir_8::MAX)] 
		= { eDir_4::UP, eDir_4::DOWN, eDir_4::LEFT, eDir_4::RIGHT, eDir_4::UP, eDir_4::UP, eDir_4::DOWN, eDir_4::DOWN };

	// �e�����̃x�N�g��
	const tnl::Vector3 DIR_POS[static_cast<int>(eDir_8::MAX)] 
		= { { 0, -1, 0 }, { 0, 1, 0 }, { -1, 0, 0 }, { 1, 0 , 0 }, { -1, -1, 0 }, { 1, -1, 0 }, { -1, 1, 0 }, { 1, 1, 0 } };

	// �t����
	const eDir_8 REVERCE_DIRECTION[static_cast<int>(eDir_8::MAX)]
		= { eDir_8::DOWN, eDir_8::UP, eDir_8::RIGHT, eDir_8::LEFT, eDir_8::DOWN_RIGHT, eDir_8::DOWN_LEFT, eDir_8::UP_RIGHT, eDir_8::UP_LEFT };

	// �L�����̉摜
	std::vector< std::vector<int> > chara_gpc_hdl_;

	// �L�����N�^�[�̖��O
	std::string name_;

	// �X�e�[�^�X�i HP�AATK�Ȃ� �j
	CharaStatus status_;

	// ---------------- �U���֘A ----------------
	// �U���Ώۂ̃L�����N�^�[�N���X
	std::shared_ptr<Character> atk_target_;
	// �U���G�t�F�N�g�̃A�j���[�V����
	Animation atk_effect_;
	// �U���G�t�F�N�g�摜
	std::vector<int> atk_effect_gpc_hdls_;

	// ---------------- �ʒu�֘A ----------------
	// ���݂̈ʒu
	tnl::Vector3 pos_ = { 0, 0, 0 };
	// �ړ�������ʒu
	tnl::Vector3 next_pos_ = { 0, 0, 0 };
	// ------------------------------------------

	// �L�����̃A�j������
	eDir_4 anim_dir_ = eDir_4::DOWN;

	// �s�����
	eActState act_state_ = eActState::IDLE;

	// �������Ă��邩����
	bool is_alive_;
	
	// �_���[�W���󂯂Ă��邩����
	bool is_damaged_;

public:
	// ================= �Q�b�^�[�A�Z�b�^�[ =================
	// �U���Ώۂ��擾 �Ώۂ����Ȃ��ꍇ�A"nullptr" ��Ԃ��B
	inline std::shared_ptr<Character> getAttackTarget() { return atk_target_; }
	// ���݂̈ʒu���擾
	inline const tnl::Vector3& getPos() const { return pos_; }
	// �ʒu���Z�b�g
	inline void setPos(const tnl::Vector3& pos) {
		pos_ = pos;
		next_pos_ = pos_;
	}
	// �ړ���̍��W���擾
	inline const tnl::Vector3& getNextPos() const { return next_pos_; }
	// �������Ă��邩����
	inline bool isAlive() const { return is_alive_; }
	// �s����Ԃ��擾
	inline eActState getActState() { return act_state_; }
	// ���O���擾
	inline const std::string& getName() const { return name_; };
	// �X�e�[�^�X�N���X���擾
	inline const CharaStatus& getStatus() const { return status_; }

	// ================= �֐��̃v���g�^�C�v =================
	// �o���l��ǉ�����
	inline void addExp(int exp) { status_.addExp(exp); }
	// �_���[�W���󂯂�
	void takeDamage(int damage);

	// ================= ���z�֐� =================
	// �s�����J�n������
	virtual void beginAction() = 0;
	// ���x���A�b�v�������s��
	virtual void executeLevelUpProcess() {};

protected:
	// �w�肵���ʒu���t�B�[���h�̒�������
	bool isInField(const tnl::Vector3& pos);

};