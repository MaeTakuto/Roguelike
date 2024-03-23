#include "chara_status.h"


CharaStatus::CharaStatus() : level_(1), max_hp_(0), hp_(0), max_mp_(0), mp_(0), atk_(0), def_(0), exp_(0) {

}

CharaStatus::~CharaStatus() {

}

int CharaStatus::healHP(int amount) {
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

void CharaStatus::levelUP(int max_hp, int atk, int def) {
	++level_;
	max_hp_ += max_hp;
	hp_ = max_hp_;
	atk_ += atk;
	def_ += def;
}

bool CharaStatus::tryConsumeMP(int amount) {

	if ( mp_ - amount < 0 ) {
		return false;
	}
	mp_ -= amount;
	return true;
}

void CharaStatus::recoveryMP(int amount) {

	if (max_mp_ <= mp_) {
		return;
	}

	mp_ += amount;
	if (mp_ >= max_mp_) {
		mp_ = max_mp_;
	}

}
