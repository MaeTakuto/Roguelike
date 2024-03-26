#include "../../dxlib_ext/dxlib_ext.h"
#include "../manager/gm_manager.h"
#include "magic_base.h"
#include "character_base.h"

Character::Character() : name_(""), status_(CharaStatus()), magic_list_(0), atk_target_(nullptr), atk_effect_(nullptr), looking_dir_{0}, act_state_ { 0 }, is_alive_(true)
{

}

// =====================================================================================
// ���@�ꗗ��Ԃ�
// =====================================================================================
const std::vector<std::shared_ptr<MagicBase>>& Character::getMagicList() const {
	return magic_list_;
}

// =====================================================================================
// �_���[�W���󂯂�
// =====================================================================================
void Character::takeDamage(int damage) {
	status_.takeDamage(damage);

	if (status_.getHP() <= 0) {
		is_alive_ = false;
	}
}

// =====================================================================================
// �w�肵���ʒu���t�B�[���h�̒�������
// =====================================================================================
bool Character::isInField(const tnl::Vector3& pos) {
	if (pos.x < 0) return false;
	if (pos.y < 0) return false;
	if (pos.x > GameManager::FIELD_WIDTH) return false;
	if (pos.y > GameManager::FIELD_HEIGHT) return false;
	return true;
}