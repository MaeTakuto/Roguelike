#include <fstream>
#include "../../dxlib_ext/dxlib_ext.h"
#include "gm_manager.h"
#include "../scene/scene_play.h"
#include "../my_library/my_library.h"
#include "../object/character/enemy/pumpkin.h"
#include "../object/character/enemy/skeleton.h"
#include "../object//character/enemy/ghost.h"
#include "../base/character_base.h"
#include "enemy_manager.h"

namespace {
	const std::string ENEMY_TABLE_CSV_PATH = "csv/enemy_table.csv";
}

// =====================================================================================
// コンストラクタ
// =====================================================================================
EnemyManager::EnemyManager() {

	enemies_.resize(ENEMY_MAX);
	
	// 敵データの登録
	enemy_data_.insert(make_pair("おばけかぼちゃ", std::make_shared<Pumpkin>()));
	enemy_data_.insert(make_pair("がいこつ", std::make_shared<Skeleton>()));
	enemy_data_.insert(make_pair("ゆうれい", std::make_shared<Ghost>()));

	enemy_table_ = tnl::LoadCsv( ENEMY_TABLE_CSV_PATH );
}

// =====================================================================================
// デストラクタ
// =====================================================================================
EnemyManager::~EnemyManager() {

	tnl::DebugTrace("EnemyManagerのデストラクタが実行されました\n");

	enemies_.clear();
}

// =====================================================================================
// アップデート
// =====================================================================================
void EnemyManager::update(float delta_time) {

	for (int i = 0; i < enemies_.size(); i++) {
		if (!enemies_[i]) {
			continue;
		}
		if (enemies_[i]->isAlive())
			enemies_[i]->update(delta_time);
	}
}

// =====================================================================================
// 描画関数
// =====================================================================================
void EnemyManager::draw(const std::shared_ptr<Camera> camera) {

	for (int i = 0; i < enemies_.size(); i++) {
		if (!enemies_[i]) {
			continue;
		}
		if (enemies_[i]->isAlive())
			enemies_[i]->draw(camera);
	}
}

void EnemyManager::drawEffect(const std::shared_ptr<Camera> camera) {
	for (int i = 0; i < enemies_.size(); i++) {
		if (!enemies_[i]) {
			continue;
		}
		if (enemies_[i]->isAlive())
			enemies_[i]->drawEffect(camera);
	}
}

// =====================================================================================
// 生存している敵の数を返す。
// =====================================================================================
int EnemyManager::getAliveEnemyNum() {

	int count = 0;

	for (int i = 0; i < enemies_.size(); ++i) {
		if (enemies_[i]->isAlive()) {
			++count;
		}
	}

	return count;
}

// =====================================================================================
// 階層の敵データを現在のフロアの敵データに更新
// =====================================================================================
void EnemyManager::updateEnemyDataToNowFloor(int floor) {
	tnl::DebugTrace("=============== updateRnemyDataToNowFloor ===============\n");

	if (floor <= 0 || floor > enemy_table_.size()) {
		tnl::DebugTrace("floor が enemy_tableの範囲外です。floor：%d\n", floor);
		return;
	}

	now_floor_enemy_data_.clear();

	for (int i = GameManager::CSV_CELL_ROW_START; i < enemy_table_[floor].size(); i += 2) {
		if (enemy_table_[floor][i].getString() == "") {
			break;
		}
		now_floor_enemy_data_.insert(make_pair(
			enemy_table_[floor][i].getString(),			// モンスター名
			enemy_table_[floor][i + 1].getInt()			// モンスターレベル
		));
	}

	for (auto pair : now_floor_enemy_data_) {
		tnl::DebugTrace( "モンスター名：%s、レベル：%d\n", pair.first.c_str(), pair.second);
	}
}

// =====================================================================================
// 敵をスポーンさせる
// =====================================================================================
void EnemyManager::createEnemy(const tnl::Vector3& pos) {

	for (int i = 0; i < enemies_.size();) {
		if (!enemies_[i]) {
			
			enemies_[i] = createRandomEnemy();
			
			if (!enemies_[i]) {
				continue;
			}

			enemies_[i]->setPos(pos);
			return;
		}
		++i;
	}
}

// =====================================================================================
// 全ての敵をデスにする
// =====================================================================================
void EnemyManager::deathAllEnemys() {

	for (int i = 0; i < enemies_.size(); i++) {
		if (!enemies_[i]) {
			continue;
		}

		enemies_[i] = nullptr;
	}
}

// =====================================================================================
// 敵の行動を決定する
// =====================================================================================
void EnemyManager::desideAction() {

	std::shared_ptr<ScenePlay> scene_play 
		= std::dynamic_pointer_cast<ScenePlay>( GameManager::getInstance()->getSceneInstance());

	if (!scene_play) {
		tnl::DebugTrace("EnemyManager::desideAction() : scene_play を取得できませんでした");
	}

	for (int i = 0; i < enemies_.size(); i++) {
		if (!enemies_[i]) {
			continue;
		}
		if (enemies_[i]->isAlive() == false) continue;
		enemies_[i]->decideAction();
		// シーンプレイの攻撃行動者に登録
		if (enemies_[i]->getActState() == eActState::ATTACK) {
			scene_play->addAttacker(enemies_[i]);
		}
	}
}

// 敵の行動を修正する
void EnemyManager::modifyEnemiesAction() {
	std::shared_ptr<ScenePlay> scene_play
		= std::dynamic_pointer_cast<ScenePlay>(GameManager::getInstance()->getSceneInstance());

	if (!scene_play) {
		tnl::DebugTrace("EnemyManager::desideAction() : scene_play を取得できませんでした");
	}

	for (int i = 0; i < enemies_.size(); ++i) {
		if (!enemies_[i]) {
			continue;
		}
		if (!enemies_[i]->isAlive()) {
			continue;
		}
		enemies_[i]->resetEnemyInMapData();
	}

	for (int i = 0; i < enemies_.size(); ++i) {
		if (!enemies_[i]) {
			continue;
		}
		if (!enemies_[i]->isAlive()) {
			continue;
		}
		enemies_[i]->decideAction();
		// シーンプレイに攻撃行動者を登録
		if (enemies_[i]->getActState() == eActState::ATTACK) {
			scene_play->addAttacker(enemies_[i]);
		}
	}
}

// =====================================================================================
// 移動を行う敵の行動を開始させる
// =====================================================================================
void EnemyManager::beginActionToMove() {
	for (int i = 0; i < enemies_.size(); i++) {
		if (!enemies_[i]) {
			continue;
		}
		if (!(enemies_[i]->isAlive())) {
			continue;
		}
		if (enemies_[i]->getActState() != eActState::MOVE) {
			continue;
		}
		enemies_[i]->beginAction();
	}
}

// =====================================================================================
// 全ての敵が行動終了しているかを判定する
// =====================================================================================
bool EnemyManager::isAllEnemyActEnd() {

	for (int i = 0; i < enemies_.size(); i++) {
		if (!enemies_[i]) {
			continue;
		}
		if (!(enemies_[i]->isAlive())) {
			continue;
		}
		if (enemies_[i]->getActState() != eActState::END) {
			return false;
		}
	}

	return true;
}

// 指定した位置にいる敵を返す。一致する敵がいなければ nullptr を返す。
std::shared_ptr<EnemyBase> EnemyManager::findEnemy(const tnl::Vector3& pos) {

	for (int i = 0; i < enemies_.size(); i++) {
		if (!enemies_[i]) {
			continue;
		}
		if (!(enemies_[i]->isAlive())) {
			continue;
		}
		if ( (enemies_[i]->getPos() - pos).length() < FLT_EPSILON )
			return enemies_[i];
	}

	return nullptr;
}

// =====================================================================================
// ランダムに敵を生成し、アドレスを返す
// =====================================================================================
std::shared_ptr<EnemyBase> EnemyManager::createRandomEnemy() {

	tnl::DebugTrace("=============== createRandomEnemy ===============\n");
	std::shared_ptr<EnemyBase> enemy = nullptr;
	std::vector<std::string> enemy_names;

	for (auto pair : now_floor_enemy_data_) {
		enemy_names.emplace_back(pair.first);
	}

	if (enemy_names.size() <= 0) {
		tnl::DebugTrace("敵データがありません\n");
		return nullptr;
	}

	int index = mtl::getRandomValue(0, enemy_names.size() - 1);

	auto it = enemy_data_.find(enemy_names[index]);
	if (it == enemy_data_.end()) {
		tnl::DebugTrace("指定された敵が存在しません. モンスター名：%s\n", enemy_names[index].c_str());
		return enemy;
	}

	enemy = it->second->createClone();
	enemy->setEnemyLevel(now_floor_enemy_data_[enemy_names[index]]);

	return enemy;
}
