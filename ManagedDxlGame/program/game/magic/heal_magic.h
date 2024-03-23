#pragma once
#include "../base/magic_base.h"

class HealMagic : public MagicBase {
public:
	HealMagic();
	~HealMagic();

	void update(float delta_time) override;
	void draw(const std::shared_ptr<Camera> camera) override;

private:
	// ‰ñ•œ—Ê
	int heal_amount_;

	float draw_elapsed_time_;
	float effect_draw_time_;

public:
	void startDrawEffectOnOwner(tnl::Vector2i pos, tnl::Vector2i size) override;
	void useMagic(std::shared_ptr<Character> owner) override;

};