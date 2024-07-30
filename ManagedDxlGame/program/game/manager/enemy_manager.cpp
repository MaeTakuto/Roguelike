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
// �R���X�g���N�^
// =====================================================================================
EnemyManager::EnemyManager() {

	enemies_.resize(ENEMY_MAX);
	
	// �G�f�[�^�̓o�^
	enemy_data_.insert(make_pair("���΂����ڂ���", std::make_shared<Pumpkin>()));
	enemy_data_.insert(make_pair("��������", std::make_shared<Skeleton>()));
	enemy_data_.insert(make_pair("�䂤�ꂢ", std::make_shared<Ghost>()));

	enemy_table_ = tnl::LoadCsv( ENEMY_TABLE_CSV_PATH );
}

// =====================================================================================
// �f�X�g���N�^
// =====================================================================================
EnemyManager::~EnemyManager() {

	tnl::DebugTrace("EnemyManager�̃f�X�g���N�^�����s����܂���\n");

	enemies_.clear();
}

// =====================================================================================
// �A�b�v�f�[�g
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
// �`��֐�
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
// �������Ă���G�̐���Ԃ��B
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
// �K�w�̓G�f�[�^�����݂̃t���A�̓G�f�[�^�ɍX�V
// =====================================================================================
void EnemyManager::updateEnemyDataToNowFloor(int floor) {
	tnl::DebugTrace("=============== updateRnemyDataToNowFloor ===============\n");

	if (floor <= 0 || floor > enemy_table_.size()) {
		tnl::DebugTrace("floor �� enemy_table�͈̔͊O�ł��Bfloor�F%d\n", floor);
		return;
	}

	now_floor_enemy_data_.clear();

	for (int i = GameManager::CSV_CELL_ROW_START; i < enemy_table_[floor].size(); i += 2) {
		if (enemy_table_[floor][i].getString() == "") {
			break;
		}
		now_floor_enemy_data_.insert(make_pair(
			enemy_table_[floor][i].getString(),			// �����X�^�[��
			enemy_table_[floor][i + 1].getInt()			// �����X�^�[���x��
		));
	}

	for (auto pair : now_floor_enemy_data_) {
		tnl::DebugTrace( "�����X�^�[���F%s�A���x���F%d\n", pair.first.c_str(), pair.second);
	}
}

// =====================================================================================
// �G���X�|�[��������
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
// �S�Ă̓G���f�X�ɂ���
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
// �G�̍s�������肷��
// =====================================================================================
void EnemyManager::desideAction() {

	std::shared_ptr<ScenePlay> scene_play 
		= std::dynamic_pointer_cast<ScenePlay>( GameManager::getInstance()->getSceneInstance());

	if (!scene_play) {
		tnl::DebugTrace("EnemyManager::desideAction() : scene_play ���擾�ł��܂���ł���");
	}

	for (int i = 0; i < enemies_.size(); i++) {
		if (!enemies_[i]) {
			continue;
		}
		if (enemies_[i]->isAlive() == false) continue;
		enemies_[i]->decideAction();
		// �V�[���v���C�̍U���s���҂ɓo�^
		if (enemies_[i]->getActState() == eActState::ATTACK) {
			scene_play->addAttacker(enemies_[i]);
		}
	}
}

// �G�̍s�����C������
void EnemyManager::modifyEnemiesAction() {
	std::shared_ptr<ScenePlay> scene_play
		= std::dynamic_pointer_cast<ScenePlay>(GameManager::getInstance()->getSceneInstance());

	if (!scene_play) {
		tnl::DebugTrace("EnemyManager::desideAction() : scene_play ���擾�ł��܂���ł���");
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
		// �V�[���v���C�ɍU���s���҂�o�^
		if (enemies_[i]->getActState() == eActState::ATTACK) {
			scene_play->addAttacker(enemies_[i]);
		}
	}
}

// =====================================================================================
// �ړ����s���G�̍s�����J�n������
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
// �S�Ă̓G���s���I�����Ă��邩�𔻒肷��
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

// �w�肵���ʒu�ɂ���G��Ԃ��B��v����G�����Ȃ���� nullptr ��Ԃ��B
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
// �����_���ɓG�𐶐����A�A�h���X��Ԃ�
// =====================================================================================
std::shared_ptr<EnemyBase> EnemyManager::createRandomEnemy() {

	tnl::DebugTrace("=============== createRandomEnemy ===============\n");
	std::shared_ptr<EnemyBase> enemy = nullptr;
	std::vector<std::string> enemy_names;

	for (auto pair : now_floor_enemy_data_) {
		enemy_names.emplace_back(pair.first);
	}

	if (enemy_names.size() <= 0) {
		tnl::DebugTrace("�G�f�[�^������܂���\n");
		return nullptr;
	}

	int index = mtl::getRandomValue(0, enemy_names.size() - 1);

	auto it = enemy_data_.find(enemy_names[index]);
	if (it == enemy_data_.end()) {
		tnl::DebugTrace("�w�肳�ꂽ�G�����݂��܂���. �����X�^�[���F%s\n", enemy_names[index].c_str());
		return enemy;
	}

	enemy = it->second->createClone();
	enemy->setEnemyLevel(now_floor_enemy_data_[enemy_names[index]]);

	return enemy;
}
