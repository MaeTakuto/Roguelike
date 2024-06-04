#include <queue>
#include "../../dxlib_ext/dxlib_ext.h"
#include "../manager/gm_manager.h"
#include "../manager/resource_manager.h"
#include "../dungeon/dungeon_manager.h"
#include "../manager/enemy_manager.h"
#include "../base/enemy_base.h"
#include "../manager/ui_manager.h"
#include "../common/camera.h"
#include "../base/character_base.h"
#include "../object/character/player.h"
#include "scene_title.h"
#include "scene_game_clear.h"
#include "scene_play.h"


namespace {
	// �K�w�ړ����̃_���W�������̕\������
	const float DRAW_TIME_DUNGEON_NAME = 2.0f;

	// �_���W�����̖��O
	const std::string DUNGEON_NAME = "�s�v�c�ȐX";

	// �_���W�����^�C�g���̈ʒu
	const tnl::Vector3 DUNGEON_NAME_POS = { 450, 100, 0 };

	// �^�C�g���̃t�H���g�T�C�Y
	const int DUNGEON_NAME_FONT_SIZE = 60;

	// �N���A�K��
	const int CLEAR_FLOOR = 5;

	// ���b�Z�[�W�̕\������
	const float MESSAGE_DRAW_TIME = 3.0f;
}

ScenePlay::ScenePlay() : camera_(nullptr), player_(nullptr), dungeon_mgr_(nullptr),  enemy_mgr_(nullptr), ui_mgr_(nullptr), 
	dungeon_floor_(1), is_created_dungeon_(false), is_drawing_dng_title_(true), is_game_clear_(false),
	is_display_mini_map_(true), mini_map_pos_(750, 180), mini_map_size_(8), mini_map_cell_gpc_hdl_(0),
	is_opened_menu_(false), fade_gpc_hdl_(0), alpha_(0), fade_time_(0.5f),
	level_up_character_(nullptr), dungeon_bgm_hdl_(0), dungeon_bgm_hdl_path_("sound/dungeon02.mp3"),
	damage_se_hdl_path_("sound/damaged.mp3"), open_select_window_se_hdl_path_("sound/springin/open_window.mp3"), level_up_se_hdl_path_("sound/springin/level_up.mp3"), 
	button_enter_se_hdl_path_("sound/button_enter.mp3"), cancel_se_hdl_path_("sound/springin/cancel.mp3")
{

	tnl::DebugTrace("ScenePlay�̃R���X�g���N�^�����s����܂���\n");
	// SetBackgroundColor(0, 0, 0);

	// ------------------- �e�N���X�̐��� ------------------------------
	player_	= std::make_shared<Player>();
	dungeon_mgr_ = std::make_shared<DungeonManager>();
	camera_	= std::make_shared<Camera>();
	enemy_mgr_ = std::make_shared<EnemyManager>();
	ui_mgr_ = std::make_shared<UI_Manager>();

	// --------------- �t�B�[���h�T�C�Y�̏�����-------------------------
	field_.resize(GameManager::FIELD_HEIGHT);
	mini_map_.resize(GameManager::FIELD_HEIGHT);
	areas_.resize(dungeon_mgr_->AREA_MAX);

	for (int i = 0; i < GameManager::FIELD_HEIGHT; i++) {
		field_[i].resize(GameManager::FIELD_WIDTH);
		mini_map_[i].resize(GameManager::FIELD_WIDTH);
	}

	// -------------- �摜�̃��[�h -------------------------------------
	gpc_hdl_data_ = tnl::LoadCsv("csv/dungeon_chip_data.csv");

	mapchip_gpc_hdl_ =
		ResourceManager::getInstance()->loadAnimation(
			gpc_hdl_data_[GameManager::CSV_CELL_ROW_START][0].getString(),
			gpc_hdl_data_[GameManager::CSV_CELL_ROW_START][1].getInt(),
			gpc_hdl_data_[GameManager::CSV_CELL_ROW_START][2].getInt(),
			gpc_hdl_data_[GameManager::CSV_CELL_ROW_START][3].getInt(),
			GameManager::CHIP_SIZE,
			GameManager::CHIP_SIZE
		);

	mini_map_cell_gpc_hdl_ = ResourceManager::getInstance()->loadGraph("graphics/blue.bmp");

	fade_gpc_hdl_ = ResourceManager::getInstance()->loadGraph("graphics/black.bmp");

	// -------------- �T�E���h�̃��[�h -----------------------------------
	dungeon_bgm_hdl_ = ResourceManager::getInstance()->loadSound(dungeon_bgm_hdl_path_);
	ResourceManager::getInstance()->loadSound(damage_se_hdl_path_);
	ResourceManager::getInstance()->loadSound(open_select_window_se_hdl_path_);
	ResourceManager::getInstance()->loadSound(level_up_se_hdl_path_);
	ResourceManager::getInstance()->loadSound(cancel_se_hdl_path_);

	// ------------- UI_Manager�N���X�̏����ݒ� -------------------------
	ui_mgr_->setUITargetCharacter(player_);
	ui_mgr_->updateStatusBar();
	ui_mgr_->setFloor(dungeon_floor_);
}

ScenePlay::~ScenePlay() {

	tnl::DebugTrace("ScenePlay�̃f�X�g���N�^�����s����܂���\n");

	StopSoundMem(dungeon_bgm_hdl_);

	ResourceManager::getInstance()->deleteSound(dungeon_bgm_hdl_path_);
	ResourceManager::getInstance()->deleteSound(damage_se_hdl_path_);
	ResourceManager::getInstance()->deleteSound(open_select_window_se_hdl_path_);
	ResourceManager::getInstance()->deleteSound(level_up_se_hdl_path_);
	
	// �摜�̍폜
	ResourceManager::getInstance()->deleteAnimation(
		gpc_hdl_data_[1][0].getString()
	);
}

// ====================================================
// �A�b�v�f�[�g
// ====================================================
void ScenePlay::update(float delta_time) {

	main_seq_.update(delta_time);

	/*int freqency = GetCurrentPositionSoundMem(dungeon_bgm_hdl_);
	if (freqency > bgm_end_freqency_)
	{
		StopSoundMem(dungeon_bgm_hdl_);
		SetCurrentPositionSoundMem(0, dungeon_bgm_hdl_);
		PlaySoundMem(dungeon_bgm_hdl_, DX_PLAYTYPE_LOOP, false);
	}*/
}

// ====================================================
// �`��
// ====================================================
void ScenePlay::draw() {

	// �_���W�����^�C�g����\�����Ă��邩
	if (is_drawing_dng_title_) {
		SetFontSize(DUNGEON_NAME_FONT_SIZE);
		DrawStringEx(DUNGEON_NAME_POS.x, DUNGEON_NAME_POS.y, -1, DUNGEON_NAME.c_str());
		DrawStringEx(DUNGEON_NAME_POS.x + 150, DUNGEON_NAME_POS.y + 60, -1, "%dF", dungeon_floor_);
	}
	else {
		// �}�b�v�`�b�v�`��
		for (int y = 0; y < field_.size(); ++y) {
			for (int x = 0; x < field_[y].size(); ++x) {
				tnl::Vector3 draw_pos = tnl::Vector3{ static_cast<float>(x) * GameManager::DRAW_CHIP_SIZE, static_cast<float>(y) * GameManager::DRAW_CHIP_SIZE, 0 }
				- camera_->getPos() + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);

				DrawExtendGraph(draw_pos.x, draw_pos.y,
					draw_pos.x + GameManager::DRAW_CHIP_SIZE,
					draw_pos.y + GameManager::DRAW_CHIP_SIZE,
					mapchip_gpc_hdl_[0], true);
				if (field_[y][x].terrain_data == eMapData::WALL) {
					DrawExtendGraph(draw_pos.x, draw_pos.y,
						draw_pos.x + GameManager::DRAW_CHIP_SIZE,
						draw_pos.y + GameManager::DRAW_CHIP_SIZE,
						mapchip_gpc_hdl_[1], true);
				}
				else if (field_[y][x].terrain_data == eMapData::STAIR) {
					DrawExtendGraph(draw_pos.x, draw_pos.y,
						draw_pos.x + GameManager::DRAW_CHIP_SIZE, draw_pos.y + GameManager::DRAW_CHIP_SIZE,
						mapchip_gpc_hdl_[2], true);
				}
			}
		}
		
		player_->draw(camera_);
		enemy_mgr_->draw(camera_);

		player_->drawEffect(camera_);

		if (is_display_mini_map_ && !is_opened_menu_) {
			drawMiniMap();
		}
		ui_mgr_->draw(camera_);

		SetFontSize(25);
		DrawStringEx(10, 25, -1, "ESC�F���j���[���J��");
		DrawStringEx(10, 50, -1, "TAB�F�}�b�v�����");
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);
	DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, fade_gpc_hdl_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	// ======= �f�o�b�O ========
	 // DrawStringEx(100, 100, -1, "�V�[���v���C");
	 //dungeon_mgr_->displayAreaNumber(camera_);
}

// �~�j�}�b�v�̃}�X���J��
void ScenePlay::openMiniMapCell() {
	
	int player_x = static_cast<int>(player_->getPos().x);
	int player_y = static_cast<int>(player_->getPos().y);

	mini_map_[player_y][player_x].is_display_cell = true;
	mini_map_[player_y][player_x].map_data = field_[player_y][player_x].map_data;

	if ( getPlace(player_->getPos()) == ePlace::CORRIDOR ) {
		for ( int i = 0; i < std::underlying_type<eDir_8>::type(eDir_8::MAX); ++i ) { 
			int x = player_x + DIR_POS[i].x;
			int y = player_y + DIR_POS[i].y;
			
			mini_map_[y][x].is_display_cell = true;
			mini_map_[y][x].map_data = field_[y][x].map_data;
		}
	}
	else {
		int area_id = getAreaId(player_->getPos());

		for (int y = areas_[area_id].room.y - 1; y < areas_[area_id].room.y + areas_[area_id].room.height + 1; ++y) {
			for (int x = areas_[area_id].room.x - 1; x < areas_[area_id].room.x + areas_[area_id].room.width + 1; ++x) {
				mini_map_[y][x].is_display_cell = true;
				mini_map_[y][x].map_data = field_[y][x].map_data;
			}
		}

	}
}

void ScenePlay::updateMiniMap() {

	for (int y = 0; y < mini_map_.size(); ++y) {
		for (int x = 0; x < mini_map_[y].size(); ++x) {
			mini_map_[y][x].map_data = field_[y][x].terrain_data;
		}
	}

	openMiniMapCell();

}

// ====================================================
// �~�j�}�b�v��`�悷��
// ====================================================
void ScenePlay::drawMiniMap() {

	for (int y = 0; y < mini_map_.size(); ++y) {
		for (int x = 0; x < mini_map_[y].size(); ++x) {

			if (!mini_map_[y][x].is_display_cell) {
				continue;
			}

			tnl::Vector2i draw_pos = mini_map_pos_ + tnl::Vector2i(x, y) * mini_map_size_;

			if (mini_map_[y][x].terrain_data == eMapData::GROUND || mini_map_[y][x].terrain_data == eMapData::STAIR) {

				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
				DrawBox(draw_pos.x, draw_pos.y, draw_pos.x + mini_map_size_, draw_pos.y + mini_map_size_, GetColor(0, 0, 255), true);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
			}

			if (mini_map_[y][x].map_data == eMapData::PLAYER) {
				draw_pos = mini_map_pos_ + tnl::Vector2i(x, y) * mini_map_size_ + tnl::Vector2i( mini_map_size_ >> 1, mini_map_size_ >> 1);
				DrawCircle(draw_pos.x, draw_pos.y, ( mini_map_size_ >> 1 ) - 1, GetColor(255, 255, 0), true, true);
			}
			else if (mini_map_[y][x].map_data == eMapData::ENEMY) {
				draw_pos = mini_map_pos_ + tnl::Vector2i(x, y) * mini_map_size_ + tnl::Vector2i(mini_map_size_ >> 1, mini_map_size_ >> 1);
				DrawCircle(draw_pos.x, draw_pos.y, (mini_map_size_ >> 1) - 1, GetColor(255, 0, 0), true, true);
			}
			if (mini_map_[y][x].terrain_data == eMapData::STAIR) {
				tnl::Vector2i draw_pos = mini_map_pos_ + tnl::Vector2i(x, y) * mini_map_size_;
				DrawBox(draw_pos.x, draw_pos.y, draw_pos.x + mini_map_size_, draw_pos.y + mini_map_size_, -1, false);
			}
		}
	}
}

// ====================================================
// �L�����N�^�[�A�b�v�f�[�g
// ====================================================
void ScenePlay::charaUpdate(float delta_time) {

	player_->update(delta_time);
	enemy_mgr_->update(delta_time);
}

// ====================================================
// ���j���[��ʂ����
// ====================================================
void ScenePlay::closeMainMenu() {

	is_opened_menu_ = false;
	ui_mgr_->closeMainMenu();
}

// ====================================================
// �G�̍s�����e���C������
// ====================================================
void ScenePlay::modifyEnemyAction() {

	while (!attackers_.empty()) {
		attackers_.pop();
	}
	enemy_mgr_->modifyEnemiesAction();
}


// ====================================================
// �w�肵���ʒu�̓G���擾����B
// ���Ȃ���΁A"nullptr" ��Ԃ�
// ====================================================
std::shared_ptr<EnemyBase> ScenePlay::findEnemy(const tnl::Vector3& pos) {

	std::shared_ptr<EnemyBase> enemy = enemy_mgr_->findEnemy(pos);

	return enemy;
}

// ====================================================
// �U�����s���L�����N�^�[��ǉ�����
// ====================================================
void ScenePlay::addAttacker(std::shared_ptr<Character> attacker) {
	if (!attacker) {
		return;
	}
	attackers_.push(attacker);
}

// ====================================================
// �U���Ώۂ�ǉ�
// ====================================================
void ScenePlay::addAttackTarget(std::shared_ptr<Character> target) {
	if (!target) {
		return;
	}
	atk_targets_.push(target);
}

// ====================================================
// attaker �� target �Ƀ_���[�W��^����B
// ====================================================
void ScenePlay::applyDamage(std::shared_ptr<Character> attacker, std::shared_ptr<Character> target, int damage) {

	target->takeDamage(damage);

	ResourceManager::getInstance()->playSound(damage_se_hdl_path_, DX_PLAYTYPE_BACK);
	std::string message = attacker->getName() + "��" + target->getName() + "��" + std::to_string(damage) + "�_���[�W��^�����B\n";

	ui_mgr_->setMessage(message, MESSAGE_DRAW_TIME);
	tnl::DebugTrace("%d�_���[�W��^�����B\n", attacker->getStatus().getAtk());

	if (!target->isAlive()) {
		defeatCharacter(attacker, target);
	}

}

// ====================================================
// ���̃t���A�ɕς��鏈��
// ====================================================
void ScenePlay::changeProcessNextFloor() {

	ui_mgr_->executeStairSelectEnd();

	// �K�����N���A�K���������ꍇ
	if ( dungeon_floor_ >= CLEAR_FLOOR ) {
		executeGameClearProcess();
		return;
	}
	main_seq_.change(&ScenePlay::seqFadeOut);
	dungeon_sequence_.change(&ScenePlay::seqPlayerAct);
	++dungeon_floor_;
	is_created_dungeon_ = false;
}

// ====================================================
// �L�����̃��x�����オ�������̏���
// ====================================================
void ScenePlay::executeLevelUpProcess(std::shared_ptr<Character> chara) {
	ResourceManager::getInstance()->playSound(level_up_se_hdl_path_, DX_PLAYTYPE_BACK);
	chara->startLevelUp();
	level_up_character_ = chara;

	if (chara.get() == player_.get()) {
		ui_mgr_->updateStatusBar();
	}

	dungeon_sequence_.change(&ScenePlay::seqCharaLevelUp);
}

// ====================================================
// ���@���g���邩�m�F����
// ====================================================
void ScenePlay::checkToUseMagic() {

	if (player_->tryUseMagic(ui_mgr_->getSelectedIndexFromMagicListCmd())) {
		dungeon_sequence_.change(&ScenePlay::seqEnemyAct);
		player_->setupMagic();
		ui_mgr_->updateStatusBar();
	}
	else {
		ui_mgr_->setMessage("MP������Ȃ�", DRAW_TIME_DUNGEON_NAME);
		dungeon_sequence_.change(&ScenePlay::seqPlayerAct);
	}

	// ���j���[�����
	ui_mgr_->executeSletctToUseMagicEnd();
	ui_mgr_->closeMagicListWindow();
	closeMainMenu();
}

// ====================================================
// �Q�[���I�[�o�[���̏��� 
// ====================================================
void ScenePlay::executeGameOverProcess() {
	ui_mgr_->clearMessage();
	std::string message = player_->getName() + "�͂���Ă��܂���";
	ui_mgr_->setMessage(message);
	dungeon_sequence_.immediatelyChange(&ScenePlay::seqGameOver);
}

// ====================================================
// �Q�[���N���A�̏��������s����
// ====================================================
void ScenePlay::executeGameClearProcess() {
	ui_mgr_->clearMessage();
	std::string message = "�_���W�����𐧔e���܂����I";
	ui_mgr_->setMessage(message);
	is_game_clear_ = true;
	GameManager::GetInstance()->setGameClear(true);
	dungeon_sequence_.immediatelyChange(&ScenePlay::seqGameOver);
}

// ====================================================
// �v���C���[������Z�����m�F����
// �Z���ɉ����������ꍇ�Atrue��Ԃ��B
// �������K�i�ł���΁A"dungeon_sequence_"��"seqStairSelect"�ɕύX�B
// ====================================================
bool ScenePlay::checkPlayerCell() {
	// �������K�i�ł����"seqStairSelect"�Ɉړ�
	if (getTerrainData(player_->getPos()) == eMapData::STAIR) {

		dungeon_sequence_.change(&ScenePlay::seqStairSelect);
		ui_mgr_->executeStairSelect();
		ui_mgr_->closeMainMenu();
		ResourceManager::getInstance()->playSound(open_select_window_se_hdl_path_, DX_PLAYTYPE_BACK);
		return true;
	}
	dungeon_sequence_.change(&ScenePlay::seqPlayerAct);
	closeMainMenu();
	ui_mgr_->setMessage("�����Ȃ�", MESSAGE_DRAW_TIME);

	return false;
}

// ====================================================
// �L�����N�^�[��|�������̏������s�� 
// ====================================================
void ScenePlay::defeatCharacter(std::shared_ptr<Character> attacker, std::shared_ptr<Character> target) {

	attacker->addExp(target->getStatus().getExp());
	setMapData(target->getNextPos(), getTerrainData(target->getNextPos()));
	std::string message = target->getName() + "��|����";
	ui_mgr_->setMessage(message, MESSAGE_DRAW_TIME);
	tnl::DebugTrace("�|����\n");

	if (enemy_mgr_->getAliveEnemyNum() <= 0) {
		std::string message = "�G�̋C�z���Ȃ��Ȃ���";
		ui_mgr_->setMessage(message, MESSAGE_DRAW_TIME);
	}
}

// ====================================================
// �U���L������؂�ւ���B
// ���Ȃ���΁A"dungeon_sequence_"��"seqCharaMove" �ɕύX�B
// ====================================================
void ScenePlay::changeAttacker() {

	attackers_.pop();

	if (!attackers_.empty()) {
		// �U���L�������玀�S���Ă�����ċA����B
		if (!attackers_.front()->isAlive()) {
			changeAttacker();
			return;
		}
		
		if (!dungeon_sequence_.isComparable(&ScenePlay::seqCharacterAttack)) {
			dungeon_sequence_.change(&ScenePlay::seqCharacterAttack);
		}
		attackers_.front()->startAttack();
		return;
	}

	enemy_mgr_->beginActionToMove();
	dungeon_sequence_.change(&ScenePlay::seqCharaMove);
}

// ====================================================
// ���b�Z�[�W�E�B���h�E�Ƀ��b�Z�[�W���Z�b�g
// ====================================================
void ScenePlay::setMessage(const std::string& message) {
	ui_mgr_->setMessage(message, MESSAGE_DRAW_TIME);
}

// ==================================================================================
//								���C���V�[�P���X
// ==================================================================================

// ====================================================
// �V�[���v���C�J�n�����s�i ��x���� �j
// ====================================================
bool ScenePlay::seqSceneStart(const float delta_time) {

	main_seq_.change(&ScenePlay::seqGenerateDungeon);

	return true;
}

// ====================================================
// �_���W���������V�[�P���X
// ====================================================
bool ScenePlay::seqGenerateDungeon(const float delta_time) {
	
	alpha_ = 0;

	// �G���폜���A�G�e�[�u�������݃t���A�̃f�[�^�ɍX�V
	enemy_mgr_->deathAllEnemys();
	enemy_mgr_->updateEnemyDataToNowFloor(dungeon_floor_);

	// �_���W�����𐶐��ƃt�B�[���h�̐ݒ�
	dungeon_mgr_->generateDungeon();
	field_ = dungeon_mgr_->getField();
	areas_ = dungeon_mgr_->getAreas();

	ui_mgr_->setFloor(dungeon_floor_);

	// �v���C���[�A�G���X�|�[��
	for (int y = 0; y < field_.size(); y++) {
		for (int x = 0; x < field_[y].size(); x++) {

			mini_map_[y][x].is_display_cell = false;
			mini_map_[y][x].map_data		= field_[y][x].map_data;
			mini_map_[y][x].terrain_data	= field_[y][x].terrain_data;

			if (field_[y][x].map_data != eMapData::PLAYER && field_[y][x].map_data != eMapData::ENEMY) {
				continue;
			}

			if (field_[y][x].map_data == eMapData::PLAYER ) {
				tnl::DebugTrace("player x = %d, y = %d\n", x, y);
				tnl::Vector3 pos = { static_cast<float>(x) , static_cast<float>(y), 0 };
				player_->setPos(pos);
			}
			else if (field_[y][x].map_data == eMapData::ENEMY) {
				tnl::DebugTrace("enemy x = %d, y = %d\n", x, y);
				enemy_mgr_->createEnemy(tnl::Vector3{ static_cast<float>(x) , static_cast<float>(y), 0});
			}
		}
	}

	updateMiniMap();

	// �_���W�����f�[�^�̃f�o�b�O
	// debugMapData();
	// debugPlaceData();

	camera_->setPos(player_->getPos());

	is_created_dungeon_ = true;

	main_seq_.change(&ScenePlay::seqDrawDungeonTitle);

	return true;
}

// ====================================================
// �_���W��������\������V�[�P���X
// ====================================================
bool ScenePlay::seqDrawDungeonTitle(const float delta_time) {

	if (main_seq_.getProgressTime() > DRAW_TIME_DUNGEON_NAME) {
		main_seq_.change(&ScenePlay::seqFadeOut);
	}

	return true;
}

// ====================================================
// �t�F�[�h�C��
// ====================================================
bool ScenePlay::seqFadeIn(const float delta_time) {

	alpha_ = 255 - (main_seq_.getProgressTime() / fade_time_ * 255.0f);

	if (alpha_ <= 0) {
		// �_���W�����𐶐������ꍇ�́A�V�[�P���X���t�F�[�h�C���B�����łȂ��ꍇ�̓_���W�����𐶐��B
		if (is_created_dungeon_) {
			main_seq_.change(&ScenePlay::seqMain);
		}
		else {
			main_seq_.change(&ScenePlay::seqGenerateDungeon);
		}
	}

	return true;
}

// ====================================================
// �_���W�����T���V�[�P���X
// ====================================================
bool ScenePlay::seqMain(const float delta_time) {
	if (dungeon_sequence_.isStart()) {
		if (!CheckSoundMem(dungeon_bgm_hdl_)) {
			PlaySoundMem(dungeon_bgm_hdl_, DX_PLAYTYPE_LOOP, true);
		}
	}

	// camera_->control();

	 dungeon_sequence_.update(delta_time);
	 camera_->setPos(player_->getPos());
	 ui_mgr_->update(delta_time);

	return true;
}

// ====================================================
// �t�F�[�h�A�E�g
// ====================================================
bool ScenePlay::seqFadeOut(const float delta_time) {
	
	alpha_ = (main_seq_.getProgressTime() / fade_time_ * 255.0f);
	if (alpha_ >= 255) {
		if (is_created_dungeon_) is_drawing_dng_title_ = false;
		else is_drawing_dng_title_ = true;
		main_seq_.change(&ScenePlay::seqFadeIn);
	}

	return true;
}


// ==================================================================================
//								�_���W�����T���V�[�P���X
// ==================================================================================

// ====================================================
// �v���C���[�̓��̓V�[�P���X
// ====================================================
bool ScenePlay::seqPlayerAct(const float delta_time) {

	charaUpdate(delta_time);

	if (player_->getActState() != eActState::IDLE && player_->getActState() != eActState::END) {
		
		dungeon_sequence_.change(&ScenePlay::seqEnemyAct);

		setMapData(player_->getPos(), getTerrainData(player_->getPos()));
		setMapData(player_->getNextPos(), eMapData::PLAYER);
		return true;
	}

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_TAB)) {
		is_display_mini_map_ = !is_display_mini_map_;
	}

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE)) {
		is_opened_menu_ = true;
		dungeon_sequence_.change(&ScenePlay::seqSelectMainMenu);
		ui_mgr_->openMainMenu();
		ResourceManager::getInstance()->playSound(open_select_window_se_hdl_path_, DX_PLAYTYPE_BACK);
		return true;
	}

	return true;
}

// ====================================================
// �G�̍s�������肷��V�[�P���X
// ====================================================
bool ScenePlay::seqEnemyAct(const float delta_time) {

	enemy_mgr_->desideAction();

	// 
	if (player_->getActState() == eActState::USE_MAGIC) {
		dungeon_sequence_.change(&ScenePlay::seqPlayerAction);
		player_->startMagic();
		return true;
	}
	// �v���C���[���ړ�����ꍇ
	else if (player_->getActState() == eActState::MOVE) {

		// �U������G�������ꍇ�A�v���C���[�݈̂ړ�
		if (!attackers_.empty()) {
			dungeon_sequence_.change(&ScenePlay::seqPlayerAction);
			return true;
		}
		dungeon_sequence_.change(&ScenePlay::seqCharaMove);
		enemy_mgr_->beginActionToMove();
		return true;
	}
	// �v���C���[���U������ꍇ
	else if (player_->getActState() == eActState::ATTACK) {
		attackers_.front()->startAttack();
		dungeon_sequence_.change(&ScenePlay::seqCharacterAttack);
	}
	return true;
}

// ====================================================
// �v���C���[�̂ݍs��
// ====================================================
bool ScenePlay::seqPlayerAction(const float delta_time) {

	charaUpdate(delta_time);

	if (player_->getActState() != eActState::END) return true;
	ui_mgr_->updateStatusBar();

	if (player_->canLevelUp()) {
		executeLevelUpProcess(player_);
		return true;
	}
	if (attackers_.empty()) {
		dungeon_sequence_.change(&ScenePlay::seqCharaMove);
		enemy_mgr_->beginActionToMove();
		return true;
	}
	else if (!attackers_.front()->isAlive()) {
		changeAttacker();
		return true;
	}

	dungeon_sequence_.change(&ScenePlay::seqCharacterAttack);
	attackers_.front()->startAttack();
	return true;
}

// ====================================================
// �L�����N�^�[�̍U���V�[�P���X
// ====================================================
bool ScenePlay::seqCharacterAttack(const float delta_time) {

	charaUpdate(delta_time);

	// �U���҂��U�����̏ꍇ�A�����܂�
	if (attackers_.front()->getActState() != eActState::END) {
		return true;
	}

	// �U���Ώۂ�����ꍇ�A�_���[�W�����V�[�P���X�Ɉړ�
	if (!atk_targets_.empty()) {
		applyDamage(attackers_.front(), atk_targets_.front(), attackers_.front()->getStatus().getAtk());
		ui_mgr_->updateStatusBar();
		dungeon_sequence_.change(&ScenePlay::seqTargetDamaged);
		return true;
	}

	changeAttacker();
	return true;
}

// ====================================================
// �U���Ώۂ̃_���[�W�������s���V�[�P���X
// ====================================================
bool ScenePlay::seqTargetDamaged(const float delta_time) {

	charaUpdate(delta_time);

	// �U���Ώۂ�|�����Ƃ��A���̊m�F
	if (!atk_targets_.front()->isAlive()) {
		if (atk_targets_.front().get() == player_.get()) {
			executeGameOverProcess();
			return true;
		}
	}

	atk_targets_.pop();

	// �U���Ώۂ��c���Ă���ꍇ�A������x�U���Ώۂ̃_���[�W�������s��
	if (!atk_targets_.empty()) {
		applyDamage(attackers_.front(), atk_targets_.front(), attackers_.front()->getStatus().getAtk());
		ui_mgr_->updateStatusBar();
		return true;
	}

	if (attackers_.front()->canLevelUp()) {
		executeLevelUpProcess(attackers_.front());
		return true;
	}

	changeAttacker();
	return true;
}

// ====================================================
// �L�����̃��x���A�b�v���s���V�[�P���X
// ====================================================
bool ScenePlay::seqCharaLevelUp(const float delta_time) {

	charaUpdate(delta_time);

	if (level_up_character_->getActState() != eActState::END) {
		return true;
	}
	std::string message = level_up_character_->getName() + "�̓��x����" + std::to_string(level_up_character_->getStatus().getLevel()) + "�ɂȂ���";
	ui_mgr_->setMessage(message, MESSAGE_DRAW_TIME);

	if (attackers_.empty()) {
		enemy_mgr_->beginActionToMove();
		dungeon_sequence_.change(&ScenePlay::seqCharaMove);
		return true;
	}

	changeAttacker();
	return true;
}

// ====================================================
// �s�����I���Ă��Ȃ��L�����N�^�[���s��������V�[�P���X
// ====================================================
bool ScenePlay::seqCharaMove(const float delta_time) {

	charaUpdate(delta_time);

	if (player_->getActState() != eActState::END) return true;
	if (enemy_mgr_->isAllEnemyActEnd() == false) return true;

	dungeon_sequence_.change(&ScenePlay::seqActEndProcess);

	return true;
}

// ====================================================
// �^�[���I����̏����V�[�P���X
// ====================================================
bool ScenePlay::seqActEndProcess(const float delta_time) {

	charaUpdate(delta_time);
	ui_mgr_->updateStatusBar();
	player_->beginAction();
	updateMiniMap();

	tnl::DebugTrace( "Place�F%d\n", std::underlying_type<ePlace>::type( getPlace( player_->getPos() ) ) );

	// �v���C���[�̈ʒu���K�i��������
	if (getTerrainData(player_->getPos()) == eMapData::STAIR) {
		dungeon_sequence_.change(&ScenePlay::seqStairSelect);
		ui_mgr_->executeStairSelect();
		ResourceManager::getInstance()->playSound(open_select_window_se_hdl_path_, DX_PLAYTYPE_BACK);
		return true;
	}

	dungeon_sequence_.change(&ScenePlay::seqPlayerAct);
	return true;
}

// ====================================================
// �K�i�I���V�[�P���X
// ====================================================
bool ScenePlay::seqStairSelect(const float delta_time) {

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

		if (ui_mgr_->getSelectedIndexFromTwoSelectCmd() == std::underlying_type<eTwoSelectCmd>::type( eTwoSelectCmd::YES ) ) {
			ResourceManager::getInstance()->playSound(button_enter_se_hdl_path_, DX_PLAYTYPE_BACK);
			changeProcessNextFloor();
		}
		else {
			ResourceManager::getInstance()->playSound(cancel_se_hdl_path_, DX_PLAYTYPE_BACK);
			dungeon_sequence_.change(&ScenePlay::seqPlayerAct);
			ui_mgr_->executeStairSelectEnd();
		}
	}

	return true;
}

// ====================================================
// �Q�[���I�[�o�[�V�[�P���X
// ====================================================
bool ScenePlay::seqGameOver(const float delta_time) {

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		if (is_game_clear_) {
			GameManager::GetInstance()->changeScene(std::make_shared<SceneGameClear>(), 2.0f);
		}
		else {
			GameManager::GetInstance()->changeScene(std::make_shared<SceneTitle>());
		}
	}
	return true;
}

// ====================================================
// ���C�����j���[�̑I���V�[�P���X
// ====================================================
bool ScenePlay::seqSelectMainMenu(const float delta_time) {

	// ���C�����j���[�����
	if ( tnl::Input::IsKeyDownTrigger( eKeys::KB_ESCAPE ) ) {
		ResourceManager::getInstance()->playSound(cancel_se_hdl_path_, DX_PLAYTYPE_BACK);
		dungeon_sequence_.change(&ScenePlay::seqPlayerAct);
		closeMainMenu();
		return true;
	}

	// ���j���[�R�}���h�����肷��
	if ( tnl::Input::IsKeyDownTrigger( eKeys::KB_RETURN ) ) {
		// ���@�I���E�B���h�E�Ɉړ�
		if (ui_mgr_->getSelectedIndexFromMainMenuCmd() == std::underlying_type<eMainMenuCmd>::type(eMainMenuCmd::MAGIC_SELECT)) {
			ui_mgr_->openMagicListWindow();
			dungeon_sequence_.change(&ScenePlay::seqSelectMagicList);
			ResourceManager::getInstance()->playSound(button_enter_se_hdl_path_, DX_PLAYTYPE_BACK);
			return true;
		}
		// �������m�F
		else if (ui_mgr_->getSelectedIndexFromMainMenuCmd() == std::underlying_type<eMainMenuCmd>::type(eMainMenuCmd::CHECK_CELL)) {
			checkPlayerCell();
			return true;
		}
		// �X�e�[�^�X�m�F�E�B���h�E���J��
		else if (ui_mgr_->getSelectedIndexFromMainMenuCmd() == std::underlying_type<eMainMenuCmd>::type(eMainMenuCmd::CHECK_STATUS)) {
			ui_mgr_->displayStatusWindow();
			dungeon_sequence_.change(&ScenePlay::seqDrawStatusWindow);
			ResourceManager::getInstance()->playSound(button_enter_se_hdl_path_, DX_PLAYTYPE_BACK);
			return true;
		}
		// ���C�����j���[�����
		else if (ui_mgr_->getSelectedIndexFromMainMenuCmd() == std::underlying_type<eMainMenuCmd>::type(eMainMenuCmd::CLOSE)) {
			ResourceManager::getInstance()->playSound(cancel_se_hdl_path_, DX_PLAYTYPE_BACK);
			dungeon_sequence_.change(&ScenePlay::seqPlayerAct);
			closeMainMenu();
			return true;
		}
	}

	return true;
}

// ====================================================
// ���@�I���E�B���h�E�̑I���V�[�P���X
// ====================================================
bool ScenePlay::seqSelectMagicList(const float delta_time) {

	// ���C�����j���[�ɖ߂�
	if ( tnl::Input::IsKeyDownTrigger( eKeys::KB_ESCAPE ) ) {
		ui_mgr_->closeMagicListWindow();
		dungeon_sequence_.change(&ScenePlay::seqSelectMainMenu);
		ResourceManager::getInstance()->playSound(cancel_se_hdl_path_, DX_PLAYTYPE_BACK);
		return true;
	}

	// �I���������@���g�p����
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		dungeon_sequence_.change(&ScenePlay::seqSelectToUseMagic);
		ui_mgr_->executeSletctToUseMagic();
		ResourceManager::getInstance()->playSound(button_enter_se_hdl_path_, DX_PLAYTYPE_BACK);
	}

	return true;
}

// ====================================================
// �I���������@���g�p���邩�I������V�[�P���X
// ====================================================
bool ScenePlay::seqSelectToUseMagic(const float delta_time) {

	// ���C�����j���[�ɖ߂�
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE)) {
		dungeon_sequence_.change(&ScenePlay::seqSelectMagicList);
		ui_mgr_->executeSletctToUseMagicEnd();
		ResourceManager::getInstance()->playSound(cancel_se_hdl_path_, DX_PLAYTYPE_BACK);
		return true;
	}

	// �I���������@���g�p����
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		if ( ui_mgr_->getSelectedIndexFromTwoSelectCmd() == std::underlying_type<eTwoSelectCmd>::type( eTwoSelectCmd::YES ) ) {
			checkToUseMagic();
		}
		else {
			dungeon_sequence_.change(&ScenePlay::seqSelectMagicList);
			ui_mgr_->executeSletctToUseMagicEnd();
			ResourceManager::getInstance()->playSound(cancel_se_hdl_path_, DX_PLAYTYPE_BACK);
		}
	}
	return true;
}

// ====================================================
// ���C�����j���[�̑I���V�[�P���X
// ====================================================
bool ScenePlay::seqDrawStatusWindow(const float delta_time) {

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE, eKeys::KB_RETURN)) {
		ui_mgr_->hideStatusWindow();
		dungeon_sequence_.change(&ScenePlay::seqSelectMainMenu);
		ResourceManager::getInstance()->playSound(cancel_se_hdl_path_, DX_PLAYTYPE_BACK);
		return true;
	}

	return true;
}