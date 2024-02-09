#pragma once


class CharaStatus {
public:
	CharaStatus();
	~CharaStatus();

	inline int getMaxHP() { return max_hp_; }
	inline int getHP() { return hp_; }
	inline int getAtk() { return atk_; }
	inline int getDef() { return def_; }

	// HP‚ð‰ñ•œ‚·‚é
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

private:
	int max_hp_;
	int hp_;
	int atk_;
	int def_;

};