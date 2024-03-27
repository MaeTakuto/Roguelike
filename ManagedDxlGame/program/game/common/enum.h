#pragma once

// 4����
enum class eDir_4 {
	NONE = -1,
	UP = 0,
	DOWN,
	LEFT,
	RIGHT,
	MAX
};

// 8����
enum class eDir_8 {
	NONE = -1,
	UP = 0,
	DOWN,
	LEFT,
	RIGHT,
	UP_LEFT,
	UP_RIGHT,
	DOWN_LEFT,
	DOWN_RIGHT,
	MAX
};

// �}�b�v�f�[�^
enum class eMapData {
	NONE = -1,		// 
	GROUND = 0,		// �n��
	WALL,			// ��
	STAIR,			// �K�i
	PLAYER,			// �v���C���[
	ENEMY			// �G
};

// �Z���̏ꏊ
enum class ePlace {
	WALL = 0,		// ��
	CORRIDOR,		// �ʘH
	ROOM,			// ����
};

// �L�����̍s�����
enum class eActState {
	IDLE,
	MOVE,
	USE_MAGIC,
	ATTACK,
	LEVEL_UP,
	END
};

// �G�̎��
enum class eEnemyType {
	NONE = -1,
	PUMPKIN = 0,
	SKELETON,
	MAX
};

// �U���^�C�v
enum class eMagicType {
	NONE = 0,		// �ʏ�U��
	FIRE,			// ���U��
};

// ����̃R�}���h
enum class eTwoSelectCmd : const int {
	YES = 0,
	NO,
	MAX
};

// ���C�����j���[�̑I���R�}���h�ꗗ
enum class eMainMenuCmd {
	MAGIC_SELECT = 0,
	CHECK_CELL,
	CHECK_STATUS,
	CLOSE,
	MAX
};

// ���@���ʂ̑Ώ�
enum class eMagicTarget {
	OWNER = 0,
	OTHER,
	MAX
};

// ���@�ꗗ
enum class eMagicName {
	HEAL = 0,
	FIRE,
	MAGNETISM,
};
