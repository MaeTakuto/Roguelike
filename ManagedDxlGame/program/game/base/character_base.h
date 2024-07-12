#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "../common/enum.h"
#include "../common/chara_status.h"

class Camera;
class MagicBase;
class Animation;

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

	// �V�[�P���X�ɗ��܂鎞��
	const float SEQUENCE_WAIT_TIME = 0.75f;

	// �ړ�������X�s�[�h
	const float MOVE_SPEED = 7.5f;

	// �L�����N�^�[�A�j���[�V�����̐؂�ւ��Ԋu
	const float CHARA_ANIM_INTERVAL = 0.25f;

	// �_���[�W���o�؂�ւ��Ԋu
	const float DAMAGE_EFFECT_CHANGE_INTERVAL = 0.05f;

	// �e�����̕\���摜�̕���
	const eDir_4 ANIM_DIR[static_cast<int>(eDir_8::MAX)] 
		= { eDir_4::UP, eDir_4::DOWN, eDir_4::LEFT, eDir_4::RIGHT, eDir_4::UP, eDir_4::UP, eDir_4::DOWN, eDir_4::DOWN };

	// �t����
	const eDir_8 REVERCE_DIRECTION[static_cast<int>(eDir_8::MAX)]
		= { eDir_8::DOWN, eDir_8::UP, eDir_8::RIGHT, eDir_8::LEFT, eDir_8::DOWN_RIGHT, eDir_8::DOWN_LEFT, eDir_8::UP_RIGHT, eDir_8::UP_LEFT };

	// ---------------- �L�����N�^�[�f�[�^�֘A ---------------------------------------

	// �L�����̉摜
	std::vector< std::vector<int> > chara_gpc_hdl_;
	// �L�����N�^�[�A�j���[�V����
	std::shared_ptr<Animation> chara_animation_;
	// �L�����N�^�[�̖��O
	std::string name_;
	// �X�e�[�^�X�i HP�AATK�Ȃ� �j
	CharaStatus status_;
	// ���@�ꗗ
	std::vector<std::shared_ptr<MagicBase>> magic_list_;

	// ���݂̈ʒu
	tnl::Vector3 pos_;
	// �ړ�������ʒu
	tnl::Vector3 next_pos_;
	// ���݌��Ă������
	eDir_8 looking_dir_;
	// �L�����̃A�j������
	eDir_4 anim_dir_;
	// �s�����
	eActState act_state_;
	// �_���[�W���󂯂Ă��邩����
	bool is_take_damage_;
	// 
	bool is_drawing_;
	// �������Ă��邩����
	bool is_alive_;

	// ---------------- �U���֘A ----------------------------------------------------
	// �U���Ώۂ̃L�����N�^�[�N���X
	std::shared_ptr<Character> atk_target_;
	// �U���G�t�F�N�g�̃A�j���[�V����
	std::shared_ptr<Animation> atk_effect_;
	// �U���G�t�F�N�g�摜
	std::vector<int> atk_effect_gpc_hdls_;

	// �_���[�W���o���s������
	int damage_production_count_;
	// �_���[�W���o����̌o�ߎ���
	float damage_production_elapsed_;

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

	// �_���[�W���󂯂Ă��邩����
	inline bool isTakeDamage() const { return is_take_damage_; };

	// �������Ă��邩����
	inline bool isAlive() const { return is_alive_; }

	// �s����Ԃ��擾
	inline eActState getActState() { return act_state_; }
	
	// ���O���擾
	inline const std::string& getName() const { return name_; };
	
	// �X�e�[�^�X�N���X���擾
	inline const CharaStatus& getStatus() { return status_; }
	
	// ���@�ꗗ���擾
	const std::vector<std::shared_ptr<MagicBase>>& getMagicList() const;
	
	// �L�����N�^�[�̌��Ă���������擾
	inline eDir_8 getLookingDir() { return looking_dir_; }

	// �o���l��ǉ�����
	inline void addExp(int exp) { status_.addExp(exp); }
	// HP���񕜂���
	inline void healHP(int amount) { status_.healHP(amount); }

	// ================= �֐��̃v���g�^�C�v =================

	// �w�肵�����@�����邩�m�F����
	bool checkMagicList(std::shared_ptr<MagicBase> magic);
	// ���@��ǉ�����
	void addMagic(std::shared_ptr<MagicBase> magic);

	// ================= ���z�֐� =================

	// ���̃��x���܂ł̕K�v�o���l���擾
	virtual int getExpToNextLevel() const = 0;
	// �w�肵���ڕW�ʒu�ɃL�����N�^�[���ړ�������
	virtual void moveToTargetPos(const tnl::Vector3& target_pos) = 0;
	// ���x�����オ�邪����
	virtual bool canLevelUp() = 0;
	// �s�����J�n������
	virtual void beginAction() = 0;
	// �U���J�n
	virtual void startAttack() = 0;
	// ���x���A�b�v�������s��
	virtual void startLevelUp() = 0;
	// �_���[�W���󂯂�
	virtual void takeDamage(int damage) = 0;

protected:
	// �w�肵���ʒu���t�B�[���h�̒�������
	bool isInField(const tnl::Vector3& pos);

};