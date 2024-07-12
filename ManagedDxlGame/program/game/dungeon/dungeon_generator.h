#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include "../common/enum.h"
#include "../common/struct_data.h"

class Camera;

// =====================================================================================
// ダンジョンの生成、管理するクラス
// =====================================================================================
class DungeonGenerator {
public:
	DungeonGenerator();
	~DungeonGenerator();

	const int AREA_MAX_SIZE = 32;
	const int AREA_MIN_SIZE = 16;
	const int AREA_MAX = 8;					// 最大エリア数
	const int AREA_SPACE = 3;
	const int ROOM_AND_ROAD_SPACE = 2;
	const int MAX_ROOM_ENTRANCE = 3;

	// ======= ダンジョン生成、再生成 ======
	void generateDungeon();

	// =============================
	//        デバッグ用
	// =============================

	// 分割したエリアデータを表示
	void displayAreaNumber(const std::shared_ptr<Camera> camera);
	void debugEntranceData();

	// ゲッター
	inline std::vector< std::vector<DungeonCell> >& getField() { return field_; }
	// inline std::vector< std::vector<int> >& getMapData() { return map_data_; }
	inline std::vector<Area>& getAreas() { return areas_; }

private:
	// int area_number_[FIELD_HEIGHT][FIELD_WIDTH] = { 0 };

	// 地形データ
	std::vector<std::vector<DungeonCell> > field_;
	// std::vector<std::vector<int> > map_data_{ GameManager::FIELD_HEIGHT, std::vector<int>( GameManager::FIELD_WIDTH ) };

	int area_count_ = 0;
	std::vector<Area> areas_;

	// 通路作成用
	int* order_connect_rooms_ = nullptr;				// ルームを繋げる順番
	int order_index_ = 0;								// orderの添え字
	int connect_error_count_ = 0;						// エラーの発見用

	// startからendまでの範囲で乱数生成
	inline int random(int start, int end) { return rand() % (end - start + 1) + start; }

	void areaDataInit();
	void fieldDataInit();
	
	// エリアを分割
	void splitArea(int area_index, int vertical);
	
	// フィールドのエリア番号を設定
	void settingAreaNumber();

	// 部屋を作成
	void createRoom();

	// フィールド作成
	void generateRoom();

	void getNextConnectRoomIndex(int area_index);

	// 部屋同士を繋げる
	void connectRoom(int area_index);

	// 部屋同士を繋げるエリアの番号をセット
	bool checkConnectAreaNumMax(int x, int y, int area_index);

	// 上下、左右の部屋のに通路を作成
	void connectUpAndDownRooms(int up_x, int up_y, int down_x, int down_y);
	void connectLeftAndRightRooms(int left_x, int left_y, int right_x, int right_y);

	// 階段を生成
	void createStair();

	// プレイヤーの生成位置を決める
	void spawnPlayer();

	// 敵の生成位置を決める
	void spawnEnemy();

	// 指定した座標から、指定した地形と一致する周囲の地形を数える
	int countTerrainForDirection(int x, int y, eMapData terrain_data);

};