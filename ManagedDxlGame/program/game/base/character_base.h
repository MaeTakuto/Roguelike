#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "../common/enum.h"
#include "../common/chara_status.h"

class Camera;

const int DIR_MAX = 8;

// キャラの行動状態
enum class eActState {
	IDLE,
	MOVE,
	ATTACK,
	END
};

// キャラクターのベースクラス
class Character {
public:
	Character() {};
	virtual ~Character() {};

	virtual void update(float delta_time) = 0;
	virtual void draw(const std::shared_ptr<Camera> camera) = 0;

	// pos_ を取得、セット
	virtual const tnl::Vector3& getPos() const = 0;
	virtual void setPos(const tnl::Vector3& pos) = 0;
	
	// next_pos_ を取得
	virtual const tnl::Vector3& getNextPos() const = 0;

	// 生存しているか判定
	virtual bool isAlive() const = 0;

	// 行動状態を取得
	virtual const eActState& getActState() const = 0;

	// 名前を取得
	virtual const std::string& getName() const = 0;

	// ステータスを取得
	virtual const CharaStatus& getStatus() const = 0;

	// 行動を開始させる
	virtual void beginAction() = 0;

	// ダメージを受ける
	virtual void takeDamage(int damage) = 0;

	// 経験値を取得
	virtual void addExp(int exp) = 0;

protected:
	// 移動させるスピード
	const float MOVE_SPEED = 0.25f;

	// 各方向の表示画像の方向
	const eDir_4 ANIM_DIR[DIR_MAX] = { eDir_4::UP, eDir_4::DOWN, eDir_4::LEFT, eDir_4::RIGHT, eDir_4::UP, eDir_4::UP, eDir_4::DOWN, eDir_4::DOWN };

	// 各方向のベクトル
	const tnl::Vector3 DIR_POS[DIR_MAX] = { { 0, -1, 0 }, { 0, 1, 0 }, { -1, 0, 0 }, { 1, 0 , 0 }, { -1, -1, 0 }, { 1, -1, 0 }, { -1, 1, 0 }, { 1, 1, 0 } };

	// キャラの画像
	std::vector< std::vector<int> > chara_gpc_hdl_;

	// 画像データ（ 画像パス、フレーム数など ）
	std::vector< std::vector<tnl::CsvCell> > gpc_hdl_data_;

	// キャラクターの名前
	std::string name_ = "";

	// ステータス（ HP、ATKなど ）
	CharaStatus status_ = CharaStatus();

	// 現在の位置
	tnl::Vector3 pos_ = { 0, 0, 0 };
	// 移動させる位置
	tnl::Vector3 next_pos_ = { 0, 0, 0 };
	tnl::Vector3 attack_dir_ = { 0, 0, 0 };

	// キャラのアニメ方向
	eDir_4 anim_dir_ = eDir_4::DOWN;

	// 行動状態
	eActState act_state_ = eActState::IDLE;

	// 生存しているか
	bool is_alive_ = true;;
	bool is_collision_ = false;

};