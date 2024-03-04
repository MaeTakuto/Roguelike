#pragma once
#include "../base/enemy_base.h"


// =====================================================================================
// ���΂����ڂ���i�G�j�̃N���X
// =====================================================================================
class Pumpkin final : public EnemyBase {
public:
	Pumpkin();
	~Pumpkin();

	void update(float delta_time) override;
	void draw(std::shared_ptr<Camera> camera) override;

private:
	// �G�̍s���V�[�P���X�̊Ǘ�
	tnl::Sequence<Pumpkin> sequence_ = tnl::Sequence<Pumpkin>(this, &Pumpkin::seqIdle);

public:
	// ============= �v���g�^�C�v�錾 =============
	
	// �s�������肷��B
	void decideAction() override;
	// �s�����J�n����
	void beginAction() override;
	// ���g���X�|�[��������
	void spawn(const tnl::Vector3& pos) override;
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

	// ============= �v���g�^�C�v�錾 =============
	// �ڕW�̈ʒu�Ɍ������� 1�}�X�ړ�����
	void moveToTarget();
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