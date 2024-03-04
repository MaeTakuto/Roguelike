#include "../../dxlib_ext/dxlib_ext.h"
#include "../manager/gm_manager.h"
#include "../manager/resource_manager.h"
#include "scene_title.h"
#include "../dungeon/dungeon_manager.h"
#include "../manager/enemy_manager.h"
#include "../base/enemy_base.h"
#include "../manager/ui_manager.h"
#include "../common/camera.h"
#include "../base/character_base.h"
#include "../character/player.h"
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

	// ���b�Z�[�W�̕\������
	const float MESSAGE_DRAW_TIME = 3.0f;
}

ScenePlay::ScenePlay() {

	tnl::DebugTrace("ScenePlay�̃R���X�g���N�^�����s����܂���\n");
	// SetBackgroundColor(0, 0, 0);

	// �e�N���X�̐���
	player_ = std::make_shared<Player>();
	dungeon_mgr_ = std::make_shared<DungeonManager>();
	camera_ = std::make_shared<Camera>();
	enemy_mgr_ = std::make_shared<EnemyManager>();
	ui_mgr_ = std::make_shared<UI_Manager>();
	// enemy_mgr_->setScenePlay(dynamic_pointer_cast<ScenePlay>( GameManager::GetInstance()->getSceneInstance()));

	// �t�B�[���h�T�C�Y�̏�����
	field_.resize(GameManager::FIELD_HEIGHT);
	// map_data_.resize(GameManager::FIELD_HEIGHT);

	areas_.resize(dungeon_mgr_->AREA_MAX);

	for (int i = 0; i < GameManager::FIELD_HEIGHT; i++) {
		field_[i].resize(GameManager::FIELD_WIDTH);
		// map_data_[i].resize(GameManager::FIELD_WIDTH);
	}

	gpc_hdl_data_ = tnl::LoadCsv("csv/dungeon_chip_data.csv");

	// �摜�̃��[�h
	mapchip_gpc_hdl_ =
		ResourceManager::getInstance()->loadAnimation(
			gpc_hdl_data_[1][0].getString(),
			gpc_hdl_data_[1][1].getInt(),
			gpc_hdl_data_[1][2].getInt(),
			gpc_hdl_data_[1][3].getInt(),
			GameManager::CHIP_SIZE,
			GameManager::CHIP_SIZE
		);

	fade_gpc_hdl_ = ResourceManager::getInstance()->loadGraph("graphics/black.bmp");
	dungeon_bgm_hdl_ = ResourceManager::getInstance()->loadSound("sound/dungeon_bgm.mp3");

	is_transition_process_ = false;

	ui_mgr_->setHP_BarStatus(player_->getStatus().getMaxHP(), player_->getStatus().getHP());
	ui_mgr_->setFloor(dungeon_floor_);
}

ScenePlay::~ScenePlay() {

	tnl::DebugTrace("ScenePlay�̃f�X�g���N�^�����s����܂���\n");
	StopSoundMem(dungeon_bgm_hdl_);
	
	// �摜�̍폜
	ResourceManager::getInstance()->deleteAnimation(
		gpc_hdl_data_[1][0].getString(),
		gpc_hdl_data_[1][1].getInt()
	);
}

// ====================================================
// �A�b�v�f�[�g
// ====================================================
void ScenePlay::update(float delta_time) {

	main_seq_.update(delta_time);

	int freqency = GetCurrentPositionSoundMem(dungeon_bgm_hdl_);
	if (freqency > bgm_end_freqency_)
	{
		StopSoundMem(dungeon_bgm_hdl_);
		SetCurrentPositionSoundMem(0, dungeon_bgm_hdl_);
		PlaySoundMem(dungeon_bgm_hdl_, DX_PLAYTYPE_LOOP, false);
	}
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


		// �f�o�b�O�i �����̓�����\�� �j
		/*for (int i = 0; i < areas_.size(); i++) {
			for (int j = 0; j < areas_[i].room.entrance.size(); j++) {
				tnl::Vector3 draw_pos = areas_[i].room.entrance[j].pos * GameManager::DRAW_CHIP_SIZE - camera_->getPos() + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);

				DrawBox(draw_pos.x, draw_pos.y, draw_pos.x + GameManager::DRAW_CHIP_SIZE, draw_pos.y + GameManager::DRAW_CHIP_SIZE, -1, true);
			}
		}*/

		player_->draw(camera_);
		enemy_mgr_->draw(camera_);
		ui_mgr_->draw(camera_);
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);
	DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, fade_gpc_hdl_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	// ======= �f�o�b�O ========
	 // DrawStringEx(100, 100, -1, "�V�[���v���C");
	 //dungeon_mgr_->displayAreaNumber(camera_);
}

// ====================================================
// �L�����N�^�[�A�b�v�f�[�g
// ====================================================
void ScenePlay::charaUpdate(float delta_time) {

	player_->update(delta_time);
	enemy_mgr_->update(delta_time);
}

std::shared_ptr<EnemyBase> ScenePlay::findEnemy(const tnl::Vector3& pos) {

	std::shared_ptr<EnemyBase> enemy = enemy_mgr_->findEnemy(pos);

	return enemy;
}

// ====================================================
// attaker �� target �Ƀ_���[�W��^����B
// ====================================================
void ScenePlay::applyDamage(std::shared_ptr<Character> attacker, std::shared_ptr<Character> target) {

	target->takeDamage(attacker->getStatus().getAtk());

	std::string message = attacker->getName() + "��" + target->getName() + "��" + std::to_string(attacker->getStatus().getAtk()) + "�_���[�W��^�����B\n";

	ui_mgr_->setMessage(message, MESSAGE_DRAW_TIME);
	tnl::DebugTrace("%d�_���[�W��^�����B\n", attacker->getStatus().getAtk());

	if (!target->isAlive()) {
		attacker->addExp(target->getStatus().getExp());
		setMapData(target->getNextPos(), getTerrainData(target->getNextPos()));
		message = target->getName() + "��|����";
		ui_mgr_->setMessage(message, MESSAGE_DRAW_TIME);
		tnl::DebugTrace("�|����\n");
	}
}

// ====================================================
// ���̃t���A�ɕς��鏈��
// ====================================================
void ScenePlay::changeProcessNextFloor() {
	main_seq_.change(&ScenePlay::seqFadeOut);
	in_dungeon_seq_.change(&ScenePlay::seqPlayerAct);
	ui_mgr_->executeStairSelectEnd();
	++dungeon_floor_;
	is_created_dungeon_ = false;
}

// ====================================================
// �L�����̃��x�����オ�������̏���
// ====================================================
void ScenePlay::charaLevelUpProcess(std::shared_ptr<Character> chara) {
	std::string message = chara->getName() + "�̓��x����" + std::to_string(chara->getStatus().getLevel()) + "�ɂȂ���";
	ui_mgr_->setMessage(message, MESSAGE_DRAW_TIME);

	std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(chara);
	if (player == nullptr) return;
	ui_mgr_->setHP_BarStatus(player->getStatus().getHP(), player->getStatus().getMaxHP());
}

// ====================================================
// �Q�[���I�[�o�[���̏��� 
// ====================================================
void ScenePlay::gameOverProcess() {
	ui_mgr_->clearMessage();
	std::string message = player_->getName() + "�͂���Ă��܂���";
	ui_mgr_->setMessage(message);
	in_dungeon_seq_.immediatelyChange(&ScenePlay::seqGameOver);
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
	enemy_mgr_->deathAllEnemys();
	dungeon_mgr_->generateDungeon();
	field_ = dungeon_mgr_->getField();
	ui_mgr_->setFloor(dungeon_floor_);
	// map_data_ = dungeon_mgr_->getMapData();
	areas_ = dungeon_mgr_->getAreas();

	// �v���C���[�A�G���X�|�[��
	for (int y = 0; y < field_.size(); y++) {
		for (int x = 0; x < field_[y].size(); x++) {
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

	// debugMapData();

	debugPlaceData();

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
		if (is_created_dungeon_) main_seq_.change(&ScenePlay::seqMain);
		else main_seq_.change(&ScenePlay::seqGenerateDungeon);
	}

	return true;
}

// ====================================================
// �_���W�����T���V�[�P���X
// ====================================================
bool ScenePlay::seqMain(const float delta_time) {
	if (in_dungeon_seq_.isStart()) {
		if(!CheckSoundMem(dungeon_bgm_hdl_))PlaySoundMem(dungeon_bgm_hdl_, DX_PLAYTYPE_LOOP, true);
	}

	// camera_->control();

	 in_dungeon_seq_.update(delta_time);
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
		
		if ( getMapData( player_->getNextPos() ) ==  eMapData::WALL )  { player_->collisionProcess(); }
		else if (getMapData(player_->getNextPos()) == eMapData::ENEMY) { player_->collisionProcess(); }
		else {
			in_dungeon_seq_.change(&ScenePlay::seqEnemyAct);

			setMapData(player_->getPos(), getTerrainData(player_->getPos()));
			setMapData(player_->getNextPos(), eMapData::PLAYER);
		}
	}

	return true;
}

// ====================================================
// �G�̍s�������肷��V�[�P���X
// ====================================================
bool ScenePlay::seqEnemyAct(const float delta_time) {

	enemy_mgr_->desideAction();

	// �v���C���[���ړ�����ꍇ
	if (player_->getActState() == eActState::MOVE) {

		atk_enemy_ = enemy_mgr_->getEnemyToAttackAction();
		// �U������G�������ꍇ
		if (atk_enemy_) {
			in_dungeon_seq_.change(&ScenePlay::seqPlayerMove);
			return true;
		}
		in_dungeon_seq_.change(&ScenePlay::seqCharaMove);
		enemy_mgr_->beginActionToMove();
		return true;
	}

	// �v���C���[���U������ꍇ
	else if(player_->getActState() == eActState::ATTACK) in_dungeon_seq_.change(&ScenePlay::seqPlayerAttack);
	return true;
}

// ====================================================
// �v���C���[�݈̂ړ�
// ====================================================
bool ScenePlay::seqPlayerMove(const float delta_time) {

	charaUpdate(delta_time);

	if (player_->getActState() != eActState::END) return true;
	ui_mgr_->setHP_BarStatus(player_->getStatus().getMaxHP(), player_->getStatus().getHP());
	in_dungeon_seq_.change(&ScenePlay::seqEnemyAttack);
	return true;
}

// ====================================================
// �v���C���[�̍U���V�[�P���X
// ====================================================
bool ScenePlay::seqPlayerAttack(const float delta_time) {

	charaUpdate(delta_time);

	if (player_->getActState() != eActState::END) return true;

	atk_enemy_ = enemy_mgr_->getEnemyToAttackAction();

	// �U������G�������ꍇ
	if (enemy_mgr_->getEnemyToAttackAction()) {
		in_dungeon_seq_.change(&ScenePlay::seqEnemyAttack);
		return true;
	}

	enemy_mgr_->beginActionToMove();
	in_dungeon_seq_.change(&ScenePlay::seqCharaMove);

	return true;
}

// ====================================================
// �G�̍U���V�[�P���X
// ====================================================
bool ScenePlay::seqEnemyAttack(const float delta_time) {
	if (in_dungeon_seq_.isStart()) {
		atk_enemy_->beginAction();
	}

	charaUpdate(delta_time);

	if (atk_enemy_->getActState() != eActState::END) {
		return true;
	}

	applyDamage(atk_enemy_, player_);
	ui_mgr_->setHP_BarStatus(player_->getStatus().getMaxHP(), player_->getStatus().getHP());

	// �v���C���[���S��
	if (!player_->isAlive()) {
		gameOverProcess();
		return true;
	}
	
	atk_enemy_ = enemy_mgr_->getEnemyToAttackAction();

	// �U������G�����Ȃ��ꍇ
	if (atk_enemy_ == nullptr) {
		in_dungeon_seq_.change(&ScenePlay::seqCharaMove);
		enemy_mgr_->beginActionToMove();
		return true;
	}
	atk_enemy_->beginAction();
	return true;
}

// ====================================================
// �s�����I���Ă��Ȃ��L�����N�^�[���s��������V�[�P���X
// ====================================================
bool ScenePlay::seqCharaMove(const float delta_time) {

	charaUpdate(delta_time);

	if (player_->getActState() != eActState::END) return true;
	if (enemy_mgr_->isAllEnemyActEnd() == false) return true;

	in_dungeon_seq_.change(&ScenePlay::seqActEndProcess);

	return true;
}

// ====================================================
// �^�[���I����̏����V�[�P���X
// ====================================================
bool ScenePlay::seqActEndProcess(const float delta_time) {

	charaUpdate(delta_time);
	// debugMapData();
	ui_mgr_->setHP_BarStatus(player_->getStatus().getMaxHP(), player_->getStatus().getHP());
	player_->beginAction();

	// �v���C���[�̈ʒu���K�i��������
	if (getTerrainData(player_->getPos()) == eMapData::STAIR) {
		in_dungeon_seq_.change(&ScenePlay::seqStairSelect);
		ui_mgr_->executeStairSelect();
		return true;
	}

	in_dungeon_seq_.change(&ScenePlay::seqPlayerAct);
	return true;
}

// ====================================================
// �^�[���I����̏����V�[�P���X
// ====================================================
bool ScenePlay::seqStairSelect(const float delta_time) {

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		if (ui_mgr_->selectStairResult()) {
			changeProcessNextFloor();
		}
		else {
			in_dungeon_seq_.change(&ScenePlay::seqPlayerAct);
			ui_mgr_->executeStairSelectEnd();
		}
	}

	return true;
}

// ====================================================
// �^�[���I����̏����V�[�P���X
// ====================================================
bool ScenePlay::seqGameOver(const float delta_time) {

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		is_transition_process_ = true;
		GameManager::GetInstance()->changeScene(std::make_shared<SceneTitle>());
	}
	return true;
}

// 