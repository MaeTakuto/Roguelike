#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include "../common/enum.h"

class Camera;

// ���̃f�[�^�N���X
class Rect {
public:
	// ���̎n�_���W�A�I�_���W
	int x = 0;
	int y = 0;
	int width = 0;
	int height = 0;

};

class Entrance {
public:
	tnl::Vector3 pos = { 0, 0, 0 };
	int id = 0;
};

class Room {
public:
	// ���̎n�_���W�A�I�_���W
	int x = 0;
	int y = 0;
	int width = 0;
	int height = 0;

	// �����̐�
	int entrance_count = 0;

	// �����̓����̈ʒu
	std::vector<Entrance> entrance;

};

// �G���A�f�[�^���Ǘ�����N���X
class Area {
public:
	Rect area;
	Room room;

	int connect_area_index = -1;
	bool is_connect = false;

	// �ʘH�����݂��邩�̔���
	eDir connect_area_dir = eDir::NONE;
};

// 1�}�X�̃f�[�^���Ǘ�����N���X
class Cell {
public:
	// �G���A�̔ԍ�
	int area_id = 0;

	// �ꏊ�i �ʘH�Ȃ̂��A�����Ȃ̂� �j
	ePlace place = ePlace::WALL;

	// �}�b�v�̃f�[�^�i �Փ˔���p �j
	eMapData map_data = eMapData::WALL;

	// �n�`�f�[�^�i ���A�� �j
	eMapData terrain_data = eMapData::WALL;

};

// �_���W�����̐����A�Ǘ�����N���X
class DungeonManager {
public:
	DungeonManager();
	~DungeonManager();

	const int AREA_MAX_SIZE = 32;
	const int AREA_MIN_SIZE = 16;
	const int AREA_MAX = 8;					// �ő�G���A��
	const int AREA_SPACE = 3;
	const int ROOM_AND_ROAD_SPACE = 2;
	const int MAX_ROOM_ENTRANCE = 3;
	const int ENEMY_MAX_INIT_SPAWN = 6;

	// ======= �_���W���������A�Đ��� ======
	void generateDungeon();

	// =============================
	//        �f�o�b�O�p
	// =============================

	// ���������G���A�f�[�^��\��
	void displayAreaNumber(const std::shared_ptr<Camera> camera);
	void debugEntranceData() {

		for (int i = 0; i < areas_.size(); i++) {
			tnl::DebugTrace("========== area = %d ==========\n", i);
			for (int j = 0; j < areas_[i].room.entrance.size(); j++) {
				tnl::DebugTrace("entrance[%d].pos = %c\n", j, areas_[i].room.entrance[j].pos.toString().c_str());
				tnl::DebugTrace("entrance[%d].id = %d\n", j, areas_[i].room.entrance[j].id);
			}
			tnl::DebugTrace("\n");
		}
	}

	// �Q�b�^�[
	inline std::vector< std::vector<Cell> >& getField() { return field_; }
	inline std::vector< std::vector<int> >& getMapData() { return map_data_; }
	inline std::vector<Area>& getAreas() { return areas_; }

private:
	// int area_number_[FIELD_HEIGHT][FIELD_WIDTH] = { 0 };

	// �n�`�f�[�^
	std::vector<std::vector<Cell> > field_{ GameManager::FIELD_HEIGHT, std::vector<Cell>( GameManager::FIELD_WIDTH ) };
	std::vector<std::vector<int> > map_data_{ GameManager::FIELD_HEIGHT, std::vector<int>( GameManager::FIELD_WIDTH ) };

	int area_count_ = 0;
	std::vector<Area> areas_;

	// �ʘH�쐬�p
	int* order_connect_rooms_ = nullptr;				// ���[�����q���鏇��
	int order_index_ = 0;								// order�̓Y����
	int connect_error_count_ = 0;						// �G���[�̔����p

	// start����end�܂ł͈̔͂ŗ�������
	inline int random(int start, int end) { return rand() % (end - start + 1) + start; }

	void areaDataInit();
	void fieldDataInit();
	
	// �G���A�𕪊�
	void splitArea(int area_index, int vertical);
	
	// �t�B�[���h�̃G���A�ԍ���ݒ�
	void settingAreaNumber();

	// �������쐬
	void createRoom();

	// ======== ������ =========
	void createSmallRoom(int area_index);
	void createMidiumRoom(int area_index);
	// =========================

	// �t�B�[���h�쐬
	void generateRoom();

	void getNextConnectRoomIndex(int area_index);

	// �������m���q����
	void connectRoom(int area_index);

	// �������m���q����G���A�̔ԍ����Z�b�g
	bool checkConnectAreaNumMax(int x, int y, int area_index);

	// �㉺�A���E�̕����̂ɒʘH���쐬
	void connectUpAndDownRooms(int up_x, int up_y, int down_x, int down_y);
	void connectLeftAndRightRooms(int left_x, int left_y, int right_x, int right_y);

	// �}�b�v�f�[�^�̍쐬�i �n�`�f�[�^�A�e�L�����̈ʒu�f�[�^ �j
	void generateMapData();

	// �v���C���[�̐����ʒu�����߂�
	void spawnPlayer();

	// �G�̐����ʒu�����߂�
	void spawnEnemy();

};