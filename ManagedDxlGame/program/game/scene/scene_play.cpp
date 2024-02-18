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

	tnl::DebugTrace("ScenePlayのコンストラクタが実行されました\n");

	// 各クラスの生成
	player_ = std::make_shared<Player>();
	dungeon_mgr_ = std::make_shared<DungeonManager>();
	camera_ = std::make_shared<Camera>();
	enemy_mgr_ = std::make_shared<EnemyManager>();
	ui_mgr_ = std::make_shared<UI_Manager>();
	// enemy_mgr_->setScenePlay(dynamic_pointer_cast<ScenePlay>( GameManager::GetInstance()->getSceneInstance()));

	// フィールドサイズの初期化
	field_.resize(GameManager::FIELD_HEIGHT);
	// map_data_.resize(GameManager::FIELD_HEIGHT);

	areas_.resize(dungeon_mgr_->AREA_MAX);

	for (int i = 0; i < GameManager::FIELD_HEIGHT; i++) {
		field_[i].resize(GameManager::FIELD_WIDTH);
		// map_data_[i].resize(GameManager::FIELD_WIDTH);
	}

	gpc_hdl_data_ = tnl::LoadCsv("csv/dungeon_chip_data.csv");

	// 画像のロード
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

	is_transition_process_ = false;

	ui_mgr_->getHP_Bar()->setMaxHP(player_->getStatus().getMaxHP());
	ui_mgr_->getHP_Bar()->setHP(player_->getStatus().getHP());
	ui_mgr_->getHP_Bar()->updateHP_Text();
}

ScenePlay::~ScenePlay() {

	tnl::DebugTrace("ScenePlayのデストラクタが実行されました\n");
	
	// 画像の削除
	ResourceManager::getInstance()->deleteAnimation(
		gpc_hdl_data_[1][0].getString(),
		gpc_hdl_data_[1][1].getInt()
	);
}

void ScenePlay::update(float delta_time) {

	main_seq_.update(delta_time);

	/*if (GameManager::GetInstance()->isTransition()) return;
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		is_transition_process_ = true;
		GameManager::GetInstance()->changeScene( std::make_shared<SceneTitle>() );
	}*/
}

void ScenePlay::draw() {

	/*for (int y = 0; y < terrain_data_.size(); y++) {
		for (int x = 0; x < terrain_data_[y].size(); x++) {
			tnl::Vector3 draw_pos = tnl::Vector3{ static_cast<float>(x * 20), static_cast<float>(y * 20), 0 } - camera_->getPos();
			DrawStringEx(draw_pos.x, draw_pos.y, -1, "%d", terrain_data_[y][x]);
		}
	}*/

	// マップチップ描画
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


	// デバッグ（ 部屋の入口を表示 ）
	/*for (int i = 0; i < areas_.size(); i++) {
		for (int j = 0; j < areas_[i].room.entrance.size(); j++) {
			tnl::Vector3 draw_pos = areas_[i].room.entrance[j].pos * GameManager::DRAW_CHIP_SIZE - camera_->getPos() + tnl::Vector3(DXE_WINDOW_WIDTH >> 1, DXE_WINDOW_HEIGHT >> 1, 0);

			DrawBox(draw_pos.x, draw_pos.y, draw_pos.x + GameManager::DRAW_CHIP_SIZE, draw_pos.y + GameManager::DRAW_CHIP_SIZE, -1, true);
		}
	}*/

	player_->draw(camera_);
	enemy_mgr_->draw(camera_);
	ui_mgr_->draw(camera_);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);
	DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, fade_gpc_hdl_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	// ======= デバッグ ========
	 // DrawStringEx(100, 100, -1, "シーンプレイ");
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

// attaker が target にダメージを与える。
void ScenePlay::applyDamage(std::shared_ptr<Character> attacker, std::shared_ptr<Character> target) {

	target->takeDamage(attacker->getStatus().getAtk());

	std::string message = attacker->getName() + "は" + target->getName() + "に" + std::to_string(attacker->getStatus().getAtk()) + "ダメージを与えた。\n";

	ui_mgr_->getMessageWindow()->setMessgae(message);
	ui_mgr_->getMessageWindow()->setTimeLimit(3.0f);
	tnl::DebugTrace("%dダメージを与えた。\n", attacker->getStatus().getAtk());

	if (!target->isAlive()) {
		setMapData(target->getNextPos(), getTerrainData(target->getNextPos()));
		message = target->getName() + "を倒した";
		ui_mgr_->getMessageWindow()->setMessgae(message);
		ui_mgr_->getMessageWindow()->setTimeLimit(3.0f);
		tnl::DebugTrace("倒した\n");
	}
}

// ==================================================================================
//								メインシーケンス
// ==================================================================================

// 
bool ScenePlay::seqSceneStart(const float delta_time) {

	enemy_mgr_->init();

	main_seq_.change(&ScenePlay::seqGenerateDungeon);

	return true;
}

// フェードイン
bool ScenePlay::seqFadeIn(const float delta_time) {
	
	alpha_ = 255 - (main_seq_.getProgressTime() / fade_time_ * 255.0f);
	
	if (alpha_ <= 0) {
		main_seq_.change(&ScenePlay::seqMain);
	}

	return true;
}

// ダンジョン生成シーケンス
bool ScenePlay::seqGenerateDungeon(const float delta_time) {
	
	enemy_mgr_->deathAllEnemys();
	dungeon_mgr_->generateDungeon();
	field_ = dungeon_mgr_->getField();
	// map_data_ = dungeon_mgr_->getMapData();
	areas_ = dungeon_mgr_->getAreas();

	for (int y = 0; y < field_.size(); y++) {
		for (int x = 0; x < field_[y].size(); x++) {
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

	main_seq_.change(&ScenePlay::seqFadeIn);

	return true;
}

// ダンジョン探索シーケンス
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

// 
bool ScenePlay::seqFadeOut(const float delta_time) {
	
	alpha_ = (main_seq_.getProgressTime() / fade_time_ * 255.0f);
	if (alpha_ >= 255) {
		main_seq_.change(&ScenePlay::seqGenerateDungeon);
	}

	return true;
}


// ==================================================================================
//								ダンジョン探索シーケンス
// ==================================================================================

// ====================================================
// プレイヤーの入力シーケンス
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
// 敵の行動を決定するシーケンス
// ====================================================
bool ScenePlay::seqEnemyAct(const float delta_time) {

	enemy_mgr_->desideAction();

	// プレイヤーが移動する場合
	if (player_->getActState() == eActState::MOVE) {

		atk_enemy_ = enemy_mgr_->getEnemyToAttackAction();
		// 攻撃する敵がいた場合
		if (atk_enemy_) {
			in_dungeon_seq_.change(&ScenePlay::seqPlayerMove);
			return true;
		}
		in_dungeon_seq_.change(&ScenePlay::seqCharaMove);
		enemy_mgr_->beginActionToMove();
		return true;
	}

	// プレイヤーが攻撃する場合
	else if(player_->getActState() == eActState::ATTACK) in_dungeon_seq_.change(&ScenePlay::seqPlayerAttack);
	return true;
}

// ====================================================
// プレイヤーのみ移動
// ====================================================
bool ScenePlay::seqPlayerMove(const float delta_time) {

	charaUpdate(delta_time);

	if (player_->getActState() != eActState::END) return true;
	ui_mgr_->getHP_Bar()->setMaxHP(player_->getStatus().getMaxHP());
	ui_mgr_->getHP_Bar()->setHP(player_->getStatus().getHP());
	ui_mgr_->getHP_Bar()->updateHP_Text();
	in_dungeon_seq_.change(&ScenePlay::seqEnemyAttack);
	return true;
}

// ====================================================
// プレイヤーの攻撃シーケンス
// ====================================================
bool ScenePlay::seqPlayerAttack(const float delta_time) {

	charaUpdate(delta_time);

	if (player_->getActState() != eActState::END) return true;

	atk_enemy_ = enemy_mgr_->getEnemyToAttackAction();

	// 攻撃する敵がいた場合
	if (enemy_mgr_->getEnemyToAttackAction()) {
		in_dungeon_seq_.change(&ScenePlay::seqEnemyAttack);
		return true;
	}

	enemy_mgr_->beginActionToMove();
	in_dungeon_seq_.change(&ScenePlay::seqCharaMove);

	return true;
}

// ====================================================
// 敵の攻撃シーケンス
// ====================================================
bool ScenePlay::seqEnemyAttack(const float delta_time) {
	if (in_dungeon_seq_.isStart()) {
		atk_enemy_->beginAction();
	}

	charaUpdate(delta_time);

	if (atk_enemy_->getActState() != eActState::END) return true;

	applyDamage(atk_enemy_, player_);
	ui_mgr_->getHP_Bar()->setMaxHP(player_->getStatus().getMaxHP());
	ui_mgr_->getHP_Bar()->setHP(player_->getStatus().getHP());
	ui_mgr_->getHP_Bar()->updateHP_Text();
	atk_enemy_ = enemy_mgr_->getEnemyToAttackAction();

	// 攻撃する敵がいない場合
	if (atk_enemy_ == nullptr) {
		in_dungeon_seq_.change(&ScenePlay::seqCharaMove);
		enemy_mgr_->beginActionToMove();
		return true;
	}
	atk_enemy_->beginAction();
	return true;
}

// ====================================================
// 行動を終えていないキャラクターを行動させるシーケンス
// ====================================================
bool ScenePlay::seqCharaMove(const float delta_time) {

	charaUpdate(delta_time);

	if (player_->getActState() != eActState::END) return true;
	if (enemy_mgr_->isAllEnemyActEnd() == false) return true;

	in_dungeon_seq_.change(&ScenePlay::seqActEndProcess);

	return true;
}

// ====================================================
// ターン終了後の処理シーケンス
// ====================================================
bool ScenePlay::seqActEndProcess(const float delta_time) {

	charaUpdate(delta_time);
	// debugMapData();
	ui_mgr_->getHP_Bar()->setMaxHP(player_->getStatus().getMaxHP());
	ui_mgr_->getHP_Bar()->setHP(player_->getStatus().getHP());
	ui_mgr_->getHP_Bar()->updateHP_Text();
	player_->beginAct();

	if (getTerrainData(player_->getPos()) == eMapData::STAIR) {
		in_dungeon_seq_.change(&ScenePlay::seqStairSelect);
		ui_mgr_->executeStairSelect();
		return true;
	}
	in_dungeon_seq_.change(&ScenePlay::seqPlayerAct);

	return true;
}

// ====================================================
// ターン終了後の処理シーケンス
// ====================================================
bool ScenePlay::seqStairSelect(const float delta_time) {

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		if (ui_mgr_->selectStairResult()) {
			main_seq_.change(&ScenePlay::seqFadeOut);
			in_dungeon_seq_.change(&ScenePlay::seqPlayerAct);
			ui_mgr_->executeStairSelectEnd();
		}
		else {
			in_dungeon_seq_.change(&ScenePlay::seqPlayerAct);
			ui_mgr_->executeStairSelectEnd();
		}
	}

	return true;
}
