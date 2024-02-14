#include "../../dxlib_ext/dxlib_ext.h"
#include "../manager/resource_manager.h"
#include "../common/camera.h"
#include "message_window.h"


MessageWindow::MessageWindow() {

	mess_str_ = new std::string[MAX_MESSAGE_LINE];

	for (int line = 0; line < MAX_MESSAGE_LINE; line++) {
		mess_str_[line] = "";
	}
}

MessageWindow::~MessageWindow() {
	
	tnl::DebugTrace("MessageWindowのデストラクタが実行されました\n");

	delete[] mess_str_;
}

void MessageWindow::update(float delta_time) {

	if (is_time_limit_) {
		time_limit_ -= delta_time;

		if (time_limit_ <= 0) {
			is_time_limit_ = false;
			is_display_ = false;
			clearMessage();
		}
	}
}

void MessageWindow::draw(std::shared_ptr<Camera> canmera) {

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 192);
	DrawBox(window_pos_.x, window_pos_.y, window_pos_.x + MESSAGE_WINDOW_WIDTH, window_pos_.y + MESSAGE_WINDOW_HEIGHT, GetColor(0, 0, 128), true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	DrawBox(window_pos_.x, window_pos_.y, window_pos_.x + MESSAGE_WINDOW_WIDTH, window_pos_.y + MESSAGE_WINDOW_HEIGHT, -1, false);

	SetFontSize(MESSAGE_STR_SIZE);
	for (int line = 0; line < MAX_MESSAGE_LINE; line++) {
		DrawStringEx(mess_str_pos_.x, mess_str_pos_.y + (MESSAGE_SPACE * line), -1, mess_str_[line].c_str());
	}
}