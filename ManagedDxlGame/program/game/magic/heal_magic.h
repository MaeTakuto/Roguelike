#pragma once
#include "../base/magic_base.h"

// =====================================================================================
// �g�p�҂�HP���񕜂����閂�@�N���X
// =====================================================================================
class HealMagic final : public MagicBase {
public:
	HealMagic();
	~HealMagic();

	void update(float delta_time) override;
	void draw(const std::shared_ptr<Camera> camera) override;

private:
	// �񕜗�
	int heal_amount_;

	float draw_elapsed_time_;
	float effect_draw_time_;

public:
	void setupToUseMagic(const std::shared_ptr<Character> owner) override;
	void startDrawEffect() override;
	void useMagic(std::shared_ptr<Character> owner) override;

};