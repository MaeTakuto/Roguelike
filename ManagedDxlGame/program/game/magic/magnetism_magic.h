#pragma once
#include "../base/magic_base.h"

class Charaacter;
class Projectile;

class MagnetismMagic final : public MagicBase {
public:
	MagnetismMagic();
	~MagnetismMagic();

	void update(float delta_time) override;
	void draw(const std::shared_ptr<Camera> camera) override;

private:
	eDir_8 launch_dir_;
	std::vector<int> magic_bullet_gpc_hdl_;
	std::shared_ptr<Projectile> magic_bullet_;
	std::shared_ptr<tnl::Vector3> target_moving_position_;

public:
	void setupToUseMagic(const std::shared_ptr<Character> owner) override;
	void startDrawEffect() override;
	void useMagic(std::shared_ptr<Character> owner) override;

};