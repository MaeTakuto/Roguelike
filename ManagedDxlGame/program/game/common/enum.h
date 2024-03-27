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

// セルの場所
enum class ePlace {
	WALL = 0,		// 壁
	CORRIDOR,		// 通路
	ROOM,			// 部屋
};

// キャラの行動状態
enum class eActState {
	IDLE,
	MOVE,
	USE_MAGIC,
	ATTACK,
	LEVEL_UP,
	END
};

// 敵の種類
enum class eEnemyType {
	NONE = -1,
	PUMPKIN = 0,
	SKELETON,
	MAX
};

// 攻撃タイプ
enum class eMagicType {
	NONE = 0,		// 通常攻撃
	FIRE,			// 炎攻撃
};

// 二択のコマンド
enum class eTwoSelectCmd : const int {
	YES = 0,
	NO,
	MAX
};

// メインメニューの選択コマンド一覧
enum class eMainMenuCmd {
	MAGIC_SELECT = 0,
	CHECK_CELL,
	CHECK_STATUS,
	CLOSE,
	MAX
};

// 魔法効果の対象
enum class eMagicTarget {
	OWNER = 0,
	OTHER,
	MAX
};

// 魔法一覧
enum class eMagicName {
	HEAL = 0,
	FIRE,
	MAGNETISM,
};
