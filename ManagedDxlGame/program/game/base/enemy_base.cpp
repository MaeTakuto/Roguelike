#include "../../dxlib_ext/dxlib_ext.h"
#include "../scene/scene_play.h"
#include "../manager/gm_manager.h"
#include "enemy_base.h"


// �R���X�g���N�^
EnemyBase::EnemyBase() : target_pos_(0, 0, 0) {

	auto scene_play = std::dynamic_pointer_cast<ScenePlay>(GameManager::GetInstance()->getSceneInstance());
	if (scene_play) scene_play_ = scene_play;
}

// �f�X�g���N�^
EnemyBase::~EnemyBase() {
	tnl::DebugTrace("EnemyBase�̃f�X�g���N�^�����s����܂���\n");
}

// =====================================================================================
// ���͂̏㉺���E�̃}�X����A�w�肵�� "eMapData" �����݂��������Ԃ�
// =====================================================================================
void EnemyBase::resetEnemyInMapData() {

	auto scene_play = scene_play_.lock();
	if (!scene_play) {
		return;
	}

	scene_play->setMapData(pos_, eMapData::ENEMY);
	if( abs( ( next_pos_ - pos_ ).length() ) > 0.1f && scene_play->getMapData(next_pos_) == eMapData::ENEMY) {
		scene_play->setMapData(next_pos_, scene_play->getTerrainData(next_pos_));
	}
	next_pos_ = pos_;
}

// =====================================================================================
// ���͂̏㉺���E�̃}�X����A�w�肵�� "eMapData" �����݂��������Ԃ�
// =====================================================================================
std::vector<eDir_4> EnemyBase::getMapDataDir_4(eMapData map_data) {

	std::vector<eDir_4> directions;

	auto scene_play = scene_play_.lock();
	if (!scene_play) {
		return directions;
	}

	for (int i = static_cast<int>(eDir_4::UP); i < static_cast<int>(eDir_4::MAX); ++i) {
		if (scene_play->getMapData(pos_ + DIR_POS[i]) == map_data) {
			directions.emplace_back(static_cast<eDir_4>(i));
		}
	}

	return directions;
}

// =====================================================================================
// ���� 8�}�X����v���C���[�����邩�m�F���A���̕�����Ԃ��B
// ���݂��Ȃ��ꍇ�A"eDir_8::NONE" ���Ԃ��Ă���
// =====================================================================================
eDir_8 EnemyBase::findPlayerDir_8() {

	auto scene_play = scene_play_.lock();
	if (!scene_play) {
		return eDir_8::NONE;
	}

	for (int i = static_cast<int>(eDir_8::UP); i < static_cast<int>(eDir_8::MAX); ++i) {
		if (scene_play->getMapData(pos_ + DIR_POS[i]) == eMapData::PLAYER) return static_cast<eDir_8>(i);
	}

	return eDir_8::NONE;
}

// =====================================================================================
// �v���C���[�ɍł��߂�������Ԃ�
// =====================================================================================
tnl::Vector3 EnemyBase::getEntrancePosToNearestPlayer(int area_id) const {

	tnl::Vector3 pos = { 0, 0, 0 };
	auto scene_play = scene_play_.lock();
	if (!scene_play) {
		return pos;
	}

	tnl::Vector3 player_pos = scene_play->getPlayer()->getNextPos();
	std::vector<Entrance> entrance = scene_play->getRoomEntrance(area_id);

	if (entrance.size() == 0) {
		tnl::DebugTrace("entrance��size�� 0 �ł�\n");
		return pos;
	}

	pos = entrance[0].pos;

	for ( int i = 1; i < entrance.size(); ++i ) {
		if (abs((pos - player_pos).length()) >= abs((entrance[i].pos - player_pos).length())) {
			pos = entrance[i].pos;
		}
	}

	return pos;
}

// =====================================================================================
// �v���C���[�����������ɂ��邩����
// =====================================================================================
bool EnemyBase::isSameRoomToPlayer() const {

	bool is_same_room = true;

	auto scene_play = scene_play_.lock();
	if (!scene_play) {
		return false;
	}

	int now_area_id = scene_play->getAreaId(pos_);

	tnl::Vector3 player_pos = scene_play->getPlayer()->getNextPos();

	// �����̓����ɂ��邩�m�F
	std::vector<Entrance> entrance = scene_play->getRoomEntrance(now_area_id);
	
	for (int i = 0; i < entrance.size(); ++i) {
		if (scene_play->getMapData(entrance[i].pos) == eMapData::PLAYER) {
			tnl::DebugTrace("������ player �𔭌�\n");
			return true;
		}
	}

	// ���������m�F
	if (scene_play->getPlace(player_pos) != ePlace::ROOM) {
		return false;
	}
	if (scene_play->getAreaId(player_pos) != now_area_id) {
		return false;
	}

	return true;
}


// =====================================================================================
// ���݂̕����̓����������_���Ɏ擾����
// =====================================================================================
tnl::Vector3 EnemyBase::getRandomEntranceToNowRoom() const {

	tnl::Vector3 pos = { 0, 0, 0 };

	auto scene_play = scene_play_.lock();
	if (!scene_play) {
		return pos;
	}

	int area_id = scene_play->getAreaId(pos_);
	std::vector<Entrance> entrance = scene_play->getRoomEntrance(area_id);

	if (entrance.size() == 0) {
		tnl::DebugTrace("Entrance�����݂��܂���\n");
		return pos;
	}

	int target_pos_index = rand() % entrance.size();
	pos = entrance[target_pos_index].pos;
	return pos;
}

// =====================================================================================
// �ړ��R�X�g���ł��������L���� CellCost�̃C���f�b�N�X���擾
// =====================================================================================
int EnemyBase::getMinimunScoreCellIndex() {
	int index = -1;

	for (int i = 0; i < static_cast<int>(eDir_8::MAX); ++i) {
		if (!cells_[i].is_enable_) {
			continue;
		}
		if (-1 == index) {
			index = i;
			continue;
		}
		if (cells_[index].cost_ > cells_[i].cost_) {
			index = i;
		}
	}

	return index;
}

// =====================================================================================
// �U���s���ɐ؂�ւ���
// =====================================================================================
void EnemyBase::changeToAttackAction(eDir_8 dir) {
	
	auto scene_play = scene_play_.lock();
	if (!scene_play) {
		tnl::DebugTrace("�U���s���ɐ؂�ւ����܂���ł���\n");
		return;
	}

	act_state_ = eActState::ATTACK;
	anim_dir_ = ANIM_DIR[std::underlying_type<eDir_8>::type(dir)];
	looking_dir_ = dir;
	
}

// =====================================================================================
// �ړ��s���ɐ؂�ւ���
// =====================================================================================
void EnemyBase::changeToMoveAction() {

	auto scene_play = scene_play_.lock();
	if (!scene_play) {
		return;
	}

	scene_play->setMapData(pos_, scene_play->getTerrainData(pos_));
	scene_play->setMapData(next_pos_, eMapData::ENEMY);
	act_state_ = eActState::MOVE;
}
