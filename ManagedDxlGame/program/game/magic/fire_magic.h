#pragma once
#include "../base/magic_base.h"

class Charaacter;
class Projectile;

// =====================================================================================
// 火の玉を飛ばし、当たったキャラクターにダメージを与える魔法クラス
// =====================================================================================
class FireMagic final : public MagicBase {
public:
	FireMagic();
	~FireMagic();

	void update(float delta_time) override;
	void draw(const std::shared_ptr<Camera> camera) override;

	void levelUpMagic() override;

	void setupToUseMagic(const std::shared_ptr<Character> owner) override;
	void startDrawEffect() override;
	void useMagic(std::shared_ptr<Character> owner) override;

private:
	std::vector<int> fire_ball_gpc_hdl_;
	std::shared_ptr<Character> atk_target_;
	std::shared_ptr<Projectile> fire_ball_;

private:
	void updateMagicExplantion() override;

};