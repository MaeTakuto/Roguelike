#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

class CharaStatus {
public:
	CharaStatus();
	~CharaStatus();

	// ゲッター
	inline int getLevel() const { return level_; }
	inline int getMaxHP() const { return max_hp_; }
	inline int getHP() const { return hp_; }
	inline int getAtk() const { return atk_; }
	inline int getDef() const { return def_; }
	inline int getExp() const { return exp_; }

	// ステータスをセット	
	inline void setStatus(int level, int max_hp, int atk, int def, int exp) {
		level_ = level;
		max_hp_ = max_hp;
		hp_ = max_hp_;
		atk_ = atk;
		def_ = def;
		exp_ = exp;
	}

	// 経験値を得る
	inline void addExp(int exp) { exp_ += exp; }

	// ダメージを受ける
	inline void takeDamage(int damage) {
		hp_ -= damage;
		hp_ = max(0, hp_);
	}

	// HPを回復する
	inline int healHP(int amount) {
		int heal_amount = 0;
		if (hp_ >= max_hp_) return heal_amount;

		heal_amount = amount;
		hp_ += heal_amount;
		if (max_hp_ < hp_) {
			heal_amount = amount - (hp_ - max_hp_);
			hp_ = max_hp_;
			return heal_amount;
		}
		return heal_amount;
	}

	// レベルアップ関数
	inline void levelUP(int max_hp, int atk, int def) {
		++level_;
		max_hp_ += max_hp;
		hp_ = max_hp_;
		atk_ += atk;
		def_ += def;
	}

private:
	// レベル
	int level_;

	// HPの最大値
	int max_hp_;
	
	// 現在のHP
	int hp_;
	
	// 攻撃力
	int atk_;

	// 防御力
	int def_;

	// 経験値
	int exp_;

};