#include "../../dxlib_ext/dxlib_ext.h"
#include "../manager/gm_manager.h"
#include "character_base.h"

Character::Character() : name_(""), status_(CharaStatus())/*, atk_target_(nullptr)*/, atk_effect_(Animation()), is_alive_(true), is_damaged_(false) {

}

// ダメージを受ける
void Character::takeDamage(int damage) {
	status_.takeDamage(damage);

	if (status_.getHP() <= 0) {
		is_alive_ = false;
	}
}

// 指定した位置がフィールドの中か判定
bool Character::isInField(const tnl::Vector3& pos) {
	if (pos.x < 0) return false;
	if (pos.y < 0) return false;
	if (pos.x > GameManager::FIELD_WIDTH) return false;
	if (pos.y > GameManager::FIELD_HEIGHT) return false;
	return true;
}