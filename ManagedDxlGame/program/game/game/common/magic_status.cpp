#include "../../dxlib_ext/dxlib_ext.h"
#include "magic_status.h"

MagicStatus::MagicStatus() : magic_id_(-1), magic_name_(""), max_level_(3), now_level_(1), consume_mp_(0),
	magic_range_(0), magic_effect_multiplier_(0.0f) 
{

}

MagicStatus::~MagicStatus() {

}

bool MagicStatus::levelUp(const std::shared_ptr<MagicStatus> add_status ) {
	
	if (now_level_ >= max_level_) {
		tnl::DebugTrace("%s‚ÌƒŒƒxƒ‹‚ÍÅ‘å‚Å‚·", magic_name_.c_str());
		return false;
	}

	++now_level_;
	consume_mp_ += add_status->getConsumeMP();
	magic_range_ += add_status->getMagicRange();
	magic_effect_multiplier_ += add_status->getMagicEffectMultiplier();
	
	return true;
}
