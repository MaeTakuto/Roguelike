#include "../../dxlib_ext/dxlib_ext.h"
#include "../manager/resource_manager.h"
#include "../common/camera.h"
#include "enemy.h"


Enemy::Enemy() {

	gpc_hdl_data_ = tnl::LoadCsv("csv/player_gpc_data.csv");

	gpc_hdl_.resize( static_cast<int>( eDir::MAX ) );

	for (int i = 1; i < gpc_hdl_data_.size(); i++) {

		gpc_hdl_[i - 1].resize(gpc_hdl_data_[i][1].getInt());

		gpc_hdl_[i - 1] = ResourceManager::getInstance()->loadAnimation
		(gpc_hdl_data_[i][0].getString(),
			gpc_hdl_data_[i][1].getInt(),
			gpc_hdl_data_[i][2].getInt(),
			gpc_hdl_data_[i][3].getInt(),
			GameManager::CHIP_SIZE,
			GameManager::CHIP_SIZE
		);
	}

	dir_ = eDir::DOWN;
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
		gpc_hdl_[static_cast<int>(dir_)][0], true);
}

// =====================================================================================
// �ҋ@�V�[�P���X
// =====================================================================================
bool Enemy::seqIdle(const float delta_time) {

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

	std::vector<eDir> directions;
	//std::vector<eDir> directions = getNearbyMapData( pos_, eMapData::PLAYER );

	//// �v���C���[���אڂ��Ă����ꍇ
	//if (directions.size() != 0) {
	//	// �U�����菈��
	//	action_error_++;
	//	return;
	//}

	// �n�ʂ�����������擾����
	directions = getNearbyMapData( pos_, eMapData::GROUND );

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

	changeToMoveAction();

}

// =====================================================================================
// �����ɂ���Ƃ��̍s��
// =====================================================================================
void Enemy::onRoomAction() {

	// �^�[�Q�b�g�������Ă����ꍇ�A���̕����Ɍ�����
	if (is_target_pos_)	moveToTarget();

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
	next_pos_ = tnl::Vector3(next_node->pos_.x, next_node->pos_.y, 0);

	// �����̐ݒ�
	if((next_pos_.y - pos_.y) > 0 ) dir_ = eDir::DOWN;
	else if ((next_pos_.y - pos_.y) < 0) dir_ = eDir::UP;
	else if ((next_pos_.x - pos_.x) > 0) dir_ = eDir::RIGHT;
	else if ((next_pos_.x - pos_.x) < 0) dir_ = eDir::LEFT;

	if (scene_play->getPlace(next_pos_) == ePlace::ROAD) is_target_pos_ = false;
	changeToMoveAction();
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

	std::vector<Entrance> entrance;

	int area_id = scene_play->getAreaId(pos_);

	entrance = scene_play->getRoomEntrance(area_id);

	if (entrance.size() == 0) {
		tnl::DebugTrace("vector����ł�\n");
		changeToMoveAction();
		return;
	}

	int target_pos_index = rand() % entrance.size();

	target_pos_ = entrance[target_pos_index].pos;
	target_entrance_id_ = entrance[target_pos_index].id;

	is_target_pos_ = true;

	moveToTarget();
}