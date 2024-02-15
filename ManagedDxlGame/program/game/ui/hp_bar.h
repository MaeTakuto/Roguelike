#pragma once


class HP_Bar {
public:
	HP_Bar();
	~HP_Bar();

	void update(float delta_time);
	void draw(const std::shared_ptr<Camera> camera);

private:
	const int HP_BAR_MAX_WIDTH = 500;
	const int HP_BAR_MAX_HEIGHT = 40;

	tnl::Vector2i hp_bar_pos_ = { 450, 50 };
	tnl::Vector2i hp_text_pos_ = hp_bar_pos_ - tnl::Vector2i(150, -5);

	int max_hp_ = 1;
	int hp_ = 1;

	std::string hp_text_ = "";

	// HPÉoÅ[ÇÃïù
	int width_ = HP_BAR_MAX_WIDTH;

public:
	inline void setMaxHP(int max_hp) { max_hp_ = max_hp; }
	inline void setHP(int hp) { hp_ = hp; }

	inline void updateHP_Text() {
		hp_text_ = "HPÅF" + std::to_string(hp_) + "/" + std::to_string(max_hp_);
	}

};