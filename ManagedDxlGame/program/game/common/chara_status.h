#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

class CharaStatus {
public:
	CharaStatus();
	~CharaStatus();

	// �Q�b�^�[

	// ���݂̃��x��
	inline int getLevel() const { return level_; }
	inline int getMaxHP() const { return max_hp_; }
	inline int getHP() const { return hp_; }
	inline int getMaxMP() const{ return max_mp_; }
	inline int getMP() const { return mp_; }
	inline int getAtk() const { return atk_; }
	inline int getDef() const { return def_; }
	inline int getExp() const { return exp_; }

	// �X�e�[�^�X���Z�b�g	
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

	// �o���l�𓾂�
	inline void addExp(int exp) { exp_ += exp; }

	// �_���[�W���󂯂�
	inline void takeDamage(int damage) {
		hp_ -= damage;
		hp_ = max(0, hp_);
	}
	// HP���񕜂���
	int healHP(int amount);

	// "amount"����MP�������BMP��0�����ɂȂ�ꍇ�A����� false��Ԃ��B
	bool tryConsumeMP(int amount);
	// MP���񕜂���
	void recoveryMP(int );


	// ���x���A�b�v�֐�
	void levelUP(int max_hp, int atk, int def);

private:
	// ���x��
	int level_;
	// HP�̍ő�l
	int max_hp_;
	// ���݂�HP
	int hp_;
	// MP�̍ő�l
	int max_mp_;
	// ���݂�MP
	int mp_;
	// �U����
	int atk_;
	// �h���
	int def_;
	// �o���l
	int exp_;

};