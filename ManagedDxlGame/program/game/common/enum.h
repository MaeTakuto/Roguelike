#pragma once

// 4方向
enum class eDir_4 {
	NONE = -1,
	UP = 0,
	DOWN,
	LEFT,
	RIGHT,
	MAX
};

// 8方向
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

// マップデータ
enum class eMapData {
	NONE = -1,		// 
	GROUND = 0,		// 地面
	WALL,			// 壁
	STAIR,			// 階段
	PLAYER,			// プレイヤー
	ENEMY			// 敵
};

enum class ePlace {
	WALL = 0,
	ROAD,
	ROOM,
};
