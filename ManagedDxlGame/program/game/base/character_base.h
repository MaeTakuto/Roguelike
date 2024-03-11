#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "../common/enum.h"
#include "../common/chara_status.h"
#include "../common/animation.h"

class Camera;

// const int DIR_MAX = 8;

// キャラの行動状態
enum class eActState {
	IDLE,
	MOVE,
	ATTACK,
	END
};

// =====================================================================================
// キャラクターのベースクラス
// =====================================================================================
class Character {
public:
	Character();
	virtual ~Character() {};

	virtual void update(float delta_time) = 0;
	virtual void draw(const std::shared_ptr<Camera> camera) = 0;
	virtual void drawEffect(const std::shared_ptr<Camera> camera) = 0;

protected:
	// キャラの画像数、チップサイズ（ キャラチップの切り取り ）
	const int CHARA_GPC_X_NUM = 4;
	const int CHARA_GPC_Y_NUM = 1;
	const int CHARA_GPC_MAX_NUM = CHARA_GPC_X_NUM * CHARA_GPC_Y_NUM;

	// 移動させるスピード
	const float MOVE_SPEED = 0.25f;

	// 各方向の表示画像の方向
	const eDir_4 ANIM_DIR[static_cast<int>(eDir_8::MAX)] 
		= { eDir_4::UP, eDir_4::DOWN, eDir_4::LEFT, eDir_4::RIGHT, eDir_4::UP, eDir_4::UP, eDir_4::DOWN, eDir_4::DOWN };

	// 各方向のベクトル
	const tnl::Vector3 DIR_POS[static_cast<int>(eDir_8::MAX)] 
		= { { 0, -1, 0 }, { 0, 1, 0 }, { -1, 0, 0 }, { 1, 0 , 0 }, { -1, -1, 0 }, { 1, -1, 0 }, { -1, 1, 0 }, { 1, 1, 0 } };

	// 逆方向
	const eDir_8 REVERCE_DIRECTION[static_cast<int>(eDir_8::MAX)]
		= { eDir_8::DOWN, eDir_8::UP, eDir_8::RIGHT, eDir_8::LEFT, eDir_8::DOWN_RIGHT, eDir_8::DOWN_LEFT, eDir_8::UP_RIGHT, eDir_8::UP_LEFT };

	// キャラの画像
	std::vector< std::vector<int> > chara_gpc_hdl_;

	// キャラクターの名前
	std::string name_;

	// ステータス（ HP、ATKなど ）
	CharaStatus status_;

	// ---------------- 攻撃関連 ----------------
	// 攻撃対象のキャラクタークラス
	std::shared_ptr<Character> atk_target_;
	// 攻撃エフェクトのアニメーション
	Animation atk_effect_;
	// 攻撃エフェクト画像
	std::vector<int> atk_effect_gpc_hdls_;

	// ---------------- 位置関連 ----------------
	// 現在の位置
	tnl::Vector3 pos_ = { 0, 0, 0 };
	// 移動させる位置
	tnl::Vector3 next_pos_ = { 0, 0, 0 };
	// ------------------------------------------

	// キャラのアニメ方向
	eDir_4 anim_dir_ = eDir_4::DOWN;

	// 行動状態
	eActState act_state_ = eActState::IDLE;

	// 生存しているか判定
	bool is_alive_;
	
	// ダメージを受けているか判定
	bool is_damaged_;

public:
	// ================= ゲッター、セッター =================
	// 攻撃対象を取得 対象がいない場合、"nullptr" を返す。
	inline std::shared_ptr<Character> getAttackTarget() { return atk_target_; }
	// 現在の位置を取得
	inline const tnl::Vector3& getPos() const { return pos_; }
	// 位置をセット
	inline void setPos(const tnl::Vector3& pos) {
		pos_ = pos;
		next_pos_ = pos_;
	}
	// 移動先の座標を取得
	inline const tnl::Vector3& getNextPos() const { return next_pos_; }
	// 生存しているか判定
	inline bool isAlive() const { return is_alive_; }
	// 行動状態を取得
	inline eActState getActState() { return act_state_; }
	// 名前を取得
	inline const std::string& getName() const { return name_; };
	// ステータスクラスを取得
	inline const CharaStatus& getStatus() const { return status_; }

	// ================= 関数のプロトタイプ =================
	// 経験値を追加する
	inline void addExp(int exp) { status_.addExp(exp); }
	// ダメージを受ける
	void takeDamage(int damage);

	// ================= 仮想関数 =================
	// 行動を開始させる
	virtual void beginAction() = 0;
	// レベルアップ処理を行う
	virtual void executeLevelUpProcess() {};

protected:
	// 指定した位置がフィールドの中か判定
	bool isInField(const tnl::Vector3& pos);

};