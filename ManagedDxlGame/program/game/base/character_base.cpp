#include "../../dxlib_ext/dxlib_ext.h"
#include "../manager/gm_manager.h"
#include "magic_base.h"
#include "character_base.h"

Character::Character() : name_(""), status_(CharaStatus()), magic_list_(0), pos_(0.0f, 0.0f, 0.0f), next_pos_(0.0f, 0.0f, 0.0f), anim_dir_(eDir_4::DOWN),
	atk_target_(nullptr), atk_effect_(nullptr), looking_dir_{0}, act_state_ { 0 },is_take_damage_(false), is_alive_(true), is_drawing_(true),
	damage_production_count_(0), damage_production_elapsed_(0.0f)
{

}

// =====================================================================================
// 魔法一覧を返す
// =====================================================================================
const std::vector<std::shared_ptr<MagicBase>>& Character::getMagicList() const {
	return magic_list_;
}

// =====================================================================================
// 指定した位置がフィールドの中か判定
// =====================================================================================
bool Character::isInField(const tnl::Vector3& pos) {
	if (pos.x < 0) return false;
	if (pos.y < 0) return false;
	if (pos.x > GameManager::FIELD_WIDTH) return false;
	if (pos.y > GameManager::FIELD_HEIGHT) return false;
	return true;
}