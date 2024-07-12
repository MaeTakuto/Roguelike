#include "../../dxlib_ext/dxlib_ext.h"
#include "magic_status.h"

MagicStatus::MagicStatus() : magic_name_(""), max_level_(3), now_level_(1), consume_mp_(0), magic_range_(0), magic_effect_multiplier_(0.0f) 
{

}

MagicStatus::~MagicStatus() {

}

bool MagicStatus::levelUp(int consume_mp, int add_magic_range, float add_magic_effect_multiplier) {
	
	if (now_level_ >= max_level_) {
		tnl::DebugTrace("%s‚ÌƒŒƒxƒ‹‚ÍÅ‘å‚Å‚·", magic_name_.c_str());
		return false;
	}
	++now_level_;
	consume_mp_ = consume_mp;
	magic_range_ += add_magic_range;
	magic_effect_multiplier_ += add_magic_effect_multiplier;
	
	return true;
}
