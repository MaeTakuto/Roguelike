#pragma once
#include "character_base.h"

class ScenePlay;

// =====================================================================================
// �ړ��}�X�̃R�X�g�A�ʒu���Ǘ�����f�[�^
// =====================================================================================
struct CellCost {
	// �Z���̈ʒu
	tnl::Vector2i pos_ = { 0, 0 };
	// �ړ��R�X�g
	int cost_ = 0;
	// ���g�̃Z�����L��������
	bool is_enable_ = true;

};

// =====================================================================================
// �G�̃x�[�X�ƂȂ�N���X
// =====================================================================================
class EnemyBase : public Character {
public:
	EnemyBase();
	virtual ~EnemyBase();

	// �A�b�v�f�[�g�֐�
	virtual void update(float delta_time) = 0;
	// �`��֐�
	virtual void draw(std::shared_ptr<Camera> camera) = 0;
	// �G�t�F�N�g�`��֐�
	virtual void drawEffect(const std::shared_ptr<Camera> camera) = 0;

protected:
	// �s���G���[�̍ő�J�E���g�i �s���G���[���̑Ή����s�� �j
	const int ACTION_ERROR_MAX = 3;
	const float ATTACK_TIME_MAX = 0.5f;

	// ScenePlay�̃C���X�^���X�i �}�b�v�f�[�^�̎擾��U�������p�Ɏg�����A�݌v�~�X�ɂ�肠�܂�ǂ��Ȃ��Ǝv���Ă��܂��B �j
	std::weak_ptr<ScenePlay> scene_play_;

	// �ڕW���������
	tnl::Vector3 target_pos_;
	// ���� 8�����̃Z���̈ړ��R�X�g���v�Z����p
	CellCost cells_[static_cast<int>(eDir_8::MAX)];

	// �ڕW�ʒu��������������
	bool is_find_target_ = false;

	// �v���C���[��������������
	bool is_find_player_ = false;

	// �s���o���Ȃ�������
	int action_error_ = 0;

	// �U������
	float attack_time_ = 0.0f;

public:
	// ================= ���z�֐� =================
	
	// �N���[���𐶐�����
	virtual std::shared_ptr<EnemyBase> createClone() const = 0;
	// �G�̃��x����ݒ肷��
	virtual void setEnemyLevel(int lv) = 0;
	// ���x�����オ�邪����
	virtual bool canLevelUp() = 0;
	// �s�������肷��B
	virtual void decideAction() = 0;
	// �s�����J�n����
	virtual void beginAction() = 0;
	// �U���J�n
	virtual void startAttack() = 0;
	// ���x���A�b�v�������s��
	virtual void startLevelUp() = 0;
	// �G���f�X������
	virtual void death() = 0;

protected:
	// ================= ���z�֐� =================

	//----------------------------------------------------------------
	// �G�̃��x���ɂ���Ăł���s����ς��悤�Ǝv���Ă��܂����B
	// ��E�E�E���x��1�F�ߋ����U���̂�, ���x���Q�F�������U�����ł���B
	
	//// ���x���P�����X�^�[�̍s�������߂�
	//virtual void decideActionForLv_1() = 0;
	//
	//// ���x���Q�����X�^�[�̍s�������߂�
	//virtual void decideActionForLv_2() = 0;
	//----------------------------------------------------------------

	// �v���C���[��ǐՂ���
	virtual void trackingPlayer() {}
	
	// �e�Z���̈ړ��R�X�g�A�L���ȃZ�������X�V
	virtual void updateCellsCost() = 0;

	// �e�Z���̈ړ��R�X�g���玟�̈ʒu�����߂�
	virtual void decideNextPosFromCellCost() {}

	// �w�肵�������̃Z�����s���ł��邩����
	virtual bool canActionToCell(const tnl::Vector2i& pos) { return false; }
	
	// �w�肵���������U���\��
	// arg...���肷�����
	virtual bool canAttackInDir(eDir_8 dir) { return false; }
	
	// �ʘH�ł̍s��
	virtual void actionInCorridor() {}
	
	// �����ł̍s��
	virtual void actionInRoom() {}
	// ============= �v���g�^�C�v�錾 =============

	// ���͂̏㉺���E�̃}�X����A�w�肵�� "eMapData" �����݂��������Ԃ�
	std::vector<eDir_4> getMapDataDir_4(eMapData map_data);

	// ���� 8�}�X����v���C���[�����邩�m�F���A���̕�����Ԃ��B���Ȃ���΁A"eDir_8::NONE" ���Ԃ��Ă���
	eDir_8 findPlayerDir_8();

	// �v���C���[�ɂ����Ƃ��߂������̈ʒu��Ԃ�
	tnl::Vector3 getEntrancePosToNearestPlayer(int area_id) const;

	// �v���C���[�����������ɂ��邩����
	bool isSameRoomToPlayer() const;

	// ���݂̕����̓����������_���Ɏ擾����
	tnl::Vector3 getRandomEntranceToNowRoom() const;

	// �ړ��R�X�g���ł��������L���� CellCost�̃C���f�b�N�X���擾
	int getMinimunScoreCellIndex();

	// �U���s���ɐ؂�ւ���
	// args...�U������
	void changeToAttackAction(eDir_8 dir);

	// �ړ��s���ɐ؂�ւ���
	void changeToMoveAction();
};

