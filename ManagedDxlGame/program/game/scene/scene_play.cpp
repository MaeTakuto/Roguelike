#include "../../dxlib_ext/dxlib_ext.h"
#include "../manager/gm_manager.h"
#include "../manager/resource_manager.h"
#include "scene_title.h"
#include "../dungeon/dungeon_manager.h"
#include "../manager/enemy_manager.h"
#include "../manager/ui_manager.h"
#include "../common/camera.h"
#include "../base/character_base.h"
#include "../character/player.h"
#include "../ui/message_window.h"
#include "../ui/hp_bar.h"
#include "scene_play.h"


ScenePlay::ScenePlay() {

	// �e�N���X�̐���
	player_ = std::make_shared<Player>();
	dungeon_mgr_ = std::make_shared<DungeonManager>();
	camera_ = std::make_shared<Camera>();
	enemy_mgr_ = std::make_shared<EnemyManager>();
	ui_mgr_ = std::make_shared<UI_Manager>();
	// enemy_mgr_->setScenePlay(dynamic_pointer_cast<ScenePlay>( GameManager::GetInstance()->getSceneInstance()));

	// �t�B�[���h�T�C�Y�̏�����
	field_.resize(GameManager::FIELD_HEIGHT);
	map_data_.resize(GameManager::FIELD_HEIGHT);

	areas_.resize(dungeon_mgr_->AREA_MAX);

	for (int i = 0; i < GameManager::FIELD_HEIGHT; i++) {
		field_[i].resize(GameManager::FIELD_WIDTH);
		map_data_[i].resize(GameManager::FIELD_WIDTH);
	}

	// �摜�̃��[�h
	mapchip_gpc_hdl_ =
		ResourceManager::getInstance()->loadAnimation(
			"graphics/mapchip.bmp",
			mapchip_all_size_,
			2,
			1,
			GameManager::CHIP_SIZE,
			GameManager::CHIP_SIZE
		);

	is_transition_process_ = false;

	ui_mgr_->getHP_Bar()->setMaxHP(player_->getStatus().getMaxHP());
	ui_mgr_->getHP_Bar()->setHP(player_->getStatus().getHP());
	ui_mgr_->getHP_Bar()->updateHP_Text();
}

ScenePlay::~ScenePlay() {

	tnl::DebugTrace("ScenePlay�̃f�X�g���N�^�����s����܂���\n");
}

void ScenePlay::update(float delta_time) {

	main_seq_.update(delta_time);

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		is_transition_process_ = true;
		GameManager::GetInstance()->changeScene( std::make_shared<SceneTitle>() );
	}
}

void ScenePlay::draw() {

	/*for (int y = 0; y < terrain_data_.size(); y++) {
		for (int x = 0; x < terrain_data_[y].size(); x++) {
			tnl::Vector3 draw_pos = tnl::Vector3{ static_cast<float>(x * 20), static_cast<float>(y * 20), 0 } - camera_->getPos();
			DrawStringEx(draw_pos.x, draw_pos.y, -1, "%d", terrain_data_[y][x]);
		}
	}*/

	// �}�b�v�`�b�v�`��
	for (int y = 0; y < field_.size(); ++y) {
		for (int x = 0; x < field_[y].size(); ++x) {
			tnl::Vector3 draw_pos = tnl::Vector3{ static_cast<float>(x) * GameManager::DRAW_CHIP_SIZE, static_cast<float>(y) * GameManager::DRAW_CHIP_SIZE, 0 }
			- camera_->getPos() + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);

			// �`��
			DrawExtendGraph(draw_pos.x, draw_pos.y,
				draw_pos.x + GameManager::DRAW_CHIP_SIZE,
				draw_pos.y + GameManager::DRAW_CHIP_SIZE,
				mapchip_gpc_hdl_[static_cast<int>( field_[y][x].terrain_data ) ], true);

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

	// ======= �f�o�b�O ========
	 // DrawStringEx(100, 100, -1, "�V�[���v���C");
	 //dungeon_mgr_->displayAreaNumber(camera_);
}

// 
void ScenePlay::charaUpdate(float delta_time) {

	player_->update(delta_time);
	enemy_mgr_->update(delta_time);
}

std::shared_ptr<Enemy> ScenePlay::findEnemy(const tnl::Vector3& pos) {

	std::shared_ptr<Enemy> enemy = enemy_mgr_->findEnemy(pos);

	return enemy;
}

// attaker �� target �Ƀ_���[�W��^����B
void ScenePlay::applyDamage(std::shared_ptr<Character> attacker, std::shared_ptr<Character> target) {

	target->takeDamage(attacker->getStatus().getAtk());

	std::string message = attacker->getName() + "��" + target->getName() + "��" + std::to_string(attacker->getStatus().getAtk()) + "�_���[�W��^�����B\n";

	ui_mgr_->getMessageWindow()->setMessgae(message);
	ui_mgr_->getMessageWindow()->setTimeLimit(3.0f);
	tnl::DebugTrace("%d�_���[�W��^�����B\n", attacker->getStatus().getAtk());

	if (!target->isAlive()) {
		setMapData(target->getNextPos(), getTerrainData(target->getNextPos()));
		message = target->getName() + "��|����";
		ui_mgr_->getMessageWindow()->setMessgae(message);
		ui_mgr_->getMessageWindow()->setTimeLimit(3.0f);
		tnl::DebugTrace("�|����\n");
	}
}

// ==================================================================================
//								���C���V�[�P���X
// ==================================================================================

// 
bool ScenePlay::seqSceneStart(const float delta_time) {

	enemy_mgr_->init();

	main_seq_.change(&ScenePlay::seqGenerateDungeon);

	return true;
}

// �_���W��������
bool ScenePlay::seqGenerateDungeon(const float delta_time) {
	
	enemy_mgr_->deathAllEnemys();
	dungeon_mgr_->generateDungeon();
	field_ = dungeon_mgr_->getField();
	map_data_ = dungeon_mgr_->getMapData();
	areas_ = dungeon_mgr_->getAreas();

	for (int y = 0; y < map_data_.size(); y++) {
		for (int x = 0; x < map_data_[y].size(); x++) {
			if (field_[y][x].map_data == eMapData::PLAYER ) {
				tnl::DebugTrace("player x = %d, y = %d\n", x, y);
				tnl::Vector3 pos = { static_cast<float>(x) , static_cast<float>(y), 0 };
				player_->setPos(pos);
			}
			else if (field_[y][x].map_data == eMapData::ENEMY) {
				tnl::DebugTrace("enemy x = %d, y = %d\n", x, y);
				enemy_mgr_->spawnEnemy(tnl::Vector3{ static_cast<float>(x) , static_cast<float>(y), 0});
			}
		}
	}

	// debugMapData();

	debugPlaceData();

	camera_->setPos(player_->getPos());

	main_seq_.change(&ScenePlay::seqMain);

	return true;
}

// �_���W�����T���V�[�P���X
bool ScenePlay::seqMain(const float delta_time) {

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_SPACE)) {
		main_seq_.change(&ScenePlay::seqGenerateDungeon);
		in_dungeon_seq_.change(&ScenePlay::seqPlayerAct);
		player_->beginAct();
	}

	// camera_->control();

	 in_dungeon_seq_.update(delta_time);
	 camera_->setPos(player_->getPos());
	 ui_mgr_->update(delta_time);


	return true;
}	

// ==================================================================================
//								�_���W�����T���V�[�P���X
// ==================================================================================

// 
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

// 
bool ScenePlay::seqEnemyAct(const float delta_time) {

	enemy_mgr_->desideAction();

	if (player_->getActState() == eActState::MOVE) {
		in_dungeon_seq_.change(&ScenePlay::seqCharaMove);
		enemy_mgr_->beginAction();
	}
	else if(player_->getActState() == eActState::ATTACK) in_dungeon_seq_.change(&ScenePlay::seqPlayerAttack);
	return true;
}

// 
bool ScenePlay::seqPlayerAttack(const float delta_time) {

	charaUpdate(delta_time);

	if (player_->getActState() != eActState::END) return true;

	enemy_mgr_->beginAction();
	in_dungeon_seq_.change(&ScenePlay::seqCharaMove);

	return true;
}

// 
bool ScenePlay::seqCharaMove(const float delta_time) {

	charaUpdate(delta_time);

	if (player_->getActState() != eActState::END) return true;
	if (enemy_mgr_->isAllEnemyActEnd() == false) return true;

	in_dungeon_seq_.change(&ScenePlay::seqActEndProcess);

	return true;
}

// 
bool ScenePlay::seqActEndProcess(const float delta_time) {

	charaUpdate(delta_time);
	// debugMapData();

	player_->beginAct();
	in_dungeon_seq_.change(&ScenePlay::seqPlayerAct);

	return true;
}
