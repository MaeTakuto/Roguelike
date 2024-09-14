#include "../../dxlib_ext/dxlib_ext.h"
#include "../base/character_base.h"
#include "../common/magic_status.h"
#include "magic_base.h"


MagicBase::MagicBase() : magic_icon_gpc_hdl_(0), magic_effect_(nullptr), magic_status_(std::make_shared<MagicStatus>()), level_up_bonus_status_(std::make_shared<MagicStatus>()),
	magic_target_{ 0 }, is_draw_effect_(false), magic_explantion_{ 5 },	magic_status_explantion_{ 4 }, magic_effect_amount_(0)
{

}

MagicBase::~MagicBase() {

}

std::vector<std::string> MagicBase::getStatusComparisonText() const
{
	int index = 0;
	std::vector<std::string> comparison_text(magic_status_explantion_.size());

	std::string comparison_status = std::to_string(magic_status_->getNowLevel());
	comparison_text[index] = magic_status_explantion_[index] + comparison_status;
	++index;

	comparison_status = std::to_string(magic_status_->getConsumeMP());
	comparison_text[index] = magic_status_explantion_[index] + comparison_status;
	++index;

	if (magic_status_->getMagicEffectMultiplier() + level_up_bonus_status_->getMagicEffectMultiplier() > 0.0f) {

		comparison_status = std::to_string(static_cast<int>( std::round(magic_status_->getMagicEffectMultiplier() * 100 ) ) ) + "％";
		comparison_text[index] = magic_status_explantion_[index] + comparison_status;
		++index;
	}

	if (magic_status_->getMagicRange() + level_up_bonus_status_->getMagicRange() > 0) {
		comparison_status = std::to_string(magic_status_->getMagicRange());
		comparison_text[index] = magic_status_explantion_[index] + comparison_status + "マス";
	}

	return comparison_text;
}

std::vector<std::string> MagicBase::getLevelUpStatusComparisonText() const
{
	int index = 0;

	std::vector<std::string> comparison_text(magic_status_explantion_.size());

	std::string comparison_status = std::to_string(magic_status_->getNowLevel()) + " → " + std::to_string(magic_status_->getNowLevel() + 1);
	comparison_text[index] = magic_status_explantion_[index] + comparison_status;
	++index;

	comparison_status = std::to_string(magic_status_->getConsumeMP()) + " → " + std::to_string(magic_status_->getConsumeMP() + level_up_bonus_status_->getConsumeMP() );
	comparison_text[index] = magic_status_explantion_[index] + comparison_status;
	++index;

	if (magic_status_->getMagicEffectMultiplier() + level_up_bonus_status_->getMagicEffectMultiplier() > 0.0f) {
		float next_multiplier = (magic_status_->getMagicEffectMultiplier() + level_up_bonus_status_->getMagicEffectMultiplier()) * 100;

		comparison_status = std::to_string(static_cast<int>(std::round( magic_status_->getMagicEffectMultiplier() * 100) ) ) 
			+ "％ → " + std::to_string(static_cast<int>(std::round(next_multiplier))) + "％";
		comparison_text[index] = magic_status_explantion_[index] + comparison_status;

		++index;
	}

	if (magic_status_->getMagicRange() + level_up_bonus_status_->getMagicRange() > 0) {
		comparison_status = std::to_string(magic_status_->getMagicRange()) + "マス → " 
			+ std::to_string(magic_status_->getMagicRange() + level_up_bonus_status_->getMagicRange()) + "マス";
		comparison_text[index] = magic_status_explantion_[index] + comparison_status;
	}

	return comparison_text;
}
