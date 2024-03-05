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
	
	// ダンジョンに生成する敵
	std::vector< std::shared_ptr<EnemyBase> > enemys_;

	// 各敵のデータを保持（ モンスター名、敵クラス ）
	std::unordered_map< std::string, std::shared_ptr<EnemyBase> > enemy_data_;
	
	// 各階層の出現する敵テーブルのデータ
	std::vector<std::vector<tnl::CsvCell>> enemy_table_;

	// 現在の階層の敵データ（ モンスター名、レベル ）
	std::unordered_map< std::string, int > now_floor_enemy_data_;

public:
	// 階層の敵データを現在のフロアの敵データに更新
	void updateEnemyDataToNowFloor(int floor);

	// 敵を生成
	void createEnemy(const tnl::Vector3& pos);
	
	// 全ての敵をデスさせる
	void deathAllEnemys();

	// 各敵の行動を決める
	void desideAction();

	// 攻撃状態の敵を取得
	std::shared_ptr<EnemyBase> getEnemyToAttackAction();

	// 移動を行う敵の行動を開始させる
	void beginActionToMove();

	// 全ての敵が行動終了しているかを判定する
	bool isAllEnemyActEnd();

	// 指定した位置にいる敵を返す。一致する敵がいなければ nullptr を返す。
	std::shared_ptr<EnemyBase> findEnemy(const tnl::Vector3& pos);

private:
	// 敵をランダムに生成
	std::shared_ptr<EnemyBase> createRandomEnemy();

};