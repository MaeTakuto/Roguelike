#include "../../dxlib_ext/dxlib_ext.h"
#include "../base/character_base.h"
#include "../common/magic_status.h"
#include "magic_base.h"


MagicBase::MagicBase() : magic_effect_(nullptr), magic_status_(std::make_shared<MagicStatus>()), magic_target_{ 0 }, is_draw_effect_(false), magic_explantion_{ 5 },
	magic_effect_amount_(0)
{

}

MagicBase::~MagicBase() {

}