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
	int target_entrance_id_ = 0;

	// �U������
	float attack_time_ = 0.0f;

public:
	// ================= ���z�֐� =================
	
	// �s�������肷��B
	virtual void decideAction() = 0;
	// �s�����J�n����
	virtual void beginAction() = 0;
	// ���g���X�|�[��������
	virtual void spawn(const tnl::Vector3& pos) = 0;
	// �G���f�X������
	virtual void death() = 0;

protected:
	// ================= ���z�֐� =================

	// �v���C���[��ǐՂ���
	virtual void trackingPlayer() {}
	
	// �e�Z���̈ړ��R�X�g�A�L���ȃZ�������X�V
	virtual void updateCellsCost() = 0;

	// �e�Z���̈ړ��R�X�g���玟�̈ʒu�����߂�
	virtual void decideNextPosFromCellCost() {}

	// �w�肵�������̃Z�����s���ł��邩����
	virtual bool canActionToCell(const tnl::Vector2i& pos) { return false; }
	
	// �w�肵���������U���\��
	// args...���肷�����
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
	const tnl::Vector3& getEntrancePosToNearestPlayer(int area_id) const;

	// �v���C���[�����������ɂ��邩����
	bool isSameRoomToPlayer();

	// ������T��
	void findEntrance();

	// �ړ��R�X�g���ł��������L���� CellCost�̃C���f�b�N�X���擾
	int getMinimunScoreCellIndex();

	// �U���s���ɐ؂�ւ���
	// args...�U������
	void changeToAttackAction(eDir_8 dir);

	// �ړ��s���ɐ؂�ւ���
	void changeToMoveAction();
};
