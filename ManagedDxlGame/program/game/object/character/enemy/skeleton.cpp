#include "../../../../dxlib_ext/dxlib_ext.h"
#include "../../../manager/gm_manager.h"
#include "../../../manager/resource_manager.h"
#include "../../../common/camera.h"
#include "../../../scene/scene_play.h"
#include "../../projectile.h"
#include "skeleton.h"

namespace {
	// �������̃X�e�[�^�X�f�[�^��CSV�p�X
	const std::string SKELETON_DATA_CSV_PATH = "csv/enemy_data/skeleton_data.csv";

	// 
	const std::string SKELETON_ATK_SE_HDL_PATH = "sound/swing.mp3";
};

// =====================================================================================
// �R���X�g���N�^
// =====================================================================================
Skeleton::Skeleton() : sequence_(tnl::Sequence<Skeleton>(this, &Skeleton::seqIdle)), bone_(std::make_shared<Projectile>()), bone_gpc_hdl_(0) {

	// ���\�[�X�}�l�[�W���[�̃C���X�^���X���擾
	auto rm_instance = ResourceManager::getInstance();

	// �X�P���g���̉摜
	CsvData& gpc_hdl_data = rm_instance->loadCsvData("csv/enemy_gpc_data.csv");

	chara_gpc_hdl_.resize(static_cast<int>(eDir_4::MAX));

	// �L�����̊e�����̉摜�f�[�^�� "chara_gpc_hdl_" �ɕۑ��B
	int gpc_index = std::underlying_type<eEnemyType>::type(eEnemyType::SKELETON) + 1;

	for (int i = 1; i < gpc_hdl_data[gpc_index].size(); i++) {

		chara_gpc_hdl_[i - 1].resize(CHARA_GPC_X_NUM);

		chara_gpc_hdl_[i - 1] = rm_instance->loadAnimation
		(gpc_hdl_data[gpc_index][i].getString(),
			CHARA_GPC_MAX_NUM,
			CHARA_GPC_X_NUM,
			CHARA_GPC_Y_NUM,
			GameManager::CHIP_SIZE,
			GameManager::CHIP_SIZE
		);
	}

	// �������̉摜�����[�h
	bone_gpc_hdl_ = rm_instance->loadGraph("graphics/bone.png");
	bone_->setProjectileGpcHdl(bone_gpc_hdl_);

	// �X�e�[�^�X�f�[�^�� CSV ����擾
	CsvData& status_data = rm_instance->loadCsvData(SKELETON_DATA_CSV_PATH);

	// �G�̊e�X�e�[�^�X���Z�b�g
	name_ = status_data[1][0].getString();
	status_.setStatus(
		status_data[1][1].getInt(),		// ���x�� 
		status_data[1][2].getInt(),		// HP
		status_data[1][3].getInt(),		// MP
		status_data[1][4].getInt(),		// ATK
		status_data[1][5].getInt(),		// DEF
		status_data[1][6].getInt()		// EXP
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

	if (bone_->isEnable()) {
		bone_->update(delta_time);
	}
}

// =====================================================================================
// �`��
// =====================================================================================
void Skeleton::draw(const std::shared_ptr<Camera> camera) {

	if (bone_->isEnable()) {
		bone_->draw(camera);
	}

	// �`��ʒu����
	tnl::Vector3 draw_pos = tnl::Vector3(pos_.x * GameManager::DRAW_CHIP_SIZE, pos_.y * GameManager::DRAW_CHIP_SIZE, 0)
		- camera->getPos() + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);

	DrawExtendGraph(draw_pos.x, draw_pos.y, draw_pos.x + GameManager::DRAW_CHIP_SIZE, draw_pos.y + GameManager::DRAW_CHIP_SIZE,
		chara_gpc_hdl_[static_cast<int>(anim_dir_)][0], true);
}

// =====================================================================================
// �`��
// =====================================================================================
void Skeleton::drawEffect(const std::shared_ptr<Camera> camera) {

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
	// ���\�[�X�}�l�[�W���[�̃C���X�^���X���擾
	auto rm_instance = ResourceManager::getInstance();

	// �X�e�[�^�X�f�[�^�� CSV ����擾
	CsvData& status_data = rm_instance->loadCsvData(SKELETON_DATA_CSV_PATH);

	if (lv >= status_data.size()) {
		tnl::DebugTrace("lv �̒l�� CSV�f�[�^�̃T�C�Y�𒴂��Ă��܂�. ���͒l�F%d\n", lv);
		return;
	}

	// �G�̊e�X�e�[�^�X���Z�b�g
	name_ = status_data[lv][0].getString();
	status_.setStatus(
		status_data[lv][1].getInt(),		// ���x�� 
		status_data[lv][2].getInt(),		// HP
		status_data[lv][3].getInt(),		// MP
		status_data[lv][4].getInt(),		// ATK
		status_data[lv][5].getInt(),		// DEF
		status_data[lv][6].getInt()			// EXP
	);
}

// =====================================================================================
// �w�肵���ڕW�ʒu�ɃL�����N�^�[���ړ�������
// =====================================================================================
void Skeleton::moveToTargetPos(const tnl::Vector3& target_pos) {
	next_pos_ = target_pos;
	act_state_ = eActState::MOVE;
	sequence_.change(&Skeleton::seqMove);
}

// =====================================================================================
// ���x�����オ�邩����
// =====================================================================================
bool Skeleton::canLevelUp() {
	return false;
}

// =====================================================================================
// �s�������߂�
// =====================================================================================
void Skeleton::decideAction() {

	auto scene_play = scene_play_.lock();
	if (scene_play == nullptr) {
		return;
	}

	// �ʘH�ɂ���ꍇ�A�ʘH�ł̍s���������s��
	if (scene_play->getPlace(pos_) == ePlace::CORRIDOR) {
		// ���� 8�}�X�Ƀv���C���[�����邩�m�F
		eDir_8 player_dir = findPlayerDir_8();
		if (player_dir != eDir_8::NONE) {
			target_pos_ = pos_ + DIR_POS[std::underlying_type<eDir_8>::type(player_dir)];
			bone_->setupToLaunchProjectile(pos_, player_dir, 32);
			changeToAttackAction(player_dir);
			return;
		}
		actionInCorridor();
		return;
	}

	// �v���C���[�����������ɂ����ꍇ
	if (isSameRoomToPlayer()) {

		is_find_player_ = true;
		target_pos_ = scene_play->getPlayer()->getNextPos();
		eDir_8 player_dir = eDir_8::NONE;

		// �������U�����ł��邩����
		if (tryCanBoneAttack(player_dir)) {
			bone_->setupToLaunchProjectile(pos_, player_dir, 32);
			changeToAttackAction(player_dir);
			return;
		}

		trackingPlayer();
		return;
	}

	// �����ł̍s���������s��
	actionInRoom();
}

// =====================================================================================
// �U���J�n
// =====================================================================================
void Skeleton::startAttack() {
	
	bone_->startToLaunchProjectile();
	ResourceManager::getInstance()->playSound(SKELETON_ATK_SE_HDL_PATH, DX_PLAYTYPE_BACK);
	sequence_.change(&Skeleton::seqAttack);
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
	}
}

// =====================================================================================
// ���x���A�b�v�������s��
// =====================================================================================
void Skeleton::startLevelUp() {

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

	if (attack_time_ > ATTACK_TIME_MAX && !bone_->isEnable()) {
		
		auto scene_play = scene_play_.lock();
		if (scene_play == nullptr) {
			tnl::DebugTrace("�U���𐳏�Ɏ��s�ł��܂���ł���\n");
			act_state_ = eActState::END;
			sequence_.change(&Skeleton::seqIdle);
			return true;
		}

		scene_play->addAttackTarget(scene_play->getPlayer());
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
// ���𓊂���U�����ł��邩���肷��
// ���g�̒�����ɂ���ꍇ�� "true"�A�����łȂ���� "false" ��Ԃ��B
// =====================================================================================
bool Skeleton::tryCanBoneAttack(eDir_8& dir) {

	float dx = target_pos_.x - pos_.x;
	float dy = target_pos_.y - pos_.y;

	// �ʒu���d�Ȃ��Ă���ꍇ�A�G���[
	if (abs(dx) < FLT_EPSILON && abs(dy) < FLT_EPSILON) {
		tnl::DebugTrace("error�F�v���C���[�ƈʒu���d�Ȃ��Ă��܂�\n");
		return false;
	}

	// ------------- �΂ߕ����ɂ���ꍇ -------------
	if (abs(abs(dx) - abs(dy)) < FLT_EPSILON) {
		// ���ɂ���ꍇ�A���ォ�����𔻒肵�A�v���C���[��������ɂ��邩�m�F����B
		if (dx < 0 && dy < 0) {
			if (isPlayerDir(pos_, eDir_8::UP_LEFT, true) ) {
				dir = eDir_8::UP_LEFT;
				return true;
			} 
		}
		else if (dx < 0 && dy > 0) {
			if (isPlayerDir(pos_, eDir_8::DOWN_LEFT, true)) {
				dir = eDir_8::DOWN_LEFT;
				return true;
			}
		}
		else if (dx > 0 && dy < 0) {
			if (isPlayerDir(pos_, eDir_8::UP_RIGHT, true)) {
				dir = eDir_8::UP_RIGHT;
				return true;
			}
		}
		// �E�ɂ���ꍇ�A�E�ォ�E���𔻒肵�A�v���C���[��������ɂ��邩�m�F����B
		else {
			if (isPlayerDir(pos_, eDir_8::DOWN_RIGHT, true)) {
				dir = eDir_8::DOWN_RIGHT;
				return true;
			}
		}
	}
	// ------------- �㉺�ɂ���ꍇ -------------
	else if (abs(dx) < FLT_EPSILON) {
		if (dy < 0) {
			if (isPlayerDir(pos_, eDir_8::UP, true)) {
				dir = eDir_8::UP;
				return true;
			}
		}
		else {
			if (isPlayerDir(pos_, eDir_8::DOWN, true)) {
				dir = eDir_8::DOWN;
				return  true;
			}
		}
	}
	// ------------- ���E�ɂ���ꍇ -------------
	else if (abs(dy) < FLT_EPSILON) {
		if (dx < 0) {
			if (isPlayerDir(pos_, eDir_8::LEFT, true)) {
				dir = eDir_8::LEFT;
				return true;
			}
		}
		else {
			if (isPlayerDir(pos_, eDir_8::RIGHT, true)) {
				dir = eDir_8::RIGHT;
				return true;
			}
		}
		
	}

	dir = eDir_8::NONE;
	return false;
}

// =====================================================================================
// �v���C���[�����邩�m�F����B
// =====================================================================================
bool Skeleton::isPlayerDir(const tnl::Vector3& pos, eDir_8 dir, bool is_loop) const {

	// �m�F����}�X�̈ʒu
	tnl::Vector3 check_pos = pos + DIR_POS[std::underlying_type<eDir_8>::type(dir)];

	auto scene_play = scene_play_.lock();
	if (!scene_play) {
		tnl::DebugTrace("canActionToCell�֐�������ɓ���ł��܂���ł���\n");
		return false;
	}

	// �����Ŗ����Ȃ����ꍇ�A����Ɏ��̃}�X�Ƀv���C���[�����邩�m�F�ɔ����Ԃ��B
	if ( scene_play->getPlace( check_pos ) != ePlace::ROOM ) {
		return scene_play->getMapData( check_pos ) == eMapData::PLAYER;
	}

	if ( scene_play->getMapData( check_pos ) == eMapData::WALL) {
		return false;
	}
	if (scene_play->getMapData(check_pos) == eMapData::ENEMY) {
		return false;
	}

	if ( scene_play->getMapData( check_pos ) == eMapData::PLAYER) {
		return true;
	}

	// �ċA�����邩
	if (is_loop) {
		return isPlayerDir( check_pos, dir, true);
	}

	return false;
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

	// �v���C���[���������ꍇ�A�ǐՂ��s��
	if (is_find_player_) {
		trackingPlayer();
		return;
	}

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

	// �v���C���[���������ꍇ�A�ǐՂ��s��
	if (is_find_player_) {
		trackingPlayer();
		return;
	}

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