#pragma once
#include "../manager/gm_manager.h"
#include "../base/character_base.h"
#include "../scene/scene_play.h"


// =====================================================================================
// 移動マスのコスト、位置を管理するデータ
// =====================================================================================
struct Node {
	tnl::Vector2i pos_ = { 0, 0 };
	int cost_ = 0;
	bool is_enable_ = true;

};

// =====================================================================================
// 敵のベースとなるクラス
// =====================================================================================
class Enemy : public Character {
public:
	Enemy();
	virtual ~Enemy();

	void update(float delta_time) override;
	void draw(const std::shared_ptr<Camera> camera) override;

private:
	// 行動エラーの最大カウント（ 行動エラー時の対応を行う ）
	const int ACTION_ERROR_MAX = 3;

	// ScenePlayのインスタンス（ マップデータの取得や攻撃処理用に使うが、設計ミスによりあまり良くないと思っています。 ）
	std::weak_ptr<ScenePlay> scene_play_;

	// 敵の行動シーケンスの管理
	tnl::Sequence<Enemy> sequence_ = tnl::Sequence<Enemy>(this, &Enemy::seqIdle);

	// 目標の位置
	tnl::Vector3 target_pos_ = { 0, 0, 0 };

	// 周りのマスの最適コストを探す用
	Node* nodes_ = nullptr;

	// 目標位置を見つけたか判定
	bool is_find_target_ = false;

	// プレイヤーを見つけたか判定
	bool is_find_player_ = false;

	// 行動出来なかった回数
	int action_error_ = 0;

	// 目標の入口番号
	int target_entrance_id_ = 0;

	float attack_time_ = 0.0f;

public:

	void init();

	// ゲッター、セッター
	inline const tnl::Vector3& getPos() const override { return pos_; }
	inline void setPos(const tnl::Vector3& pos) { pos_ = pos; }
	inline const tnl::Vector3& getNextPos() const override { return next_pos_; }
	inline bool isAlive() const override { return is_alive_; }
	inline const eActState& getActState() const override { return act_state_; }
	inline const std::string& getName() const override { return name_; };
	inline const CharaStatus& getStatus() const override { return status_; }

	// 経験値を取得
	void addExp(int exp) override;

	// 行動を開始する
	void beginAction() override;

	// ダメージを受ける
	void takeDamage(int damage) override;

	// 死亡判定にする
	void death();

	// 行動を決める
	void desideAction();

	// 自身をスポーンさせる
	void spawn(const tnl::Vector3& pos);

private:
	// 行動シーケンス
	bool seqIdle(const float delta_time);
	bool seqActionStundby(const float delta_time);
	bool seqMove(const float delta_time);
	bool seqAttack(const float delta_time);

	// 通路での行動処理
	void onRoadAction();

	// 部屋での行動処理
	void onRoomAction();

	void moveToTarget();
	void findTargetInRoom();
	void findPlayer();
	Node* getMinimunScoreNodeForEnabled();

	bool isEnableMapPosition(const tnl::Vector2i& pos);

	// =====================================================================================
	// 向いている方向を反対を取得する
	// =====================================================================================
	eDir_4 getOppositeDir(eDir_4 dir);

	// =====================================================================================
	// 指定した方向の１マス先を next_pos_ にセット
	// =====================================================================================
	inline void setNextPosInDir(eDir_4 dir) {

		switch (dir) {
		case eDir_4::UP:
			next_pos_.y -= 1;
			anim_dir_ = dir;
			break;
		case eDir_4::DOWN:
			next_pos_.y += 1;
			anim_dir_ = dir;
			break;
		case eDir_4::LEFT:
			next_pos_.x -= 1;
			anim_dir_ = dir;
			break;
		case eDir_4::RIGHT:
			next_pos_.x += 1;
			anim_dir_ = dir;
			break;
		default:
			return;
			break;
		}
	}

	// =====================================================================================
	// 指定した地形データを、指定した位置の周りの4方向から探し、存在する方向を返す。
	// =====================================================================================
	inline std::vector<eDir_4> getNearbyMapData(const tnl::Vector3& pos, eMapData map_data) {
		
		auto scene_play = scene_play_.lock();		
		std::vector<eDir_4> directions;

		if (scene_play == nullptr) return directions;

		if (scene_play->getMapData(tnl::Vector3(pos.x, pos.y - 1, pos.z)) == map_data) directions.emplace_back(eDir_4::UP);
		if (scene_play->getMapData(tnl::Vector3(pos.x, pos.y + 1, pos.z)) == map_data) directions.emplace_back(eDir_4::DOWN);
		if (scene_play->getMapData(tnl::Vector3(pos.x - 1, pos.y, pos.z)) == map_data) directions.emplace_back(eDir_4::LEFT);
		if (scene_play->getMapData(tnl::Vector3(pos.x + 1, pos.y, pos.z)) == map_data) directions.emplace_back(eDir_4::RIGHT);

		return directions;
	}

	// =====================================================================================
	// 移動シーケンスに切り替える。
	// =====================================================================================
	inline void changeToMoveSeq() {

		auto scene_play = scene_play_.lock();
		if (scene_play == nullptr) return;

		scene_play->setMapData(pos_, scene_play->getTerrainData(pos_));
		scene_play->setMapData(next_pos_, eMapData::ENEMY);
		// sequence_.change(&Enemy::seqActionStundby);
		act_state_ = eActState::MOVE;
	}

	// =====================================================================================
	// 攻撃シーケンスに切り替える。
	// =====================================================================================
	inline void changeToAttackSeq() {
		// sequence_.change(&Enemy::seqActionStundby);
		act_state_ = eActState::ATTACK;
	}

	// =====================================================================================
	// プレイヤーを 8方向確認して、存在していた場合その方向を返す。
	// =====================================================================================
	inline eDir_8 findPlayerDir_8() {

		auto scene_play = scene_play_.lock();
		if (scene_play == nullptr) return eDir_8::NONE;

		for (int i = static_cast<int>(eDir_8::UP); i < static_cast<int>(eDir_8::MAX); ++i) {
			if (scene_play->getMapData(pos_ + DIR_POS[i]) == eMapData::PLAYER) return static_cast<eDir_8>(i);
		}
		//if (scene_play->getMapData(pos_ + DIR_POS[static_cast<int>(eDir_8::UP)]) == eMapData::PLAYER)				return eDir_8::UP;
		//else if (scene_play->getMapData(pos_ + DIR_POS[static_cast<int>(eDir_8::DOWN)]) == eMapData::PLAYER)		return eDir_8::DOWN;
		//else if (scene_play->getMapData(pos_ + DIR_POS[static_cast<int>(eDir_8::LEFT)]) == eMapData::PLAYER)		return eDir_8::LEFT;
		//else if (scene_play->getMapData(pos_ + DIR_POS[static_cast<int>(eDir_8::RIGHT)]) == eMapData::PLAYER)		return eDir_8::RIGHT;
		//else if (scene_play->getMapData(pos_ + DIR_POS[static_cast<int>(eDir_8::UP_LEFT)]) == eMapData::PLAYER)		return eDir_8::UP_LEFT;
		//else if (scene_play->getMapData(pos_ + DIR_POS[static_cast<int>(eDir_8::UP_RIGHT)]) == eMapData::PLAYER)	return eDir_8::UP_RIGHT;
		//else if (scene_play->getMapData(pos_ + DIR_POS[static_cast<int>(eDir_8::DOWN_LEFT)]) == eMapData::PLAYER)	return eDir_8::DOWN_LEFT;
		//else if (scene_play->getMapData(pos_ + DIR_POS[static_cast<int>(eDir_8::DOWN_RIGHT)]) == eMapData::PLAYER)	return eDir_8::DOWN_RIGHT;

		return eDir_8::NONE;
	}

	// =====================================================================================
	// 指定した位置のセルが指定したマップデータか判定
	// =====================================================================================
	inline bool checkMapDataFromPos(const tnl::Vector3& pos, eMapData map_data) {

		auto scene_play = scene_play_.lock();
		if (scene_play == nullptr) return false;

		return scene_play->getMapData(pos) == map_data;
	}

	// =====================================================================================
	// 指定した方向が有効か
	// =====================================================================================
	inline bool isEnableDir(eDir_8 dir) {
		

		if (dir == eDir_8::UP_LEFT) {
			if (checkMapDataFromPos(pos_ + DIR_POS[static_cast<int>(dir)], eMapData::WALL)) return false;
			if (checkMapDataFromPos(pos_ + DIR_POS[static_cast<int>(eDir_8::UP)], eMapData::WALL)) return false;
			if (checkMapDataFromPos(pos_ + DIR_POS[static_cast<int>(eDir_8::LEFT)], eMapData::WALL)) return false;
			return true;
		}
		else if (dir == eDir_8::UP_RIGHT) {
			if (checkMapDataFromPos(pos_ + DIR_POS[static_cast<int>(dir)], eMapData::WALL)) return false;
			if (checkMapDataFromPos(pos_ + DIR_POS[static_cast<int>(eDir_8::UP)], eMapData::WALL)) return false;
			if (checkMapDataFromPos(pos_ + DIR_POS[static_cast<int>(eDir_8::RIGHT)], eMapData::WALL)) return false;
			return true;
		}
		else if (dir == eDir_8::DOWN_LEFT) {
			if (checkMapDataFromPos(pos_ + DIR_POS[static_cast<int>(dir)], eMapData::WALL)) return false;
			if (checkMapDataFromPos(pos_ + DIR_POS[static_cast<int>(eDir_8::DOWN)], eMapData::WALL)) return false;
			if (checkMapDataFromPos(pos_ + DIR_POS[static_cast<int>(eDir_8::LEFT)], eMapData::WALL)) return false;
			return true;
		}
		else if (dir == eDir_8::DOWN_RIGHT) {
			if (checkMapDataFromPos(pos_ + DIR_POS[static_cast<int>(dir)], eMapData::WALL)) return false;
			if (checkMapDataFromPos(pos_ + DIR_POS[static_cast<int>(eDir_8::DOWN)], eMapData::WALL)) return false;
			if (checkMapDataFromPos(pos_ + DIR_POS[static_cast<int>(eDir_8::RIGHT)], eMapData::WALL)) return false;
			return true;
		}
		return !checkMapDataFromPos(pos_ + DIR_POS[static_cast<int>(dir)], eMapData::WALL);
	}

	// =====================================================================================
	// 指定した方向が有効か
	// =====================================================================================
	bool checkAttackForPlayer() {
		eDir_8 dir = findPlayerDir_8();

		if (dir == eDir_8::NONE) return false;
		return isEnableDir(dir);
	}
};