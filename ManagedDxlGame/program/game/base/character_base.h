#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include "../common/enum.h"
#include "../common/chara_status.h"

class Camera;
class MagicBase;
class Animation;

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

	// シーケンスに留まる時間
	const float SEQUENCE_WAIT_TIME = 0.75f;

	// 移動させるスピード
	const float MOVE_SPEED = 7.5f;

	// キャラクターアニメーションの切り替え間隔
	const float CHARA_ANIM_INTERVAL = 0.25f;

	// ダメージ演出切り替え間隔
	const float DAMAGE_EFFECT_CHANGE_INTERVAL = 0.05f;

	// 各方向の表示画像の方向
	const eDir_4 ANIM_DIR[static_cast<int>(eDir_8::MAX)] 
		= { eDir_4::UP, eDir_4::DOWN, eDir_4::LEFT, eDir_4::RIGHT, eDir_4::UP, eDir_4::UP, eDir_4::DOWN, eDir_4::DOWN };

	// 逆方向
	const eDir_8 REVERCE_DIRECTION[static_cast<int>(eDir_8::MAX)]
		= { eDir_8::DOWN, eDir_8::UP, eDir_8::RIGHT, eDir_8::LEFT, eDir_8::DOWN_RIGHT, eDir_8::DOWN_LEFT, eDir_8::UP_RIGHT, eDir_8::UP_LEFT };

	// ---------------- キャラクターデータ関連 ---------------------------------------

	// キャラの画像
	std::vector< std::vector<int> > chara_gpc_hdl_;
	// キャラクターアニメーション
	std::shared_ptr<Animation> chara_animation_;
	// キャラクターの名前
	std::string name_;
	// ステータス（ HP、ATKなど ）
	CharaStatus status_;
	// 魔法一覧
	std::vector<std::shared_ptr<MagicBase>> magic_list_;

	// 現在の位置
	tnl::Vector3 pos_;
	// 移動させる位置
	tnl::Vector3 next_pos_;
	// 現在見ている方向
	eDir_8 looking_dir_;
	// キャラのアニメ方向
	eDir_4 anim_dir_;
	// 行動状態
	eActState act_state_;
	// ダメージを受けているか判定
	bool is_take_damage_;
	// 
	bool is_drawing_;
	// 生存しているか判定
	bool is_alive_;

	// ---------------- 攻撃関連 ----------------------------------------------------
	// 攻撃対象のキャラクタークラス
	std::shared_ptr<Character> atk_target_;
	// 攻撃エフェクトのアニメーション
	std::shared_ptr<Animation> atk_effect_;
	// 攻撃エフェクト画像
	std::vector<int> atk_effect_gpc_hdls_;

	// ダメージ演出を行った回数
	int damage_production_count_;
	// ダメージ演出からの経過時間
	float damage_production_elapsed_;

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

	// ダメージを受けているか判定
	inline bool isTakeDamage() const { return is_take_damage_; };

	// 生存しているか判定
	inline bool isAlive() const { return is_alive_; }

	// 行動状態を取得
	inline eActState getActState() { return act_state_; }
	
	// 名前を取得
	inline const std::string& getName() const { return name_; };
	
	// ステータスクラスを取得
	inline const CharaStatus& getStatus() { return status_; }
	
	// 魔法一覧を取得
	const std::vector<std::shared_ptr<MagicBase>>& getMagicList() const;
	
	// キャラクターの見ている方向を取得
	inline eDir_8 getLookingDir() { return looking_dir_; }

	// 経験値を追加する
	inline void addExp(int exp) { status_.addExp(exp); }
	// HPを回復する
	inline void healHP(int amount) { status_.healHP(amount); }

	// ================= 関数のプロトタイプ =================

	// 指定した魔法があるか確認する
	bool checkMagicList(std::shared_ptr<MagicBase> magic);
	// 魔法を追加する
	void addMagic(std::shared_ptr<MagicBase> magic);

	// ================= 仮想関数 =================

	// 次のレベルまでの必要経験値を取得
	virtual int getExpToNextLevel() const = 0;
	// 指定した目標位置にキャラクターを移動させる
	virtual void moveToTargetPos(const tnl::Vector3& target_pos) = 0;
	// レベルが上がるが判定
	virtual bool canLevelUp() = 0;
	// 行動を開始させる
	virtual void beginAction() = 0;
	// 攻撃開始
	virtual void startAttack() = 0;
	// レベルアップ処理を行う
	virtual void startLevelUp() = 0;
	// ダメージを受ける
	virtual void takeDamage(int damage) = 0;

protected:
	// 指定した位置がフィールドの中か判定
	bool isInField(const tnl::Vector3& pos);

};