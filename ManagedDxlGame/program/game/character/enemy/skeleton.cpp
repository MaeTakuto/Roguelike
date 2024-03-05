#include "../../../dxlib_ext/dxlib_ext.h"
#include "../../manager/gm_manager.h"
#include "../../manager/resource_manager.h"
#include "../../common/camera.h"
#include "../../scene/scene_play.h"
#include "skeleton.h"

namespace
{
	const std::string SKELETON_DATA_CSV_PATH = "csv/enemy_data/skeleton_data.csv";
};

// =====================================================================================
// �R���X�g���N�^
// =====================================================================================
Skeleton::Skeleton() {
	std::vector<std::vector<tnl::CsvCell>> gpc_hdl_data = tnl::LoadCsv("csv/enemy_gpc_data.csv");

	chara_gpc_hdl_.resize(static_cast<int>(eDir_4::MAX));

	// �L�����̊e�����̉摜�f�[�^�� "chara_gpc_hdl_" �ɕۑ��B
	int gpc_index = std::underlying_type<eEnemyType>::type(eEnemyType::SKELETON) + 1;

	for (int i = 1; i < gpc_hdl_data[gpc_index].size(); i++) {

		chara_gpc_hdl_[i - 1].resize(CHARA_GPC_X_NUM);

		chara_gpc_hdl_[i - 1] = ResourceManager::getInstance()->loadAnimation
		(gpc_hdl_data[gpc_index][i].getString(),
			CHARA_GPC_MAX_NUM,
			CHARA_GPC_X_NUM,
			CHARA_GPC_Y_NUM,
			GameManager::CHIP_SIZE,
			GameManager::CHIP_SIZE
		);
	}

	// �X�e�[�^�X�f�[�^�� CSV ����擾
	std::vector<std::vector<tnl::CsvCell>> status_data = tnl::LoadCsv(SKELETON_DATA_CSV_PATH);

	// �G�̊e�X�e�[�^�X���Z�b�g
	name_ = status_data[1][0].getString();
	status_.setStatus(
		status_data[1][1].getInt(),		// ���x�� 
		status_data[1][2].getInt(),		// HP
		status_data[1][3].getInt(),		// ATK
		status_data[1][4].getInt(),		// DEF
		status_data[1][5].getInt()		// EXP
	);

	anim_dir_ = eDir_4::DOWN;
	act_state_ = eActState::IDLE;

	is_alive_ = true;
}

// =====================================================================================
// �f�X�g���N�^
// =====================================================================================
Skeleton::~Skeleton() {
	tnl::DebugTrace("Skeleton�̃f�X�g���N�^�����s����܂����B\n");
}

// =====================================================================================
// �A�b�v�f�[�g
// =====================================================================================
void Skeleton::update(float delta_time) {

	sequence_.update(delta_time);
}

// =====================================================================================
// �`��
// =====================================================================================
void Skeleton::draw(const std::shared_ptr<Camera> camera) {

	// �`��ʒu����
	tnl::Vector3 draw_pos = tnl::Vector3(pos_.x * GameManager::DRAW_CHIP_SIZE, pos_.y * GameManager::DRAW_CHIP_SIZE, 0)
		- camera->getPos() + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);

	DrawExtendGraph(draw_pos.x, draw_pos.y, draw_pos.x + GameManager::DRAW_CHIP_SIZE, draw_pos.y + GameManager::DRAW_CHIP_SIZE,
		chara_gpc_hdl_[static_cast<int>(anim_dir_)][0], true);
}

// =====================================================================================
// �N���[���𐶐�����
// =====================================================================================
std::shared_ptr<EnemyBase> Skeleton::createClone() const {

	std::shared_ptr<Skeleton> clone = std::make_shared<Skeleton>();
	return clone;
}

// =====================================================================================
// �G�̋�����ݒ肷��
// =====================================================================================
void Skeleton::setEnemyLevel(int lv) {

	if (lv <= 0) {
		tnl::DebugTrace("lv �� 1�ȏ�̒l���Z�b�g���Ă�������. ���͒l�F%d\n", lv);
		return;
	}

	// �X�e�[�^�X�f�[�^�� CSV ����擾
	std::vector<std::vector<tnl::CsvCell>> status_data = tnl::LoadCsv(SKELETON_DATA_CSV_PATH);

	if (lv >= status_data.size()) {
		tnl::DebugTrace("lv �̒l�� CSV�f�[�^�̃T�C�Y�𒴂��Ă��܂�. ���͒l�F%d\n", lv);
		return;
	}

	// �G�̊e�X�e�[�^�X���Z�b�g
	name_ = status_data[lv][0].getString();
	status_.setStatus(
		status_data[lv][1].getInt(),		// ���x�� 
		status_data[lv][2].getInt(),		// HP
		status_data[lv][3].getInt(),		// ATK
		status_data[lv][4].getInt(),		// DEF
		status_data[lv][5].getInt()			// EXP
	);
}

// =====================================================================================
// �s�������߂�
// =====================================================================================
void Skeleton::decideAction() {

	auto scene_play = scene_play_.lock();
	if (scene_play == nullptr) {
		return;
	}

	// ���� 8�}�X�Ƀv���C���[�����邩�m�F
	eDir_8 player_dir = findPlayerDir_8();
	if (player_dir != eDir_8::NONE) {

		is_find_player_ = true;

		// �v���C���[�̂���������U���\������
		if (canAttackInDir(player_dir)) {
			changeToAttackAction(player_dir);
			return;
		}
	}

	// �v���C���[���������ꍇ�A�ǐՂ��s��
	if (is_find_player_) {
		trackingPlayer();
		return;
	}

	// �ʘH�ɂ���ꍇ�A�ʘH�ł̍s���������s��
	if (scene_play->getPlace(pos_) == ePlace::CORRIDOR) {
		actionInCorridor();
	}
	else {
		actionInRoom();
	}
}

// =====================================================================================
// �s�����J�n����
// =====================================================================================
void Skeleton::beginAction() {

	if (!sequence_.isComparable(&Skeleton::seqIdle)) {
		tnl::DebugTrace("sequence_ ��Idle�ɂȂ��Ă��܂���\n");
		return;
	}
	if (act_state_ == eActState::MOVE) {
		sequence_.immediatelyChange(&Skeleton::seqMove);
		return;
	}
	if (act_state_ == eActState::ATTACK) {
		sequence_.immediatelyChange(&Skeleton::seqAttack);
	}
}

// =====================================================================================
// ���M���f�X������
// =====================================================================================
void Skeleton::death() {
	is_alive_ = false;
}

// =====================================================================================
// �ҋ@�V�[�P���X
// =====================================================================================
bool Skeleton::seqIdle(const float delta_time) {

	return true;
}

// =====================================================================================
// next_pos_ �Ɉړ�������B
// =====================================================================================
bool Skeleton::seqMove(const float delta_time) {

	if (abs(next_pos_.y - pos_.y) > 0.1f || abs(next_pos_.x - pos_.x) > 0.1f) {
		pos_ += (next_pos_ - pos_) * MOVE_SPEED;
	}
	else {
		pos_ = next_pos_;
		act_state_ = eActState::END;
		sequence_.change(&Skeleton::seqIdle);
	}

	return true;
}

// ====================================================
// �U���V�[�P���X
// ====================================================
bool Skeleton::seqAttack(const float delta_time) {

	attack_time_ += delta_time;

	if (attack_time_ > ATTACK_TIME_MAX) {
		attack_time_ = 0.0f;
		act_state_ = eActState::END;
		sequence_.change(&Skeleton::seqIdle);
	}
	return true;
}

// =====================================================================================
// "next_pos_" ��ڕW�̈ʒu�Ɍ������� 1�}�X�ړ�����
// =====================================================================================
void Skeleton::setNextPosToTarget() {

	// �Z���̍X�V
	updateCellsCost();
	// �e�Z���̃R�X�g�����ƂɎ��̈ʒu�����߂�
	decideNextPosFromCellCost();
	// �ړ��s���ɐ؂�ւ�
	changeToMoveAction();
}

// =====================================================================================
// �v���C���[��ǐՂ���
// =====================================================================================
void Skeleton::trackingPlayer() {

	auto scene_play = scene_play_.lock();
	if (!scene_play) {
		tnl::DebugTrace("trackingPlayer�֐�������ɓ���ł��܂���ł���\n");
		return;
	}

	tnl::Vector3 player_pos = scene_play->getPlayer()->getNextPos();

	// �����ɂ���ꍇ���āA�v���C���[���ʏ�ɂ���ꍇ
	if (scene_play->getPlace(pos_) == ePlace::ROOM && scene_play->getPlace(player_pos) == ePlace::CORRIDOR) {

		tnl::Vector3 entrance_pos = getEntrancePosToNearestPlayer(scene_play->getAreaId(pos_));

		// �����̓�������A�v���C���[�����ȏ㋗��������Ă����猩�����A�����_���ɓ�����ڎw���B
		if (abs((entrance_pos - player_pos).length()) > 3.0f) {
			is_find_player_ = false;
			target_pos_ = getRandomEntranceToNowRoom();
			is_find_target_ = true;
			setNextPosToTarget();
			return;
		}
	}

	target_pos_ = scene_play->getPlayer()->getNextPos();
	setNextPosToTarget();
}

// =====================================================================================
// �e�Z���̈ړ��R�X�g�A�L���ȃZ�����X�V���s��
// =====================================================================================
void Skeleton::updateCellsCost() {

	auto scene_play = scene_play_.lock();
	if (!scene_play) {
		tnl::DebugTrace("updateCellCost�֐�������ɓ���ł��܂���ł���\n");
		return;
	}

	// �e�Z���̈ʒu�A�ړ��R�X�g���v�Z���A���̈ʒu���ړ��ł��邩������Z�b�g����
	for (int i = 0; i < static_cast<int>(eDir_8::MAX); ++i) {
		cells_[i].pos_ = tnl::Vector2i(pos_.x + DIR_POS[i].x, pos_.y + DIR_POS[i].y);
		cells_[i].cost_ = abs(target_pos_.x - cells_[i].pos_.x) + abs(target_pos_.y - cells_[i].pos_.y);
		cells_[i].is_enable_ = false;

		// �΂ߕ������m�F����Ƃ�
		if (i >= static_cast<int>(eDir_8::UP_LEFT)) {
			if (scene_play->getMapData(tnl::Vector3(cells_[i].pos_.x, cells_[i].pos_.y, 0)) != eMapData::GROUND) continue;;
			if (!canActionToCell(tnl::Vector2i((pos_.x + DIR_POS[i].x), pos_.y))) continue;
			if (!canActionToCell(tnl::Vector2i(pos_.x, (pos_.y + DIR_POS[i].y)))) continue;
			cells_[i].is_enable_ = true;
			continue;
		}

		if (scene_play->getMapData(tnl::Vector3(cells_[i].pos_.x, cells_[i].pos_.y, 0)) == eMapData::GROUND) {
			cells_[i].is_enable_ = true;
		}
	}
}

// =====================================================================================
// �e�Z���̈ړ��R�X�g���玟�̈ʒu�����߂�
// =====================================================================================
void Skeleton::decideNextPosFromCellCost() {

	int index = getMinimunScoreCellIndex();

	// �L���ȃZ�������݂��Ă����ꍇ
	if (index != -1) {
		next_pos_ = tnl::Vector3(cells_[index].pos_.x, cells_[index].pos_.y, 0);
		anim_dir_ = ANIM_DIR[index];
	}
	else {
		next_pos_ = pos_;
	}
}

// =====================================================================================
// �w�肵�������̃Z�����s���ł��邩����
// =====================================================================================
bool Skeleton::canActionToCell(const tnl::Vector2i& pos) {

	auto scene_play = scene_play_.lock();
	if (!scene_play) {
		tnl::DebugTrace("canActionToCell�֐�������ɓ���ł��܂���ł���\n");
		return false;
	}

	if (!isInField(tnl::Vector3(pos.x, pos.y, 0))) {
		return false;
	}
	if (scene_play->getMapData(tnl::Vector3(pos.x, pos.y, 0)) == eMapData::WALL) {
		return false;
	}
	return true;
}

// =====================================================================================
// �w�肵���������U���\��
// =====================================================================================
bool Skeleton::canAttackInDir(eDir_8 dir) {

	auto scene_play = scene_play_.lock();
	if (!scene_play) {
		tnl::DebugTrace("canAttackInDir�֐�������ɓ���ł��܂���ł���\n");
		return false;
	}

	// dir ���΂ߕ����̏ꍇ 
	if (dir == eDir_8::UP_LEFT) {
		if (scene_play->getMapData(pos_ + DIR_POS[static_cast<int>(eDir_8::UP)]) == eMapData::WALL) return false;
		if (scene_play->getMapData(pos_ + DIR_POS[static_cast<int>(eDir_8::LEFT)]) == eMapData::WALL) return false;
	}
	else if (dir == eDir_8::UP_RIGHT) {
		if (scene_play->getMapData(pos_ + DIR_POS[static_cast<int>(eDir_8::UP)]) == eMapData::WALL) return false;
		if (scene_play->getMapData(pos_ + DIR_POS[static_cast<int>(eDir_8::RIGHT)]) == eMapData::WALL) return false;
	}
	else if (dir == eDir_8::DOWN_LEFT) {
		if (scene_play->getMapData(pos_ + DIR_POS[static_cast<int>(eDir_8::DOWN)]) == eMapData::WALL) return false;
		if (scene_play->getMapData(pos_ + DIR_POS[static_cast<int>(eDir_8::LEFT)]) == eMapData::WALL) return false;
	}
	else if (dir == eDir_8::DOWN_RIGHT) {
		if (scene_play->getMapData(pos_ + DIR_POS[static_cast<int>(eDir_8::DOWN)]) == eMapData::WALL) return false;
		if (scene_play->getMapData(pos_ + DIR_POS[static_cast<int>(eDir_8::RIGHT)]) == eMapData::WALL) return false;
	}
	return true;
}

// =====================================================================================
// �ʘH�ł̍s��
// =====================================================================================
void Skeleton::actionInCorridor() {

	// �s���G���[�̃J�E���g�񐔂�3��ȏ�̏ꍇ�A��������߂�
	if (action_error_ >= ACTION_ERROR_MAX) {
		eDir_8 reverce_dir = REVERCE_DIRECTION[std::underlying_type<eDir_4>::type(anim_dir_)];
		anim_dir_ = ANIM_DIR[std::underlying_type<eDir_8>::type(reverce_dir)];
		action_error_ = 0;
	}

	// �n�ʂ�����������擾����
	std::vector<eDir_4> directions = getMapDataDir_4(eMapData::GROUND);

	// ���������̗v�f�����폜����
	eDir_8 reverce_dir = REVERCE_DIRECTION[std::underlying_type<eDir_4>::type(anim_dir_)];
	auto it = std::find(directions.begin(), directions.end(), ANIM_DIR[std::underlying_type<eDir_8>::type(reverce_dir)]);
	if (it != directions.end()) directions.erase(it);

	// �ړ��ł���������Ȃ��ꍇ�A�s���G���[���J�E���g����
	if (directions.size() == 0) {
		action_error_++;
	}
	// �ړ��ł�������� 1�̏ꍇ
	else if (directions.size() == 1) {
		next_pos_ = pos_ + DIR_POS[std::underlying_type<eDir_4>::type(directions[0])];
		anim_dir_ = directions[0];
	}
	// �ړ��ł����������������ꍇ�A�����_���ɑI��
	else {
		int index = rand() % directions.size();
		next_pos_ = pos_ + DIR_POS[std::underlying_type<eDir_4>::type(directions[index])];
		anim_dir_ = directions[index];
	}

	// �ړ��s���ɐ؂�ւ�
	changeToMoveAction();
}

// =====================================================================================
// �����ł̍s��
// =====================================================================================
void Skeleton::actionInRoom() {

	auto scene_play = scene_play_.lock();
	if (!scene_play) {
		tnl::DebugTrace("actionInRoom�֐�������ɓ���ł��܂���ł���\n");
		return;
	}

	// �v���C���[���������������ꍇ�A�v���C���[��ǐՂ���
	if (isSameRoomToPlayer()) {
		is_find_player_ = true;
		target_pos_ = scene_play->getPlayer()->getNextPos();
		setNextPosToTarget();
		return;
	}

	// �ړI�n�������Ă��Ȃ��ꍇ�A������T��
	if (!is_find_target_) {
		target_pos_ = getRandomEntranceToNowRoom();
		is_find_target_ = true;
	}

	// �ڕW�ʒu�Ɍ�����
	setNextPosToTarget();

	// �ʘH�̓����ɒ�������Atarget�t���O��؂�
	if ((next_pos_ - target_pos_).length() < FLT_EPSILON) {
		is_find_target_ = false;
	}
}