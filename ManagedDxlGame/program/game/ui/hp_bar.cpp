#include "../../dxlib_ext/dxlib_ext.h"
#include "../common/camera.h"
#include "hp_bar.h"


HP_Bar::HP_Bar() {

	updateHP_Text();
}

HP_Bar::~HP_Bar() {

}

void HP_Bar::update(float delta_time) {

}

void HP_Bar::draw(const std::shared_ptr<Camera> camera) {

	DrawStringEx(hp_text_pos_.x, hp_text_pos_.y, -1, hp_text_.c_str());

	DrawBox(hp_bar_pos_.x, hp_bar_pos_.y, hp_bar_pos_.x + HP_BAR_MAX_WIDTH, hp_bar_pos_.y + HP_BAR_MAX_HEIGHT, GetColor(50, 50, 50), true);
	DrawBox(hp_bar_pos_.x, hp_bar_pos_.y, hp_bar_pos_.x + HP_BAR_MAX_WIDTH * hp_/ max_hp_, hp_bar_pos_.y + HP_BAR_MAX_HEIGHT, GetColor(0, 192, 255), true);
}