#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

class CharaStatus {
public:
	CharaStatus();
	~CharaStatus();

	// �Q�b�^�[
	inline int getLevel() const { return level_; }
	inline int getMaxHP() const { return max_hp_; }
	inline int getHP() const { return hp_; }
	inline int getAtk() const { return atk_; }
	inline int getDef() const { return def_; }
	inline int getExp() const { return exp_; }

	// �X�e�[�^�X���Z�b�g	
	inline void setStatus(int level, int max_hp, int atk, int def, int exp) {
		level_ = level;
		max_hp_ = max_hp;
		hp_ = max_hp_;
		atk_ = atk;
		def_ = def;
		exp_ = exp;
	}

	// �o���l�𓾂�
	inline void addExp(int exp) { exp_ += exp; }

	// �_���[�W���󂯂�
	inline void takeDamage(int damage) {
		hp_ -= damage;
		hp_ = max(0, hp_);
	}

	// HP���񕜂���
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

	// ���x���A�b�v�֐�
	inline void levelUP(int max_hp, int atk, int def) {
		++level_;
		max_hp_ += max_hp;
		hp_ = max_hp_;
		atk_ += atk;
		def_ += def;
	}

private:
	// ���x��
	int level_;

	// HP�̍ő�l
	int max_hp_;
	
	// ���݂�HP
	int hp_;
	
	// �U����
	int atk_;

	// �h���
	int def_;

	// �o���l
	int exp_;

};