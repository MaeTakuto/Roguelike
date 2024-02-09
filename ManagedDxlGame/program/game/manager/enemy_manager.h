#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "../scene/scene_play.h"
#include "../character/enemy.h"

class Camera;
class Enemy;

class EnemyManager {
public:
	EnemyManager();
	~EnemyManager();

	void update(float delta_time);
	void draw(const std::shared_ptr<Camera> camera);

private:
	const int ENEMY_MAX = 10;
	
	std::vector< std::shared_ptr<Enemy> > enemys_;

public:
	// 
	inline void init() {

		for (int i = 0; i < enemys_.size(); i++) {
			enemys_[i]->init();
		}
	}

	// �G���X�|�[��������
	inline void spawnEnemy(const tnl::Vector3& pos) {

		for (int i = 0; i < enemys_.size(); i++) {
			if (enemys_[i]->isAlive() == false) {
				enemys_[i]->spawn(pos);
				return;
			}
		}
	}

	// �S�Ă̓G���f�X�ɂ���
	inline void deathAllEnemys() {

		for (int i = 0; i < enemys_.size(); i++) {
			enemys_[i]->death();
		}
	}

	// �G�̍s�������肷��
	inline void beginAction() {

		for (int i = 0; i < enemys_.size(); i++) {
			// tnl::DebugTrace("%d\n", i);
			if (enemys_[i]->isAlive() == false) continue;
			enemys_[i]->desideAction();
		}
	}
	
	// �S�Ă̓G���s���I�����Ă��邩�𔻒肷��
	inline bool isAllEnemyActEnd() {

		for (int i = 0; i < enemys_.size(); i++) {
			if (enemys_[i]->isAlive() == false) continue;
			if (enemys_[i]->getActState() != eActState::END) return false;
		}

		return true;
	}

};