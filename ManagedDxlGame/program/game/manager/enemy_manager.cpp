#include "../../dxlib_ext/dxlib_ext.h"
#include "../character/pumpkin.h"
#include "enemy_manager.h"


// =====================================================================================
// �R���X�g���N�^
// =====================================================================================
EnemyManager::EnemyManager() {

	enemys_.resize(ENEMY_MAX);

}

// =====================================================================================
// �f�X�g���N�^
// =====================================================================================
EnemyManager::~EnemyManager() {

	tnl::DebugTrace("EnemyManager�̃f�X�g���N�^�����s����܂���\n");

	enemys_.clear();
}

// =====================================================================================
// �A�b�v�f�[�g
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
// �`��֐�
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
// �G���X�|�[��������
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
// �S�Ă̓G���f�X�ɂ���
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
// �G�̍s�������肷��
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
// �U���s��������G���擾�B���Ȃ��ꍇ�� nullptr
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
// �ړ����s���G�̍s�����J�n������
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
// �S�Ă̓G���s���I�����Ă��邩�𔻒肷��
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

// �w�肵���ʒu�ɂ���G��Ԃ��B��v����G�����Ȃ���� nullptr ��Ԃ��B
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