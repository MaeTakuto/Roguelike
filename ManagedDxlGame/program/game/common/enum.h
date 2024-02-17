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

enum class ePlace {
	WALL = 0,
	ROAD,
	ROOM,
};
