#include "../../dxlib_ext/dxlib_ext.h"
#include "../character/pumpkin.h"
#include "enemy_manager.h"


// =====================================================================================
// コンストラクタ
// =====================================================================================
EnemyManager::EnemyManager() {

	enemys_.resize(ENEMY_MAX);

}

// =====================================================================================
// デストラクタ
// =====================================================================================
EnemyManager::~EnemyManager() {

	tnl::DebugTrace("EnemyManagerのデストラクタが実行されました\n");

	enemys_.clear();
}

// =====================================================================================
// アップデート
// =====================================================================================
void EnemyManager::update(float delta_time) {

	for (int i = 0; i < enemys_.size(); i++) {
		if (!enemys_[i]) {
			continue;
		}
		if (enemys_[i]->isAlive())
			enemys_[i]->update(delta_time);
	}
}

// =====================================================================================
// 描画関数
// =====================================================================================
void EnemyManager::draw(const std::shared_ptr<Camera> camera) {

	for (int i = 0; i < enemys_.size(); i++) {
		if (!enemys_[i]) {
			continue;
		}
		if (enemys_[i]->isAlive())
			enemys_[i]->draw(camera);
	}
}

// =====================================================================================
// 敵をスポーンさせる
// =====================================================================================
void EnemyManager::createEnemy(const tnl::Vector3& pos) {

	for (int i = 0; i < enemys_.size(); i++) {
		if (!enemys_[i]) {
			enemys_[i] = std::make_shared<Pumpkin>();
			enemys_[i]->setPos(pos);
			return;
		}
	}
}

// =====================================================================================
// 全ての敵をデスにする
// =====================================================================================
void EnemyManager::deathAllEnemys() {

	for (int i = 0; i < enemys_.size(); i++) {
		if (!enemys_[i]) {
			continue;
		}

		enemys_[i] = nullptr;
	}
}

// =====================================================================================
// 敵の行動を決定する
// =====================================================================================
void EnemyManager::desideAction() {

	for (int i = 0; i < enemys_.size(); i++) {
		if (!enemys_[i]) {
			continue;
		}
		if (enemys_[i]->isAlive() == false) continue;
		enemys_[i]->decideAction();
	}
}

// =====================================================================================
// 攻撃行動をする敵を取得。いない場合は nullptr
// =====================================================================================
std::shared_ptr<EnemyBase> EnemyManager::getEnemyToAttackAction() {
	for (int i = 0; i < enemys_.size(); i++) {
		if (!enemys_[i]) {
			continue;
		}
		if (!(enemys_[i]->isAlive())) {
			continue;
		}
		if (enemys_[i]->getActState() != eActState::ATTACK) {
			continue;
		}
		return enemys_[i];
	}
	return nullptr;
}

// =====================================================================================
// 移動を行う敵の行動を開始させる
// =====================================================================================
void EnemyManager::beginActionToMove() {
	for (int i = 0; i < enemys_.size(); i++) {
		if (!enemys_[i]) {
			continue;
		}
		if (!(enemys_[i]->isAlive())) {
			continue;
		}
		if (enemys_[i]->getActState() != eActState::MOVE) {
			continue;
		}
		enemys_[i]->beginAction();
	}
}

// =====================================================================================
// 全ての敵が行動終了しているかを判定する
// =====================================================================================
bool EnemyManager::isAllEnemyActEnd() {

	for (int i = 0; i < enemys_.size(); i++) {
		if (!enemys_[i]) {
			continue;
		}
		if (!(enemys_[i]->isAlive())) {
			continue;
		}
		if (enemys_[i]->getActState() != eActState::END) {
			return false;
		}
	}

	return true;
}

// 指定した位置にいる敵を返す。一致する敵がいなければ nullptr を返す。
std::shared_ptr<EnemyBase> EnemyManager::findEnemy(const tnl::Vector3& pos) {

	for (int i = 0; i < enemys_.size(); i++) {
		if (!enemys_[i]) {
			continue;
		}
		if (!(enemys_[i]->isAlive())) {
			continue;
		}
		if ( (enemys_[i]->getPos() - pos).length() < FLT_EPSILON )
			return enemys_[i];
	}

	return nullptr;
}