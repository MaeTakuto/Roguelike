#include "../../dxlib_ext/dxlib_ext.h"
#include "../manager/gm_manager.h"
#include "dungeon_generator.h"
#include "../manager/enemy_manager.h"
#include "../common/camera.h"


// =====================================================================================
// コンストラクタ
// =====================================================================================
DungeonGenerator::DungeonGenerator() {

	areas_.resize(AREA_MAX);

	order_connect_rooms_ = new int[AREA_MAX];

	field_.resize(GameManager::FIELD_HEIGHT);

	for (int i = 0; i < field_.size(); ++i) {
		field_[i].resize(GameManager::FIELD_WIDTH);
	}

	for (int i = 0; i < AREA_MAX; i++) {
		order_connect_rooms_[i] = 0;

		// エリア内の部屋の変数の初期化
		areas_[i].room.entrance_count = 0;
	}

}

// =====================================================================================
// 
// =====================================================================================
DungeonGenerator::~DungeonGenerator() {

	tnl::DebugTrace("DungeonManagerのデストラクタが実行されました。\n");

	delete[] order_connect_rooms_;
}

// =====================================================================================
// ダンジョン生成
// =====================================================================================
void DungeonGenerator::generateDungeon() {

	areaDataInit();
	fieldDataInit();

	// エリアを分割
	splitArea(0, rand() % 2);
	if (areas_[0].area.width >= AREA_MAX_SIZE && areas_[0].area.height >= AREA_MAX_SIZE) { splitArea(0, rand() % 2); }

	settingAreaNumber();

	for (int i = 0; i < area_count_; i++) {
		tnl::DebugTrace("----- index = %d ------\n", i);
		tnl::DebugTrace("x = %d, y = %d, width = %d, height = %d\n", areas_[i].area.x, areas_[i].area.y, areas_[i].area.width, areas_[i].area.height);
	}

	// 部屋を作成
	createRoom();

	// 部屋をマップ上に生成
	generateRoom();

	order_connect_rooms_[order_index_] = rand() % area_count_;
	areas_[ order_connect_rooms_[order_index_] ].is_connect = true;

	// 通路を作成
	while (order_index_ + 1 < area_count_) {
		connectRoom(order_connect_rooms_[order_index_]);
		tnl::DebugTrace("order_index + 1 = %d\n", order_index_ + 1);
	}

	// 階段を生成
	createStair();

	// プレイヤーの生成位置を決める
	spawnPlayer();

	// 敵の生成位置を決める
	spawnEnemy();

	debugEntranceData();

}

// =====================================================================================
// エントランスの位置と部屋のIDをデバッグで表示する
// =====================================================================================
void DungeonGenerator::debugEntranceData() {

	for (int i = 0; i < areas_.size(); i++) {
		tnl::DebugTrace("========== area = %d ==========\n", i);
		for (int j = 0; j < areas_[i].room.entrance.size(); j++) {
			tnl::DebugTrace("entrance[%d].pos = %c\n", j, areas_[i].room.entrance[j].pos.toString().c_str());
			tnl::DebugTrace("entrance[%d].id = %d\n", j, areas_[i].room.entrance[j].id);
		}
		tnl::DebugTrace("\n");
	}
}

// =====================================================================================
// 分割したエリアデータを表示
// =====================================================================================
void DungeonGenerator::displayAreaNumber(const std::shared_ptr<Camera> camera) {

	SetFontSize(DEFAULT_FONT_SIZE);

	for (int y = 0; y < GameManager::FIELD_HEIGHT; y++) {
		for (int x = 0; x < GameManager::FIELD_WIDTH; x++) {
			tnl::Vector3 draw_pos = tnl::Vector3{ static_cast<float>(x * 20), static_cast<float>(y * 20), 0 } - camera->getPos();
			DrawStringEx(draw_pos.x, draw_pos.y, -1, "%d", field_[y][x].area_id);
		}
	}
}

// =====================================================================================
// エリアの初期化
// =====================================================================================
void DungeonGenerator::areaDataInit() {

	for (int i = 0; i < AREA_MAX; i++) {
		areas_[i].area.x = 0;
		areas_[i].area.y = 0;
		areas_[i].area.width  = 0;
		areas_[i].area.height = 0;

		areas_[i].room.x = 0;
		areas_[i].room.y = 0;
		areas_[i].room.width = 0;
		areas_[i].room.height = 0;
		areas_[i].room.entrance_count = 0;

		areas_[i].connect_area_index = -1;
		/*areas_[i].room.connect_area_dir = eDir::NONE;*/
		areas_[i].is_connect = false;

		order_connect_rooms_[i] = 0;

		areas_[i].room.entrance.clear();
	}

	area_count_ = 1;

	areas_[0].area.x = 0;
	areas_[0].area.y = 0;
	areas_[0].area.width = GameManager::FIELD_WIDTH;
	areas_[0].area.height = GameManager::FIELD_HEIGHT;
	
	order_index_ = 0;
	connect_error_count_ = 0;
}

// =====================================================================================
// フィールドデータの初期化
// =====================================================================================
void DungeonGenerator::fieldDataInit() {

	for (int y = 0; y < GameManager::FIELD_HEIGHT; y++) {
		for (int x = 0; x < GameManager::FIELD_WIDTH; x++) {
			field_[y][x].terrain_data = eMapData::WALL;
			field_[y][x].map_data = eMapData::WALL;
			field_[y][x].place = ePlace::WALL;
			field_[y][x].area_id = 0;
		}
	}
}

// =====================================================================================
// エリアをランダムに分割
// =====================================================================================
void DungeonGenerator::splitArea(int area_index, int vertical) {

	if (area_count_ >= AREA_MAX) return;

	int new_area_index = area_count_;
	int width = areas_[area_index].area.width;
	int height = areas_[area_index].area.height;

	// 横を分割
	if (vertical) {

		if (areas_[area_index].area.height / 2 < AREA_MIN_SIZE) return;

		areas_[area_index].area.height /= 2;

		areas_[new_area_index].area.x = areas_[area_index].area.x;
		areas_[new_area_index].area.y = areas_[area_index].area.y + areas_[area_index].area.height;
		areas_[new_area_index].area.width = areas_[area_index].area.width;
		areas_[new_area_index].area.height = height - areas_[area_index].area.height;

	}
	else {
		if (areas_[area_index].area.width / 2 < AREA_MIN_SIZE) return;

		areas_[area_index].area.width /= 2;

		areas_[new_area_index].area.x = areas_[area_index].area.x +  areas_[area_index].area.width;
		areas_[new_area_index].area.y = areas_[area_index].area.y;
		areas_[new_area_index].area.width = width - areas_[area_index].area.width;
		areas_[new_area_index].area.height = areas_[area_index].area.height;

	}

	area_count_++;
	splitArea(area_index, rand() % 2);
	splitArea(new_area_index, rand() % 2);

	if (areas_[area_index].area.width > AREA_MAX_SIZE) { splitArea(area_index, 0); }
	if (areas_[new_area_index].area.width > AREA_MAX_SIZE) { splitArea(new_area_index, 0); }
}

// =====================================================================================
// エリア番号をエリアデータに設定
// =====================================================================================
void DungeonGenerator::settingAreaNumber() {

	tnl::DebugTrace("area_count = %d\n", area_count_);

	// 分割したエリアデータを表示
	for (int i = 0; i < area_count_; i++) {

		for (int y = areas_[i].area.y; y < areas_[i].area.y + areas_[i].area.height; y++) {
			for (int x = areas_[i].area.x; x < areas_[i].area.x + areas_[i].area.width; x++) {
				field_[y][x].area_id = i;
			}
		}
	}
}

// =====================================================================================
// 部屋を作成
// =====================================================================================
void DungeonGenerator::createRoom() {

	for (int area_index = 0; area_index < area_count_; area_index++) {

		int rand_x = rand() % (AREA_SPACE);
		int rand_y = rand() % (AREA_SPACE);
		int rand_width = rand() % (AREA_SPACE) + rand_x;
		int rand_height = rand() % (AREA_SPACE) + rand_y;

		areas_[area_index].room.x = areas_[area_index].area.x + AREA_SPACE + rand_x;
		areas_[area_index].room.y = areas_[area_index].area.y + AREA_SPACE + rand_y;
		areas_[area_index].room.width = areas_[area_index].area.width - ( AREA_SPACE * 2 ) - rand_width;
		areas_[area_index].room.height = areas_[area_index].area.height - ( AREA_SPACE * 2 ) - rand_height;
	}

}

// =====================================================================================
// 
// =====================================================================================
void DungeonGenerator::connectRoom(int area_index) {

	// 隣のエリアインデックス
	int neighbor_index;

	tnl::DebugTrace("========== connectRoom(area_index = %d) ===========\n", area_index);

	getNextConnectRoomIndex(area_index);

	neighbor_index = areas_[area_index].connect_area_index;

	// 次のエリアが見つかった場合
	if (areas_[area_index].connect_area_index != -1) {

		// 次に部屋を繋げるエリア番号のセット
		areas_[neighbor_index].is_connect = true;
		order_connect_rooms_[order_index_ + 1] = neighbor_index;
		order_index_++;

		// ============================ 通路を作成 ===============================

		// d1：移動前のドアの座標、d2：移動後のドアの座標
		int dx1, dy1, dx2, dy2;

		int index = order_connect_rooms_[order_index_ - 1 - connect_error_count_];
		tnl::DebugTrace("order[%d] = %d\n", order_index_, order_connect_rooms_[order_index_]);
		tnl::DebugTrace("d1 index = %d\n", index);

		// 各部屋から通路を伸ばし
		// 
		switch (areas_[area_index].connect_area_dir) {
		case eDir_4::UP:
			// 両ルームの適当な座標指定
			index = order_connect_rooms_[order_index_ - 1 - connect_error_count_];

			dy1 = areas_[index].room.y;

			do {
				dx1 = random(areas_[index].room.x + ROOM_AND_ROAD_SPACE, areas_[index].room.x + areas_[index].room.width - 1 - ROOM_AND_ROAD_SPACE);
			} while (countTerrainForDirection(dx1, dy1 - 1, eMapData::GROUND) >= 2);

			// 部屋の入口の座標を設定
			areas_[index].room.entrance.emplace_back();
			areas_[index].room.entrance[areas_[index].room.entrance_count].pos = tnl::Vector3(static_cast<float>(dx1), static_cast<float>(dy1 - 1), 0);
			areas_[index].room.entrance[areas_[index].room.entrance_count].id = areas_[index].room.entrance_count;
			areas_[index].room.entrance_count++;

			// 繋げる側の部屋の座標を決める
			index = order_connect_rooms_[order_index_];
			tnl::DebugTrace("d2 index = %d\n", index);

			dy2 = areas_[index].room.y + areas_[index].room.height - 2;

			do {
				dx2 = random(areas_[index].room.x + ROOM_AND_ROAD_SPACE, areas_[index].room.x + areas_[index].room.width - 1 - ROOM_AND_ROAD_SPACE);
			} while (countTerrainForDirection(dx2, areas_[index].room.y + areas_[index].room.height, eMapData::GROUND) >= 2);

			tnl::DebugTrace("dx1 = %d, dy1 = %d, dx2 = %d, dy2 = %d\n", dx1, dy1, dx2, dy2);
			tnl::DebugTrace("上通路作成\n");

			// 部屋の入口の座標を設定
			areas_[index].room.entrance.emplace_back();
			areas_[index].room.entrance[areas_[index].room.entrance_count].pos = tnl::Vector3(static_cast<float>(dx2), static_cast<float>(areas_[index].room.y + areas_[index].room.height), 0);
			areas_[index].room.entrance[areas_[index].room.entrance_count].id = areas_[index].room.entrance_count;

			areas_[index].room.entrance_count++;

			connectUpAndDownRooms(dx2, dy2, dx1, dy1);
			break;

		case eDir_4::DOWN:
			// 両ルームの適当な座標指定
			index = order_connect_rooms_[order_index_ - 1 - connect_error_count_];
			dx1 = random(areas_[index].room.x + ROOM_AND_ROAD_SPACE, areas_[index].room.x + areas_[index].room.width - 1 - ROOM_AND_ROAD_SPACE);
			dy1 = areas_[index].room.y + areas_[index].room.height - 1;

			do {
				dx1 = random(areas_[index].room.x + ROOM_AND_ROAD_SPACE, areas_[index].room.x + areas_[index].room.width - 1 - ROOM_AND_ROAD_SPACE);
			} while (countTerrainForDirection(dx1, areas_[index].room.y + areas_[index].room.height, eMapData::GROUND) >= 2);

			// 部屋の入口の座標を設定
			areas_[index].room.entrance.emplace_back();
			areas_[index].room.entrance[areas_[index].room.entrance_count].pos = tnl::Vector3(static_cast<float>(dx1), static_cast<float>(areas_[index].room.y + areas_[index].room.height), 0);
			areas_[index].room.entrance[areas_[index].room.entrance_count].id = areas_[index].room.entrance_count;

			areas_[index].room.entrance_count++;

			// 繋げる側の部屋の座標を決める
			index = order_connect_rooms_[order_index_];
			tnl::DebugTrace("d2 index = %d\n", index);
			
			//dx2 = random(areas_[index].room.x + ROOM_AND_ROAD_SPACE, areas_[index].room.x + areas_[index].room.width - 1 - ROOM_AND_ROAD_SPACE);
			dy2 = areas_[index].room.y;

			do {
				dx2 = random(areas_[index].room.x + ROOM_AND_ROAD_SPACE, areas_[index].room.x + areas_[index].room.width - 1 - ROOM_AND_ROAD_SPACE);
			} while (countTerrainForDirection(dx2, dy2 - 1, eMapData::GROUND) >= 2);

			tnl::DebugTrace("dx1 = %d, dy1 = %d, dx2 = %d, dy2 = %d\n", dx1, dy1, dx2, dy2);
			tnl::DebugTrace("下通路作成\n");

			// 部屋の入口を設定
			areas_[index].room.entrance.emplace_back();
			areas_[index].room.entrance[areas_[index].room.entrance_count].pos = (tnl::Vector3(static_cast<float>(dx2), static_cast<float>(dy2 - 1), 0));
			areas_[index].room.entrance[areas_[index].room.entrance_count].id = areas_[index].room.entrance_count;
			areas_[index].room.entrance_count++;
			
			connectUpAndDownRooms(dx1, dy1, dx2, dy2);
			break;

		case eDir_4::LEFT:
			// 両ルームの適当な座標指定
			index = order_connect_rooms_[order_index_ - 1 - connect_error_count_];
			dx1 = areas_[index].room.x;
			
			do {
				dy1 = random(areas_[index].room.y + ROOM_AND_ROAD_SPACE, areas_[index].room.y + areas_[index].room.height - 1 - ROOM_AND_ROAD_SPACE);
			} while (countTerrainForDirection(dx1 - 1, dy1, eMapData::GROUND) >= 2);

			// 部屋の入口を設定
			areas_[index].room.entrance.emplace_back();
			areas_[index].room.entrance[areas_[index].room.entrance_count].pos = ( tnl::Vector3(static_cast<float>(dx1 - 1), static_cast<float>(dy1), 0) );
			areas_[index].room.entrance[areas_[index].room.entrance_count].id = areas_[index].room.entrance_count;
			areas_[index].room.entrance_count++;

			// 繋げる側の部屋の座標を決める
			index = order_connect_rooms_[order_index_];
			tnl::DebugTrace("d2 index = %d\n", index);

			dx2 = areas_[index].room.x + areas_[index].room.width - 2;

			do {
				dy2 = random(areas_[index].room.y + ROOM_AND_ROAD_SPACE, areas_[index].room.y + areas_[index].room.height - 1 - ROOM_AND_ROAD_SPACE);
			} while (countTerrainForDirection(areas_[index].room.x + areas_[index].room.width, dy2, eMapData::GROUND) >= 2);

			tnl::DebugTrace("dx1 = %d, dy1 = %d, dx2 = %d, dy2 = %d\n", dx1, dy1, dx2, dy2);
			tnl::DebugTrace("左通路作成\n");
			
			// 部屋の入口の座標を設定
			areas_[index].room.entrance.emplace_back();
			areas_[index].room.entrance[areas_[index].room.entrance_count].pos = tnl::Vector3(static_cast<float>(areas_[index].room.x + areas_[index].room.width), static_cast<float>(dy2), 0);
			areas_[index].room.entrance[areas_[index].room.entrance_count].id = areas_[index].room.entrance_count;

			areas_[index].room.entrance_count++;

			connectLeftAndRightRooms(dx2, dy2, dx1, dy1);
			break;

		case eDir_4::RIGHT:
			// 両ルームの適当な座標指定
			index = order_connect_rooms_[order_index_ - 1 - connect_error_count_];

			dx1 = areas_[index].room.x + areas_[index].room.width - 2;

			do {
				dy1 = random(areas_[index].room.y + ROOM_AND_ROAD_SPACE, areas_[index].room.y + areas_[index].room.height - 1 - ROOM_AND_ROAD_SPACE);
			} while (countTerrainForDirection(areas_[index].room.x + areas_[index].room.width, dy1, eMapData::GROUND) >= 2);

			// 部屋の入口の座標を設定
			areas_[index].room.entrance.emplace_back();
			areas_[index].room.entrance[areas_[index].room.entrance_count].pos = tnl::Vector3(static_cast<float>(areas_[index].room.x + areas_[index].room.width), static_cast<float>(dy1), 0);
			areas_[index].room.entrance[areas_[index].room.entrance_count].id = areas_[index].room.entrance_count;
			areas_[index].room.entrance_count++;

			// 繋げる側の部屋の座標を決める
			index = order_connect_rooms_[order_index_];
			tnl::DebugTrace("d2 index = %d\n", index);
			
			dx2 = areas_[index].room.x;

			do {
				dy2 = random(areas_[index].room.y + ROOM_AND_ROAD_SPACE, areas_[index].room.y + areas_[index].room.height - 1 - ROOM_AND_ROAD_SPACE);
			} while (countTerrainForDirection(dx2 - 1, dy2, eMapData::GROUND) >= 2);

			tnl::DebugTrace("dx1 = %d, dy1 = %d, dx2 = %d, dy2 = %d\n", dx1, dy1, dx2, dy2);
			tnl::DebugTrace("右通路作成\n");

			// 部屋の入口の座標を設定
			areas_[index].room.entrance.emplace_back();
			areas_[index].room.entrance[areas_[index].room.entrance_count].pos = tnl::Vector3(static_cast<float>(dx2 - 1), static_cast<float>(dy2), 0);
			areas_[index].room.entrance[areas_[index].room.entrance_count].id = areas_[index].room.entrance_count;

			areas_[index].room.entrance_count++;
			
			connectLeftAndRightRooms(dx1, dy1, dx2, dy2);
			break;
		}

		connect_error_count_ = 0;
	}
	// 見つかった場合
	else {
		connect_error_count_++;
		tnl::DebugTrace("connect_error_：%d\n", connect_error_count_);
		tnl::DebugTrace("order_index_：%d\n", order_index_);

		if (order_index_ < connect_error_count_) {
			order_index_ = area_count_;
			return;
		}

		int prev_index = order_connect_rooms_[order_index_ - connect_error_count_];
		areas_[prev_index].connect_area_index = -1;

		connectRoom(prev_index);
	}
}

// =====================================================================================
// 隣接するエリアの番号を比較し、
// エリア番号が一番大きいエリアの方向に通路を繋げる設定をする。
// =====================================================================================
void DungeonGenerator::getNextConnectRoomIndex(int area_index) {

	int x, y;

	// forループ終了の変数
	int ex, ey;

	if (areas_[area_index].area.y > 0) {

		tnl::DebugTrace("上確認\n");

		y = areas_[area_index].area.y - 1;
		ex = areas_[area_index].area.x + areas_[area_index].area.width;

		for (x = areas_[area_index].area.x; x < ex; x += areas_[field_[y][x].area_id].area.width) {

			// すでにエリアの通路が確認されていた場合、次のエリアまで跳ぶ
			if (areas_[field_[y][x].area_id].is_connect) continue;

			if (checkConnectAreaNumMax(x, y, area_index)) areas_[area_index].connect_area_dir = eDir_4::UP;
		}
	}
	// -------- 下側 --------
	if (areas_[area_index].area.y + areas_[area_index].area.height < GameManager::FIELD_HEIGHT) {

		tnl::DebugTrace("下確認\n");

		y = areas_[area_index].area.y + areas_[area_index].area.height;
		ex = areas_[area_index].area.x + areas_[area_index].area.width;

		for (x = areas_[area_index].area.x; x < ex; x += areas_[field_[y][x].area_id].area.width) {

			// すでにエリアの通路が確認されていた場合、次のエリアまで跳ぶ
			if (areas_[field_[y][x].area_id].is_connect) continue;

			if (checkConnectAreaNumMax(x, y, area_index)) areas_[area_index].connect_area_dir = eDir_4::DOWN;
		}
	}
	// -------- 左側 --------
	if ( areas_[area_index].area.x > 0 ) {

		tnl::DebugTrace("左確認\n");

		x = areas_[area_index].area.x - 1;
		ey = areas_[area_index].area.y + areas_[area_index].area.height;

		// 自エリアの始まりから終わりまで、x、y座標のエリア番号の高さ分代入し、左エリアの最大エリア番号を探す
		for (y = areas_[area_index].area.y; y < ey; y += areas_[field_[y][x].area_id].area.height)
		{
			if (areas_[field_[y][x].area_id].is_connect) continue;

			if (checkConnectAreaNumMax(x, y, area_index)) areas_[area_index].connect_area_dir = eDir_4::LEFT;
		}
	}
	// -------- 右側 --------
	if (areas_[area_index].area.x + areas_[area_index].area.width < GameManager::FIELD_WIDTH) {

		tnl::DebugTrace("右確認\n");

		x = areas_[area_index].area.x + areas_[area_index].area.width;
		ey = areas_[area_index].area.y + areas_[area_index].area.height;

		// 自エリアの始まりから終わりまで、x、y座標のエリア番号の高さ分代入し、左エリアの最大エリア番号を探す
		for (y = areas_[area_index].area.y; y < ey; y += areas_[field_[y][x].area_id].area.height)
		{
			if (areas_[field_[y][x].area_id].is_connect) continue;

			if (checkConnectAreaNumMax(x, y, area_index)) areas_[area_index].connect_area_dir = eDir_4::RIGHT;
		}
	}
}

// =====================================================================================
// 繋げようとしているエリアと指定された座標のエリアの番号が最大値か比較し、
// 最大値の場合、繋げるエリア"connect_area_index"を引数で指定された座標のエリアの番号にする。
// =====================================================================================
bool DungeonGenerator::checkConnectAreaNumMax(int x, int y, int area_index) {

	if (areas_[area_index].connect_area_index < field_[y][x].area_id) {

		areas_[area_index].connect_area_index = field_[y][x].area_id;
		tnl::DebugTrace("隣接するエリア番号の最大値を更新。エリア番号：%d\n", areas_[area_index].connect_area_index);
		return true;
	}

	return false;
}

// =====================================================================================
// 隣接する部屋の上下の通路作成
// =====================================================================================
void DungeonGenerator::connectUpAndDownRooms(int up_x, int up_y, int down_x, int down_y) {

	tnl::DebugTrace("down_y = %d, up_y = %d\n", down_y, up_y);
	while (down_y != up_y) {
		if (abs(down_y - up_y) == 1) {
			down_y--;
			tnl::DebugTrace("down_y = %d\n", down_y);
			field_[down_y][down_x].terrain_data = eMapData::GROUND;
			field_[down_y][down_x].map_data = eMapData::GROUND;
			field_[down_y][down_x].place = ePlace::CORRIDOR;
		}
		else {
			down_y--, up_y++;
			tnl::DebugTrace("down_y = %d, up_y = %d\n", down_y, up_y);
			field_[down_y][down_x].terrain_data = eMapData::GROUND;
			field_[down_y][down_x].map_data = eMapData::GROUND;
			field_[down_y][down_x].place = ePlace::CORRIDOR;

			if (field_[up_y][up_x].terrain_data == eMapData::GROUND) {
				continue;
			}
			field_[up_y][up_x].terrain_data = eMapData::GROUND;
			field_[up_y][up_x].map_data = eMapData::GROUND;
			field_[up_y][up_x].place = ePlace::CORRIDOR;
		}
	}

	if (down_y == up_y) {
		while (down_x != up_x) {
			if (down_x < up_x) down_x++;
			else down_x--;
			// if (field_[down_y + 1][down_x].terrain_data == eMapData::GROUND && field_[down_y - 1][down_x].terrain_data == eMapData::GROUND) return;
			/*if (terrain_data_[down_y][down_x] == static_cast<int>(eMapData::GROUND)) break;*/
			field_[down_y][down_x].terrain_data = eMapData::GROUND;
			field_[down_y][down_x].map_data = eMapData::GROUND;
			field_[down_y][down_x].place = ePlace::CORRIDOR;

		}
	}
}

// =====================================================================================
// 隣接する部屋の左右の通路作成
// =====================================================================================
void DungeonGenerator::connectLeftAndRightRooms(int left_x, int left_y, int right_x, int right_y) {
	
	while (left_x != right_x) {
		if (abs(left_x - right_x) == 1) {
			left_x++;
			field_[left_y][left_x].terrain_data = eMapData::GROUND;
			field_[left_y][left_x].map_data = eMapData::GROUND;
			field_[left_y][left_x].place = ePlace::CORRIDOR;
		}
		else {
			left_x++, right_x--;
			field_[right_y][right_x].terrain_data = eMapData::GROUND;
			field_[right_y][right_x].map_data = eMapData::GROUND;
			field_[right_y][right_x].place = ePlace::CORRIDOR;

			if (field_[left_y][left_x].terrain_data == eMapData::GROUND) continue;
			field_[left_y][left_x].terrain_data = eMapData::GROUND;
			field_[left_y][left_x].map_data = eMapData::GROUND;
			field_[left_y][left_x].place = ePlace::CORRIDOR;
		}
	}

	if (left_x == right_x) {
		while (left_y != right_y) {
			if (left_y < right_y) left_y++;
			else left_y--;
			// if (field_[left_y][left_x + 1].terrain_data == eMapData::GROUND && field_[left_y][left_x - 1].terrain_data == eMapData::GROUND) return;
			field_[left_y][left_x].terrain_data = eMapData::GROUND;
			field_[left_y][left_x].map_data = eMapData::GROUND;
			field_[left_y][left_x].place = ePlace::CORRIDOR;
		}
	}
}

// =====================================================================================
// フィールドを作成
// =====================================================================================
void DungeonGenerator::generateRoom() {

	for (int i = 0; i < area_count_; i++) {

		tnl::DebugTrace("area = %d, x = %d, y = %d, width = %d, height = %d\n", i, areas_[i].room.x, areas_[i].room.y, areas_[i].room.width, areas_[i].room.height);
		for (int y = areas_[i].room.y; y < areas_[i].room.y + areas_[i].room.height; y++) {
			for (int x = areas_[i].room.x; x < areas_[i].room.x + areas_[i].room.width; x++) {
				field_[y][x].terrain_data = eMapData::GROUND;
				field_[y][x].map_data = eMapData::GROUND;
				field_[y][x].place = ePlace::ROOM;
			}
		}
	}
}

// =====================================================================================
// 階段の生成
// =====================================================================================
void DungeonGenerator::createStair() {

	int area_index = rand() % area_count_;

	int create_x = ( rand() % ( areas_[area_index].room.width - 2 ) ) + areas_[area_index].room.x + 1;
	int create_y = ( rand() % ( areas_[area_index].room.height - 2 ) ) + areas_[area_index].room.y + 1;

	tnl::DebugTrace("player x = %d, y = %d\n", create_x, create_y);
	field_[create_y][create_x].terrain_data = eMapData::STAIR;
	field_[create_y][create_x].map_data = eMapData::STAIR;
}

// =====================================================================================
// プレイヤーの生成位置を決める
// =====================================================================================
void DungeonGenerator::spawnPlayer() {

	while (1) {
		int area_index = rand() % area_count_;

		int spawn_x = rand() % areas_[area_index].room.width + areas_[area_index].room.x;
		int spawn_y = rand() % areas_[area_index].room.height + areas_[area_index].room.y;

		if (field_[spawn_y][spawn_x].map_data == eMapData::GROUND) {
			tnl::DebugTrace("player x = %d, y = %d\n", spawn_x, spawn_y);
			field_[spawn_y][spawn_x].map_data = eMapData::PLAYER;
			break;
		}
	}
}

// =====================================================================================
// 敵の生成位置を決める
// =====================================================================================
void DungeonGenerator::spawnEnemy() {

	int spawn_num = 0;

	while (spawn_num < EnemyManager::ENEMY_MAX) {
		int area_index = rand() % area_count_;

		int spawn_x = rand() % areas_[area_index].room.width + areas_[area_index].room.x;
		int spawn_y = rand() % areas_[area_index].room.height + areas_[area_index].room.y;

		if (field_[spawn_y][spawn_x].map_data == eMapData::GROUND) {
			tnl::DebugTrace("enemy[%d] x = %d, y = %d\n", spawn_num, spawn_x, spawn_y);
			field_[spawn_y][spawn_x].map_data = eMapData::ENEMY;
			spawn_num++;
		}
	}
}

int DungeonGenerator::countTerrainForDirection(int x, int y, eMapData terrain_data) {

	int count = 0;

	for (int i = 0; i < std::underlying_type<eDir_4>::type(eDir_4::MAX); ++i) {
		if (field_[y + DIR_POS[i].y][x + DIR_POS[i].x].terrain_data == terrain_data) {
			++count;
		}
	}

	return 0;
}
