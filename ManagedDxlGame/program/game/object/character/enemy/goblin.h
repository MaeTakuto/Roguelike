#pragma once
#include "../../../base/enemy_base.h"


class Goblin : public EnemyBase {
public:
	Goblin();
	~Goblin();

	void update(float delta_time) override;
	void draw(std::shared_ptr<Camera> camera) override;
	void drawEffect(const std::shared_ptr<Camera> camera) override;

private:
	// �G�̍s���V�[�P���X�̊Ǘ�
	tnl::Sequence<Goblin> sequence_ = tnl::Sequence<Goblin>(this, &Goblin::seqIdle);

public:
	// ============= �v���g�^�C�v�錾 =============

	// ���̃��x���܂ł̕K�v�o���l���擾
	int getExpToNextLevel() const override { return 0; }
	// �N���[���𐶐�����
	std::shared_ptr<EnemyBase> createClone() const override;
	// �G�̃��x����ݒ肷��
	void setEnemyLevel(int lv) override;
	// �w�肵���ڕW�ʒu�ɃL�����N�^�[���ړ�������
	void moveToTargetPos(const tnl::Vector3& target_pos) override;
	// ���x�����オ�邩����
	bool canLevelUp() override;
	// �s�������肷��B
	void decideAction() override;
	// �U�����J�n����
	void startAttack() override;
	// �s�����J�n����
	void beginAction() override;
	// ���x���A�b�v�������s��
	void startLevelUp() override;
	// �_���[�W���󂯂�
	void takeDamage(int damage) override;
	// �G���f�X������
	void death() override;
	//// 
	//bool tryUseMagic(int magic_index) override { return false; };

private:
	// =================== �s���V�[�P���X ===================
	// �ҋ@���
	bool seqIdle(const float delta_time);
	// �ړ����
	bool seqMove(const float delta_time);
	// �U�����
	bool seqAttack(const float delta_time);
	// �_���[�W���󂯂�V�[�P���X
	bool seqTakeDamage(const float delta_time);

	// ============= �v���g�^�C�v�錾 =============
	
	// 
	void startAction();

	// �ڕW�̈ʒu�Ɍ������� 1�}�X�ړ�����
	void setNextPosToTarget();
	// �v���C���[��ǐՂ���
	void trackingPlayer() override;
	// �e�Z���̈ړ��R�X�g�A�L���ȃZ�������X�V
	void updateCellsCost() override;
	// �e�Z���̈ړ��R�X�g���玟�̈ʒu�����߂�
	void decideNextPosFromCellCost() override;
	// �w�肵�������̃Z�����s���ł��邩����
	bool canActionToCell(const tnl::Vector2i& pos) override;
	// �w�肵���������U���\��
	bool canAttackInDir(eDir_8 dir) override;
	// �ʘH�ł̍s��
	void actionInCorridor() override;
	// �����ł̍s��
	void actionInRoom() override;
};
