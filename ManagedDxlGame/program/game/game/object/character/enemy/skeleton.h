#pragma once
#include "../../../base/enemy_base.h"

class Projectile;

// =====================================================================================
// �_���W�������ɏo������A"��������"�̃N���X
// =====================================================================================
class Skeleton final : public EnemyBase {
public:
	Skeleton();
	~Skeleton();

	void update(float delta_time) override;
	void draw(const std::shared_ptr<Camera> camera) override;
	void drawEffect(const std::shared_ptr<Camera> camera) override;

private:
	// �G�̍s���V�[�P���X�̊Ǘ�
	tnl::Sequence<Skeleton> sequence_;

	//eDir_8 bone_throw_dir_;
	// ������
	std::shared_ptr<Projectile> bone_;

	// �������̉摜
	int bone_gpc_hdl_;

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
	// ���x�����オ�邪����
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
	// ���𓊂���U�����ł��邩���肵�A�\�ł���� "dir" �ɕ������i�[����
	bool tryCanBoneAttack(eDir_8& dir);
	// �v���C���[�����邩�m�F����B
	bool isPlayerDir(const tnl::Vector3& pos, eDir_8 dir, bool is_loop = false) const;
	// �w�肵���������U���\�����������
	bool canAttackInDir(eDir_8 dir) override;
	// �ʘH�ł̍s������������
	void actionInCorridor() override;
	// �����ł̍s������������
	void actionInRoom() override;

};