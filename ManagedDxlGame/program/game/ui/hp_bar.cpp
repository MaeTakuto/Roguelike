#include "../../dxlib_ext/dxlib_ext.h"
#include "../common/camera.h"
#include "hp_bar.h"


StatusBar::StatusBar() : status_bar_pos_(450, 50), status_bar_size_( 500, 40 ), status_text_pos_( status_bar_pos_ - tnl::Vector2i( 150, -5 ) ),
	max_status_(1), now_status_(1), status_type_text_("ê›íËÇµÇƒÇ≠ÇæÇ≥Ç¢"), status_text_(""), message_font_size_(30)
{

	updateStatus_Text(max_status_, now_status_);
}

StatusBar::~StatusBar() {

}

void StatusBar::update(float delta_time) {

}

void StatusBar::draw(const std::shared_ptr<Camera> camera) {

	SetFontSize(message_font_size_);
	DrawStringEx(status_text_pos_.x, status_text_pos_.y, -1, status_text_.c_str());

	DrawBox(status_bar_pos_.x, status_bar_pos_.y, status_bar_pos_.x + status_bar_size_.x, status_bar_pos_.y + status_bar_size_.y, GetColor(50, 50, 50), true);
	DrawBox(status_bar_pos_.x, status_bar_pos_.y, status_bar_pos_.x + status_bar_size_.x * now_status_ / max_status_, status_bar_pos_.y + status_bar_size_.y, GetColor(0, 192, 255), true);
}

void StatusBar::updateStatus_Text(int max_status, int now_status) {
	max_status_ = max_status;
	now_status_ = now_status;
	status_text_ = status_type_text_ + "ÅF" + std::to_string(now_status_) + "/" + std::to_string(max_status_);
}
