#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

// =====================================================================================
// キャラクターのHPや攻撃力を管理するステータスクラス
// =====================================================================================
class CharaStatus {
public:
	CharaStatus();
	~CharaStatus();

	CharaStatus(const CharaStatus& status);

	// ---------------- ゲッター ---------------------------------

	// 現在のレベルを取得
	inline int getLevel() const { return level_; }
	// 最大HPを取得
	inline int getMaxHP() const { return max_hp_; }
	inline int getHP() const { return hp_; }
	inline int getMaxMP() const{ return max_mp_; }
	inline int getMP() const { return mp_; }
	inline int getAtk() const { return atk_; }
	inline int getDef() const { return def_; }
	inline int getExp() const { return exp_; }

	// ---------------- セッター ---------------------------------
	
	// ステータスをセット	
	inline void setStatus(int level, int max_hp, int max_mp, int atk, int def, int exp) {
		level_ = level;
		max_hp_ = max_hp;
		hp_ = max_hp_;
		max_mp_ = max_mp;
		mp_ = max_mp_;
		atk_ = atk;
		def_ = def;
		exp_ = exp;
	}

	// ---------------- その他 ---------------------------------

	// 経験値を得る
	inline void addExp(int exp) { exp_ += exp; }

	// ダメージを受ける
	inline void takeDamage(int damage) {
		hp_ -= damage;
		hp_ = max(0, hp_);
	}
	// HPを回復する
	int healHP(int amount);

	// "amount"分のMPを消費する。MPが0未満になる場合、消費せず falseを返す。
	bool tryConsumeMP(int amount);
	// MPを回復する
	void recoveryMP(int );


	// レベルアップ関数
	void levelUP(int max_hp, int max_mp, int atk, int def);

private:
	// レベル
	int level_;
	// HPの最大値
	int max_hp_;
	// 現在のHP
	int hp_;
	// MPの最大値
	int max_mp_;
	// 現在のMP
	int mp_;
	// 攻撃力
	int atk_;
	// 防御力
	int def_;
	// 経験値
	int exp_;

public:

	// --------------------- operator ---------------------

	CharaStatus& operator=(const CharaStatus& other) {
		this->level_ = other.level_;
		this->max_hp_ = other.max_hp_;
		this->hp_ = other.hp_;
		this->max_mp_ = other.max_mp_;
		this->mp_ = other.mp_;
		this->atk_ = other.atk_;
		this->def_ = other.def_;
		this->exp_ = other.exp_;

		return *this;
	}

};