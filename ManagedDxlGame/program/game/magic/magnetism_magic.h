#pragma once
#include "../base/magic_base.h"

class Charaacter;
class Projectile;

// =====================================================================================
// 使用者を魔法弾の当たった位置まで移動させる魔法クラス
// =====================================================================================
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
	std::shared_ptr<MagicBase> createClone() override;

	void levelUpMagic() override;

	void setupToUseMagic(const std::shared_ptr<Character> owner) override;
	void startDrawEffect() override;
	void useMagic(std::shared_ptr<Character> owner) override;

private:
	void updateMagicExplantion() override;

};