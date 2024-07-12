#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include "../common/enum.h"
#include "../common/struct_data.h"

class Camera;

// =====================================================================================
// �_���W�����̐����A�Ǘ�����N���X
// =====================================================================================
class DungeonGenerator {
public:
	DungeonGenerator();
	~DungeonGenerator();

	const int AREA_MAX_SIZE = 32;
	const int AREA_MIN_SIZE = 16;
	const int AREA_MAX = 8;					// �ő�G���A��
	const int AREA_SPACE = 3;
	const int ROOM_AND_ROAD_SPACE = 2;
	const int MAX_ROOM_ENTRANCE = 3;

	// ======= �_���W���������A�Đ��� ======
	void generateDungeon();

	// =============================
	//        �f�o�b�O�p
	// =============================

	// ���������G���A�f�[�^��\��
	void displayAreaNumber(const std::shared_ptr<Camera> camera);
	void debugEntranceData();

	// �Q�b�^�[
	inline std::vector< std::vector<DungeonCell> >& getField() { return field_; }
	// inline std::vector< std::vector<int> >& getMapData() { return map_data_; }
	inline std::vector<Area>& getAreas() { return areas_; }

private:
	// int area_number_[FIELD_HEIGHT][FIELD_WIDTH] = { 0 };

	// �n�`�f�[�^
	std::vector<std::vector<DungeonCell> > field_;
	// std::vector<std::vector<int> > map_data_{ GameManager::FIELD_HEIGHT, std::vector<int>( GameManager::FIELD_WIDTH ) };

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

	// �K�i�𐶐�
	void createStair();

	// �v���C���[�̐����ʒu�����߂�
	void spawnPlayer();

	// �G�̐����ʒu�����߂�
	void spawnEnemy();

	// �w�肵�����W����A�w�肵���n�`�ƈ�v������͂̒n�`�𐔂���
	int countTerrainForDirection(int x, int y, eMapData terrain_data);

};