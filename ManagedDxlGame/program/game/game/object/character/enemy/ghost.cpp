#include "../../../../dxlib_ext/dxlib_ext.h"
#include "../../../manager/gm_manager.h"
#include "../../../manager/resource_manager.h"
#include "../../../common/camera.h"
#include "../../../common/animation.h"
#include "../../../scene/scene_play.h"
#include "ghost.h"

namespace
{
	const std::string GHOST_DATA_CSV_PATH = "csv/enemy_data/ghost_data.csv";
};


Ghost::Ghost() {

	// ���\�[�X�}�l�[�W���[�̃C���X�^���X���擾
	auto rm_instance = ResourceManager::getInstance();

	CsvData& gpc_hdl_data = rm_instance->loadCsvData("csv/enemy_data/enemy_gpc_data.csv");

	chara_gpc_hdl_.resize(static_cast<int>(eDir_4::MAX));

	// �L�����̊e�����̉摜�f�[�^�� "chara_gpc_hdl_" �ɕۑ��B
	int gpc_index = std::underlying_type<eEnemyType>::type(eEnemyType::PUMPKIN) + 3;

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

	// ------------------- �L�����N�^�[�A�j���[�V�����̐ݒ� -------------------------------------------------------

	chara_animation_ = std::make_shared<Animation>();
	chara_animation_->setAnimGraphicHandle(chara_gpc_hdl_[std::underlying_type<eDir_4>::type(anim_dir_)]);
	chara_animation_->setLoopAnimation(true);
	chara_animation_->setFrameChangeInterval(CHARA_ANIM_INTERVAL);
	chara_animation_->startAnimation();

	// ------------------- �G�t�F�N�g�̐ݒ� -----------------------------------------------------------------------

	CsvData& effect_data = rm_instance->loadCsvData("csv/effect_gpc_data.csv");

	std::vector<tnl::CsvCell> start_cell = effect_data[GameManager::CSV_CELL_ROW_START];

	atk_effect_ = std::make_shared<Animation>();

	atk_effect_->setAnimGraphicHandle(
		rm_instance->loadAnimation(
			start_cell[1].getString(),								// �摜�p�X,
			start_cell[2].getInt() * start_cell[3].getInt(),		// �t���[������
			start_cell[2].getInt(),									// �������̕�����
			start_cell[3].getInt(),									// �c�����̕�����
			start_cell[4].getInt(),									// �������̕����T�C�Y
			start_cell[5].getInt()									// �c�����̕����T�C�Y
		)
	);

	atk_effect_->setFrameChangeInterval(0.05f);

	// -----------------------------------------------------------------------------------------------------------

	// �X�e�[�^�X�f�[�^�� CSV ����擾
	CsvData status_data = rm_instance->loadCsvData(GHOST_DATA_CSV_PATH);

	// �G�̊e�X�e�[�^�X���Z�b�g
	name_ = status_data[GameManager::CSV_CELL_ROW_START][0].getString();
	status_.setStatus(
		status_data[GameManager::CSV_CELL_ROW_START][1].getInt(),		// ���x�� 
		status_data[GameManager::CSV_CELL_ROW_START][2].getInt(),		// HP
		status_data[GameManager::CSV_CELL_ROW_START][3].getInt(),		// MP
		status_data[GameManager::CSV_CELL_ROW_START][4].getInt(),		// ATK
		status_data[GameManager::CSV_CELL_ROW_START][5].getInt(),		// DEF
		status_data[GameManager::CSV_CELL_ROW_START][5].getInt()		// EXP
	);

	anim_dir_ = eDir_4::DOWN;
	act_state_ = eActState::IDLE;

	is_alive_ = true;
}

Ghost::~Ghost() {
	tnl::DebugTrace("Pumpkin�̃f�X�g���N�^�����s����܂����B\n");
}

// =====================================================================================
// �A�b�v�f�[�g
// =====================================================================================
void Ghost::update(float delta_time) {

	chara_animation_->setAnimGraphicHandle(chara_gpc_hdl_[std::underlying_type<eDir_4>::type(anim_dir_)]);
	chara_animation_->update(delta_time);
	sequence_.update(delta_time);
}

// =====================================================================================
// �`��
// =====================================================================================
void Ghost::draw(const std::shared_ptr<Camera> camera) {

	if (!is_drawing_) {
		return;
	}

	// �`��ʒu����
	tnl::Vector2i draw_pos = tnl::Vector2i(static_cast<int>(pos_.x * GameManager::DRAW_CHIP_SIZE), static_cast<int>(pos_.y * GameManager::DRAW_CHIP_SIZE))
		+ tnl::Vector2i(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1);

	chara_animation_->setDrawPos(draw_pos);
	chara_animation_->setDrawSize(tnl::Vector2i(GameManager::DRAW_CHIP_SIZE, GameManager::DRAW_CHIP_SIZE));
	chara_animation_->draw(camera);
}

// =====================================================================================
// �`��
// =====================================================================================
void Ghost::drawEffect(const std::shared_ptr<Camera> camera) {

	atk_effect_->draw(camera);
}

// =====================================================================================
// �N���[���𐶐�����
// =====================================================================================
std::shared_ptr<EnemyBase> Ghost::createClone() const {

	std::shared_ptr<Ghost> clone = std::make_shared<Ghost>();
	return clone;
}

// =====================================================================================
// �G�̋�����ݒ肷��
// =====================================================================================
void Ghost::setEnemyLevel(int lv) {

	if (lv < GameManager::CSV_CELL_ROW_START) {
		tnl::DebugTrace("lv �� 1�ȏ�̒l���Z�b�g���Ă�������. ���͒l�F%d\n", lv);
		return;
	}

	// ���\�[�X�}�l�[�W���[�̃C���X�^���X���擾
	auto rm_instance = ResourceManager::getInstance();

	// �X�e�[�^�X�f�[�^�� CSV ����擾
	CsvData& status_data = rm_instance->loadCsvData(GHOST_DATA_CSV_PATH);

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
void Ghost::moveToTargetPos(const tnl::Vector3& target_pos) {

}

// =====================================================================================
// ���x�����オ�邩����
// =====================================================================================
bool Ghost::canLevelUp() {
	return false;
}

// =====================================================================================
// �s�������߂�
// =====================================================================================
void Ghost::decideAction() {

	auto scene_play = scene_play_.lock();
	if (scene_play == nullptr) {
		return;
	}

	target_pos_ = scene_play->getPlayer()->getNextPos();

	// ���� 8�}�X�Ƀv���C���[�����邩�m�F
	eDir_8 player_dir = findPlayerDir_8();
	if (player_dir != eDir_8::NONE) {
		changeToAttackAction(player_dir);
		return;
	}

	setNextPosToTarget();
}

// =====================================================================================
// �U�����J�n����
// =====================================================================================
void Ghost::startAttack() {

	tnl::Vector2i attack_pos = tnl::Vector2i(static_cast<int>(pos_.x), static_cast<int>(pos_.y))
		+ tnl::Vector2i(
			static_cast<int>(DIR_POS[std::underlying_type<eDir_8>::type(looking_dir_)].x),
			static_cast<int>(DIR_POS[std::underlying_type<eDir_8>::type(looking_dir_)].y)
		);

	tnl::Vector2i effect_draw_pos = attack_pos * GameManager::DRAW_CHIP_SIZE
		+ tnl::Vector2i(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1);

	tnl::Vector2i effect_draw_size = tnl::Vector2i(GameManager::DRAW_CHIP_SIZE, GameManager::DRAW_CHIP_SIZE);

	atk_effect_->setDrawPos(effect_draw_pos);
	atk_effect_->setDrawSize(effect_draw_size);
	atk_effect_->startAnimation();
	ResourceManager::getInstance()->playSound("sound/se/physical_attack1.mp3", DX_PLAYTYPE_BACK);

	sequence_.change(&Ghost::seqAttack);
}

// =====================================================================================
// �s�����J�n����
// =====================================================================================
void Ghost::beginAction() {

	if (!sequence_.isComparable(&Ghost::seqIdle)) {
		tnl::DebugTrace("sequence_ ��Idle�ɂȂ��Ă��܂���\n");
		return;
	}
	if (act_state_ == eActState::MOVE) {
		sequence_.immediatelyChange(&Ghost::seqMove);
		return;
	}
}

// =====================================================================================
// ���x���A�b�v�������s��
// =====================================================================================
void Ghost::startLevelUp() {

}

void Ghost::takeDamage(int damage) {

	status_.takeDamage(damage);

	is_take_damage_ = true;
	sequence_.change(&Ghost::seqTakeDamage);
}

// =====================================================================================
// ���M���f�X������
// =====================================================================================
void Ghost::death() {
	is_alive_ = false;
}

// =====================================================================================
// �ҋ@�V�[�P���X
// =====================================================================================
bool Ghost::seqIdle(const float delta_time) {

	return true;
}

// =====================================================================================
// next_pos_ �Ɉړ�������B
// =====================================================================================
bool Ghost::seqMove(const float delta_time) {

	if (abs(next_pos_.y - pos_.y) > 0.2f || abs(next_pos_.x - pos_.x) > 0.2f) {
		pos_ += DIR_POS[std::underlying_type<eDir_8>::type(looking_dir_)] * MOVE_SPEED * delta_time;
	}
	else {
		pos_ = next_pos_;
		act_state_ = eActState::END;
		sequence_.change(&Ghost::seqIdle);
	}

	return true;
}

// ====================================================
// �U���V�[�P���X
// ====================================================
bool Ghost::seqAttack(const float delta_time) {

	atk_effect_->update(delta_time);

	if (atk_effect_->isEnable()) {
		return true;
	}

	int sound_hdl = ResourceManager::getInstance()->loadSound("sound/se/physical_attack2.mp3");

	if (CheckSoundMem(sound_hdl)) {
		return true;
	}

	auto scene_play = scene_play_.lock();
	if (scene_play == nullptr) {
		tnl::DebugTrace("�U���𐳏�Ɏ��s�ł��܂���ł���\n");
		act_state_ = eActState::END;
		sequence_.change(&Ghost::seqIdle);
	}

	scene_play->addAttackTarget(scene_play->getPlayer());
	attack_time_ = 0.0f;
	act_state_ = eActState::END;
	sequence_.change(&Ghost::seqIdle);
	return true;
}

// ====================================================
// �_���[�W���󂯂�V�[�P���X
// ====================================================
bool Ghost::seqTakeDamage(const float delta_time) {

	damage_production_elapsed_ += delta_time;

	if (sequence_.getProgressTime() < DAMAGE_EFFECT_CHANGE_INTERVAL * damage_production_count_) {
		return true;
	}

	++damage_production_count_;
	is_drawing_ = !is_drawing_;

	if (damage_production_count_ < 7) {
		return true;
	}

	is_take_damage_ = false;
	is_drawing_ = true;
	damage_production_count_ = 0;
	sequence_.change(&Ghost::seqIdle);

	if (status_.getHP() <= 0) {
		is_alive_ = false;
	}

	return true;
}

// =====================================================================================
// "next_pos_" ��ڕW�̈ʒu�Ɍ������� 1�}�X�ړ�����
// =====================================================================================
void Ghost::setNextPosToTarget() {

	// �Z���̍X�V
	updateCellsCost();

	// �ړ�������߂�
	rand() % 2 ? decideNextPosFromCellCost() : decideNextPosFromRandom();

	// �ړ��s���ɐ؂�ւ�
	changeToMoveAction();
}

// =====================================================================================
// �e�Z���̈ړ��R�X�g�A�L���ȃZ�����X�V���s��
// =====================================================================================
void Ghost::updateCellsCost() {

	auto scene_play = scene_play_.lock();
	if (!scene_play) {
		tnl::DebugTrace("updateCellCost�֐�������ɓ���ł��܂���ł���\n");
		return;
	}

	eMapData cell_data = eMapData::NONE;

	// �e�Z���̈ʒu�A�ړ��R�X�g���v�Z���A���̈ʒu���ړ��ł��邩������Z�b�g����
	for (int i = 0; i < static_cast<int>(eDir_8::MAX); ++i) {
		cells_[i].pos_ = tnl::Vector2i(pos_.x + DIR_POS[i].x, pos_.y + DIR_POS[i].y);
		cells_[i].dir_ = static_cast<eDir_8>(i);
		cells_[i].cost_ = abs(target_pos_.x - cells_[i].pos_.x) + abs(target_pos_.y - cells_[i].pos_.y);
		cells_[i].is_enable_ = false;

		cell_data = scene_play->getMapData(tnl::Vector3(cells_[i].pos_.x, cells_[i].pos_.y, 0));

		if (cell_data == eMapData::NONE || cell_data == eMapData::PLAYER || cell_data == eMapData::ENEMY) {
			continue;
		}

		cells_[i].is_enable_ = true;
	}
}

// =====================================================================================
// �e�Z���̈ړ��R�X�g���玟�̈ʒu�����߂�
// =====================================================================================
void Ghost::decideNextPosFromCellCost() {

	int index = getMinimunScoreCellIndex();

	// �L���ȃZ�������݂��Ă����ꍇ
	if (index != -1) {
		next_pos_ = tnl::Vector3(cells_[index].pos_.x, cells_[index].pos_.y, 0);
		anim_dir_ = ANIM_DIR[index];
		looking_dir_ = static_cast<eDir_8>(index);
	}
	else {
		next_pos_ = pos_;
	}

}

// =====================================================================================
// �e�Z���̈ړ��R�X�g���玟�̈ʒu�����߂�
// =====================================================================================
void Ghost::decideNextPosFromRandom() {

	std::vector<CellCost> moveable_cells;
	int count = 0;

	for (int i = 0; i < std::underlying_type<eDir_8>::type(eDir_8::MAX); ++i) {
		if (!cells_[i].is_enable_) {
			continue;
		}
		moveable_cells.emplace_back(cells_[i]);
		++count;
	}

	if (moveable_cells.size() <= 0) {
		next_pos_ = pos_;
		return;
	}

	int index = rand() % count;

	next_pos_ = tnl::Vector3(moveable_cells[index].pos_.x, moveable_cells[index].pos_.y, 0);
	anim_dir_ = ANIM_DIR[std::underlying_type<eDir_8>::type(moveable_cells[index].dir_)];
	looking_dir_ = moveable_cells[index].dir_;
}

