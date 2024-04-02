#pragma once
#include "../base/magic_base.h"

// =====================================================================================
// 使用者のHPを回復させる魔法クラス
// =====================================================================================
class HealMagic final : public MagicBase {
public:
	HealMagic();
	~HealMagic();

	void update(float delta_time) override;
	void draw(const std::shared_ptr<Camera> camera) override;

private:
	// 回復量
	int heal_amount_;

	float draw_elapsed_time_;
	float effect_draw_time_;

public:
	void setupToUseMagic(const std::shared_ptr<Character> owner) override;
	void startDrawEffect() override;
	void useMagic(std::shared_ptr<Character> owner) override;

};