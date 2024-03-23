#pragma once
#include "../base/magic_base.h"

class Charaacter;
class Projectile;

class FireMagic final : public MagicBase {
public:
	FireMagic();
	~FireMagic();

	void update(float delta_time) override;
	void draw(const std::shared_ptr<Camera> camera) override;

	void startDrawEffectOnOther(tnl::Vector2i pos, tnl::Vector2i size, eDir_8 other_dir) override;
	void useMagic(std::shared_ptr<Character> owner) override;

private:
	std::vector<int> fire_ball_gpc_hdl_;
	std::shared_ptr<Character> atk_target_;
	std::shared_ptr<Projectile> fire_ball_;

};