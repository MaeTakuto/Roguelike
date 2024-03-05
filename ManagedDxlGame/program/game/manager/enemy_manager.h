#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

class Camera;
class EnemyBase;

class EnemyManager {
public:
	EnemyManager();
	~EnemyManager();

	void update(float delta_time);
	void draw(const std::shared_ptr<Camera> camera);

private:
	const int ENEMY_MAX = 10;
	
	// �_���W�����ɐ�������G
	std::vector< std::shared_ptr<EnemyBase> > enemys_;

	// �e�G�̃f�[�^��ێ��i �����X�^�[���A�G�N���X �j
	std::unordered_map< std::string, std::shared_ptr<EnemyBase> > enemy_data_;
	
	// �e�K�w�̏o������G�e�[�u���̃f�[�^
	std::vector<std::vector<tnl::CsvCell>> enemy_table_;

	// ���݂̊K�w�̓G�f�[�^�i �����X�^�[���A���x�� �j
	std::unordered_map< std::string, int > now_floor_enemy_data_;

public:
	// �K�w�̓G�f�[�^�����݂̃t���A�̓G�f�[�^�ɍX�V
	void updateEnemyDataToNowFloor(int floor);

	// �G�𐶐�
	void createEnemy(const tnl::Vector3& pos);
	
	// �S�Ă̓G���f�X������
	void deathAllEnemys();

	// �e�G�̍s�������߂�
	void desideAction();

	// �U����Ԃ̓G���擾
	std::shared_ptr<EnemyBase> getEnemyToAttackAction();

	// �ړ����s���G�̍s�����J�n������
	void beginActionToMove();

	// �S�Ă̓G���s���I�����Ă��邩�𔻒肷��
	bool isAllEnemyActEnd();

	// �w�肵���ʒu�ɂ���G��Ԃ��B��v����G�����Ȃ���� nullptr ��Ԃ��B
	std::shared_ptr<EnemyBase> findEnemy(const tnl::Vector3& pos);

private:
	// �G�������_���ɐ���
	std::shared_ptr<EnemyBase> createRandomEnemy();

};