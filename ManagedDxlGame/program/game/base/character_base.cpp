#include "../../dxlib_ext/dxlib_ext.h"
#include "../manager/gm_manager.h"
#include "character_base.h"


// �_���[�W���󂯂�
void Character::takeDamage(int damage) {
	status_.takeDamage(damage);

	if (status_.getHP() <= 0) {
		is_alive_ = false;
	}
}

// �w�肵���ʒu���t�B�[���h�̒�������
bool Character::isInField(const tnl::Vector3& pos) {
	if (pos.x < 0) return false;
	if (pos.y < 0) return false;
	if (pos.x > GameManager::FIELD_WIDTH) return false;
	if (pos.y > GameManager::FIELD_HEIGHT) return false;
	return true;
}