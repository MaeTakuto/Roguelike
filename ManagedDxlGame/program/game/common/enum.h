#pragma once

// �ʘH
enum class eDir {
	NONE = -1,
	UP = 0,
	DOWN,
	LEFT,
	RIGHT,
	MAX
};

// �}�b�v�f�[�^
enum class eMapData {
	NONE = -1,
	GROUND = 0,
	WALL,
	PLAYER,
	ENEMY
};

enum class ePlace {
	WALL = 0,
	ROAD,
	ROOM,
};
