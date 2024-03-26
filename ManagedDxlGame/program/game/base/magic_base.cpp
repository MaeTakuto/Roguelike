#include "../../dxlib_ext/dxlib_ext.h"
#include "../base/character_base.h"
#include "magic_base.h"


MagicBase::MagicBase() : magic_effect_(nullptr), magic_name_(""), magic_target_{ 0 }, is_draw_effect_(false), consumed_mp_(0), magic_explantion_{3}
{

}

MagicBase::~MagicBase() {

}