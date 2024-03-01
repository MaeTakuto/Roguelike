#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "../common/enum.h"
#include "../common/chara_status.h"

class Camera;

const int DIR_MAX = 8;

// �L�����̍s�����
enum class eActState {
	IDLE,
	MOVE,
	ATTACK,
	END
};

// �L�����N�^�[�̃x�[�X�N���X
class Character {
public:
	Character() {};
	virtual ~Character() {};

	virtual void update(float delta_time) = 0;
	virtual void draw(const std::shared_ptr<Camera> camera) = 0;

	// pos_ ���擾�A�Z�b�g
	virtual const tnl::Vector3& getPos() const = 0;
	virtual void setPos(const tnl::Vector3& pos) = 0;
	
	// next_pos_ ���擾
	virtual const tnl::Vector3& getNextPos() const = 0;

	// �������Ă��邩����
	virtual bool isAlive() const = 0;

	// �s����Ԃ��擾
	virtual const eActState& getActState() const = 0;

	// ���O���擾
	virtual const std::string& getName() const = 0;

	// �X�e�[�^�X���擾
	virtual const CharaStatus& getStatus() const = 0;

	// �s�����J�n������
	virtual void beginAction() = 0;

	// �_���[�W���󂯂�
	virtual void takeDamage(int damage) = 0;

	// �o���l���擾
	virtual void addExp(int exp) = 0;

protected:
	// �ړ�������X�s�[�h
	const float MOVE_SPEED = 0.25f;

	// �e�����̕\���摜�̕���
	const eDir_4 ANIM_DIR[DIR_MAX] = { eDir_4::UP, eDir_4::DOWN, eDir_4::LEFT, eDir_4::RIGHT, eDir_4::UP, eDir_4::UP, eDir_4::DOWN, eDir_4::DOWN };

	// �e�����̃x�N�g��
	const tnl::Vector3 DIR_POS[DIR_MAX] = { { 0, -1, 0 }, { 0, 1, 0 }, { -1, 0, 0 }, { 1, 0 , 0 }, { -1, -1, 0 }, { 1, -1, 0 }, { -1, 1, 0 }, { 1, 1, 0 } };

	// �L�����̉摜
	std::vector< std::vector<int> > chara_gpc_hdl_;

	// �摜�f�[�^�i �摜�p�X�A�t���[�����Ȃ� �j
	std::vector< std::vector<tnl::CsvCell> > gpc_hdl_data_;

	// �L�����N�^�[�̖��O
	std::string name_ = "";

	// �X�e�[�^�X�i HP�AATK�Ȃ� �j
	CharaStatus status_ = CharaStatus();

	// ���݂̈ʒu
	tnl::Vector3 pos_ = { 0, 0, 0 };
	// �ړ�������ʒu
	tnl::Vector3 next_pos_ = { 0, 0, 0 };
	tnl::Vector3 attack_dir_ = { 0, 0, 0 };

	// �L�����̃A�j������
	eDir_4 anim_dir_ = eDir_4::DOWN;

	// �s�����
	eActState act_state_ = eActState::IDLE;

	// �������Ă��邩
	bool is_alive_ = true;;
	bool is_collision_ = false;

};