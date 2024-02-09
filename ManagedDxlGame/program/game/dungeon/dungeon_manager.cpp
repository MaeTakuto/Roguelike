#include "../../dxlib_ext/dxlib_ext.h"
#include "../manager/gm_manager.h"
#include "dungeon_manager.h"
#include "../common/camera.h"


// �R���X�g���N�^
DungeonManager::DungeonManager() {

	areas_.resize(AREA_MAX);

	order_connect_rooms_ = new int[AREA_MAX];

	for (int i = 0; i < AREA_MAX; i++) {
		order_connect_rooms_[i] = 0;

		// �G���A���̕����̕ϐ��̏�����
		areas_[i].room.entrance_count = 0;
	}

}

// 
DungeonManager::~DungeonManager() {

	tnl::DebugTrace("DungeonManager�̃f�X�g���N�^�����s����܂����B\n");

	delete[] order_connect_rooms_;
}

// �_���W��������
void DungeonManager::generateDungeon() {

	areaDataInit();
	fieldDataInit();

	// �G���A�𕪊�
	splitArea(0, rand() % 2);
	if (areas_[0].area.width >= AREA_MAX_SIZE && areas_[0].area.height >= AREA_MAX_SIZE) { splitArea(0, rand() % 2); }

	settingAreaNumber();

	for (int i = 0; i < area_count_; i++) {
		tnl::DebugTrace("----- index = %d ------\n", i);
		tnl::DebugTrace("x = %d, y = %d, width = %d, height = %d\n", areas_[i].area.x, areas_[i].area.y, areas_[i].area.width, areas_[i].area.height);
	}

	createRoom();
	generateRoom();

	order_connect_rooms_[order_index_] = rand() % area_count_;
	areas_[ order_connect_rooms_[order_index_] ].is_connect = true;

	// �ʘH���쐬
	while (order_index_ + 1 < area_count_) {
		connectRoom(order_connect_rooms_[order_index_]);
		tnl::DebugTrace("order_index + 1 = %d\n", order_index_ + 1);
	}

	generateMapData();
	spawnPlayer();
	spawnEnemy();

	debugEntranceData();

}

// �G���A�̏�����
void DungeonManager::areaDataInit() {

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

// �t�B�[���h�f�[�^�̏�����
void DungeonManager::fieldDataInit() {

	for (int y = 0; y < GameManager::FIELD_HEIGHT; y++) {
		for (int x = 0; x < GameManager::FIELD_WIDTH; x++) {
			field_[y][x].terrain_data = eMapData::WALL;
			field_[y][x].map_data = eMapData::WALL;
			field_[y][x].place = ePlace::WALL;
			field_[y][x].area_id = 0;
		}
	}
}

// �G���A�������_���ɕ���
void DungeonManager::splitArea(int area_index, int vertical) {

	if (area_count_ >= AREA_MAX) return;

	int new_area_index = area_count_;
	int width = areas_[area_index].area.width;
	int height = areas_[area_index].area.height;

	// ���𕪊�
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

// �G���A�ԍ����G���A�f�[�^�ɐݒ�
void DungeonManager::settingAreaNumber() {

	tnl::DebugTrace("area_count = %d\n", area_count_);

	// ���������G���A�f�[�^��\��
	for (int i = 0; i < area_count_; i++) {

		for (int y = areas_[i].area.y; y < areas_[i].area.y + areas_[i].area.height; y++) {
			for (int x = areas_[i].area.x; x < areas_[i].area.x + areas_[i].area.width; x++) {
				field_[y][x].area_id = i;
			}
		}
	}
}

// �������쐬
void DungeonManager::createRoom() {

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

// ���������쐬
void DungeonManager::createSmallRoom(int area_index) {
	
	int rand_x = rand() % (AREA_SPACE);
	int rand_y = rand() % (AREA_SPACE);
	int rand_width = rand() % (AREA_SPACE) + rand_x;
	int rand_height = rand() % (AREA_SPACE) + rand_y;

	areas_[area_index].room.x = areas_[area_index].area.x + AREA_SPACE + rand_x;
	areas_[area_index].room.y = areas_[area_index].area.y + AREA_SPACE + rand_y;
	areas_[area_index].room.width = areas_[area_index].area.width - ( AREA_SPACE * 2 ) - rand_width;
	areas_[area_index].room.height = areas_[area_index].area.height - ( AREA_SPACE * 2 ) - rand_height;
}

// ������
void DungeonManager::createMidiumRoom(int area_index) {

}


// 
void DungeonManager::connectRoom(int area_index) {

	int neighbor_index;

	tnl::DebugTrace("========== connectRoom(area_index = %d) ===========\n", area_index);

	getNextConnectRoomIndex(area_index);

	neighbor_index = areas_[area_index].connect_area_index;

	// ���̃G���A�����������ꍇ
	if (areas_[area_index].connect_area_index != -1) {

		areas_[neighbor_index].is_connect = true;
		order_connect_rooms_[order_index_ + 1] = neighbor_index;
		order_index_++;

		// ============================ �ʘH���쐬 ===============================

		// d1�F�ړ��O�̃h�A�̍��W�Ad2�F�ړ���̃h�A�̍��W
		int dx1, dy1, dx2, dy2;

		int index = order_connect_rooms_[order_index_ - 1 - connect_error_count_];
		tnl::DebugTrace("order[%d] = %d\n", order_index_, order_connect_rooms_[order_index_]);
		tnl::DebugTrace("d1 index = %d\n", index);

		// �����[���̓K���ȍ��W�w��
		/*dx1 = random(areas_[index].room.x + ROOM_AND_ROAD_SPACE, areas_[index].room.x + areas_[index].room.width - 1 - ROOM_AND_ROAD_SPACE);
		dy1 = random(areas_[index].room.y + ROOM_AND_ROAD_SPACE, areas_[index].room.y + areas_[index].room.height - 1 - ROOM_AND_ROAD_SPACE);

		index = order_connect_rooms_[order_index_];
		tnl::DebugTrace("d2 index = %d\n", index);
		dx2 = random(areas_[index].room.x + ROOM_AND_ROAD_SPACE, areas_[index].room.x + areas_[index].room.width - 1 - ROOM_AND_ROAD_SPACE);
		dy2 = random(areas_[index].room.y + ROOM_AND_ROAD_SPACE, areas_[index].room.y + areas_[index].room.height - 1 - ROOM_AND_ROAD_SPACE);*/

		//tnl::DebugTrace("dx1 = %d, dy1 = %d, dx2 = %d, dy2 = %d\n", dx1, dy1, dx2, dy2);

		// �e��������ʘH��L�΂�
		// 
		switch (areas_[area_index].connect_area_dir) {
		case eDir::UP:
			// �����[���̓K���ȍ��W�w��
			index = order_connect_rooms_[order_index_ - 1 - connect_error_count_];
			dx1 = random(areas_[index].room.x + ROOM_AND_ROAD_SPACE, areas_[index].room.x + areas_[index].room.width - 1 - ROOM_AND_ROAD_SPACE);
			dy1 = areas_[index].room.y;

			// �����̓����̍��W��ݒ�
			areas_[index].room.entrance.emplace_back();
			areas_[index].room.entrance[areas_[index].room.entrance_count].pos = tnl::Vector3(static_cast<float>(dx1), static_cast<float>(dy1 - 1), 0);
			areas_[index].room.entrance[areas_[index].room.entrance_count].id = areas_[index].room.entrance_count;
			areas_[index].room.entrance_count++;

			// �q���鑤�̕����̍��W�����߂�
			index = order_connect_rooms_[order_index_];
			tnl::DebugTrace("d2 index = %d\n", index);

			dx2 = random(areas_[index].room.x + ROOM_AND_ROAD_SPACE, areas_[index].room.x + areas_[index].room.width - 1 - ROOM_AND_ROAD_SPACE);
			dy2 = areas_[index].room.y + areas_[index].room.height - 1 - rand() % (ROOM_AND_ROAD_SPACE + 1);
			tnl::DebugTrace("dx1 = %d, dy1 = %d, dx2 = %d, dy2 = %d\n", dx1, dy1, dx2, dy2);
			tnl::DebugTrace("��ʘH�쐬\n");

			// �����̓����̍��W��ݒ�
			areas_[index].room.entrance.emplace_back();
			areas_[index].room.entrance[areas_[index].room.entrance_count].pos = tnl::Vector3(static_cast<float>(dx2), static_cast<float>(areas_[index].room.y + areas_[index].room.height), 0);
			areas_[index].room.entrance[areas_[index].room.entrance_count].id = areas_[index].room.entrance_count;

			areas_[index].room.entrance_count++;

			connectUpAndDownRooms(dx2, dy2, dx1, dy1);
			break;

		case eDir::DOWN:
			// �����[���̓K���ȍ��W�w��
			index = order_connect_rooms_[order_index_ - 1 - connect_error_count_];
			dx1 = random(areas_[index].room.x + ROOM_AND_ROAD_SPACE, areas_[index].room.x + areas_[index].room.width - 1 - ROOM_AND_ROAD_SPACE);
			dy1 = areas_[index].room.y + areas_[index].room.height - 1 - rand() % (ROOM_AND_ROAD_SPACE + 1);

			// �����̓����̍��W��ݒ�
			areas_[index].room.entrance.emplace_back();
			areas_[index].room.entrance[areas_[index].room.entrance_count].pos = tnl::Vector3(static_cast<float>(dx1), static_cast<float>(areas_[index].room.y + areas_[index].room.height), 0);
			areas_[index].room.entrance[areas_[index].room.entrance_count].id = areas_[index].room.entrance_count;

			areas_[index].room.entrance_count++;

			// �q���鑤�̕����̍��W�����߂�
			index = order_connect_rooms_[order_index_];
			tnl::DebugTrace("d2 index = %d\n", index);
			
			dx2 = random(areas_[index].room.x + ROOM_AND_ROAD_SPACE, areas_[index].room.x + areas_[index].room.width - 1 - ROOM_AND_ROAD_SPACE);
			dy2 = areas_[index].room.y;
			tnl::DebugTrace("dx1 = %d, dy1 = %d, dx2 = %d, dy2 = %d\n", dx1, dy1, dx2, dy2);
			tnl::DebugTrace("���ʘH�쐬\n");

			// �����̓�����ݒ�
			areas_[index].room.entrance.emplace_back();
			areas_[index].room.entrance[areas_[index].room.entrance_count].pos = (tnl::Vector3(static_cast<float>(dx2), static_cast<float>(dy2 - 1), 0));
			areas_[index].room.entrance[areas_[index].room.entrance_count].id = areas_[index].room.entrance_count;
			areas_[index].room.entrance_count++;
			
			connectUpAndDownRooms(dx1, dy1, dx2, dy2);
			break;

		case eDir::LEFT:
			// �����[���̓K���ȍ��W�w��
			index = order_connect_rooms_[order_index_ - 1 - connect_error_count_];
			dx1 = areas_[index].room.x;
			dy1 = random(areas_[index].room.y + ROOM_AND_ROAD_SPACE, areas_[index].room.y + areas_[index].room.height - 1 - ROOM_AND_ROAD_SPACE);

			// �����̓�����ݒ�
			areas_[index].room.entrance.emplace_back();
			areas_[index].room.entrance[areas_[index].room.entrance_count].pos = ( tnl::Vector3(static_cast<float>(dx1 - 1), static_cast<float>(dy1), 0) );
			areas_[index].room.entrance[areas_[index].room.entrance_count].id = areas_[index].room.entrance_count;
			areas_[index].room.entrance_count++;

			// �q���鑤�̕����̍��W�����߂�
			index = order_connect_rooms_[order_index_];
			tnl::DebugTrace("d2 index = %d\n", index);

			dx2 = areas_[index].room.x + areas_[index].room.width - 1 - rand() % ( ROOM_AND_ROAD_SPACE + 1 );
			dy2 = random(areas_[index].room.y + ROOM_AND_ROAD_SPACE, areas_[index].room.y + areas_[index].room.height - 1 - ROOM_AND_ROAD_SPACE);
			tnl::DebugTrace("dx1 = %d, dy1 = %d, dx2 = %d, dy2 = %d\n", dx1, dy1, dx2, dy2);
			tnl::DebugTrace("���ʘH�쐬\n");
			
			// �����̓����̍��W��ݒ�
			areas_[index].room.entrance.emplace_back();
			areas_[index].room.entrance[areas_[index].room.entrance_count].pos = tnl::Vector3(static_cast<float>(areas_[index].room.x + areas_[index].room.width), static_cast<float>(dy2), 0);
			areas_[index].room.entrance[areas_[index].room.entrance_count].id = areas_[index].room.entrance_count;

			areas_[index].room.entrance_count++;

			connectLeftAndRightRooms(dx2, dy2, dx1, dy1);
			break;

		case eDir::RIGHT:
			// �����[���̓K���ȍ��W�w��
			index = order_connect_rooms_[order_index_ - 1 - connect_error_count_];
			dx1 = areas_[index].room.x + areas_[index].room.width - 1 - rand() % (ROOM_AND_ROAD_SPACE + 1);
			dy1 = random(areas_[index].room.y + ROOM_AND_ROAD_SPACE, areas_[index].room.y + areas_[index].room.height - 1 - ROOM_AND_ROAD_SPACE);

			// �����̓����̍��W��ݒ�
			areas_[index].room.entrance.emplace_back();
			areas_[index].room.entrance[areas_[index].room.entrance_count].pos = tnl::Vector3(static_cast<float>(areas_[index].room.x + areas_[index].room.width), static_cast<float>(dy1), 0);
			areas_[index].room.entrance[areas_[index].room.entrance_count].id = areas_[index].room.entrance_count;
			areas_[index].room.entrance_count++;

			// �q���鑤�̕����̍��W�����߂�
			index = order_connect_rooms_[order_index_];
			tnl::DebugTrace("d2 index = %d\n", index);
			
			dx2 = areas_[index].room.x;
			dy2 = random(areas_[index].room.y + ROOM_AND_ROAD_SPACE, areas_[index].room.y + areas_[index].room.height - 1 - ROOM_AND_ROAD_SPACE);
			tnl::DebugTrace("dx1 = %d, dy1 = %d, dx2 = %d, dy2 = %d\n", dx1, dy1, dx2, dy2);
			tnl::DebugTrace("�E�ʘH�쐬\n");

			// �����̓����̍��W��ݒ�
			areas_[index].room.entrance.emplace_back();
			areas_[index].room.entrance[areas_[index].room.entrance_count].pos = tnl::Vector3(static_cast<float>(dx2 - 1), static_cast<float>(dy2), 0);
			areas_[index].room.entrance[areas_[index].room.entrance_count].id = areas_[index].room.entrance_count;

			areas_[index].room.entrance_count++;
			
			connectLeftAndRightRooms(dx1, dy1, dx2, dy2);
			break;
		}

		connect_error_count_ = 0;
	}
	// ���������ꍇ
	else {
		connect_error_count_++;
		tnl::DebugTrace("connect_error_�F%d\n", connect_error_count_);
		tnl::DebugTrace("order_index_�F%d\n", order_index_);

		if (order_index_ < connect_error_count_) {
			order_index_ = area_count_;
			return;
		}

		int prev_index = order_connect_rooms_[order_index_ - connect_error_count_];
		areas_[prev_index].connect_area_index = -1;

		connectRoom(prev_index);
	}
}

// �אڂ���G���A�̔ԍ����r���A�G���A�ԍ�����ԑ傫���G���A�̕����ɒʘH���q����ݒ������B
void DungeonManager::getNextConnectRoomIndex(int area_index) {

	int x, y;

	// for���[�v�I���̕ϐ�
	int ex, ey;

	if (areas_[area_index].area.y > 0) {

		tnl::DebugTrace("��m�F\n");

		y = areas_[area_index].area.y - 1;
		ex = areas_[area_index].area.x + areas_[area_index].area.width;

		for (x = areas_[area_index].area.x; x < ex; x += areas_[field_[y][x].area_id].area.width) {

			// ���łɃG���A�̒ʘH���m�F����Ă����ꍇ�A���̃G���A�܂Œ���
			if (areas_[field_[y][x].area_id].is_connect) continue;

			if (checkConnectAreaNumMax(x, y, area_index)) areas_[area_index].connect_area_dir = eDir::UP;
		}
	}
	// -------- ���� --------
	if (areas_[area_index].area.y + areas_[area_index].area.height < GameManager::FIELD_HEIGHT) {

		tnl::DebugTrace("���m�F\n");

		y = areas_[area_index].area.y + areas_[area_index].area.height;
		ex = areas_[area_index].area.x + areas_[area_index].area.width;

		for (x = areas_[area_index].area.x; x < ex; x += areas_[field_[y][x].area_id].area.width) {

			// ���łɃG���A�̒ʘH���m�F����Ă����ꍇ�A���̃G���A�܂Œ���
			if (areas_[field_[y][x].area_id].is_connect) continue;

			if (checkConnectAreaNumMax(x, y, area_index)) areas_[area_index].connect_area_dir = eDir::DOWN;
		}
	}
	// -------- ���� --------
	if ( areas_[area_index].area.x > 0 ) {

		tnl::DebugTrace("���m�F\n");

		x = areas_[area_index].area.x - 1;
		ey = areas_[area_index].area.y + areas_[area_index].area.height;

		// ���G���A�̎n�܂肩��I���܂ŁAx�Ay���W�̃G���A�ԍ��̍�����������A���G���A�̍ő�G���A�ԍ���T��
		for (y = areas_[area_index].area.y; y < ey; y += areas_[field_[y][x].area_id].area.height)
		{
			if (areas_[field_[y][x].area_id].is_connect) continue;

			if (checkConnectAreaNumMax(x, y, area_index)) areas_[area_index].connect_area_dir = eDir::LEFT;
		}
	}
	// -------- �E�� --------
	if (areas_[area_index].area.x + areas_[area_index].area.width < GameManager::FIELD_WIDTH) {

		tnl::DebugTrace("�E�m�F\n");

		x = areas_[area_index].area.x + areas_[area_index].area.width;
		ey = areas_[area_index].area.y + areas_[area_index].area.height;

		// ���G���A�̎n�܂肩��I���܂ŁAx�Ay���W�̃G���A�ԍ��̍�����������A���G���A�̍ő�G���A�ԍ���T��
		for (y = areas_[area_index].area.y; y < ey; y += areas_[field_[y][x].area_id].area.height)
		{
			if (areas_[field_[y][x].area_id].is_connect) continue;

			if (checkConnectAreaNumMax(x, y, area_index)) areas_[area_index].connect_area_dir = eDir::RIGHT;
		}
	}
}

// 
bool DungeonManager::checkConnectAreaNumMax(int x, int y, int area_index) {

	if (areas_[area_index].connect_area_index < field_[y][x].area_id) {

		areas_[area_index].connect_area_index = field_[y][x].area_id;
		tnl::DebugTrace("�אڂ���G���A�ԍ��̍ő�l���X�V�B�G���A�ԍ��F%d\n", areas_[area_index].connect_area_index);
		return true;
	}

	return false;
}

// �אڂ��镔���̏㉺�̒ʘH�쐬
void DungeonManager::connectUpAndDownRooms(int up_x, int up_y, int down_x, int down_y) {

	tnl::DebugTrace("down_y = %d, up_y = %d\n", down_y, up_y);
	while (down_y != up_y) {
		if (abs(down_y - up_y) == 1) {
			down_y--;
			tnl::DebugTrace("down_y = %d\n", down_y);
			field_[down_y][down_x].terrain_data = eMapData::GROUND;
			field_[down_y][down_x].map_data = eMapData::GROUND;
			field_[down_y][down_x].place = ePlace::ROAD;
		}
		else {
			down_y--, up_y++;
			tnl::DebugTrace("down_y = %d, up_y = %d\n", down_y, up_y);
			field_[down_y][down_x].terrain_data = eMapData::GROUND;
			field_[down_y][down_x].map_data = eMapData::GROUND;
			field_[down_y][down_x].place = ePlace::ROAD;

			if (field_[up_y][up_x].terrain_data == eMapData::GROUND) continue;
			field_[up_y][up_x].terrain_data = eMapData::GROUND;
			field_[up_y][up_x].map_data = eMapData::GROUND;
			field_[up_y][up_x].place = ePlace::ROAD;
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
			field_[down_y][down_x].place = ePlace::ROAD;

		}
	}
}

// �אڂ��镔���̍��E�̒ʘH�쐬
void DungeonManager::connectLeftAndRightRooms(int left_x, int left_y, int right_x, int right_y) {
	
	while (left_x != right_x) {
		if (abs(left_x - right_x) == 1) {
			left_x++;
			field_[left_y][left_x].terrain_data = eMapData::GROUND;
			field_[left_y][left_x].map_data = eMapData::GROUND;
			field_[left_y][left_x].place = ePlace::ROAD;
		}
		else {
			left_x++, right_x--;
			field_[right_y][right_x].terrain_data = eMapData::GROUND;
			field_[right_y][right_x].map_data = eMapData::GROUND;
			field_[right_y][right_x].place = ePlace::ROAD;

			if (field_[left_y][left_x].terrain_data == eMapData::GROUND) continue;
			field_[left_y][left_x].terrain_data = eMapData::GROUND;
			field_[left_y][left_x].map_data = eMapData::GROUND;
			field_[left_y][left_x].place = ePlace::ROAD;
		}
	}

	if (left_x == right_x) {
		while (left_y != right_y) {
			if (left_y < right_y) left_y++;
			else left_y--;
			// if (field_[left_y][left_x + 1].terrain_data == eMapData::GROUND && field_[left_y][left_x - 1].terrain_data == eMapData::GROUND) return;
			field_[left_y][left_x].terrain_data = eMapData::GROUND;
			field_[left_y][left_x].map_data = eMapData::GROUND;
			field_[left_y][left_x].place = ePlace::ROAD;
		}
	}
}

// �t�B�[���h���쐬
void DungeonManager::generateRoom() {

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

// 
void DungeonManager::displayAreaNumber(const std::shared_ptr<Camera> camera) {

	SetFontSize(DEFAULT_FONT_SIZE);

	for (int y = 0; y < GameManager::FIELD_HEIGHT; y++) {
		for (int x = 0; x < GameManager::FIELD_WIDTH; x++) {
			tnl::Vector3 draw_pos = tnl::Vector3{ static_cast<float>(x * 20), static_cast<float>(y * 20), 0 } - camera->getPos();
			DrawStringEx(draw_pos.x, draw_pos.y, -1, "%d", field_[y][x].area_id);
		}
	}
}

// 
void DungeonManager::generateMapData() {

	for (int y = 0; y < field_.size(); y++) {
		for (int x = 0; x < field_[y].size(); x++) {
			map_data_[y][x] = static_cast<int>(field_[y][x].place);
		}
	}
}

// �v���C���[�̐����ʒu�����߂�
void DungeonManager::spawnPlayer() {

	int area_index = rand() % area_count_;

	int spawn_x = rand() % areas_[area_index].room.width + areas_[area_index].room.x;
	int spawn_y = rand() % areas_[area_index].room.height + areas_[area_index].room.y;

	tnl::DebugTrace("player x = %d, y = %d\n", spawn_x, spawn_y);
	field_[spawn_y][spawn_x].map_data = eMapData::PLAYER;

}

// �G�̐����ʒu�����߂�
void DungeonManager::spawnEnemy() {

	int spawn_num = 0;

	while (spawn_num < ENEMY_MAX_INIT_SPAWN) {
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
