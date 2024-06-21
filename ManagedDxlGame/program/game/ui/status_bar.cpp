#include "../../dxlib_ext/dxlib_ext.h"
#include "../common/camera.h"
#include "../manager/resource_manager.h"
#include "status_bar.h"


StatusBar::StatusBar() : status_bar_front_gpc_hdl_{0}, status_bar_back_gpc_hdl_(0), status_bar_back_pos_(450, 50), status_bar_back_size_(0, 0),
	status_bar_front_pos_(status_bar_back_pos_), status_bar_front_size_(0, 0), status_text_pos_(status_bar_back_pos_ - tnl::Vector2i(150, -5)),
	selected_color_(0), max_status_(1), now_status_(1), status_type_text_("İ’è‚µ‚Ä‚­‚¾‚³‚¢"), status_text_(""), message_font_size_(30), status_bar_outline_size_(2)
{

	status_bar_front_gpc_hdl_[std::underlying_type<eStatusBarColor>::type( eStatusBarColor::GREEN )] 
		= ResourceManager::getInstance()->loadGraph("graphics/hp_bar_front_green.png");

	status_bar_front_gpc_hdl_[std::underlying_type<eStatusBarColor>::type(eStatusBarColor::LIGHT_BLUE)]
		= ResourceManager::getInstance()->loadGraph("graphics/hp_bar_front_light_blue.png");
	
	status_bar_back_gpc_hdl_ = ResourceManager::getInstance()->loadGraph("graphics/hp_bar_back.png");

	int width = 0;
	int height = 0;
	float scale = 1.5f;
	GetGraphSize(status_bar_back_gpc_hdl_, &width, &height);
	status_bar_back_size_ = tnl::Vector2i(width, height);
	GetGraphSize(status_bar_front_gpc_hdl_[0], &width, &height);
	status_bar_front_size_ = tnl::Vector2i(width, height);

	status_bar_front_pos_ += ( status_bar_back_size_ - status_bar_front_size_ ) / 2;

	// CreateBaseImageToFile()

	updateStatus_Text(max_status_, now_status_);
}

StatusBar::~StatusBar() {

}

void StatusBar::update(float delta_time) {

}

void StatusBar::draw() {

	SetFontSize(message_font_size_);
	DrawStringEx(status_text_pos_.x, status_text_pos_.y, -1, status_text_.c_str());

	DrawExtendGraph(status_bar_back_pos_.x, status_bar_back_pos_.y, 
		status_bar_back_pos_.x + status_bar_back_size_.x, status_bar_back_pos_.y + status_bar_back_size_.y, status_bar_back_gpc_hdl_, true);
	
	DrawExtendGraph(status_bar_front_pos_.x, status_bar_front_pos_.y, 
		status_bar_front_pos_.x + status_bar_front_size_.x * now_status_ / max_status_, status_bar_front_pos_.y + status_bar_front_size_.y, 
		status_bar_front_gpc_hdl_[selected_color_], true
	);

	/*DrawBox(status_bar_pos_.x, status_bar_pos_.y, status_bar_pos_.x + status_bar_size_.x, status_bar_pos_.y + status_bar_size_.y, GetColor(50, 50, 50), true);
	DrawBox(status_bar_pos_.x, status_bar_pos_.y, status_bar_pos_.x + status_bar_size_.x * now_status_ / max_status_, status_bar_pos_.y + status_bar_size_.y, GetColor(0, 192, 255), true);
	*/
	//// ˜gü‚Ì•`‰æ
	//int status_bar_outline_size = status_bar_outline_size_ / 2;
	//int outline_color = GetColor(224, 224, 224);

	//DrawLine(status_bar_pos_.x - status_bar_outline_size, status_bar_pos_.y,
	//	status_bar_pos_.x + status_bar_size_.x + status_bar_outline_size, status_bar_pos_.y,
	//	outline_color, status_bar_outline_size_);
	//
	//DrawLine(status_bar_pos_.x, status_bar_pos_.y - status_bar_outline_size,
	//	status_bar_pos_.x, status_bar_pos_.y + status_bar_size_.y + status_bar_outline_size,
	//	outline_color, status_bar_outline_size_);
	//
	//DrawLine(status_bar_pos_.x + status_bar_size_.x, status_bar_pos_.y - status_bar_outline_size,
	//	status_bar_pos_.x + status_bar_size_.x, status_bar_pos_.y + status_bar_size_.y + status_bar_outline_size,
	//	outline_color, status_bar_outline_size_);
	//
	//DrawLine(status_bar_pos_.x - status_bar_outline_size, status_bar_pos_.y + status_bar_size_.y,
	//	status_bar_pos_.x + status_bar_size_.x + status_bar_outline_size, status_bar_pos_.y + status_bar_size_.y,
	//	outline_color, status_bar_outline_size_);
}

void StatusBar::updateStatus_Text(int max_status, int now_status) {
	max_status_ = max_status;
	now_status_ = now_status;
	status_text_ = status_type_text_ + "F" + std::to_string(now_status_) + "/" + std::to_string(max_status_);
}
