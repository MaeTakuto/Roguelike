#include "../../dxlib_ext/dxlib_ext.h"
#include "../scene/scene_play.h"
#include "../manager/gm_manager.h"
#include "enemy_base.h"


// コンストラクタ
EnemyBase::EnemyBase() : target_pos_(0, 0, 0) {

	auto scene_play = std::dynamic_pointer_cast<ScenePlay>(GameManager::GetInstance()->getSceneInstance());
	if (scene_play) scene_play_ = scene_play;
}

// デストラクタ
EnemyBase::~EnemyBase() {
	tnl::DebugTrace("EnemyBaseのデストラクタが実行されました\n");
}

// =====================================================================================
// 周囲の上下左右のマスから、指定した "eMapData" が存在する方向を返す
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
// 周囲の上下左右のマスから、指定した "eMapData" が存在する方向を返す
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
// 周囲 8マスからプレイヤーがいるか確認し、その方向を返す。
// 存在しない場合、"eDir_8::NONE" が返ってくる
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
// プレイヤーに最も近い入口を返す
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
		tnl::DebugTrace("entranceのsizeが 0 です\n");
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
// プレイヤーが同じ部屋にいるか判定
// =====================================================================================
bool EnemyBase::isSameRoomToPlayer() const {

	bool is_same_room = true;

	auto scene_play = scene_play_.lock();
	if (!scene_play) {
		return false;
	}

	int now_area_id = scene_play->getAreaId(pos_);

	tnl::Vector3 player_pos = scene_play->getPlayer()->getNextPos();

	// 部屋の入口にいるか確認
	std::vector<Entrance> entrance = scene_play->getRoomEntrance(now_area_id);
	
	for (int i = 0; i < entrance.size(); ++i) {
		if (scene_play->getMapData(entrance[i].pos) == eMapData::PLAYER) {
			tnl::DebugTrace("入口に player を発見\n");
			return true;
		}
	}

	// 部屋内を確認
	if (scene_play->getPlace(player_pos) != ePlace::ROOM) {
		return false;
	}
	if (scene_play->getAreaId(player_pos) != now_area_id) {
		return false;
	}

	return true;
}


// =====================================================================================
// 現在の部屋の入口をランダムに取得する
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
		tnl::DebugTrace("Entranceが存在しません\n");
		return pos;
	}

	int target_pos_index = rand() % entrance.size();
	pos = entrance[target_pos_index].pos;
	return pos;
}

// =====================================================================================
// 移動コストが最も小さく有効な CellCostのインデックスを取得
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
// 攻撃行動に切り替える
// =====================================================================================
void EnemyBase::changeToAttackAction(eDir_8 dir) {
	
	auto scene_play = scene_play_.lock();
	if (!scene_play) {
		tnl::DebugTrace("攻撃行動に切り替えられませんでした\n");
		return;
	}

	act_state_ = eActState::ATTACK;
	anim_dir_ = ANIM_DIR[std::underlying_type<eDir_8>::type(dir)];
	looking_dir_ = dir;
	
}

// =====================================================================================
// 移動行動に切り替える
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
