#include "../../dxlib_ext/dxlib_ext.h"
#include "../manager/resource_manager.h"
#include "../common/camera.h"
#include "enemy.h"


Enemy::Enemy() {

	gpc_hdl_data_ = tnl::LoadCsv("csv/enemy_gpc_data.csv");

	chara_gpc_hdl_.resize( static_cast<int>( eDir_4::MAX ) );

	for (int i = 1; i < gpc_hdl_data_.size(); i++) {

		chara_gpc_hdl_[i - 1].resize(gpc_hdl_data_[i][1].getInt());

		chara_gpc_hdl_[i - 1] = ResourceManager::getInstance()->loadAnimation
		(gpc_hdl_data_[i][0].getString(),
			gpc_hdl_data_[i][1].getInt(),
			gpc_hdl_data_[i][2].getInt(),
			gpc_hdl_data_[i][3].getInt(),
			GameManager::CHIP_SIZE,
			GameManager::CHIP_SIZE
		);
	}

	name_ = "���΂����ڂ���";
	status_.setStatus(1, 8, 5, 0, 5);
	dir_ = eDir_4::DOWN;
	act_state_ = eActState::IDLE;
	nodes_ = new Node[8];

	is_alive_ = true;
	is_collision_ = false;

}

// =====================================================================================
// 
// =====================================================================================
Enemy::~Enemy() {

	tnl::DebugTrace("Enemy�̃f�X�g���N�^�����s����܂����B\n");
}

// =====================================================================================
// �G�̏������i �R���X�g���N�^�ŏ������ł��Ȃ����̂̏��� �j
// =====================================================================================
void Enemy::init() {

	auto scene_play = std::dynamic_pointer_cast<ScenePlay>(GameManager::GetInstance()->getSceneInstance());

	if (scene_play) scene_play_ = scene_play;

}

// =====================================================================================
// �X�V
// =====================================================================================
void Enemy::update(float delta_time) {

	sequence_.update(delta_time);
}

// =====================================================================================
// �`��
// =====================================================================================
void Enemy::draw(const std::shared_ptr<Camera> camera) {

	// �`��ʒu����
	tnl::Vector3 draw_pos = tnl::Vector3(pos_.x * GameManager::DRAW_CHIP_SIZE, pos_.y * GameManager::DRAW_CHIP_SIZE, 0)
		- camera->getPos() + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);

	DrawExtendGraph(draw_pos.x, draw_pos.y, draw_pos.x + GameManager::DRAW_CHIP_SIZE, draw_pos.y + GameManager::DRAW_CHIP_SIZE,
		chara_gpc_hdl_[static_cast<int>(dir_)][0], true);
}

// =====================================================================================
// �ҋ@�V�[�P���X
// =====================================================================================
bool Enemy::seqIdle(const float delta_time) {

	return true;
}

// =====================================================================================
// �s���ҋ@�V�[�P���X
// =====================================================================================
bool Enemy::seqActionStundby(const float delta_time) {

	return true;
}

// =====================================================================================
// next_pos_ �Ɉړ�������B
// =====================================================================================
bool Enemy::seqMove(const float delta_time) {

	if (abs(next_pos_.y - pos_.y) > 0.1f || abs(next_pos_.x - pos_.x) > 0.1f) {
		pos_ += (next_pos_ - pos_) * MOVE_SPEED;
	}
	else {
		pos_ = next_pos_;
		act_state_ = eActState::END;
		sequence_.change(&Enemy::seqIdle);
	}

	return true;
}

bool Enemy::seqAttack(const float delta_time) {

	auto scene_play = scene_play_.lock();
	if (scene_play == nullptr) return true;
	count_ += delta_time;

	if (count_ > 0.5f) {
		count_ = 0.0f;
		act_state_ = eActState::END;
		sequence_.change(&Enemy::seqIdle);
	}
	return true;
}

// =====================================================================================
// �ʘH�ł̍s��
// =====================================================================================
void Enemy::onRoadAction() {

	auto scene_play = scene_play_.lock();
	if (scene_play == nullptr) return;

	// �s���G���[�̃J�E���g�񐔂�3��ȏ�̏ꍇ�A�������ɕ�����������
	if (action_error_ >= ACTION_ERROR_MAX) {
		dir_ = getOppositeDir(dir_);
		action_error_ = 0;
	}

	// �v���C���[�������Ă����ꍇ
	if (is_find_player_) {
		// �v���C���[�ɍU���ł���ꍇ
		if (checkAttackForPlayer()) {
			changeToAttackSeq();
			return;
		}
		else {
			target_pos_ = scene_play->getPlayerPos();
			moveToTarget();
			return;
		}
	}

	 eDir_8 dir = findPlayerDir_8();

	if (dir != eDir_8::NONE) {
		is_find_player_ = true;
		if (isEnableDir(dir)) {
			changeToAttackSeq();
			return;
		}
		else {
			target_pos_ = scene_play->getPlayerPos();
			moveToTarget();
			return;
		}
	}

	// �n�ʂ�����������擾����
	std::vector<eDir_4> directions = getNearbyMapData( pos_, eMapData::GROUND );

	// ���������̗v�f�����폜����
	auto it = std::find(directions.begin(), directions.end(), getOppositeDir(dir_));
	if (it != directions.end()) directions.erase(it);

	// �ړ��ł���������Ȃ��ꍇ�A�s���G���[���J�E���g����
	if (directions.size() == 0) { action_error_++; }
	else if (directions.size() == 1) { setNextPosInDir(directions[0]); }
	// �ړ��ł����������������ꍇ�A�����_���ɑI��
	else {
		int index = rand() % directions.size();
		setNextPosInDir(directions[index]);
	}

	changeToMoveSeq();
}

// =====================================================================================
// �����ɂ���Ƃ��̍s��
// =====================================================================================
void Enemy::onRoomAction() {

	auto scene_play = scene_play_.lock();
	if (scene_play == nullptr) return;

	// �v���C���[���������ꍇ�A�U���ł��邩�m�F����
	if (is_find_player_) {
		if (checkAttackForPlayer()) {
			changeToAttackSeq();
			return;
		}
		else target_pos_ = scene_play->getPlayerPos();
	}
	else {
		findPlayer();
		if (is_find_player_) {
			if (checkAttackForPlayer()) {
				changeToAttackSeq();
				return;
			}
		}
	}

	// �^�[�Q�b�g�܂��̓v���C���[�������Ă����ꍇ�A���̕����Ɍ�����
	if (is_find_target_ || is_find_player_)	moveToTarget();
	// �^�[�Q�b�g��T��
	else findTargetInRoom();

}

// =====================================================================================
// �^�[�Q�b�g�Ɍ������Ĉړ�����
// =====================================================================================
void Enemy::moveToTarget() {

	auto scene_play = scene_play_.lock();
	if (scene_play == nullptr) return;

	// 8����
	tnl::Vector2i dir[DIR_MAX] = { tnl::Vector2i::up, tnl::Vector2i::down, tnl::Vector2i::left, tnl::Vector2i::right,
								   tnl::Vector2i(-1, -1), tnl::Vector2i(1, -1), tnl::Vector2i(-1, 1), tnl::Vector2i(1, 1) };

	for (int i = 0; i < DIR_MAX; ++i) {
		nodes_[i].pos_ = tnl::Vector2i(pos_.x, pos_.y) + dir[i];

		// �}�X�̃R�X�g��ݒ肷��B
		nodes_[i].cost_ = abs(target_pos_.x - nodes_[i].pos_.x) + abs(target_pos_.y - nodes_[i].pos_.y);
		nodes_[i].is_enable_ = false;

		if (!isEnableMapPosition(nodes_[i].pos_)) continue;

		// �΂߂̃Z���̊m�F
		if ( (nodes_[i].pos_.x - pos_.x) != 0 && (nodes_[i].pos_.y - pos_.y) != 0) {

			if (scene_play->getMapData(tnl::Vector3(nodes_[i].pos_.x, nodes_[i].pos_.y, 0)) != eMapData::GROUND) continue;;
			if ( !isEnableMapPosition( tnl::Vector2i((pos_.x + dir[i].x), pos_.y) ) || scene_play->getMapData( tnl::Vector3(pos_.x + dir[i].x, pos_.y, 0) ) == eMapData::WALL ) continue;
			if ( !isEnableMapPosition( tnl::Vector2i(pos_.x, (pos_.y + dir[i].y)) ) || scene_play->getMapData( tnl::Vector3(pos_.x, pos_.y + dir[i].y, 0) ) == eMapData::WALL ) continue;
			nodes_[i].is_enable_ = true;
			continue;
		}

		if(scene_play->getMapData(tnl::Vector3(nodes_[i].pos_.x, nodes_[i].pos_.y, 0)) == eMapData::GROUND) nodes_[i].is_enable_ = true;
	}

	Node* next_node = getMinimunScoreNodeForEnabled();
	if (next_node) next_pos_ = tnl::Vector3(next_node->pos_.x, next_node->pos_.y, 0);
	else next_pos_ = pos_;

	// �����̐ݒ�
	if((next_pos_.y - pos_.y) > 0 ) dir_ = eDir_4::DOWN;
	else if ((next_pos_.y - pos_.y) < 0) dir_ = eDir_4::UP;
	else if ((next_pos_.x - pos_.x) > 0) dir_ = eDir_4::RIGHT;
	else if ((next_pos_.x - pos_.x) < 0) dir_ = eDir_4::LEFT;

	if (scene_play->getPlace(next_pos_) == ePlace::ROAD) is_find_target_ = false;
	changeToMoveSeq();
}

// =====================================================================================
// �^�[�Q�b�g��T���AmoveToTarget() �����s����B
// =====================================================================================
Node* Enemy::getMinimunScoreNodeForEnabled() {
	Node* p = nullptr;

	for (int i = 0; i < DIR_MAX; ++i) {
		if (nodes_[i].is_enable_ == false) continue;
		if (nullptr == p) p = &nodes_[i];
		if (p->cost_ > nodes_[i].cost_) p = &nodes_[i];
	}

	return p;
}

// =====================================================================================
// �^�[�Q�b�g��T���AmoveToTarget() �����s����B
// =====================================================================================
void Enemy::findTargetInRoom() {

	auto scene_play = scene_play_.lock();

	if (scene_play == nullptr) return;

	int area_id = scene_play->getAreaId(pos_);
	std::vector<Entrance> entrance = scene_play->getRoomEntrance(area_id);

	if (entrance.size() == 0) {
		tnl::DebugTrace("vector����ł�\n");
		changeToMoveSeq();
		return;
	}

	int target_pos_index = rand() % entrance.size();

	target_pos_ = entrance[target_pos_index].pos;
	target_entrance_id_ = entrance[target_pos_index].id;

	is_find_target_ = true;

	moveToTarget();
}

// =====================================================================================
// �v���C���[��T��
// =====================================================================================
void Enemy::findPlayer() {

	if (is_find_player_) return;

	auto scene_play = scene_play_.lock();
	if (scene_play == nullptr) return;

	int id = scene_play->getAreaId(pos_);

	// ���������ɂ����Ƃ�
	if (scene_play->isPlayerInRoom(id)) {
		is_find_player_ = true;
		target_pos_ = scene_play->getPlayer()->getPos();
		return;
	}
	std::vector<Entrance> entrance = scene_play->getRoomEntrance(id);

	// ���݂��镔���̓����ɂ����Ƃ�
	for (int i = 0; i < entrance.size(); ++i) {
		if (scene_play->getMapData(entrance[i].pos) == eMapData::PLAYER) {
			is_find_player_ = true;
			target_pos_ = scene_play->getPlayer()->getPos();
			return;
		}
	}
}